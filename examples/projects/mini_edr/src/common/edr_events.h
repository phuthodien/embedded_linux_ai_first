/* SPDX-License-Identifier: GPL-2.0 */
/*
 * edr_events.h — Shared event definitions between kernel module and userspace
 *
 * This header is used by both kernel module (edr_kmodule.c) and
 * userspace agent (edr_agent.c). Use __KERNEL__ guard for kernel-specific types.
 */

#ifndef _EDR_EVENTS_H
#define _EDR_EVENTS_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <linux/types.h>  /* Use kernel types for userspace too (via libc) */
#endif

/* Netlink protocol and multicast group */
#define EDR_NETLINK_PROTO	31
#define EDR_NETLINK_GRP		1

/* Buffer sizes for prototype (production should use larger values) */
#define EDR_COMM_LEN		16
#define EDR_PATH_LEN		256

/* Event types */
#define EVT_PROCESS_EXEC	1
#define EVT_FILE_OPEN		10
#define EVT_NET_CONNECT		20

/*
 * struct edr_event — unified event structure
 *
 * Sent from kernel module to userspace via netlink multicast.
 * All fields are fixed-size for simplicity in prototype.
 */
struct edr_event {
	__u32 type;			/* EVT_PROCESS_EXEC, EVT_FILE_OPEN, etc. */
	__u64 timestamp_ns;		/* ktime_get_real_ns() */
	__u32 pid;
	__u32 ppid;
	__u32 uid;
	__u32 gid;
	char  comm[EDR_COMM_LEN];	/* process name */

	union {
		/* EVT_PROCESS_EXEC */
		struct {
			char filename[EDR_PATH_LEN];
		} exec;

		/* EVT_FILE_OPEN */
		struct {
			char path[EDR_PATH_LEN];
			__u32 flags;
		} file;

		/* EVT_NET_CONNECT */
		struct {
			__u16 family;		/* AF_INET or AF_INET6 */
			__u16 port;		/* destination port */
			union {
				__u32 v4;	/* IPv4 address */
				__u8  v6[16];	/* IPv6 address */
			} addr;
		} net;
	};
};

#endif /* _EDR_EVENTS_H */
