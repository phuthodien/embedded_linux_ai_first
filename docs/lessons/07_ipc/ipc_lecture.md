# Bài Giảng: IPC trong Embedded Linux
**Tác giả:** Lưu An Phú — CEO Vinalinux  
**Khóa học:** Lập trình trên Linux — 60 giờ / 20 buổi  
**Thời lượng:** 2 giờ

---

## Phần 1 — Tại sao cần IPC?

### 1.1 Thế giới MCU: không cần IPC

Trên vi điều khiển (STM32, ESP32...), toàn bộ firmware chạy trong **một không gian địa chỉ duy nhất**. Mọi task — dù dùng RTOS hay bare-metal — đều có thể đọc/ghi trực tiếp vào bất kỳ biến nào:

```c
// Task A ghi
sensor_buffer[idx] = read_adc();

// Task B đọc — trực tiếp, không cần cơ chế gì thêm
process(sensor_buffer[idx]);
```

Giao tiếp giữa các task chỉ là **chia sẻ biến toàn cục** hoặc dùng queue nội bộ của RTOS. Đơn giản, trực tiếp.

---

### 1.2 Thế giới Linux: process isolation

Linux áp dụng **virtual memory** — mỗi process sống trong không gian địa chỉ ảo riêng biệt, được kernel bảo vệ. Process A **không thể** đọc bộ nhớ của Process B theo cách thông thường.

```
Process A          Process B
┌──────────┐      ┌──────────┐
│ 0x0000.. │      │ 0x0000.. │  ← cùng địa chỉ ảo
│  code    │      │  code    │    nhưng map tới
│  heap    │      │  heap    │    physical page
│  stack   │      │  stack   │    hoàn toàn khác
└──────────┘      └──────────┘
     │                  │
     └──────┬───────────┘
            │ kernel
       [page tables]
```

Đây là tính năng — không phải bug. Isolation giúp hệ thống ổn định: một process crash không kéo theo process khác. Nhưng nó đặt ra câu hỏi: **làm sao để hai process giao tiếp với nhau?**

Câu trả lời là **IPC — Inter-Process Communication**.

---

### 1.3 Taxonomy: ba loại nhu cầu giao tiếp

Trước khi đi vào từng cơ chế, cần phân biệt **mục đích** giao tiếp:

| Loại | Mục đích | Ví dụ cơ chế |
|---|---|---|
| **Data transfer** | Truyền dữ liệu từ A sang B | Pipe, FIFO, Message Queue, Socket |
| **Synchronization** | Phối hợp thứ tự thực thi | Semaphore, Mutex, eventfd |
| **Signaling** | Thông báo sự kiện, không kèm data lớn | Signal, eventfd |

Nhiều bài toán thực tế cần **kết hợp** cả hai — ví dụ: Shared Memory (data transfer) + Semaphore (synchronization).

---

## Phần 2 — Bản đồ toàn cảnh IPC

### 2.1 Danh sách đầy đủ các cơ chế IPC trên Linux

Linux cung cấp rất nhiều cơ chế IPC, được phát triển qua nhiều thập kỷ. Dưới đây là toàn bộ danh sách — kể cả những cơ chế ít gặp trong embedded:

| Cơ chế | Nguồn gốc | Ghi chú |
|---|---|---|
| **Anonymous Pipe** | UNIX truyền thống | Chỉ dùng giữa parent/child |
| **Named Pipe (FIFO)** | UNIX truyền thống | Unrelated processes, dùng qua filesystem |
| **Signal** | UNIX truyền thống | Signaling, không truyền data lớn |
| **System V Shared Memory** | System V IPC | Legacy, vẫn còn trong nhiều hệ thống cũ |
| **System V Semaphore** | System V IPC | Legacy, API phức tạp |
| **System V Message Queue** | System V IPC | Legacy |
| **POSIX Shared Memory** | POSIX | Hiện đại hơn SysV, dùng `shm_open()` |
| **POSIX Semaphore** | POSIX | Hiện đại hơn SysV, dùng `sem_open()` |
| **POSIX Message Queue** | POSIX | Có priority, dùng `mq_open()` |
| **Unix Domain Socket** | BSD | Giao tiếp local, API giống TCP socket |
| **TCP/UDP Socket** | BSD | Qua network stack, dùng được cross-machine |
| **eventfd** | Linux-specific | Lightweight signaling, hay dùng với epoll |
| **signalfd** | Linux-specific | Đọc signal như đọc file descriptor |
| **memfd** | Linux-specific | Anonymous shared memory, không cần filesystem |
| **D-Bus** | Freedesktop | Higher-level, phổ biến trên desktop Linux |
| **Netlink Socket** | Linux-specific | Giao tiếp giữa kernel và userspace |

> **Lưu ý cho embedded developer:** Không cần biết hết tất cả — cần biết *cái nào tồn tại* để không tự phát minh lại bánh xe, và biết *khi nào tìm đến cái nào*.

---

### 2.2 Tư duy lựa chọn IPC theo bài toán

Thay vì học thuộc "dùng cái này cho bài toán kia", hãy tập đặt **bốn câu hỏi** sau khi gặp bài toán IPC:

**Câu hỏi 1: Các process có quan hệ gì với nhau?**
- Parent/child process → Anonymous Pipe là đủ
- Unrelated processes → cần cơ chế có tên (FIFO, POSIX shm, socket...)

**Câu hỏi 2: Dữ liệu truyền có cấu trúc không?**
- Byte stream (không quan tâm ranh giới message) → Pipe, Socket
- Structured message (cần ranh giới rõ ràng, có priority) → Message Queue

**Câu hỏi 3: Tốc độ quan trọng đến mức nào?**
- Copy qua kernel chấp nhận được → Pipe, Message Queue
- Zero-copy, latency thấp nhất → Shared Memory (nhưng phải tự đồng bộ)

**Câu hỏi 4: Cần giao tiếp cross-machine không?**
- Chỉ local → Unix Domain Socket (nhanh hơn TCP vì bỏ qua network stack)
- Cross-machine → TCP/UDP Socket

---

### 2.3 Bảng so sánh tổng quan

| Cơ chế | Tốc độ | Có cấu trúc | Unrelated process | Persist khi process chết | Embedded phổ biến |
|---|---|---|---|---|---|
| Anonymous Pipe | Trung bình | Không | ✗ | ✗ | ✓ |
| Named Pipe (FIFO) | Trung bình | Không | ✓ | ✗ | ✓ |
| Signal | Rất nhanh | Không (chỉ số) | ✓ | ✗ | ✓ |
| POSIX Shared Memory | **Nhanh nhất** | Tùy thiết kế | ✓ | ✗* | ✓ |
| POSIX Semaphore | Nhanh | — | ✓ | ✗* | ✓ |
| POSIX Message Queue | Trung bình | ✓ | ✓ | ✓** | ✓ |
| Unix Domain Socket | Nhanh | Tùy | ✓ | ✗ | ✓ |
| TCP Socket | Chậm nhất | Tùy | ✓ | ✗ | Khi cần remote |
| eventfd | Rất nhanh | Không | ✓ | ✗ | Ít gặp |
| D-Bus | Chậm | ✓ | ✓ | ✗ | Desktop, không embedded |

> \* POSIX shm/sem tồn tại trong `/dev/shm` cho đến khi bị xóa thủ công hoặc reboot.  
> \** POSIX mq tồn tại cho đến khi bị `mq_unlink()` hoặc reboot.

---

## Phần 3 — Pipe & FIFO

### 3.1 Anonymous Pipe — giao tiếp parent/child

Pipe là cơ chế IPC đơn giản nhất trên Linux, ra đời từ những ngày đầu của UNIX. Mô hình rất trực quan: dữ liệu chảy **một chiều** từ đầu ghi vào đầu đọc — giống như ống nước.

```
Process A (writer)          Process B (reader)
     │                            │
     │ write(fd[1], ...)          │ read(fd[0], ...)
     ▼                            ▼
  [fd[1]] ──────────────────► [fd[0]]
            kernel buffer
            (64KB mặc định)
```

Kernel cấp phát một buffer trong bộ nhớ. Process A ghi vào, Process B đọc ra — **không có file nào trên disk**, không có địa chỉ nào để đặt tên.

**Tạo pipe:**

```c
int fd[2];
pipe(fd);
// fd[0] = đầu đọc (read end)
// fd[1] = đầu ghi (write end)
```

**Pattern chuẩn với fork:**

```c
int fd[2];
pipe(fd);

pid_t pid = fork();
if (pid == 0) {
    // Child: chỉ đọc
    close(fd[1]);                      // đóng đầu ghi
    char buf[64];
    read(fd[0], buf, sizeof(buf));
    printf("Child nhận: %s\n", buf);   // output: Child nhận: hello
    close(fd[0]);
} else {
    // Parent: chỉ ghi
    close(fd[0]);                      // đóng đầu đọc
    write(fd[1], "hello", 5);
    close(fd[1]);
    wait(NULL);
}
```

> **Tại sao phải `close()` đầu không dùng?** Nếu parent giữ cả hai đầu mở, kernel không biết khi nào pipe kết thúc — child sẽ bị block ở `read()` mãi mãi. Đây là pitfall phổ biến nhất khi mới dùng pipe.

**Giới hạn của anonymous pipe:**
- Chỉ dùng được giữa **process có quan hệ** (parent/child, hoặc cùng fork từ một cha)
- Một chiều — muốn hai chiều phải tạo hai pipe
- Buffer giới hạn (~64KB trên Linux) — ghi vượt quá sẽ block

---

### 3.2 Named Pipe (FIFO) — mở rộng sang unrelated process

FIFO giải quyết giới hạn lớn nhất của pipe: **hai process hoàn toàn không liên quan** cũng có thể giao tiếp, thông qua một "tên" trên filesystem.

```bash
mkfifo /tmp/sensor_pipe
```

Trông như một file bình thường trên disk, nhưng **không có dữ liệu nào thực sự được lưu xuống** — kernel chỉ dùng filesystem entry như một điểm hẹn để hai process tìm thấy nhau.

```
producer (process A)              consumer (process B)
         │                                │
         │ open("/tmp/sensor_pipe", O_WRONLY)
         │                                │ open("/tmp/sensor_pipe", O_RDONLY)
         │                                │
         ▼                                ▼
      [write end] ──── kernel ────► [read end]
```

> **Blocking behavior quan trọng:** `open()` trên FIFO sẽ **block** cho đến khi cả hai đầu đều có process mở. Producer gọi `open(O_WRONLY)` sẽ chờ consumer; consumer gọi `open(O_RDONLY)` sẽ chờ producer. Đây là cơ chế đồng bộ ngầm — và cũng là pitfall nếu không hiểu.

**Producer:**

```c
int fd = open("/tmp/sensor_pipe", O_WRONLY);
float temp = 36.5;
write(fd, &temp, sizeof(temp));
close(fd);
```

**Consumer:**

```c
int fd = open("/tmp/sensor_pipe", O_RDONLY);
float temp;
read(fd, &temp, sizeof(temp));
printf("Nhiệt độ: %.1f°C\n", temp);  // output: Nhiệt độ: 36.5°C
close(fd);
```

---

### 3.3 Khi nào dùng Pipe/FIFO — khi nào không?

| Tình huống | Lựa chọn |
|---|---|
| Parent cần stream data sang child | Anonymous Pipe |
| Hai daemon độc lập cần stream data | FIFO |
| Cần truyền structured message có priority | → Message Queue |
| Cần tốc độ cao, zero-copy | → Shared Memory |
| Cần two-way communication phức tạp | → Unix Domain Socket |

**Embedded use case điển hình:** Một process thu thập dữ liệu cảm biến liên tục ghi vào FIFO, một process logging đọc ra và ghi vào flash — tách biệt hoàn toàn hai nhiệm vụ, không cần chia sẻ bộ nhớ.

---

## Phần 4 — Signal

### 4.1 Signal là gì?

Signal là cơ chế **thông báo bất đồng bộ** — kernel (hoặc một process khác) "gõ vai" process đang chạy để báo một sự kiện nào đó xảy ra. Process không cần polling, không cần chờ — signal đến bất cứ lúc nào.

Analogy quen thuộc với embedded developer: signal giống **interrupt trên MCU**. Chương trình đang chạy bình thường, có sự kiện → nhảy vào handler → xử lý xong → quay lại.

```
Process đang chạy bình thường
    │
    │  ... main loop ...
    │
    ◄── signal SIGTERM đến (từ kernel hoặc process khác)
    │
    ▼
[signal handler chạy]
    │
    ▼
quay lại main loop (hoặc exit)
```

**Một số signal phổ biến:**

| Signal | Giá trị | Ý nghĩa mặc định | Embedded use case |
|---|---|---|---|
| `SIGTERM` | 15 | Terminate (có thể bắt) | Graceful shutdown |
| `SIGINT` | 2 | Interrupt (Ctrl+C) | Dừng chương trình |
| `SIGKILL` | 9 | Kill (không thể bắt) | Force kill |
| `SIGSEGV` | 11 | Segmentation fault | Debug crash |
| `SIGALRM` | 14 | Timer hết hạn | Watchdog đơn giản |
| `SIGUSR1/2` | 10/12 | User-defined | Custom event |
| `SIGCHLD` | 17 | Child process kết thúc | Quản lý process con |

---

### 4.2 `signal()` vs `sigaction()` — dùng cái nào?

Có hai cách đăng ký signal handler. Embedded developer hay gặp `signal()` trước vì trông đơn giản hơn — nhưng trong thực tế production code, **luôn dùng `sigaction()`**.

**Dùng `signal()` — đơn giản nhưng nhiều bẫy:**

```c
void handler(int sig) {
    printf("Nhận signal %d\n", sig);
}

signal(SIGTERM, handler);
```

Vấn đề: hành vi của `signal()` **không nhất quán giữa các hệ thống**. Trên một số implementation, sau khi handler chạy xong, signal bị reset về `SIG_DFL` — nghĩa là lần sau process sẽ bị kill thay vì gọi lại handler.

**Dùng `sigaction()` — rõ ràng, portable, production-ready:**

```c
void handler(int sig) {
    // xử lý signal
}

struct sigaction sa = {0};
sa.sa_handler = handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART;   // tự động restart syscall bị interrupt

sigaction(SIGTERM, &sa, NULL);
sigaction(SIGINT, &sa, NULL);
```

`SA_RESTART` đặc biệt quan trọng trong embedded: nếu signal đến đúng lúc process đang `read()` từ UART hay `select()` trên socket, `SA_RESTART` đảm bảo syscall tự động retry thay vì trả về lỗi `EINTR` — tránh phải xử lý thêm một tầng error.

---

### 4.3 Pitfall quan trọng: async-signal-safe

Signal handler có thể bị gọi **bất kỳ lúc nào** — kể cả đúng lúc process đang thực thi một hàm khác. Điều này tạo ra một ràng buộc nghiêm khắc: **chỉ được gọi các hàm async-signal-safe bên trong handler**.

**Ví dụ nguy hiểm:**

```c
void handler(int sig) {
    printf("Shutting down...\n");  // NGUY HIỂM
    free(some_ptr);                // NGUY HIỂM
}
```

`printf()` dùng internal buffer và lock — nếu signal đến đúng lúc main code đang `printf()`, hai luồng cùng tranh lock → **deadlock**.

**Pattern an toàn — dùng flag:**

```c
volatile sig_atomic_t g_shutdown = 0;

void handler(int sig) {
    g_shutdown = 1;   // atomic write — an toàn
}

int main() {
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &sa, NULL);

    while (!g_shutdown) {
        // main loop
        do_work();
    }

    // cleanup an toàn ở đây — ngoài handler
    printf("Shutting down gracefully\n");  // output: Shutting down gracefully
    cleanup();
    return 0;
}
```

> `volatile sig_atomic_t` là kiểu dữ liệu duy nhất đảm bảo việc ghi/đọc là atomic trong context của signal handler. Thiếu `volatile` → compiler có thể cache giá trị trong register và không bao giờ thấy thay đổi từ handler.

---

### 4.4 Embedded use case: graceful shutdown và watchdog

**Graceful shutdown** là use case phổ biến nhất — systemd gửi `SIGTERM` trước khi kill service, process cần có cơ hội đóng file, flush buffer, release resource:

```
systemd                    embedded daemon
   │                            │
   │──── SIGTERM ──────────────►│
   │                            │ handler: g_shutdown = 1
   │                            │ main loop detect → cleanup
   │                            │ close UART, flush log...
   │◄─── exit(0) ───────────────│
   │
   │ (nếu không exit trong timeout)
   │──── SIGKILL ──────────────►│  (bị kill cứng)
```

**Watchdog đơn giản với SIGALRM:**

```c
void watchdog_handler(int sig) {
    syslog(LOG_CRIT, "Watchdog timeout — system hung");
    abort();
}

signal(SIGALRM, watchdog_handler);
alarm(5);  // trigger SIGALRM sau 5 giây

while (1) {
    do_work();
    alarm(5);  // reset watchdog mỗi vòng lặp
}
```

---

## Phần 5 — Shared Memory + Semaphore

### 5.1 Tại sao Shared Memory nhanh nhất?

Nhớ lại mô hình IPC với Pipe hay Message Queue: dữ liệu phải đi qua kernel buffer — process A ghi vào kernel, process B đọc từ kernel. Có **hai lần copy**.

```
Pipe / Message Queue:
Process A                kernel buffer              Process B
[user space] ──copy──► [kernel space] ──copy──► [user space]
```

Shared Memory hoạt động khác hoàn toàn: kernel map **cùng một vùng physical memory** vào address space của cả hai process. Không có copy nào cả.

```
Shared Memory:
Process A                                         Process B
[user space]                                      [user space]
   │                                                   │
   └──────────────► [physical RAM] ◄───────────────────┘
                    (shared page)
```

Đây là lý do Shared Memory là cơ chế **data transfer nhanh nhất** trong Linux IPC — đặc biệt quan trọng khi truyền dữ liệu lớn như frame camera, audio buffer, hay sensor array tần số cao.

---

### 5.2 POSIX Shared Memory — API chuẩn

POSIX shm dùng một "tên" trên filesystem ảo `/dev/shm` để hai process tìm thấy nhau — tương tự FIFO nhưng thay vì truyền data qua kernel, hai bên cùng trỏ vào một vùng RAM.

**Pattern chuẩn — Producer (tạo và ghi):**

```c
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_NAME "/sensor_buf"
#define SHM_SIZE 4096

// Tạo shared memory
int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
ftruncate(fd, SHM_SIZE);

// Map vào address space
float *buf = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);
close(fd);  // fd không cần sau khi mmap

buf[0] = 36.5;  // ghi trực tiếp vào shared memory
```

**Consumer (mở và đọc):**

```c
int fd = shm_open(SHM_NAME, O_RDONLY, 0666);

float *buf = mmap(NULL, SHM_SIZE, PROT_READ,
                  MAP_SHARED, fd, 0);
close(fd);

printf("Nhiệt độ: %.1f°C\n", buf[0]);  // output: Nhiệt độ: 36.5°C
```

**Cleanup:**

```c
munmap(buf, SHM_SIZE);
shm_unlink(SHM_NAME);  // xóa entry trong /dev/shm
```

---

### 5.3 Race condition — cái bẫy tất yếu của Shared Memory

Shared Memory nhanh vì không có kernel kiểm soát trung gian — nhưng đó cũng là lý do nó **nguy hiểm**: hai process cùng truy cập một vùng nhớ mà không có gì bảo vệ.

**Analogy:** Hãy hình dung một tấm bảng trắng trong văn phòng. Hai người cùng muốn ghi lên bảng — nếu không có quy tắc "ai đang ghi thì người kia chờ", kết quả trên bảng sẽ là nội dung hỗn loạn của cả hai.

```
Producer đang ghi dở struct (chưa xong)
    buf->temp = 36.5;
    // ← context switch xảy ra ở đây!
    buf->pressure = ...;  // chưa ghi

Consumer đọc đúng lúc này
    → đọc được temp đúng nhưng pressure sai
    → data corruption im lặng, không có error nào
```

Đây là loại bug nguy hiểm nhất: **không crash, không báo lỗi** — chỉ cho kết quả sai.

---

### 5.4 POSIX Semaphore — người gác cổng

Semaphore là một bộ đếm nguyên tử do kernel quản lý, với hai thao tác:
- **`sem_wait()`** — giảm counter, nếu counter = 0 thì block (chờ)
- **`sem_post()`** — tăng counter, unblock một waiter nếu có

Pattern chuẩn khi kết hợp với Shared Memory:

```c
// Khởi tạo semaphore
sem_t *sem = sem_open("/sensor_sem", O_CREAT, 0666, 1);
// Giá trị ban đầu = 1 → mutex pattern
```

**Producer:**

```c
sem_wait(sem);        // khóa
buf[0] = 36.5;        // ghi an toàn
buf[1] = 1013.25;
sem_post(sem);        // mở khóa
```

**Consumer:**

```c
sem_wait(sem);        // chờ nếu producer đang ghi
float temp = buf[0];
float pressure = buf[1];
sem_post(sem);        // mở khóa

printf("Temp: %.1f, Pressure: %.2f\n", temp, pressure);
// output: Temp: 36.5, Pressure: 1013.25
```

```
Producer          Semaphore (counter)       Consumer
   │                    [1]                    │
   │── sem_wait() ──►  [0]                    │
   │   (locked)                               │
   │                                          │── sem_wait() ──► BLOCK
   │   ghi data...                            │   (chờ)
   │── sem_post() ──►  [1]                    │
   │                                          │◄─ unblock
   │                                          │   đọc data...
   │                                          │── sem_post() ──► [1]
```

---

### 5.5 Embedded use case: sensor data pipeline

Mô hình phổ biến trong embedded Linux — một process thu thập, một process xử lý, tốc độ cao:

```c
// Đặt cả data lẫn semaphore trong shared memory
typedef struct {
    sem_t lock;
    uint32_t timestamp;
    float channels[8];   // 8 kênh ADC
    int new_data;
} SensorShm;

// Producer (chạy ở 1kHz)
sem_wait(&shm->lock);
shm->timestamp = get_timestamp();
for (int i = 0; i < 8; i++)
    shm->channels[i] = read_adc(i);
shm->new_data = 1;
sem_post(&shm->lock);

// Consumer (xử lý khi có data mới)
sem_wait(&shm->lock);
if (shm->new_data) {
    process_channels(shm->channels, 8);
    shm->new_data = 0;
}
sem_post(&shm->lock);
```

> **Lưu ý:** Semaphore đặt trong shared memory phải được khởi tạo với `sem_init(&shm->lock, 1, 1)` — tham số thứ hai `pshared=1` cho biết semaphore dùng giữa các process, không phải giữa các thread.

---

## Phần 6 — Message Queue

### 6.1 Vấn đề Pipe chưa giải quyết được

Pipe và FIFO truyền dữ liệu tốt — nhưng là **byte stream**: kernel không biết đâu là ranh giới giữa các message. Nếu producer ghi hai message liên tiếp, consumer có thể đọc được nửa message đầu và nửa message sau trong một lần `read()`.

```
Producer ghi:
  write(fd, "MSG1", 4)
  write(fd, "MSG2", 4)

Kernel buffer: [M][S][G][1][M][S][G][2]

Consumer đọc với buffer 6 bytes:
  read(fd, buf, 6) → "MSG1MS"   ← ranh giới message bị mất
```

Embedded developer quen với UART biết rõ vấn đề này — phải tự thêm header, length field, hay delimiter để phân tách message. POSIX Message Queue giải quyết vấn đề này ở tầng kernel.

---

### 6.2 POSIX Message Queue — đặc điểm cốt lõi

POSIX mq có ba tính chất phân biệt với Pipe:

**1. Message boundary được bảo toàn:** mỗi lần `mq_send()` là một message hoàn chỉnh, mỗi lần `mq_receive()` nhận đúng một message — không bao giờ bị cắt đôi.

**2. Priority:** mỗi message có một giá trị priority (0–31). `mq_receive()` luôn trả về message có priority **cao nhất** trước, bất kể thứ tự gửi.

**3. Persist cho đến khi bị unlink:** queue tồn tại trong `/dev/mqueue` ngay cả khi producer đã exit — consumer vẫn đọc được message còn lại.

---

### 6.3 API và ví dụ

**Producer — gửi message có priority:**

```c
#include <mqueue.h>

typedef struct {
    int sensor_id;
    float value;
} SensorMsg;

struct mq_attr attr = {
    .mq_maxmsg  = 10,
    .mq_msgsize = sizeof(SensorMsg)
};

mqd_t mq = mq_open("/sensor_queue", O_CREAT | O_WRONLY, 0666, &attr);

SensorMsg msg = {.sensor_id = 3, .value = 85.0};

// Gửi cảnh báo nhiệt độ cao với priority 10
mq_send(mq, (char*)&msg, sizeof(msg), 10);

// Gửi telemetry thường với priority 1
msg.value = 36.5;
mq_send(mq, (char*)&msg, sizeof(msg), 1);

mq_close(mq);
```

**Consumer — luôn nhận message priority cao trước:**

```c
mqd_t mq = mq_open("/sensor_queue", O_RDONLY);

SensorMsg msg;
unsigned int priority;

// Lần đọc đầu tiên → nhận message priority 10 (cảnh báo)
mq_receive(mq, (char*)&msg, sizeof(msg), &priority);
printf("Priority %u: sensor %d = %.1f\n", priority, msg.sensor_id, msg.value);
// output: Priority 10: sensor 3 = 85.0

// Lần đọc thứ hai → nhận message priority 1 (telemetry)
mq_receive(mq, (char*)&msg, sizeof(msg), &priority);
printf("Priority %u: sensor %d = %.1f\n", priority, msg.sensor_id, msg.value);
// output: Priority 1: sensor 3 = 36.5

mq_close(mq);
mq_unlink("/sensor_queue");
```

> **Compile flag:** POSIX mq yêu cầu link với `-lrt` trên một số hệ thống: `gcc main.c -lrt`

---

### 6.4 Khi nào chọn Message Queue thay vì Pipe?

| Tiêu chí | Pipe/FIFO | Message Queue |
|---|---|---|
| Ranh giới message | Tự xử lý | Kernel bảo toàn |
| Priority | Không có | Có (0–31) |
| Persist sau khi producer exit | Không | Có |
| Tốc độ | Nhanh hơn | Chậm hơn một chút |
| API | Đơn giản | Phức tạp hơn |

**Embedded use case điển hình — task dispatcher:**

```
[Sensor process]  ──mq_send(priority=10)──►
[Button ISR]      ──mq_send(priority=5)───►  [Main dispatcher] → xử lý theo priority
[Telemetry]       ──mq_send(priority=1)───►
```

---

## Phần 7 — Giao tiếp Driver ↔ User App

### 7.1 Kernel space là một thế giới khác

Khi viết một kernel module, bạn không còn đang viết một "chương trình" theo nghĩa thông thường. Cần hiểu rõ môi trường thực thi để tránh những sai lầm căn bản.

**Kernel không phải một process:**

```
User space                        Kernel space
┌─────────────┐  ┌─────────────┐  ┌─────────────────────────────┐
│  Process A  │  │  Process B  │  │                             │
│  pid=1234   │  │  pid=5678   │  │   kernel + tất cả modules   │
│  virt addr  │  │  virt addr  │  │   chạy chung một addr space │
│  space riêng│  │  space riêng│  │   không có PID              │
└─────────────┘  └─────────────┘  └─────────────────────────────┘
      │                │                        │
      └────────────────┴────────────────────────┘
                    physical RAM
```

Kernel module khi được load vào kernel — nó **trở thành một phần của kernel**. Không có process boundary, không có memory protection giữa các module. Một module viết sai địa chỉ có thể corrupt toàn bộ kernel.

**Môi trường thực thi đặc biệt:**

| Đặc điểm | User space | Kernel space |
|---|---|---|
| Memory allocation | `malloc()` / `free()` | `kmalloc()` / `kfree()` |
| In ra màn hình | `printf()` | `printk()` |
| Xử lý lỗi | exception → signal | oops → kernel panic |
| Stack size | Vài MB | ~8KB (rất nhỏ) |
| Có thể sleep? | Tùy ý | Chỉ trong một số context |
| Floating point | Bình thường | **Không được dùng** |

> **Stack 8KB là giới hạn nghiêm khắc:** kernel module không được đệ quy sâu, không được khai báo array lớn trên stack. Đây là nguồn gốc của nhiều kernel stack overflow bug trong driver.

**Hai context thực thi trong kernel:**

```
Process context:                    Interrupt context:
─────────────────                   ──────────────────
App gọi read()/write()/ioctl()      Hardware interrupt xảy ra
→ kernel thực thi code driver       → kernel gọi interrupt handler
→ có thể sleep, có thể schedule     → KHÔNG được sleep
→ có current process                → KHÔNG có current process
```

---

### 7.2 Tại sao IPC thông thường không dùng được từ kernel?

Nhìn lại toàn bộ các cơ chế IPC đã học — tất cả đều có một điểm chung: chúng là **POSIX API dành cho userspace**.

**Pipe, FIFO, Message Queue:**

```c
// Userspace app — hoàn toàn hợp lệ
int fd = open("/tmp/sensor_pipe", O_RDONLY);

// Kernel module — KHÔNG THỂ làm điều này
// open() là syscall → kernel module không gọi syscall của chính mình
```

Kernel module không gọi syscall — nó *là* nơi syscall được thực thi. Gọi `open()`, `mq_open()`, `shm_open()` từ kernel module là vô nghĩa về mặt kiến trúc.

**POSIX Shared Memory:**

`shm_open()` tạo một file trong `/dev/shm` — đây là userspace VFS operation. Kernel module không có file descriptor table, không có working directory, không thể dùng API này.

**Signal — trường hợp đặc biệt:**

```
Kernel → Userspace:  send_sig(SIGIO, task, 0)  ✓  hoạt động được
Userspace → Kernel:  kill(kernel_pid, ...)      ✗  kernel không có PID
```

**Kết luận:** Cần các cơ chế được thiết kế đặc biệt cho **kernel↔userspace boundary** — nơi một bên là privileged kernel code, bên kia là unprivileged user process.

---

### 7.3 Các mô hình IPC phù hợp cho kernel ↔ app

#### ioctl + poll() — cặp đôi kinh điển

`ioctl` là cơ chế để userspace app **ra lệnh** cho driver và nhận kết quả nhỏ. `poll()` là cơ chế để app **chờ event** từ driver mà không blocking CPU.

```
User app                          Kernel driver
   │                                   │
   │──── ioctl(fd, CMD_SET_RATE, 100) ►│ driver xử lý lệnh
   │◄─── return 0 (ok) ────────────────│
   │                                   │
   │──── poll(fd, POLLIN, timeout) ───►│ app chờ data sẵn sàng
   │     (block, không tốn CPU)        │ hardware interrupt xảy ra
   │                                   │ driver gọi wake_up_interruptible()
   │◄─── POLLIN event ─────────────────│
   │──── read(fd, buf, len) ──────────►│ đọc data
```

**Phía kernel driver:**

```c
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case CMD_SET_RATE:
            sample_rate = arg;
            return 0;
        case CMD_GET_STATUS:
            return copy_to_user((void*)arg, &status, sizeof(status));
    }
    return -EINVAL;
}

static unsigned int my_poll(struct file *f, poll_table *wait) {
    poll_wait(f, &my_wait_queue, wait);
    if (data_ready)
        return POLLIN | POLLRDNORM;
    return 0;
}

// Khi có data (trong interrupt handler):
data_ready = 1;
wake_up_interruptible(&my_wait_queue);  // unblock poll()
```

**Phía userspace app:**

```c
int fd = open("/dev/mysensor", O_RDWR);

ioctl(fd, CMD_SET_RATE, 100);

struct pollfd pfd = {.fd = fd, .events = POLLIN};
poll(&pfd, 1, 1000);  // timeout 1 giây

if (pfd.revents & POLLIN) {
    read(fd, &data, sizeof(data));
    printf("Sensor: %.2f\n", data);  // output: Sensor: 36.50
}
```

---

#### mmap trên device file — zero-copy cho data lớn

Khi data lớn (DMA buffer, camera frame, ADC stream), copy từ kernel sang userspace qua `read()` tốn kém. Driver có thể expose trực tiếp kernel buffer lên userspace qua `mmap`.

```
Userspace app                    Kernel driver
[virtual addr] ──────────────► [DMA buffer trong kernel]
                  mmap()         (cùng physical page)
                  không copy
```

**Phía kernel driver:**

```c
static int my_mmap(struct file *f, struct vm_area_struct *vma) {
    return remap_pfn_range(vma,
        vma->vm_start,
        virt_to_phys(dma_buf) >> PAGE_SHIFT,
        vma->vm_end - vma->vm_start,
        vma->vm_page_prot);
}
```

**Phía userspace app:**

```c
int fd = open("/dev/mysensor", O_RDWR);

uint8_t *frame = mmap(NULL, FRAME_SIZE, PROT_READ,
                      MAP_SHARED, fd, 0);

process_frame(frame, FRAME_SIZE);  // đọc trực tiếp — không có copy nào

munmap(frame, FRAME_SIZE);
```

> **Đây là kỹ thuật V4L2 (Video4Linux) dùng cho camera:** toàn bộ pipeline camera trên embedded Linux đều dùng mmap để tránh copy frame buffer — vì một frame 1080p = 6MB, copy 30 lần/giây = 180MB/s chỉ để di chuyển data.

---

#### Netlink Socket — driver chủ động push event

Hai cơ chế trên đều theo hướng **app chủ động hỏi driver**. Netlink đảo chiều: **driver chủ động push event** lên userspace mà không cần app polling.

```
Kernel driver                    User app
   │                                │
   │  hardware event xảy ra        │ đang chờ trên recvmsg()
   │                                │
   │──── netlink_unicast() ────────►│ nhận event ngay lập tức
   │     (push, không cần poll)     │ xử lý
```

**Phía userspace app — lắng nghe Netlink:**

```c
int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_USER);

struct sockaddr_nl addr = {
    .nl_family = AF_NETLINK,
    .nl_pid    = getpid()
};
bind(sock, (struct sockaddr*)&addr, sizeof(addr));

char buf[4096];
recv(sock, buf, sizeof(buf), 0);

struct nlmsghdr *nlh = (struct nlmsghdr*)buf;
printf("Event từ driver: %s\n", (char*)NLMSG_DATA(nlh));
// output: Event từ driver: TEMP_ALERT:85.0
```

**Phía kernel driver — gửi event:**

```c
struct sk_buff *skb = nlmsg_new(msg_size, GFP_ATOMIC);
struct nlmsghdr *nlh = nlmsg_put(skb, 0, 0, NLMSG_DONE, msg_size, 0);

snprintf(NLMSG_DATA(nlh), msg_size, "TEMP_ALERT:%.1f", temp);

netlink_unicast(nl_sock, skb, app_pid, MSG_DONTWAIT);
```

**Tổng kết ba mô hình:**

| Mô hình | Chiều giao tiếp | Dùng khi |
|---|---|---|
| `ioctl` | App → Driver (lệnh) | App cần điều khiển driver, nhận kết quả nhỏ |
| `poll` + `read` | Driver → App (data) | App chờ data, không muốn busy-wait |
| `mmap` trên device | Driver → App (buffer) | Data lớn, cần zero-copy |
| Netlink | Driver → App (event) | Driver cần chủ động push event |

---

## Phần 8 — Tổng kết

### 8.1 Recap — Decision Guide

Khi gặp bài toán IPC, đặt bốn câu hỏi theo thứ tự:

```
1. Giao tiếp giữa ai?
   ├── Kernel ↔ App → ioctl/poll, mmap, Netlink
   └── Process ↔ Process → tiếp tục câu hỏi 2

2. Các process có quan hệ parent/child không?
   ├── Có → Anonymous Pipe là đủ
   └── Không → tiếp tục câu hỏi 3

3. Cần truyền data hay chỉ báo hiệu?
   ├── Chỉ báo hiệu → Signal, eventfd
   └── Truyền data → tiếp tục câu hỏi 4

4. Ưu tiên tốc độ hay cấu trúc message?
   ├── Tốc độ cao, data lớn → Shared Memory + Semaphore
   ├── Cần message có priority → Message Queue
   ├── Byte stream đơn giản → FIFO
   └── Cần two-way, phức tạp → Unix Domain Socket
```

---

### 8.2 Bảng tổng hợp cuối khóa

| Cơ chế | Tốc độ | Cấu trúc | Unrelated | Kernel↔App | Embedded dùng |
|---|---|---|---|---|---|
| Anonymous Pipe | Trung bình | Stream | ✗ | ✗ | ✓ |
| FIFO | Trung bình | Stream | ✓ | ✗ | ✓ |
| Signal | Rất nhanh | Không | ✓ | Một chiều | ✓ |
| Shared Memory + Sem | **Nhanh nhất** | Tùy thiết kế | ✓ | ✗ | ✓ |
| Message Queue | Trung bình | ✓ có priority | ✓ | ✗ | ✓ |
| Unix Domain Socket | Nhanh | Tùy | ✓ | ✗ | Bài sau |
| ioctl + poll | Nhanh | Nhỏ | — | ✓ | ✓ |
| mmap device | **Nhanh nhất** | Buffer lớn | — | ✓ | ✓ |
| Netlink | Nhanh | Event | — | ✓ | ✓ |

---

### 8.3 Pointer sang bài tiếp theo

**Unix Domain Socket** — về mặt API giống hệt TCP socket nhưng chỉ chạy local, nhanh hơn TCP vì bỏ qua network stack. Sẽ học chi tiết trong bài **Network Programming**.

**D-Bus** — higher-level IPC phổ biến trên desktop Linux (systemd, BlueZ, NetworkManager đều dùng). Không phổ biến trong embedded vì overhead lớn — đề cập để học viên biết tên khi gặp.

---

### 8.4 Take-home Exercise

**Bài 1 — Pipe pipeline:**
Viết ba process kết nối thành chuỗi qua pipe:
```
[sensor] → pipe → [filter] → pipe → [logger]
```
`sensor` sinh dữ liệu ngẫu nhiên mỗi 100ms, `filter` loại bỏ giá trị ngoài ngưỡng, `logger` ghi ra file. Câu hỏi cốt lõi: điều gì xảy ra nếu `filter` exit đột ngột — `sensor` có biết không?

**Bài 2 — Shared Memory race condition:**
Viết producer và consumer dùng shared memory **không có semaphore**. Chạy với data lớn và quan sát corruption. Sau đó thêm semaphore vào và so sánh kết quả. Mục tiêu: tự chứng kiến race condition thay vì chỉ đọc lý thuyết.

**Bài 3 — Signal graceful shutdown:**
Viết một daemon giả lập (vòng lặp vô tận ghi log mỗi giây). Đăng ký `SIGTERM` handler dùng pattern `volatile sig_atomic_t`. Dùng `systemctl stop` hoặc `kill` để dừng — đảm bảo daemon flush log trước khi exit. Câu hỏi: điều gì xảy ra nếu dùng `kill -9` thay vì `kill -15`?
