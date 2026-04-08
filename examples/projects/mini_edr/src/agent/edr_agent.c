/*
 * edr_agent.c — Userspace EDR agent
 *
 * Receives events from kernel module via netlink multicast and
 * prints them to stdout.
 *
 * Usage: sudo ./edr_agent
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <arpa/inet.h>

#include "edr_agent.h"

static volatile sig_atomic_t running = 1;

static void signal_handler(int sig)
{
	(void)sig;
	running = 0;
}

/*
 * Convert event type to string.
 */
const char *event_type_str(__u32 type)
{
	switch (type) {
	case EVT_PROCESS_EXEC:
		return "process.exec";
	case EVT_FILE_OPEN:
		return "file.open";
	case EVT_NET_CONNECT:
		return "network.connect";
	default:
		return "unknown";
	}
}

/*
 * Format timestamp from nanoseconds to human-readable format.
 */
void format_timestamp(__u64 ts_ns, char *buf, size_t len)
{
	time_t sec = ts_ns / 1000000000ULL;
	unsigned int msec = (ts_ns % 1000000000ULL) / 1000000;
	struct tm *tm;

	tm = localtime(&sec);
	if (tm) {
		snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d.%03u",
			 tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
			 tm->tm_hour, tm->tm_min, tm->tm_sec, msec);
	} else {
		snprintf(buf, len, "%llu.%03u",
			 (unsigned long long)(ts_ns / 1000000000ULL), msec);
	}
}

/*
 * Format event to string buffer.
 * Returns number of characters written (excluding null terminator).
 */
int format_event(const struct edr_event *evt, char *buf, size_t len)
{
	char ts_buf[64];
	char addr_buf[INET6_ADDRSTRLEN];
	int written = 0;
	int ret;

	format_timestamp(evt->timestamp_ns, ts_buf, sizeof(ts_buf));

	ret = snprintf(buf + written, len - written,
		       "[%s] %s pid=%u ppid=%u uid=%u comm=%s",
		       ts_buf, event_type_str(evt->type),
		       evt->pid, evt->ppid, evt->uid, evt->comm);
	if (ret < 0 || (size_t)ret >= len - written)
		return -1;
	written += ret;

	switch (evt->type) {
	case EVT_PROCESS_EXEC:
		ret = snprintf(buf + written, len - written,
			       " filename=%s", evt->exec.filename);
		break;

	case EVT_FILE_OPEN:
		ret = snprintf(buf + written, len - written,
			       " path=%s flags=0x%x", evt->file.path, evt->file.flags);
		break;

	case EVT_NET_CONNECT:
		if (evt->net.family == AF_INET) {
			inet_ntop(AF_INET, &evt->net.addr.v4,
				  addr_buf, sizeof(addr_buf));
		} else if (evt->net.family == AF_INET6) {
			inet_ntop(AF_INET6, evt->net.addr.v6,
				  addr_buf, sizeof(addr_buf));
		} else {
			snprintf(addr_buf, sizeof(addr_buf), "unknown");
		}
		ret = snprintf(buf + written, len - written,
			       " dst=%s:%u", addr_buf, evt->net.port);
		break;

	default:
		ret = 0;
		break;
	}

	if (ret < 0 || (size_t)ret >= len - written)
		return -1;
	written += ret;

	return written;
}

/*
 * Print event to stdout.
 */
static void print_event(const struct edr_event *evt)
{
	char buf[1024];

	if (format_event(evt, buf, sizeof(buf)) > 0) {
		printf("%s\n", buf);
		fflush(stdout);
	}
}

#ifndef EDR_TESTING
int main(int argc, char *argv[])
{
	int sock_fd;
	struct sockaddr_nl src_addr;
	char buffer[4096];
	ssize_t len;

	(void)argc;
	(void)argv;

	/* Setup signal handlers */
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	/* Create netlink socket */
	sock_fd = socket(AF_NETLINK, SOCK_RAW, EDR_NETLINK_PROTO);
	if (sock_fd < 0) {
		perror("socket");
		return 1;
	}

	/* Bind to multicast group */
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	src_addr.nl_groups = EDR_NETLINK_GRP;

	if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
		perror("bind");
		close(sock_fd);
		return 1;
	}

	printf("EDR Agent started. Listening for events...\n");
	printf("Press Ctrl+C to stop.\n\n");

	/* Main loop: receive and print events */
	while (running) {
		struct nlmsghdr *nlh;

		len = recv(sock_fd, buffer, sizeof(buffer), 0);
		if (len < 0) {
			if (errno == EINTR)
				continue;
			perror("recv");
			break;
		}

		if (len == 0)
			continue;

		/* Parse netlink message */
		nlh = (struct nlmsghdr *)buffer;
		while (NLMSG_OK(nlh, len)) {
			if (nlh->nlmsg_type == NLMSG_DONE) {
				struct edr_event *evt;

				evt = (struct edr_event *)NLMSG_DATA(nlh);
				print_event(evt);
			}
			nlh = NLMSG_NEXT(nlh, len);
		}
	}

	printf("\nEDR Agent stopped.\n");
	close(sock_fd);

	return 0;
}
#endif /* EDR_TESTING */
