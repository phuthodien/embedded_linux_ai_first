# Internal Interfaces — Linux EDR Agent

**Version:** 1.1
**Date:** 2026-02-05

> Phase 1: Chỉ internal C interfaces. gRPC sẽ bổ sung ở Phase 2.

---

## 1. Event Source Interface (C)

Mỗi sensor (eBPF hoặc kernel module) phải implement interface này thông qua function pointers:

```c
/*
 * event_source — interface chung cho tất cả sensors.
 * Mỗi sensor cung cấp một instance với các function pointers tương ứng.
 */
struct event_source {
	const char *name;                     /* "ebpf" hoặc "kmodule" */

	int  (*init)(struct event_source *);  /* khởi tạo sensor */
	void (*cleanup)(struct event_source *);/* giải phóng resources */
	int  (*start)(struct event_source *); /* bắt đầu thu thập */
	int  (*stop)(struct event_source *);  /* dừng thu thập */

	/*
	 * read_event — đọc 1 event từ sensor.
	 * Return: 0 nếu có event, -1 nếu không có, < -1 nếu lỗi.
	 * Event được ghi vào buf, buf_size là kích thước tối đa.
	 */
	int  (*read_event)(struct event_source *, void *buf, size_t buf_size);

	void *priv;                           /* sensor-specific data */
};
```

---

## 2. Event Callback Interface

Thay vì polling, sensor có thể dùng callback khi event sẵn sàng:

```c
/*
 * event_handler — callback được gọi khi sensor có event mới.
 * ctx: context do caller truyền vào khi đăng ký.
 * raw_event: con trỏ đến raw event data.
 * len: kích thước raw event.
 */
typedef void (*event_handler_fn)(void *ctx, const void *raw_event, size_t len);

/* Đăng ký callback cho sensor */
int event_source_set_handler(struct event_source *src,
			     event_handler_fn handler, void *ctx);
```

---

## 3. Sensor Manager

```c
enum sensor_type {
	SENSOR_AUTO,    /* tự detect */
	SENSOR_EBPF,
	SENSOR_KMODULE,
};

/*
 * sensor_manager — quản lý việc chọn và khởi tạo sensor.
 */
struct sensor_manager {
	enum sensor_type type;
	struct event_source *active_sensor;
};

/* Detect kernel version và chọn sensor phù hợp */
int sensor_manager_init(struct sensor_manager *mgr, enum sensor_type preferred);

/* Bắt đầu thu thập events */
int sensor_manager_start(struct sensor_manager *mgr);

/* Dừng và cleanup */
void sensor_manager_stop(struct sensor_manager *mgr);
```

---

## 4. Console Output (Phase 1)

Phase 1 output format — mỗi event một dòng:

```
[timestamp] event_type.action pid=PID ppid=PPID uid=UID comm=COMM details...
```

Ví dụ:

```
[1707123456.789] process.exec pid=1234 ppid=1000 uid=0 comm=bash filename=/bin/ls argv="ls -la"
[1707123456.800] file.open pid=1234 ppid=1000 uid=0 comm=ls path=/etc/passwd flags=O_RDONLY
[1707123456.810] network.connect pid=5678 ppid=1000 uid=1000 comm=curl dst=10.0.0.1:443 proto=tcp
```
