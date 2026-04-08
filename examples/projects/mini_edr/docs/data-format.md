# Data Format — Linux EDR Agent

**Version:** 1.1
**Date:** 2026-02-05

---

## 1. Event Types

| Event Type | Action | Description |
|------------|--------|-------------|
| process | exec | Process được execute |
| process | fork | Process fork child |
| process | exit | Process exit |
| process | setuid | Process thay đổi uid |
| file | open | File được open |
| file | write | File được write |
| file | unlink | File bị xóa |
| file | rename | File bị rename |
| network | connect | Outbound connection |
| network | accept | Inbound connection |
| network | bind | Socket bind |

---

## 2. Raw Event Structs (C)

### Common Header

```c
/* Event types */
#define EVT_PROCESS_EXEC	1
#define EVT_PROCESS_FORK	2
#define EVT_PROCESS_EXIT	3
#define EVT_PROCESS_SETUID	4
#define EVT_FILE_OPEN		10
#define EVT_FILE_WRITE		11
#define EVT_FILE_UNLINK		12
#define EVT_FILE_RENAME		13
#define EVT_NET_CONNECT		20
#define EVT_NET_ACCEPT		21
#define EVT_NET_BIND		22

#define COMM_LEN	16
#define PATH_MAX_LEN	4096
#define ARGV_MAX_LEN	4096
```

### eBPF Raw Event

```c
struct ebpf_event {
	__u32 event_type;
	__u64 timestamp;        /* ktime_get_ns() */
	__u32 pid;
	__u32 ppid;
	__u32 uid;
	__u32 gid;
	char  comm[COMM_LEN];

	union {
		struct {
			char  filename[PATH_MAX_LEN];
			char  argv[ARGV_MAX_LEN];
		} exec;

		struct {
			char  path[PATH_MAX_LEN];
			__u32 flags;
			__u32 mode;
		} file;

		struct {
			__u16 family;       /* AF_INET, AF_INET6 */
			__u16 protocol;     /* IPPROTO_TCP, IPPROTO_UDP */
			__u16 src_port;
			__u16 dst_port;
			union {
				__u32 v4_addr;
				__u8  v6_addr[16];
			} src;
			union {
				__u32 v4_addr;
				__u8  v6_addr[16];
			} dst;
		} network;
	};
};
```

> **Note**: Với eBPF, struct size lớn (do PATH_MAX_LEN=4096) sẽ ảnh hưởng
> ring buffer performance. Trong implementation thực tế, nên dùng variable-length
> data qua BPF ring buffer API. Struct trên là logical format, implementation
> có thể tối ưu bằng cách gửi actual length thay vì fixed buffer.

### Kernel Module Raw Event

```c
struct kmod_event {
	uint32_t type;
	uint64_t timestamp;
	uint32_t pid;
	uint32_t ppid;
	uint32_t uid;
	uint32_t gid;
	char     comm[COMM_LEN];

	uint32_t data_len;
	char     data[];        /* variable length, format tùy event type */
};
```

Kernel module dùng flexible array (`data[]`) nên không bị giới hạn fixed-size.
Format của `data[]` phụ thuộc vào `type`:

| type | data format |
|------|-------------|
| EVT_PROCESS_EXEC | `filename\0argv\0` (null-separated strings) |
| EVT_FILE_* | `path\0` + 4 bytes flags + 4 bytes mode |
| EVT_NET_* | `struct kmod_net_data` (xem bên dưới) |

```c
struct kmod_net_data {
	uint16_t family;
	uint16_t protocol;
	uint16_t src_port;
	uint16_t dst_port;
	union {
		uint32_t v4_addr;
		uint8_t  v6_addr[16];
	} src;
	union {
		uint32_t v4_addr;
		uint8_t  v6_addr[16];
	} dst;
};
```

---

## 3. Console Output Format (Phase 1)

Mỗi event in ra stdout 1 dòng:

```
[timestamp] event_type.action pid=PID ppid=PPID uid=UID comm=COMM details...
```

Ví dụ:

```
[1707123456.789] process.exec pid=1234 ppid=1000 uid=0 comm=bash filename=/bin/ls argv="ls -la"
[1707123456.800] file.open pid=1234 ppid=1000 uid=0 comm=ls path=/etc/passwd flags=O_RDONLY
[1707123456.810] network.connect pid=5678 ppid=1000 uid=1000 comm=curl dst=10.0.0.1:443 proto=tcp
[1707123456.820] network.connect pid=5679 ppid=1000 uid=1000 comm=curl dst=[2001:db8::1]:443 proto=tcp6
```
