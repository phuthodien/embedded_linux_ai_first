/*
 * test_edr_agent.c — Unit tests for edr_agent.c
 *
 * Simple test framework without external dependencies.
 * Compile with: gcc -DEDR_TESTING -o test_edr_agent test_edr_agent.c ../src/agent/edr_agent.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "../src/agent/edr_agent.h"

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
	printf("Running %s...", #name); \
	tests_run++; \
	test_##name(); \
	tests_passed++; \
	printf(" PASSED\n"); \
} while (0)

#define ASSERT_STREQ(a, b) do { \
	if (strcmp((a), (b)) != 0) { \
		printf(" FAILED\n  Expected: %s\n  Got: %s\n", (b), (a)); \
		exit(1); \
	} \
} while (0)

#define ASSERT_TRUE(cond) do { \
	if (!(cond)) { \
		printf(" FAILED\n  Condition false: %s\n", #cond); \
		exit(1); \
	} \
} while (0)

#define ASSERT_EQ(a, b) do { \
	if ((a) != (b)) { \
		printf(" FAILED\n  Expected: %ld, Got: %ld\n", (long)(b), (long)(a)); \
		exit(1); \
	} \
} while (0)

/* ========== Tests for event_type_str ========== */

TEST(event_type_str_process_exec)
{
	ASSERT_STREQ(event_type_str(EVT_PROCESS_EXEC), "process.exec");
}

TEST(event_type_str_file_open)
{
	ASSERT_STREQ(event_type_str(EVT_FILE_OPEN), "file.open");
}

TEST(event_type_str_net_connect)
{
	ASSERT_STREQ(event_type_str(EVT_NET_CONNECT), "network.connect");
}

TEST(event_type_str_unknown)
{
	ASSERT_STREQ(event_type_str(999), "unknown");
}

TEST(event_type_str_zero)
{
	ASSERT_STREQ(event_type_str(0), "unknown");
}

/* ========== Tests for format_timestamp ========== */

TEST(format_timestamp_basic)
{
	char buf[64];
	/* 2024-01-15 10:30:00.123 UTC = 1705315800123000000 ns */
	__u64 ts = 1705315800123000000ULL;

	format_timestamp(ts, buf, sizeof(buf));

	/* Check format: YYYY-MM-DD HH:MM:SS.mmm */
	ASSERT_TRUE(strlen(buf) == 23);
	ASSERT_TRUE(buf[4] == '-');
	ASSERT_TRUE(buf[7] == '-');
	ASSERT_TRUE(buf[10] == ' ');
	ASSERT_TRUE(buf[13] == ':');
	ASSERT_TRUE(buf[16] == ':');
	ASSERT_TRUE(buf[19] == '.');
}

TEST(format_timestamp_zero)
{
	char buf[64];

	format_timestamp(0, buf, sizeof(buf));

	/* Should produce valid output for epoch */
	ASSERT_TRUE(strlen(buf) > 0);
}

TEST(format_timestamp_small_buffer)
{
	char buf[10];

	/* Should not crash with small buffer */
	format_timestamp(1705315800123000000ULL, buf, sizeof(buf));
	/* Output will be truncated but should be null-terminated */
	ASSERT_TRUE(strlen(buf) < sizeof(buf));
}

TEST(format_timestamp_milliseconds)
{
	char buf[64];
	/* Test different millisecond values */
	__u64 ts_000 = 1705315800000000000ULL;  /* .000 */
	__u64 ts_500 = 1705315800500000000ULL;  /* .500 */
	__u64 ts_999 = 1705315800999000000ULL;  /* .999 */

	format_timestamp(ts_000, buf, sizeof(buf));
	ASSERT_TRUE(strstr(buf, ".000") != NULL);

	format_timestamp(ts_500, buf, sizeof(buf));
	ASSERT_TRUE(strstr(buf, ".500") != NULL);

	format_timestamp(ts_999, buf, sizeof(buf));
	ASSERT_TRUE(strstr(buf, ".999") != NULL);
}

/* ========== Tests for format_event ========== */

TEST(format_event_process_exec)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_PROCESS_EXEC;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 1234;
	evt.ppid = 1000;
	evt.uid = 0;
	strncpy(evt.comm, "bash", sizeof(evt.comm));
	strncpy(evt.exec.filename, "/bin/ls", sizeof(evt.exec.filename));

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "process.exec") != NULL);
	ASSERT_TRUE(strstr(buf, "pid=1234") != NULL);
	ASSERT_TRUE(strstr(buf, "ppid=1000") != NULL);
	ASSERT_TRUE(strstr(buf, "uid=0") != NULL);
	ASSERT_TRUE(strstr(buf, "comm=bash") != NULL);
	ASSERT_TRUE(strstr(buf, "filename=/bin/ls") != NULL);
}

TEST(format_event_file_open)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_FILE_OPEN;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 5678;
	evt.ppid = 1234;
	evt.uid = 1000;
	strncpy(evt.comm, "cat", sizeof(evt.comm));
	strncpy(evt.file.path, "/etc/passwd", sizeof(evt.file.path));
	evt.file.flags = 0x0;  /* O_RDONLY */

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "file.open") != NULL);
	ASSERT_TRUE(strstr(buf, "pid=5678") != NULL);
	ASSERT_TRUE(strstr(buf, "path=/etc/passwd") != NULL);
	ASSERT_TRUE(strstr(buf, "flags=0x0") != NULL);
}

TEST(format_event_net_connect_ipv4)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_NET_CONNECT;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 9999;
	evt.ppid = 1;
	evt.uid = 1000;
	strncpy(evt.comm, "curl", sizeof(evt.comm));
	evt.net.family = AF_INET;
	evt.net.port = 443;
	inet_pton(AF_INET, "10.0.0.1", &evt.net.addr.v4);

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "network.connect") != NULL);
	ASSERT_TRUE(strstr(buf, "dst=10.0.0.1:443") != NULL);
}

TEST(format_event_net_connect_ipv6)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_NET_CONNECT;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 8888;
	evt.ppid = 1;
	evt.uid = 0;
	strncpy(evt.comm, "wget", sizeof(evt.comm));
	evt.net.family = AF_INET6;
	evt.net.port = 80;
	inet_pton(AF_INET6, "2001:db8::1", evt.net.addr.v6);

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "network.connect") != NULL);
	ASSERT_TRUE(strstr(buf, "dst=2001:db8::1:80") != NULL);
}

TEST(format_event_net_connect_unknown_family)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_NET_CONNECT;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 7777;
	evt.ppid = 1;
	evt.uid = 0;
	strncpy(evt.comm, "test", sizeof(evt.comm));
	evt.net.family = 99;  /* Unknown family */
	evt.net.port = 1234;

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "dst=unknown:1234") != NULL);
}

TEST(format_event_unknown_type)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = 999;  /* Unknown type */
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 1111;
	evt.ppid = 1;
	evt.uid = 0;
	strncpy(evt.comm, "unknown", sizeof(evt.comm));

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(strstr(buf, "unknown") != NULL);
	ASSERT_TRUE(strstr(buf, "pid=1111") != NULL);
}

TEST(format_event_small_buffer)
{
	struct edr_event evt = {0};
	char buf[10];  /* Too small */
	int ret;

	evt.type = EVT_PROCESS_EXEC;
	evt.timestamp_ns = 1705315800123000000ULL;
	evt.pid = 1234;
	strncpy(evt.comm, "bash", sizeof(evt.comm));

	ret = format_event(&evt, buf, sizeof(buf));

	/* Should return -1 for buffer overflow */
	ASSERT_EQ(ret, -1);
}

TEST(format_event_exact_buffer)
{
	struct edr_event evt = {0};
	char buf[1024];
	int ret;

	evt.type = EVT_PROCESS_EXEC;
	evt.timestamp_ns = 0;
	evt.pid = 1;
	evt.ppid = 0;
	evt.uid = 0;
	strncpy(evt.comm, "a", sizeof(evt.comm));
	strncpy(evt.exec.filename, "/b", sizeof(evt.exec.filename));

	ret = format_event(&evt, buf, sizeof(buf));

	ASSERT_TRUE(ret > 0);
	ASSERT_TRUE(ret < (int)sizeof(buf));
}

/* ========== Main ========== */

int main(void)
{
	printf("\n=== EDR Agent Unit Tests ===\n\n");

	/* event_type_str tests */
	RUN_TEST(event_type_str_process_exec);
	RUN_TEST(event_type_str_file_open);
	RUN_TEST(event_type_str_net_connect);
	RUN_TEST(event_type_str_unknown);
	RUN_TEST(event_type_str_zero);

	/* format_timestamp tests */
	RUN_TEST(format_timestamp_basic);
	RUN_TEST(format_timestamp_zero);
	RUN_TEST(format_timestamp_small_buffer);
	RUN_TEST(format_timestamp_milliseconds);

	/* format_event tests */
	RUN_TEST(format_event_process_exec);
	RUN_TEST(format_event_file_open);
	RUN_TEST(format_event_net_connect_ipv4);
	RUN_TEST(format_event_net_connect_ipv6);
	RUN_TEST(format_event_net_connect_unknown_family);
	RUN_TEST(format_event_unknown_type);
	RUN_TEST(format_event_small_buffer);
	RUN_TEST(format_event_exact_buffer);

	printf("\n=== Results: %d/%d tests passed ===\n\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
