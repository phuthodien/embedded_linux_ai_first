// SPDX-License-Identifier: GPL-2.0
/*
 * edr_kmodule.c — EDR kernel module for event collection
 *
 * Uses kprobes to hook into LSM security functions and sends events
 * to userspace via netlink multicast.
 *
 * Hooked functions:
 *   - security_bprm_check: process execution
 *   - security_file_open: file open
 *   - security_socket_connect: network connect
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/binfmts.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <net/sock.h>
#include <net/netlink.h>

#include "../common/edr_events.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinalinux");
MODULE_DESCRIPTION("EDR Agent Kernel Module - Event Collection");
MODULE_VERSION("0.1");

/* Netlink socket for sending events to userspace */
static struct sock *nl_sock;

/* Forward declarations */
static int send_event(struct edr_event *evt);
static void fill_common(struct edr_event *evt, __u32 type);

/*
 * Check if we should skip this event.
 * Returns true if event should be filtered out.
 */
static bool should_skip(void)
{
	/* Skip kernel threads */
	if (current->flags & PF_KTHREAD)
		return true;

	/* Skip our own agent to avoid feedback loop */
	if (strncmp(current->comm, "edr_agent", 9) == 0)
		return true;

	return false;
}

/*
 * Check if path should be filtered (noisy paths).
 * Returns true if path should be skipped.
 */
static bool skip_path(const char *path)
{
	if (!path)
		return true;

	/* Skip procfs, sysfs, devfs */
	if (strncmp(path, "/proc/", 6) == 0 ||
	    strncmp(path, "/sys/", 5) == 0 ||
	    strncmp(path, "/dev/", 5) == 0)
		return true;

	return false;
}

/*
 * Fill common fields in event structure.
 */
static void fill_common(struct edr_event *evt, __u32 type)
{
	const struct cred *cred;

	memset(evt, 0, sizeof(*evt));
	evt->type = type;
	evt->timestamp_ns = ktime_get_real_ns();
	evt->pid = task_tgid_nr(current);
	evt->ppid = task_tgid_nr(current->real_parent);

	cred = current_cred();
	evt->uid = from_kuid_munged(current_user_ns(), cred->uid);
	evt->gid = from_kgid_munged(current_user_ns(), cred->gid);

	strscpy(evt->comm, current->comm, sizeof(evt->comm));
}

/*
 * Send event to userspace via netlink multicast.
 * Returns 0 on success, negative on error.
 */
static int send_event(struct edr_event *evt)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int ret;

	if (!nl_sock)
		return -ENODEV;

	skb = nlmsg_new(sizeof(*evt), GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	nlh = nlmsg_put(skb, 0, 0, NLMSG_DONE, sizeof(*evt), 0);
	if (!nlh) {
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	memcpy(nlmsg_data(nlh), evt, sizeof(*evt));

	/* Broadcast to multicast group */
	ret = netlink_broadcast(nl_sock, skb, 0, EDR_NETLINK_GRP, GFP_ATOMIC);
	if (ret < 0 && ret != -ESRCH) {
		/* ESRCH means no listeners, that's ok */
		return ret;
	}

	return 0;
}

/* ========== Kprobe handlers ========== */

/*
 * Handler for security_bprm_check (process exec).
 * Prototype: int security_bprm_check(struct linux_binprm *bprm)
 */
static int exec_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	struct linux_binprm *bprm;
	struct edr_event evt;
	const char *filename;

	if (should_skip())
		return 0;

	/* First argument: struct linux_binprm *bprm */
	bprm = (struct linux_binprm *)regs->di;
	if (!bprm || !bprm->file)
		return 0;

	filename = bprm->filename;
	if (skip_path(filename))
		return 0;

	fill_common(&evt, EVT_PROCESS_EXEC);
	strscpy(evt.exec.filename, filename, sizeof(evt.exec.filename));

	send_event(&evt);
	return 0;
}

/*
 * Handler for security_file_open (file open).
 * Prototype: int security_file_open(struct file *file)
 */
static int file_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	struct file *file;
	struct edr_event evt;
	char *buf, *path;

	if (should_skip())
		return 0;

	/* First argument: struct file *file */
	file = (struct file *)regs->di;
	if (!file || !file->f_path.dentry)
		return 0;

	/* Get path - need temporary buffer */
	buf = (char *)__get_free_page(GFP_ATOMIC);
	if (!buf)
		return 0;

	path = d_path(&file->f_path, buf, PAGE_SIZE);
	if (IS_ERR(path)) {
		free_page((unsigned long)buf);
		return 0;
	}

	if (skip_path(path)) {
		free_page((unsigned long)buf);
		return 0;
	}

	fill_common(&evt, EVT_FILE_OPEN);
	strscpy(evt.file.path, path, sizeof(evt.file.path));
	evt.file.flags = file->f_flags;

	free_page((unsigned long)buf);

	send_event(&evt);
	return 0;
}

/*
 * Handler for security_socket_connect (network connect).
 * Prototype: int security_socket_connect(struct socket *sock,
 *                                        struct sockaddr *address, int addrlen)
 */
static int connect_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	struct sockaddr *addr;
	struct edr_event evt;

	if (should_skip())
		return 0;

	/* Second argument: struct sockaddr *address */
	addr = (struct sockaddr *)regs->si;
	if (!addr)
		return 0;

	fill_common(&evt, EVT_NET_CONNECT);
	evt.net.family = addr->sa_family;

	if (addr->sa_family == AF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)addr;

		evt.net.port = ntohs(sin->sin_port);
		evt.net.addr.v4 = sin->sin_addr.s_addr;
	} else if (addr->sa_family == AF_INET6) {
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;

		evt.net.port = ntohs(sin6->sin6_port);
		memcpy(evt.net.addr.v6, &sin6->sin6_addr, 16);
	} else {
		/* Skip non-IP sockets (unix, netlink, etc.) */
		return 0;
	}

	send_event(&evt);
	return 0;
}

/* Kprobe structures */
static struct kprobe kp_exec = {
	.symbol_name = "security_bprm_check",
	.pre_handler = exec_pre_handler,
};

static struct kprobe kp_file = {
	.symbol_name = "security_file_open",
	.pre_handler = file_pre_handler,
};

static struct kprobe kp_connect = {
	.symbol_name = "security_socket_connect",
	.pre_handler = connect_pre_handler,
};

/* Netlink callback (not used, we only send) */
static void nl_recv_msg(struct sk_buff *skb)
{
	/* We don't receive commands in Phase 1 */
}

/* Netlink configuration */
static struct netlink_kernel_cfg nl_cfg = {
	.input = nl_recv_msg,
	.groups = EDR_NETLINK_GRP,
};

static int __init edr_init(void)
{
	int ret;

	pr_info("edr_kmodule: loading\n");

	/* Create netlink socket */
	nl_sock = netlink_kernel_create(&init_net, EDR_NETLINK_PROTO, &nl_cfg);
	if (!nl_sock) {
		pr_err("edr_kmodule: failed to create netlink socket\n");
		return -ENOMEM;
	}

	/* Register kprobes - continue even if some fail */
	ret = register_kprobe(&kp_exec);
	if (ret < 0)
		pr_warn("edr_kmodule: exec kprobe failed: %d\n", ret);
	else
		pr_info("edr_kmodule: exec kprobe registered\n");

	ret = register_kprobe(&kp_file);
	if (ret < 0)
		pr_warn("edr_kmodule: file kprobe failed: %d\n", ret);
	else
		pr_info("edr_kmodule: file kprobe registered\n");

	ret = register_kprobe(&kp_connect);
	if (ret < 0)
		pr_warn("edr_kmodule: connect kprobe failed: %d\n", ret);
	else
		pr_info("edr_kmodule: connect kprobe registered\n");

	pr_info("edr_kmodule: loaded successfully\n");
	return 0;
}

static void __exit edr_exit(void)
{
	pr_info("edr_kmodule: unloading\n");

	/* Unregister kprobes */
	if (kp_exec.addr)
		unregister_kprobe(&kp_exec);
	if (kp_file.addr)
		unregister_kprobe(&kp_file);
	if (kp_connect.addr)
		unregister_kprobe(&kp_connect);

	/* Release netlink socket */
	if (nl_sock)
		netlink_kernel_release(nl_sock);

	pr_info("edr_kmodule: unloaded\n");
}

module_init(edr_init);
module_exit(edr_exit);
