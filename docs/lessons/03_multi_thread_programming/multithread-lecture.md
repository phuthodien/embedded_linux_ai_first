# Lập trình Multithread trên Linux
> **FPT Software · Corporate Training Center**
> *AI-First: Tập trung hiểu bản chất — AI hỗ trợ coding — Developer review & verify*

---

# PART 1 — Thread Basics & Linux Internals

## 1.1 Thread là gì & sinh ra để giải quyết vấn đề gì?

### Định nghĩa

**Thread là một luồng thực thi trong chương trình.** Mỗi thread biết hai thứ: *đang chạy đến dòng nào* (program counter) và *đang giữ những biến cục bộ nào* (stack riêng).

Một chương trình có thể có nhiều thread cùng tồn tại — chúng chia sẻ chung vùng nhớ (heap, global variables) nhưng mỗi thread có stack và luồng thực thi độc lập.

### Tại sao cần thread?

CPU không phải lúc nào cũng có việc để làm. Đôi khi nó phải **chờ**: chờ đọc file, chờ nhận data từ mạng, chờ sensor trả về giá trị. Trong lúc đó, nếu chương trình chỉ có 1 luồng thực thi — toàn bộ chương trình **đứng hình**.

```
Không có thread:

main ──── xử lý A ──── chờ I/O ░░░░░░░░░░ ──── xử lý B ──►
                                CPU trống!

Có thread:

Thread 1 ──── xử lý A ──── chờ I/O ░░░░░░░░░░ ──────────►
Thread 2 ──────────────────────── xử lý B ───────────────►
                                CPU luôn có việc!
```

Thread giải quyết 2 bài toán:

| Bài toán | Ý nghĩa |
|---|---|
| **Concurrency** | Khi thread A bị block, thread B vẫn chạy — chương trình không bị đứng |
| **Parallelism** | Trên multi-core, nhiều thread chạy thực sự đồng thời — tăng tốc độ xử lý |

### Mỗi thread có riêng

| Riêng mỗi thread | Lý do |
|---|---|
| Stack | Mỗi thread có call stack riêng |
| Program Counter | Đang chạy đến dòng nào |
| Register set | CPU context khi bị preempt |
| Thread ID (`tid`) | Định danh trong kernel |

---

## 1.2 Thread Lifecycle & Scheduler (CFS)

### Các trạng thái của thread

Tại bất kỳ thời điểm nào, mỗi thread đang ở một trong hai trạng thái:

```
  pthread_create()
        │
        ▼
   RUNNING ◄─────────────────────┐
  (đang được CPU thực thi)       │
        │                        │
   chờ I/O,                 I/O xong,
   chờ dữ liệu...           dữ liệu sẵn sàng...
        │                        │
        ▼                        │
  SLEEPING  ─────────────────────┘
  (nhường CPU, chờ được đánh thức)
```

Khi thread cần chờ một sự kiện bên ngoài (đọc file, nhận data từ mạng...), nó **tự nguyện nhường CPU** — đi vào trạng thái sleep. Khi sự kiện xảy ra, OS đánh thức thread và đưa nó trở lại hàng chờ để chạy tiếp.

### CFS — Completely Fair Scheduler

OS cần quyết định: khi có nhiều thread đang ở trạng thái RUNNING, **thread nào được chạy tiếp?**

Linux dùng **CFS (Completely Fair Scheduler)** với nguyên tắc đơn giản: theo dõi tổng thời gian CPU mỗi thread đã dùng (`vruntime`). **Thread nào dùng ít CPU hơn thì được ưu tiên chạy tiếp.**

```
Thread A: vruntime = 10ms  ← được chọn chạy tiếp
Thread B: vruntime = 25ms
Thread C: vruntime = 18ms
```

Sau khi Thread A chạy thêm một lúc, `vruntime` của nó tăng lên. OS lại chọn thread có `vruntime` thấp nhất tiếp theo — cứ thế xoay vòng.

```
Kết quả: mỗi thread đều nhận được phần CPU tương đối công bằng
         không thread nào bị "chết đói" (starvation)
```

### Time slice — Mỗi lần chạy bao lâu?

Mỗi lần được chọn, thread không chạy mãi mãi. OS cho thread chạy một khoảng thời gian ngắn (**time slice**, thường vài milliseconds), sau đó **preempt** — tạm dừng và cho thread khác chạy.

```
Timeline:
──[Thread A]──[Thread B]──[Thread C]──[Thread A]──[Thread B]──►
  time slice   time slice  time slice
```

Đây là lý do các thread trông như chạy "đồng thời" dù chỉ có 1 core CPU — thực ra chúng đang **luân phiên rất nhanh**.

---

## 1.3 pthreads API — Cơ chế, không phải thuộc lòng

### Tạo thread

```c
#include <pthread.h>

void *worker(void *arg) {
    int n = *(int *)arg;
    printf("Thread nhận: %d\n", n);
    return NULL;
}

int main() {
    pthread_t tid;
    int value = 42;

    pthread_create(&tid, NULL, worker, &value);
    pthread_join(tid, NULL);  // chờ thread kết thúc
    return 0;
}
```

Compile: `gcc main.c -lpthread -o main`

### join vs detach

Mỗi thread sau khi chạy xong sẽ giữ lại một ít tài nguyên (như zombie process) cho đến khi có người "dọn". Có 2 cách dọn:

```c
// Cách 1: join — main chờ thread xong, lấy kết quả
pthread_join(tid, &retval);

// Cách 2: detach — kernel tự dọn khi thread xong, không cần chờ
pthread_detach(tid);
// hoặc set attribute trước khi create:
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
```

> ⚠️ **Gotcha:** Không join **và** không detach = resource leak. Một trong hai bắt buộc phải gọi.

### Truyền nhiều argument vào thread

pthreads chỉ cho truyền **1 pointer**. Khi cần nhiều argument, dùng struct:

```c
typedef struct {
    int   start;
    int   end;
    long  result;
} ThreadArg;

void *count_odd(void *arg) {
    ThreadArg *a = (ThreadArg *)arg;
    a->result = 0;
    for (int i = a->start; i <= a->end; i++)
        if (i % 2 != 0) a->result++;
    return NULL;
}
```

---

# PART 2 — Synchronization

## 2.1 Race Condition — Bản chất thực sự

### Ví dụ

Chương trình tạo 2 thread, cả hai cùng tăng một biến đếm chung:

```c
#include <pthread.h>
#include <stdio.h>

int counter = 0;

void *increment(void *arg) {
    for (int i = 0; i < 1000000; i++)
        counter++;
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("counter = %d\n", counter);
}
```

Chạy thử:
```
Kỳ vọng: counter = 2000000
Thực tế:  counter = 1347823  ← mỗi lần chạy ra số khác nhau!
```

### Tại sao sai?

`counter++` trông như 1 dòng, nhưng CPU thực hiện **3 bước riêng biệt**:

```
LOAD    đọc giá trị counter từ RAM vào register
ADD     cộng 1 trong register
STORE   ghi register trở lại RAM
```

Nếu 2 thread bị OS preempt giữa chừng:

```
         Thread 1                    Thread 2
         ────────                    ────────

 ┌─►  LOAD  reg1 = counter (= 0)
 │
 │    ◄── bị preempt, OS chuyển sang Thread 2
 │
 │                               LOAD  reg2 = counter (= 0)
 │                               ADD   reg2 = 1
 │                               STORE counter = 1
 │
 │    ◄── OS chuyển lại Thread 1
 │
 └─   ADD   reg1 = 1
      STORE counter = 1          ← GHI ĐÈ kết quả của Thread 2!

Kỳ vọng: counter = 2
Thực tế:  counter = 1  → mất 1 lần tăng
```

Đây là **race condition** — kết quả phụ thuộc vào việc OS preempt thread tại thời điểm nào, hoàn toàn không kiểm soát được. Lỗi này đặc biệt nguy hiểm vì **không tái hiện được một cách nhất quán** — đôi khi đúng, đôi khi sai.

---

## 2.2 Mutex — Từ futex đến pthread_mutex

### Bản chất: futex

`pthread_mutex_lock()` không phải syscall thuần túy. Nó dùng **futex** (Fast Userspace Mutex) — cơ chế lai rất thông minh:

```
Lần 1: mutex đang free
  → atomic operation trong userspace → lấy lock ngay
  → KHÔNG cần vào kernel, KHÔNG tốn syscall

Lần 2: mutex đang bị giữ bởi thread khác
  → gọi syscall futex(FUTEX_WAIT) → thread đi vào sleep
  → khi unlock: syscall futex(FUTEX_WAKE) → đánh thức thread chờ
```

**Ý nghĩa thực tế:** Mutex uncontended (không có tranh chấp) gần như miễn phí về hiệu năng.

### Sử dụng mutex

```c
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        counter++;              // chỉ 1 thread vào được đây tại một thời điểm
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
```

### Wrapper pattern — Cách dùng đúng trong thực tế

Thay vì để lock rải rác khắp code, gói data + lock vào một module:

**counter.h**
```c
#ifndef COUNTER_H
#define COUNTER_H

typedef struct Counter Counter;

Counter *counter_create();
void     counter_inc(Counter *c);
int      counter_get(Counter *c);
void     counter_destroy(Counter *c);

#endif
```

**counter.c**
```c
#include <pthread.h>
#include <stdlib.h>
#include "counter.h"

struct Counter {
    pthread_mutex_t lock;
    int value;
};

Counter *counter_create() {
    Counter *c = malloc(sizeof(Counter));
    pthread_mutex_init(&c->lock, NULL);
    c->value = 0;
    return c;
}

void counter_inc(Counter *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

int counter_get(Counter *c) {
    pthread_mutex_lock(&c->lock);
    int val = c->value;
    pthread_mutex_unlock(&c->lock);
    return val;
}

void counter_destroy(Counter *c) {
    pthread_mutex_destroy(&c->lock);
    free(c);
}
```

**main.c**
```c
#include <pthread.h>
#include <stdio.h>
#include "counter.h"

Counter *counter;

void *worker(void *arg) {
    for (int i = 0; i < 1000000; i++)
        counter_inc(counter);
    return NULL;
}

int main() {
    counter = counter_create();

    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter = %d\n", counter_get(counter)); // luôn = 2000000
    counter_destroy(counter);
    return 0;
}
```

Compile: `gcc main.c counter.c -lpthread -o main`

> 🔑 **Nguyên tắc:** Lock là bảo vệ **data**, không phải bảo vệ **code**. Đặt lock càng gần data càng tốt.

---

## 2.3 Deadlock — Phát hiện & Tránh

### Deadlock là gì?

Hai thread chờ nhau mãi mãi, không ai tiến triển được.

```
Thread A giữ lock_1, chờ lock_2
Thread B giữ lock_2, chờ lock_1
→ Cả hai đứng yên mãi mãi
```

### Ví dụ deadlock đơn giản

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

void *thread_1(void *arg) {
    pthread_mutex_lock(&lock_a);
    printf("Thread 1 lấy được lock_a, đang chờ lock_b...\n");
    sleep(1);                          // nhường thời gian để thread 2 kịp lock_b
    pthread_mutex_lock(&lock_b);       // ← chờ mãi mãi
    printf("Thread 1 done\n");         // không bao giờ in được dòng này
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
    return NULL;
}

void *thread_2(void *arg) {
    pthread_mutex_lock(&lock_b);
    printf("Thread 2 lấy được lock_b, đang chờ lock_a...\n");
    sleep(1);
    pthread_mutex_lock(&lock_a);       // ← chờ mãi mãi
    printf("Thread 2 done\n");         // không bao giờ in được dòng này
    pthread_mutex_unlock(&lock_a);
    pthread_mutex_unlock(&lock_b);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_create(&t2, NULL, thread_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);            // main treo tại đây mãi mãi
    return 0;
}
```

Output khi chạy:
```
Thread 1 lấy được lock_a, đang chờ lock_b...
Thread 2 lấy được lock_b, đang chờ lock_a...
(treo mãi mãi)
```

Hình vẽ trạng thái deadlock:
```
Thread 1  ──giữ──►  lock_a  ◄──chờ──  Thread 2
Thread 1  ──chờ──►  lock_b  ◄──giữ──  Thread 2

→ Vòng tròn khép kín, không ai nhường ai
```

### Điều kiện để xảy ra deadlock (Coffman Conditions)

Deadlock xảy ra khi có **đủ cả 4**:
1. Mỗi resource chỉ 1 thread dùng được tại 1 thời điểm
2. Thread giữ resource A trong khi chờ resource B
3. Không ai có thể lấy resource khỏi thread đang giữ
4. Có vòng tròn chờ đợi (A chờ B, B chờ A)

Phá **bất kỳ 1** trong 4 → deadlock không xảy ra.

### Cách phòng tránh: Lock Ordering

Quy tắc đơn giản nhất: **luôn lock theo thứ tự cố định** trong toàn bộ codebase.

```c
// ✅ ĐÚNG — cả 2 thread đều lock theo thứ tự: lock_1 trước, lock_2 sau
pthread_mutex_lock(&lock_1);
pthread_mutex_lock(&lock_2);
// ... critical section ...
pthread_mutex_unlock(&lock_2);
pthread_mutex_unlock(&lock_1);

// ❌ SAI — thread này lock ngược lại → circular wait
pthread_mutex_lock(&lock_2);
pthread_mutex_lock(&lock_1);
```

### Phát hiện bằng GDB

Khi chương trình bị treo do deadlock, attach GDB vào và in stack trace của tất cả các thread:

```bash
# Terminal 1: chạy chương trình
./deadlock_program

# Terminal 2: attach GDB vào process đang treo
gdb -p $(pgrep deadlock_program)
```

Trong GDB:
```
(gdb) thread apply all bt
```

Output:
```
Thread 1 (Thread 0x7f... (LWP 12345)):
#0  __lll_lock_wait () at lowlevellock.c:46
#1  pthread_mutex_lock () at pthread_mutex_lock.c:80
#2  thread_1 () at main.c:12        ← đang chờ lock_b
#3  start_thread () at pthread_create.c:477

Thread 2 (Thread 0x7f... (LWP 12346)):
#0  __lll_lock_wait () at lowlevellock.c:46
#1  pthread_mutex_lock () at pthread_mutex_lock.c:80
#2  thread_2 () at main.c:24        ← đang chờ lock_a
#3  start_thread () at pthread_create.c:477
```

Đọc output:
```
Cả 2 thread đều đang kẹt tại pthread_mutex_lock()
Thread 1 kẹt ở main.c:12  → nhìn vào code: đang chờ lock_b
Thread 2 kẹt ở main.c:24  → nhìn vào code: đang chờ lock_a
→ Xác nhận circular wait
```

> 💡 **Workflow thực tế:** Copy toàn bộ output `thread apply all bt` → paste vào AI → yêu cầu phân tích xem thread nào đang chờ thread nào.

### Phát hiện bằng Helgrind

```bash
valgrind --tool=helgrind ./program
# Output sẽ báo: "lock order violated"
```

---

## 2.4 Semaphore — Khi nào thực sự dùng?

### Mutex vs Semaphore

**Mutex** là ổ khóa **1 chìa** — tại một thời điểm chỉ có đúng 1 thread được vào.

**Semaphore** là ổ khóa **N chìa** — tại một thời điểm có thể có tối đa N thread được vào cùng lúc.

```
Mutex (1 chìa):                Semaphore (3 chìa):

  ┌───────────┐                  ┌───────────┐
  │  critical │                  │  critical │
  │  section  │                  │  section  │
  └───────────┘                  └───────────┘
       🔑                          🔑 🔑 🔑
  chỉ 1 thread                  tối đa 3 thread
  vào được                      vào cùng lúc
```

Dùng **Mutex** khi: chỉ 1 thread được phép đụng vào data tại một thời điểm.

Dùng **Semaphore** khi: muốn giới hạn số thread truy cập đồng thời — ví dụ tối đa 3 thread được đọc file cùng lúc, hay tối đa 5 kết nối database.

### Ví dụ: Producer-Consumer

```c
#include <semaphore.h>

#define BUF_SIZE 5
int buffer[BUF_SIZE];
int in = 0, out = 0;

sem_t empty;  // số slot trống
sem_t filled; // số slot có data
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    int item = 0;
    while (1) {
        sem_wait(&empty);              // chờ có slot trống
        pthread_mutex_lock(&lock);
        buffer[in] = item++;
        in = (in + 1) % BUF_SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&filled);             // báo có data mới
    }
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&filled);             // chờ có data
        pthread_mutex_lock(&lock);
        int item = buffer[out];
        out = (out + 1) % BUF_SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&empty);              // báo có slot trống
        printf("Consumed: %d\n", item);
    }
}

// Khởi tạo:
// sem_init(&empty, 0, BUF_SIZE);
// sem_init(&filled, 0, 0);
```

---

## 2.5 Condition Variable — Spurious Wakeup

### Vấn đề

Sau khi `mutex_lock()` thành công, thread đang giữ lock. Nếu lúc này điều kiện chưa thỏa mãn (queue rỗng chẳng hạn), thread cần chờ. Nhưng nếu nó chờ mà **vẫn giữ lock** → thread khác không thể vào để tạo data → **deadlock**.

```
Thread Consumer                    Thread Producer
──────────────                     ───────────────
mutex_lock()      ← giữ lock
queue_empty() → true
chờ...            ← vẫn giữ lock!

                                   mutex_lock()  ← bị block mãi mãi
                                   queue_push()  ← không bao giờ tới được
```

`pthread_cond_wait()` giải quyết đúng vấn đề này bằng cách làm **2 việc một lúc, nguyên tử**:
- **Unlock mutex** — nhường lock cho thread khác
- **Sleep** — chờ được signal

Khi được đánh thức, nó lại làm 2 việc nguyên tử:
- **Lock mutex lại**
- **Return** — thread tiếp tục chạy

```
Thread Consumer                    Thread Producer
──────────────                     ───────────────
mutex_lock()
queue_empty() → true
cond_wait()
  → unlock mutex  ← nhả lock!
  → sleep...
                                   mutex_lock()  ← lấy được lock
                                   queue_push()
                                   cond_signal()
                                   mutex_unlock()

  → wake up
  → lock mutex lại
queue_pop()       ← an toàn
mutex_unlock()
```

### Tại sao phải dùng `while`, không phải `if`?

**Spurious wakeup:** `pthread_cond_wait()` đôi khi tự return mà **không có ai signal** — đây là hành vi được POSIX chấp nhận (do đặc thù của futex trong kernel). Nếu dùng `if`, thread tiếp tục chạy khi điều kiện vẫn còn false.

```c
// ❌ SAI — dùng if, dễ bị spurious wakeup:
if (queue_empty())
    pthread_cond_wait(&cond, &mutex);
process();  // có thể chạy khi queue vẫn empty!

// ✅ ĐÚNG — dùng while, kiểm tra lại sau mỗi lần wake:
while (queue_empty())
    pthread_cond_wait(&cond, &mutex);
process();  // đảm bảo queue không empty
```

### signal vs broadcast

```c
pthread_cond_signal(&cond);     // đánh thức 1 thread đang chờ
pthread_cond_broadcast(&cond);  // đánh thức TẤT CẢ thread đang chờ
```

Dùng `broadcast` khi thay đổi state có thể thỏa mãn nhiều thread cùng lúc.


---

## 🐛 Bug #2 — Spurious Wakeup

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int data_ready = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void *consumer(void *arg) {
    pthread_mutex_lock(&mutex);

    if (!data_ready)                          // BUG: dùng if thay vì while
        pthread_cond_wait(&cond, &mutex);

    printf("Processing data...\n");           // có thể chạy khi data_ready = 0!
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *producer(void *arg) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    data_ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t tc, tp;
    pthread_create(&tc, NULL, consumer, NULL);
    pthread_create(&tp, NULL, producer, NULL);
    pthread_join(tc, NULL);
    pthread_join(tp, NULL);
    return 0;
}
```

**Nhiệm vụ:**
1. Giải thích spurious wakeup là gì
2. Tại sao `pthread_cond_wait()` cần nhận `mutex` làm argument?
3. Dùng AI fix: đổi `if` → `while`

---

# PART 3 — Tools & Design

## 3.1 Thread Sanitizer & Debugging Workflow

### Thread Sanitizer là gì?

TSan là một công cụ được tích hợp sẵn trong GCC và Clang. Khi compile với flag `-fsanitize=thread`, TSan **chèn thêm code vào chương trình** tại mỗi điểm đọc/ghi memory. Code được chèn thêm này theo dõi mọi thao tác truy cập memory của tất cả các thread trong suốt quá trình chạy.

### TSan detect race condition thế nào?

TSan dùng thuật toán **happens-before** để phát hiện race condition. Ý tưởng đơn giản:

> Hai thao tác trên cùng một vùng nhớ là race condition nếu:
> - Ít nhất một trong hai là **ghi**
> - Không có quan hệ happens-before giữa chúng (tức là không có lock, join, hay barrier nào đảm bảo thứ tự)

```
Thread 1 ghi x = 1   ──┐
                        ├── cùng địa chỉ, không có lock → RACE!
Thread 2 đọc x       ──┘
```

TSan theo dõi mọi `lock/unlock`, `thread create/join` để xây dựng quan hệ happens-before. Nếu 2 thread truy cập cùng địa chỉ mà không có quan hệ đó → báo race.

### Giới hạn của TSan

| Giới hạn | Lý do |
|---|---|
| Chỉ detect race **đã xảy ra** khi chạy | Nếu race không được trigger trong lần chạy đó thì không phát hiện được |
| **Một số race biến mất khi bật TSan** | TSan làm chậm chương trình ~5–15x, thay đổi timing giữa các thread → race condition phụ thuộc timing hẹp có thể không còn xảy ra |
| Tốn RAM hơn ~5–10x | TSan duy trì shadow memory để theo dõi |
| Không detect deadlock | Dùng Helgrind cho deadlock |

> ⚠️ **Kết luận thực tế:** TSan là công cụ hữu ích nhưng không phải lưới an toàn tuyệt đối. Không có TSan warning không đồng nghĩa với code thread-safe.

### Workflow AI-First

```
1. Dùng AI generate code
        ↓
2. Build với TSan:
   gcc -fsanitize=thread -g -lpthread -o prog prog.c
        ↓
3. Chạy → copy output → paste vào AI
        ↓
4. AI giải thích lỗi + suggest fix
        ↓
5. Hiểu WHY, không chỉ apply fix mù quáng
```

### Ví dụ output TSan

```
WARNING: ThreadSanitizer: data race (pid=1234)
  Write of size 4 at 0x... by thread T2:
    #0 increment() main.c:8       ← thread 2 đang ghi
  Previous read of size 4 at 0x... by thread T1:
    #0 increment() main.c:8       ← thread 1 vừa đọc trước đó
  Location is global 'counter' of size 4 at 0x...
```

### Helgrind — Phát hiện Lock Ordering Violation

```bash
valgrind --tool=helgrind ./program
```

Helgrind theo dõi thứ tự lock và báo khi phát hiện nguy cơ deadlock, ngay cả khi deadlock chưa thực sự xảy ra.

---

## 3.2 Thiết kế Thread-Safe API

### Reentrancy vs Thread-safety

**Reentrant** — đoạn code cho phép bị ngắt giữa chừng và gọi lại chính nó mà không bị lỗi. Điều kiện để reentrant: hàm **không dùng global hay static state** — mọi thứ nằm trên stack hoặc được truyền vào qua argument.

**Thread-safe** — hàm có thể được nhiều thread gọi đồng thời mà không bị lỗi. Đạt được bằng nhiều cách: không dùng shared state, dùng lock, dùng atomic operation...

```
Reentrant:    không dùng shared state → tự nhiên an toàn khi bị ngắt và gọi lại
Thread-safe:  an toàn khi nhiều thread gọi đồng thời → có thể dùng shared state nhưng phải bảo vệ đúng cách
```

Hàm reentrant thì thread-safe vì không dùng shared state. Nhưng hàm thread-safe thì **không nhất thiết** reentrant — nó có thể dùng shared state được bảo vệ bằng lock.

### Case study: `strtok` vs `strtok_r`

```c
// strtok: KHÔNG thread-safe
// Dùng internal static buffer — 2 threads gọi đồng thời → conflict
char *token = strtok(str, ",");

// strtok_r: thread-safe
// Caller tự cung cấp state buffer
char *saveptr;
char *token = strtok_r(str, ",", &saveptr);
```

> Quy tắc: Trước khi dùng bất kỳ hàm thư viện C nào trong multithread code, kiểm tra man page xem có note "MT-Safe" không.

### Opaque Handle Pattern

Cách tốt nhất để viết thread-safe API: **ẩn lock bên trong, chỉ expose hàm**.

```c
// ✅ Caller không cần biết có lock — không thể quên unlock
typedef struct Queue Queue;

Queue *queue_create(int capacity);
void   queue_push(Queue *q, int val);
int    queue_pop(Queue *q);
void   queue_destroy(Queue *q);
```

```c
// Implementation
struct Queue {
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;
    int            *buf;
    int             head, tail, size, capacity;
};

void queue_push(Queue *q, int val) {
    pthread_mutex_lock(&q->lock);
    q->buf[q->tail] = val;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}
```


---

# Bài thực hành

## Lab 1 — Đếm số lẻ song song

**Yêu cầu:** Đếm số lẻ từ 1 đến 100 triệu. Chia range cho N threads xử lý song song, so sánh thời gian với single-thread.

**Gợi ý:**
- Chia đều range: thread `i` xử lý từ `i*(100M/N)` đến `(i+1)*(100M/N)`
- Dùng struct để truyền `start`, `end`, nhận lại `result`
- Đo thời gian bằng `clock_gettime(CLOCK_MONOTONIC, ...)`
- Main thread cộng tổng kết quả từ tất cả threads sau khi join

**Câu hỏi sau thực hành:**
- Thời gian có giảm tuyến tính theo số thread không?
- Tại sao không cần mutex ở bài này?

---

## Lab 2 — Thread-Safe Logger

**Yêu cầu:** Viết một logger mà nhiều thread có thể gọi đồng thời để ghi log vào cùng 1 file, không bị lẫn lộn nội dung.

**Interface cần implement:**
```c
Logger *logger_create(const char *filename);
void    logger_log(Logger *l, const char *tag, const char *msg);
void    logger_destroy(Logger *l);
```

**Gợi ý:**
- Dùng mutex bảo vệ thao tác ghi file
- Mỗi dòng log có format: `[timestamp] [tag] message`
- Test bằng cách tạo 5 threads, mỗi thread ghi 100 dòng

**Câu hỏi sau thực hành:**
- Nếu bỏ mutex, file log bị ảnh hưởng thế nào?
- Làm thế nào để verify logger hoạt động đúng?

---

## Lab 3 — Producer-Consumer Queue

**Yêu cầu:** Implement một bounded queue thread-safe với 2 producer threads và 3 consumer threads.

**Yêu cầu kỹ thuật:**
- Queue có capacity tối đa 10 items
- Producer block khi queue đầy
- Consumer block khi queue rỗng
- Dùng mutex + condition variable (không dùng semaphore)
- Chạy trong 5 giây rồi graceful shutdown

**Interface:**
```c
typedef struct BoundedQueue BoundedQueue;

BoundedQueue *bq_create(int capacity);
void          bq_push(BoundedQueue *q, int val);   // block nếu đầy
int           bq_pop(BoundedQueue *q);              // block nếu rỗng
void          bq_destroy(BoundedQueue *q);
```

**Câu hỏi sau thực hành:**
- Tại sao cần 2 condition variables (`not_full` và `not_empty`)?
- Điều gì xảy ra nếu dùng `signal` thay vì `broadcast` khi unblock?

---

*© FPT Software · Corporate Training Center · Internal Use*
