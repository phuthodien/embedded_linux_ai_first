# Bài giảng: Linux Process
**Khóa học:** Lập trình trên Linux  
**Thời lượng:** 3 giờ  
**Đối tượng:** Embedded developer chuyển sang Linux

---

## Phần 1 — Process là gì trên Linux? (20 phút)

### 1.0 Vấn đề ban đầu — Tại sao cần có khái niệm Process?

Những chiếc máy tính đầu tiên chạy theo mô hình đơn giản: **một chương trình, một máy**. Khi muốn chạy chương trình mới, người vận hành phải dừng chương trình hiện tại, nạp chương trình mới vào bộ nhớ, rồi chạy lại từ đầu. CPU hầu hết thời gian ngồi chờ — chờ người dùng nhập liệu, chờ đọc băng từ, chờ in xong. Tài nguyên đắt tiền bị lãng phí.

Câu hỏi đặt ra: **Tại sao không chạy nhiều chương trình cùng lúc?**

Nhưng ngay lập tức xuất hiện loạt vấn đề nan giải:

- **Bộ nhớ:** Chương trình A và B cùng nạp vào RAM — ai ở đâu? Nếu A ghi nhầm vào vùng nhớ của B thì sao?
- **CPU:** Hai chương trình cùng muốn chạy — ai được chạy trước? Chương trình nào chạy vòng lặp vô tận có thể chiếm CPU mãi mãi.
- **Tài nguyên:** Cả hai cùng muốn ghi vào một file — kết quả sẽ là gì?
- **Lỗi:** Chương trình A crash — B có bị kéo theo không?

Để giải quyết tất cả những vấn đề này, hệ điều hành cần một đơn vị quản lý — một "hộp cát" cô lập cho mỗi chương trình đang chạy, có ranh giới rõ ràng về bộ nhớ, quyền hạn, và thời gian CPU. Đơn vị đó được gọi là **process**.

> Process không phải là chương trình. Process là **môi trường được kiểm soát** mà trong đó chương trình được phép thực thi.

---

### 1.1 Góc nhìn từ embedded developer

Khi lập trình vi điều khiển (MCU), developer quen với mô hình:

- **1 chip = 1 chương trình** chạy hoàn toàn một mình
- Chương trình **toàn quyền** truy cập RAM, ngoại vi, CPU
- Không có khái niệm "chương trình khác đang chạy song song"
- Khi reset → chương trình bắt đầu lại từ đầu

Linux hoàn toàn khác:

- **Hàng chục đến hàng trăm chương trình** chạy đồng thời
- Mỗi chương trình bị **cô lập** — không thể đọc/ghi bộ nhớ của chương trình khác
- Kernel là **trọng tài**: phân phối CPU, RAM, quyền truy cập tài nguyên
- Khi một chương trình crash, các chương trình còn lại **không bị ảnh hưởng**

> **Câu hỏi thảo luận:** Trên STM32, nếu một task FreeRTOS ghi đè bộ nhớ của task khác, điều gì xảy ra? Trên Linux thì sao?

---

### 1.2 Program vs Process

| | Program | Process |
|---|---|---|
| Bản chất | File binary trên disk (ELF) | Thực thể đang chạy trong RAM |
| Tồn tại | Tĩnh, không thay đổi | Động, có vòng đời |
| Số lượng | 1 file | Có thể tạo nhiều process từ 1 file |
| Ví dụ | `/bin/bash` | Mỗi terminal bạn mở là 1 process bash riêng |

Khi kernel **load** một program để chạy, nó tạo ra một process bằng cách:
1. Cấp phát không gian địa chỉ ảo
2. Load code và data từ file ELF vào RAM
3. Tạo kernel stack và user stack
4. Cấp PID và đưa vào hàng đợi scheduler

---

### 1.3 task_struct — Kernel quản lý process như thế nào

Kernel đại diện mỗi process bằng một struct gọi là `task_struct` (định nghĩa trong `include/linux/sched.h`).

Đây là "hồ sơ" của process trong kernel, chứa toàn bộ thông tin cần thiết:

```
task_struct
├── pid          — Process ID
├── ppid         — Parent PID
├── state        — Trạng thái hiện tại (R/S/D/T/Z)
├── mm           — Con trỏ đến memory map (địa chỉ ảo)
├── files        — Danh sách file descriptors đang mở
├── signal       — Thông tin xử lý signal
├── sched_class  — Thuật toán scheduling áp dụng
├── prio         — Priority hiện tại
└── ...          — Hàng trăm trường khác
```

> Không cần thuộc lòng các trường. Quan trọng là hiểu: **mọi thứ kernel cần biết về một process đều nằm trong task_struct**.

---

### 1.4 PID và PPID

**PID (Process ID):**
- Số nguyên dương, duy nhất tại một thời điểm
- Dải từ 1 đến 32768 (mặc định), có thể tăng lên 4194304
- Khi process kết thúc, PID được giải phóng và có thể cấp lại

**Một số PID đặc biệt:**
- PID 1: `init` (hoặc `systemd`) — process đầu tiên sau khi kernel boot
- PID 2: `kthreadd` — quản lý các kernel thread

**PPID (Parent PID):** mọi process đều có một process cha. Điều này tạo ra **cây process**.

```bash
# Quan sát cây process
pstree -p

# Xem PID và PPID của process hiện tại
ps -o pid,ppid,comm
```

---

### 1.5 Process address space

Mỗi process có không gian địa chỉ ảo riêng, chia thành các vùng:

```
Địa chỉ cao
┌─────────────────┐
│   Kernel space  │  ← Chỉ kernel truy cập được
├─────────────────┤ 0xC0000000 (32-bit) / 0xFFFF800000000000 (64-bit)
│      Stack      │  ← Tăng từ cao xuống thấp
│        ↓        │
│   (trống)       │
│        ↑        │
│      Heap       │  ← Tăng từ thấp lên cao (malloc/free)
├─────────────────┤
│  BSS segment    │  ← Biến global chưa khởi tạo (= 0)
├─────────────────┤
│  Data segment   │  ← Biến global đã khởi tạo
├─────────────────┤
│  Text segment   │  ← Code (read-only)
└─────────────────┘
Địa chỉ thấp
```

> **Liên hệ với bài Virtual Memory:** Đây là không gian địa chỉ ảo — kernel + MMU ánh xạ chúng sang địa chỉ vật lý thực sự. Hai process có thể có cùng địa chỉ ảo nhưng trỏ đến vùng RAM vật lý hoàn toàn khác nhau.

**Quan sát thực tế:**
```bash
# Xem memory map của một process
cat /proc/<pid>/maps

# Xem thống kê bộ nhớ
cat /proc/<pid>/status | grep -i vm
```

---

### 1.6 Process Environment

Mỗi process khi được tạo ra không chỉ có code và bộ nhớ — nó còn mang theo một **môi trường (environment)** gồm hai thành phần: command line arguments và environment variables.

#### Command line arguments

Khi kernel khởi tạo process, nó truyền vào hàm `main()` hai tham số:

```c
int main(int argc, char *argv[])
```

- `argc` — số lượng argument (luôn >= 1)
- `argv[0]` — đường dẫn của chính chương trình
- `argv[1]`, `argv[2]`... — các tham số người dùng truyền vào

```c
// Ví dụ: ./myapp -v --output /tmp/log.txt
// argc = 4
// argv[0] = "./myapp"
// argv[1] = "-v"
// argv[2] = "--output"
// argv[3] = "/tmp/log.txt"
```

#### Environment variables

Ngoài arguments, mỗi process còn nhận một danh sách **biến môi trường** dưới dạng các cặp `KEY=VALUE`.

```c
#include <stdlib.h>

// Đọc một biến môi trường
char *path = getenv("PATH");
char *user = getenv("USER");
char *home = getenv("HOME");
```

Toàn bộ danh sách env vars có thể truy cập qua biến global:

```c
extern char **environ;  // Mảng các chuỗi "KEY=VALUE", kết thúc bằng NULL

// In tất cả env vars
for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
}
```

```
# Output ví dụ:
PATH=/usr/local/sbin:/usr/local/bin:/usr/bin
USER=root
HOME=/root
LD_LIBRARY_PATH=/usr/local/lib
```

#### Env vars được kế thừa như thế nào?

Đây là điểm quan trọng: khi `fork()` được gọi, process con **kế thừa toàn bộ env vars của cha**. Đây là cơ chế mà shell truyền môi trường xuống cho các lệnh con.

```
bash (HOME=/root, PATH=...)
  └── fork() → child bash
        └── exec("./myapp") → myapp nhận HOME=/root, PATH=... từ bash
```

#### Các env vars quan trọng trên embedded Linux

| Biến | Ý nghĩa |
|------|---------|
| `PATH` | Danh sách thư mục kernel tìm kiếm binary khi gọi `exec()` |
| `LD_LIBRARY_PATH` | Thư mục tìm kiếm shared library (.so) khi load chương trình |
| `HOME` | Thư mục home của user hiện tại |
| `USER` | Tên user đang chạy process |

> **Liên hệ thực tế với systemd:** Khi deploy service trên embedded Linux, có thể truyền config vào chương trình qua env vars thay vì hardcode — linh hoạt hơn khi thay đổi môi trường dev/production:
>
> ```ini
> [Service]
> Environment="LOG_LEVEL=debug"
> Environment="SENSOR_PORT=/dev/ttyS1"
> ExecStart=/usr/local/bin/myservice
> ```

---

## Phần 2 — Process Lifecycle & States (35 phút)

### 2.1 Năm trạng thái của process

```
                    ┌──────────────────────────────────────┐
                    │                                      │
         fork()     ▼          schedule()                  │
  [NEW] ────────► [RUNNABLE] ─────────────► [RUNNING] ─────┘
                    ▲                          │
                    │      preempt/yield       │
                    └──────────────────────────┘
                                               │
                          sleep/wait/IO        │        exit()
                    ┌─────────────────────────►│◄──────────────► [ZOMBIE]
                    │                          │
               [SLEEPING]                 [STOPPED]
               S: interruptible           (SIGSTOP)
               D: uninterruptible
```

| Ký hiệu | Tên | Ý nghĩa |
|---------|-----|---------|
| `R` | Running / Runnable | Đang chạy trên CPU hoặc đang chờ được scheduler chọn |
| `S` | Sleeping (interruptible) | Chờ sự kiện, có thể bị đánh thức bởi signal |
| `D` | Sleeping (uninterruptible) | Chờ I/O kernel — **không thể bị kill** |
| `T` | Stopped | Bị tạm dừng bởi SIGSTOP hoặc đang bị debugger trace |
| `Z` | Zombie | Đã kết thúc nhưng chưa được cha "thu hồi" |

---

### 2.2 Ý nghĩa từng trạng thái — góc nhìn thực tế

**Trạng thái R (Running/Runnable):**
- Process đang được scheduler sắp xếp, có thể có nhiều process ở trạng thái R nhưng chỉ 1 process chạy trên mỗi CPU core tại một thời điểm
- Nếu thấy nhiều process ở R liên tục → CPU đang bị bão hòa

**Trạng thái S (Sleeping interruptible):**
- Trạng thái bình thường nhất — process đang chờ (chờ keyboard, chờ network, chờ timer)
- Có thể nhận signal và bị đánh thức sớm
- Ví dụ: `read()` chờ dữ liệu từ serial port

**Trạng thái D (Sleeping uninterruptible):**
- Process đang ở sâu trong kernel, chờ I/O hoàn thành (thường là disk I/O hoặc NFS)
- **Không thể bị kill** — kể cả `kill -9`
- Nếu thấy process kẹt ở D lâu → vấn đề hardware hoặc driver
- Trên embedded: thường gặp khi driver I2C/SPI bị treo

**Trạng thái T (Stopped):**
- Nhận `SIGSTOP` (hoặc Ctrl+Z trong terminal) → process bị "đóng băng"
- `SIGCONT` để tiếp tục
- GDB attach vào process cũng đưa process vào trạng thái T tạm thời

**Trạng thái Z (Zombie):**
- Process đã chạy xong `exit()`, nhưng entry trong kernel vẫn còn
- Lý do: cha chưa gọi `wait()` để đọc exit status
- Zombie **không dùng CPU hay RAM** đáng kể, nhưng chiếm một PID slot
- Nếu zombie tích lũy nhiều → hết PID → không thể tạo process mới

---

### 2.3 Zombie và Orphan — pitfall trên embedded

**Zombie process:**

```c
// Ví dụ tạo zombie cố ý
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Process con: kết thúc ngay
        printf("Con (PID %d) kết thúc\n", getpid());
        exit(0);
    }
    // Process cha: không gọi wait(), ngủ 30 giây
    printf("Cha (PID %d) đang ngủ, con đã thành zombie\n", getpid());
    sleep(30);
    return 0;
}
```

```
# Output khi chạy và quan sát:
$ ./zombie_demo &
Cha (PID 1234) đang ngủ, con đã thành zombie
Con (PID 1235) kết thúc

$ ps aux | grep Z
user  1235  0.0  0.0  0  0  pts/0  Z+  10:00  0:00 [zombie_demo] <defunct>
```

**Orphan process:**
- Khi process cha kết thúc trước con, con trở thành "mồ côi"
- Kernel tự động **re-parent**: gán process mồ côi về dưới PID 1 (systemd/init)
- Orphan không nguy hiểm — systemd sẽ gọi `wait()` khi chúng kết thúc

> **Câu hỏi thảo luận:** Trên hệ thống embedded chạy 24/7, một daemon bị memory leak nhẹ tạo zombie mỗi giờ. Sau bao lâu hệ thống bị ảnh hưởng? Làm sao phát hiện sớm?

---

### 2.4 Công cụ quan sát trạng thái

```bash
# Xem trạng thái tất cả process
ps aux
# Cột STAT: R, S, D, T, Z
# Ký tự phụ: + (foreground), s (session leader), l (multi-thread)

# Xem realtime, nhấn f để chọn cột
top
htop  # Thân thiện hơn, màu sắc

# Xem chi tiết một process cụ thể
cat /proc/<pid>/status

# Đếm số zombie đang tồn tại
ps aux | awk '{print $8}' | grep -c Z
```

---

## Phần 3 — Sinh ra và kết thúc process (30 phút)

### 3.1 fork() — nhân đôi process

`fork()` tạo ra một bản sao của process hiện tại.

**Concept:**
- Sau `fork()`, có **hai process độc lập** chạy song song
- Process con là bản sao hoàn chỉnh của cha: code, data, heap, stack, file descriptors
- Điểm khác biệt duy nhất: giá trị trả về của `fork()`
  - Trả về **PID của con** cho process **cha**
  - Trả về **0** cho process **con**
  - Trả về **-1** nếu thất bại (không đủ tài nguyên)

**Copy-on-Write (COW) — tại sao fork() nhanh:**

Thay vì copy toàn bộ RAM ngay lập tức, kernel dùng kỹ thuật Copy-on-Write:
- Ban đầu, cha và con **chia sẻ cùng trang vật lý** (chỉ đọc)
- Chỉ khi một trong hai **ghi** vào một trang → kernel mới tạo bản copy riêng
- Kết quả: `fork()` một process 100MB xong gần như ngay lập tức

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 10;
    pid_t pid = fork();

    if (pid > 0) {
        x = 20;  // Cha thay đổi x → kernel tạo copy riêng
        printf("Cha: x = %d, PID = %d\n", x, getpid());
    } else if (pid == 0) {
        printf("Con: x = %d, PID = %d\n", x, getpid());
        // Con thấy x = 10, không bị ảnh hưởng bởi cha
    }
    return 0;
}
```

```
# Output:
Cha: x = 20, PID = 1234
Con: x = 10, PID = 1235
```

---

### 3.2 exec() — thay thế image

`exec()` **thay thế** toàn bộ nội dung của process hiện tại bằng một chương trình mới.

**Concept:**
- PID **không đổi** — vẫn là cùng một process
- Code, data, heap, stack cũ bị xóa hoàn toàn
- Chương trình mới được load vào không gian địa chỉ cũ
- Nếu `exec()` thành công, code phía sau nó **không bao giờ được thực thi**

**Tại sao cần exec() khi đã có fork()?**

`fork()` tạo bản sao của process cha — con sẽ chạy cùng code với cha. Để con chạy một **chương trình khác hoàn toàn**, cần `exec()`.

**Luồng fork → exec → wait (cách shell chạy lệnh):**

```
Shell (bash)
    │
    ├── fork() ──────────────────► Child process (copy của bash)
    │                                     │
    │                              exec("/bin/ls") ← thay thế bằng ls
    │                                     │
    │   wait()                      ls chạy...
    │    │                                │
    │    └────────────────────────────────┘
    │         (nhận exit status)
    │
    └── (tiếp tục nhận lệnh tiếp theo)
```

---

### 3.3 wait() — thu hồi process con

`wait()` block process cha cho đến khi một process con kết thúc.

**Tại sao bắt buộc phải gọi wait():**
- Khi con kết thúc, kernel giữ lại một phần thông tin (exit status) cho cha đọc
- Nếu cha không gọi `wait()` → thông tin này không bao giờ được giải phóng → **zombie**
- `wait()` chính là hành động "cha xác nhận con đã kết thúc" → kernel mới thực sự giải phóng

**Exit codes:**
- Con gọi `exit(0)` → thành công
- Con gọi `exit(1)` hoặc `exit(-1)` → thất bại
- Cha dùng macro `WEXITSTATUS(status)` để đọc giá trị trả về

```bash
# Trong shell, exit code của lệnh vừa chạy:
ls /etc
echo $?   # 0 = thành công

ls /không_tồn_tại
echo $?   # 2 = lỗi
```

---

## Phần 4 — Scheduling & Priority (40 phút)

### 4.1 Tại sao cần scheduler?

**Vấn đề:** Hệ thống có thể có 200 process ở trạng thái Runnable, nhưng chỉ có 4 CPU core. Ai chạy trước?

**Vai trò của scheduler:**
- Quyết định process nào được chạy trên CPU nào
- Quyết định chạy bao lâu (time slice)
- Đảm bảo không có process nào bị "chết đói" (starvation)
- Đáp ứng yêu cầu real-time nếu có

> **Liên hệ với embedded:** FreeRTOS có scheduler dựa trên priority — task priority cao hơn luôn chạy trước. Linux phức tạp hơn nhiều: có nhiều scheduling policy, priority không hoàn toàn quyết định thứ tự.

---

### 4.2 CFS — Completely Fair Scheduler

CFS là scheduler mặc định của Linux cho các process thông thường (`SCHED_OTHER`).

**Triết lý của CFS:** "Mỗi process xứng đáng nhận được một phần CPU công bằng."

**Intuition — không cần hiểu red-black tree:**

Hãy tưởng tượng CFS giữ một "đồng hồ ảo" (`vruntime`) cho mỗi process:
- Mỗi khi process chạy trên CPU, vruntime của nó tăng lên
- CFS luôn chọn process có **vruntime thấp nhất** để chạy tiếp
- Process nào chưa được chạy lâu → vruntime thấp → được ưu tiên
- Kết quả: tất cả process được phục vụ công bằng theo thời gian

**Ảnh hưởng của nice value lên CFS:**
- `nice` không thay đổi thứ tự, mà thay đổi **tốc độ tăng vruntime**
- nice = -20 (ưu tiên cao): vruntime tăng chậm → được chọn thường xuyên hơn
- nice = +19 (ưu tiên thấp): vruntime tăng nhanh → ít được chọn hơn

---

### 4.3 nice value

`nice` value là cơ chế điều chỉnh độ ưu tiên của process thông thường.

- Dải giá trị: **-20** (ưu tiên cao nhất) đến **+19** (ưu tiên thấp nhất)
- Mặc định: **0**
- Chỉ **root** mới có thể set nice âm (tăng ưu tiên)
- User thường chỉ có thể tăng nice (giảm ưu tiên)

```bash
# Chạy lệnh với nice value cụ thể
nice -n 10 ./my_program        # nice = +10 (ưu tiên thấp hơn bình thường)
nice -n -5 ./my_program        # nice = -5 (cần root)

# Thay đổi nice của process đang chạy
renice -n 5 -p <pid>           # Đặt nice = 5
renice -n 5 -u username        # Tất cả process của user đó

# Xem nice value (cột NI trong top/ps)
ps -o pid,ni,comm -p <pid>
top  # Nhấn 'r' để renice interactively
```

**Use case trên embedded:**
- Chạy `gzip` compress log file với nice +15 → không ảnh hưởng đến ứng dụng chính
- Backup data đêm khuya với nice +19

---

### 4.4 Real-time scheduling

Khi `nice` và CFS không đủ — khi cần **đảm bảo thời gian phản hồi** — dùng real-time scheduling.

**Ba scheduling policy chính:**

| Policy | Đặc điểm | Use case |
|--------|----------|----------|
| `SCHED_OTHER` | CFS mặc định, dùng nice | Ứng dụng thông thường |
| `SCHED_FIFO` | RT priority, chạy đến khi tự yield hoặc bị RT cao hơn preempt | Driver, control loop cứng |
| `SCHED_RR` | RT priority + time slice (Round Robin) | RT nhưng cần chia sẻ CPU |

**RT Priority:** 1 (thấp nhất) đến 99 (cao nhất)
- Process `SCHED_FIFO` priority 50 sẽ **preempt** mọi `SCHED_OTHER` process
- Process `SCHED_FIFO` priority 80 sẽ preempt `SCHED_FIFO` priority 50

```bash
# Xem scheduling policy của process
chrt -p <pid>

# Chạy với SCHED_FIFO priority 50
chrt -f 50 ./my_realtime_program

# Chạy với SCHED_RR priority 30
chrt -r 30 ./my_program

# Thay đổi policy của process đang chạy
chrt -f -p 50 <pid>
```

---

### 4.5 Pitfall trên embedded: những cạm bẫy với RT scheduling

**Pitfall 1: RT process chiếm 100% CPU**

`SCHED_FIFO` process không bị preempt bởi CFS. Nếu nó rơi vào vòng lặp vô tận (bug), toàn bộ CPU bị chiếm — kernel không thể can thiệp, terminal không phản hồi, watchdog có thể reset board.

```bash
# Phòng ngừa: giới hạn CPU time cho RT processes
# /proc/sys/kernel/sched_rt_runtime_us (mặc định: 950000 = 95% CPU)
# RT processes không được dùng quá 95% CPU trong mỗi giây
echo 800000 > /proc/sys/kernel/sched_rt_runtime_us
```

**Pitfall 2: Priority Inversion**

```
Tình huống:
- Task A: SCHED_FIFO priority 80, đang chờ mutex M
- Task B: SCHED_OTHER, đang giữ mutex M
- Task C: SCHED_FIFO priority 60, đang chạy

Kết quả: Task A (priority 80) bị block bởi Task B (priority thấp)
vì Task C (priority 60) không nhường CPU cho Task B
→ Task A bị "inversion" — thực tế chạy sau Task C dù priority cao hơn
```

Giải pháp: **Priority Inheritance** — khi B giữ mutex mà A đang chờ, B tạm thời được nâng priority lên bằng A. Linux hỗ trợ qua `pthread_mutexattr_setprotocol(PTHREAD_PRIO_INHERIT)`.

**Pitfall 3: Watchdog bị starved**

Nếu watchdog daemon chạy với `SCHED_OTHER` mà hệ thống có nhiều RT process, watchdog có thể không được CPU → không feed watchdog → board reset.

Giải pháp: chạy watchdog với RT priority thấp (ví dụ `SCHED_RR` priority 10) để đảm bảo nó luôn được chạy.

---

## Phần 5 — Lab (35 phút)

### Lab 1: Quan sát process states và tạo zombie

**Mục tiêu:** Nhìn thấy tận mắt trạng thái Z trong `ps`, hiểu /proc filesystem.

**Bước 1:** Compile và chạy chương trình tạo zombie

```c
// zombie.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("[Con] PID = %d, kết thúc ngay\n", getpid());
        exit(42);
    }
    printf("[Cha] PID = %d, con là %d. Không gọi wait(). Ngủ 60 giây...\n",
           getpid(), pid);
    sleep(60);
    return 0;
}
```

```bash
gcc zombie.c -o zombie
./zombie &
```

**Bước 2:** Quan sát zombie

```bash
# Thấy trạng thái Z trong cột STAT
ps aux | grep zombie

# Đọc trực tiếp từ /proc
cat /proc/<pid_con>/status | grep -E "Name|Pid|PPid|State"
```

**Bước 3:** Thêm wait() và so sánh

```c
// Trong process cha, thêm:
int status;
waitpid(pid, &status, 0);
printf("[Cha] Con kết thúc với exit code: %d\n", WEXITSTATUS(status));
```

**Câu hỏi kiểm tra:** Sau khi cha gọi `wait()`, còn thấy zombie trong `ps` không?

---

### Lab 2: GDB attach vào running process

**Mục tiêu:** Debug một process đang chạy mà không cần restart.

**Bước 1:** Viết chương trình target (chạy mãi mãi)

```c
// target.c
#include <stdio.h>
#include <unistd.h>

int counter = 0;

void worker() {
    while (1) {
        counter++;
        sleep(1);
    }
}

int main() {
    printf("PID: %d\n", getpid());
    worker();
    return 0;
}
```

```bash
gcc -g target.c -o target   # -g để có debug symbols
./target &
```

**Bước 2:** Attach GDB vào process đang chạy

```bash
# Cần quyền root hoặc điều chỉnh ptrace scope
sudo gdb -p <pid>

# Hoặc:
gdb ./target <pid>
```

**Bước 3:** Các thao tác trong GDB

```
(gdb) info threads          # Xem các thread
(gdb) bt                    # Backtrace — đang ở hàm nào?
(gdb) frame 0               # Vào stack frame 0
(gdb) print counter         # Đọc giá trị biến
(gdb) break worker          # Đặt breakpoint
(gdb) continue              # Tiếp tục chạy
(gdb) next                  # Chạy từng dòng
(gdb) set var counter = 0   # Thay đổi biến trực tiếp!
(gdb) detach                # Tách ra, process tiếp tục chạy
(gdb) quit
```

**Lưu ý trên embedded (BeagleBone Black):**
```bash
# Nếu gặp lỗi "ptrace: Operation not permitted"
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```

---

### Lab 3: Cấu hình process tự khởi động với systemd

**Mục tiêu:** Biến một chương trình C thành system service, tự động start khi boot.

**Bước 1:** Chuẩn bị chương trình

```c
// myservice.c
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

int main() {
    openlog("myservice", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "MyService đã khởi động");

    while (1) {
        syslog(LOG_INFO, "MyService đang chạy...");
        sleep(10);
    }

    closelog();
    return 0;
}
```

```bash
gcc myservice.c -o myservice
sudo cp myservice /usr/local/bin/
sudo chmod +x /usr/local/bin/myservice
```

**Bước 2:** Tạo systemd service file

```ini
# /etc/systemd/system/myservice.service
[Unit]
Description=My Demo Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/myservice
Restart=on-failure
RestartSec=5s
User=root

[Install]
WantedBy=multi-user.target
```

```bash
sudo nano /etc/systemd/system/myservice.service
```

**Bước 3:** Kích hoạt và kiểm tra

```bash
# Reload để systemd nhận service mới
sudo systemctl daemon-reload

# Bật service
sudo systemctl start myservice

# Xem trạng thái
sudo systemctl status myservice

# Cấu hình tự start khi boot
sudo systemctl enable myservice

# Xem log
sudo journalctl -u myservice -f

# Dừng và tắt autostart
sudo systemctl stop myservice
sudo systemctl disable myservice
```

**Câu hỏi kiểm tra:** Reboot board và kiểm tra xem service có tự chạy không. PID lần này có giống lần trước không? Tại sao?

---

## Tóm tắt

| Chủ đề | Điểm cốt lõi |
|--------|-------------|
| Process vs Program | Program là file tĩnh; Process là thực thể sống có PID, state, tài nguyên riêng |
| task_struct | Kernel quản lý mọi thông tin về process qua struct này |
| Process states | R/S/D/T/Z — D không thể kill, Z là zombie cần cha thu hồi |
| fork/exec/wait | fork() nhân đôi (COW), exec() thay thế image, wait() thu hồi zombie |
| CFS | Scheduler công bằng dựa trên vruntime, nice ảnh hưởng tốc độ tăng vruntime |
| RT Scheduling | SCHED_FIFO/RR cho real-time — cẩn thận priority inversion và CPU starvation |
| systemd | Công cụ chuẩn để quản lý service trên embedded Linux hiện đại |
