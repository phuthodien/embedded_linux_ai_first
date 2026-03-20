# Bài Giảng: Kernel Module & Kernel Space Programming Mindset

**Tác giả:** Lưu An Phú — CEO Vinalinux  
**Thời lượng:** 2 giờ  
**Đối tượng:** Embedded MCU developer chuyển sang Linux  

---

## Phần 1: Tại sao Kernel Module tồn tại? (10 phút)

### Pain point quen thuộc với embedded developer

Với MCU, khi muốn thêm tính năng mới — ví dụ hỗ trợ thêm một loại cảm biến — anh phải làm gì? Sửa code, biên dịch lại toàn bộ firmware, flash lại chip. Nếu sản phẩm đang chạy ngoài hiện trường, đây là một vấn đề thực sự.

Linux kernel ban đầu được thiết kế theo kiến trúc **monolithic** — toàn bộ kernel (scheduler, memory manager, driver, filesystem...) được biên dịch thành một binary duy nhất chạy trong kernel space. Về lý thuyết, điều này có nghĩa là mỗi khi muốn thêm driver mới, anh phải biên dịch lại toàn bộ kernel và reboot hệ thống.

Thực tế, đây là điều không thể chấp nhận được trong môi trường production — server không thể reboot mỗi khi cần thêm driver card mạng mới, hay thiết bị nhúng không thể reflash mỗi khi cần hỗ trợ thêm một peripheral.

### Giải pháp: Loadable Kernel Module (LKM)

Kernel module là cơ chế cho phép **biên dịch một tính năng kernel thành file binary riêng** (`.ko`), và **nạp vào kernel đang chạy** mà không cần reboot.

```bash
# Nạp module vào kernel
insmod my_driver.ko

# Gỡ module khỏi kernel
rmmod my_driver
```

Đây không phải là một process mới được tạo ra. Module được **ánh xạ trực tiếp vào kernel image** đang chạy trong RAM — code của module chạy trong cùng không gian địa chỉ với kernel, với cùng mức quyền hạn.

### Hệ quả quan trọng

> **Một bug trong kernel module = một bug trong kernel.**

Không có sandbox, không có memory protection giữa module và kernel core. Đây là điểm khác biệt căn bản so với lập trình userspace mà anh cần ghi nhớ xuyên suốt bài học này.

---

## Phần 2: Kernel Module là gì — Bên dưới lớp vỏ (15 phút)

### File `.ko` không phải file binary thông thường

Khi anh biên dịch một ứng dụng userspace, kết quả là một ELF binary có thể chạy độc lập. Kernel module cũng là ELF, nhưng có những section đặc biệt mà binary userspace không có:

- **`.modinfo`** — chứa metadata: tên module, license, author, và quan trọng nhất là **vermagic**
- **`.gnu.linkonce.this_module`** — struct mô tả module với kernel
- **`Module.symvers`** — bảng các symbol mà module export ra cho module khác dùng

Anh có thể kiểm tra bằng:

```bash
modinfo my_driver.ko
```

```
filename:       my_driver.ko
license:        GPL
author:         Vinalinux
vermagic:       6.1.0 SMP mod_unload ARMv7
```

### vermagic — cơ chế bảo vệ version

Đây là điểm nhiều developer bị "bẫy" lần đầu. Khi `insmod` được gọi, kernel sẽ kiểm tra **vermagic** trong `.modinfo` của file `.ko` với vermagic của kernel đang chạy. Nếu không khớp, kernel từ chối load:

```
ERROR: could not insert module my_driver.ko: Invalid module format
```

vermagic bao gồm: **kernel version + cấu hình biên dịch** (SMP, preempt, ARMv7...). Đây là lý do tại sao file `.ko` biên dịch cho kernel 6.1.0 sẽ không load được trên kernel 6.1.1, dù chỉ khác nhau một con số nhỏ.

> **Hệ quả thực tế**: Khi cross-compile kernel module cho board, anh phải dùng **đúng kernel source tree** tương ứng với kernel đang chạy trên board — không phải kernel source bất kỳ cùng version.

### Symbol linking — module tìm hàm kernel ở đâu?

Khi anh gọi `printk()` hay `request_irq()` trong module, compiler không biết địa chỉ thực của các hàm này tại thời điểm biên dịch. Địa chỉ chỉ được resolve lúc module được load vào kernel.

Kernel duy trì một bảng symbol toàn cục:

```bash
# Xem tất cả symbol đang có trong kernel
cat /proc/kallsyms | grep request_irq
```

```
c0521234 T request_irq
```

Chỉ những hàm được kernel **export** bằng macro `EXPORT_SYMBOL()` mới có thể được module gọi. Đây là API boundary giữa kernel core và module.

```c
/* Trong kernel source — hàm này module mới gọi được */
EXPORT_SYMBOL(request_irq);

/* Hàm internal — module không gọi được */
static void internal_irq_setup(void) { ... }
```

### Makefile — cross compile kernel module cho BBB

```makefile
ARCH          := arm
CROSS_COMPILE := /home/phula/work/bbb/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-
KDIR          := /path/to/bbb/kernel/source

obj-m += my_driver.o

all:
	make -C $(KDIR) M=$(PWD) modules \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE)

clean:
	make -C $(KDIR) M=$(PWD) clean
```

Chạy trên máy dev (x86), kết quả `.ko` copy sang BBB rồi `insmod`.

`ARCH` và `CROSS_COMPILE` ở đây không phải để build lại kernel — mà để hệ thống build của kernel **biết nó đang tạo ra code cho kiến trúc ARM**, từ đó chọn đúng compiler flags và header. Nếu thiếu hai biến này, module sẽ được biên dịch cho x86 và **vermagic sẽ không khớp** với kernel trên BBB.

---

## Phần 2.5: Hello World Kernel Module (10 phút)

### Code tối giản nhất có thể chạy được

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init my_module_init(void)
{
    printk(KERN_INFO "Hello from kernel space!\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Goodbye from kernel space!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luu An Phu - Vinalinux");
MODULE_DESCRIPTION("Hello World Kernel Module");
```

**Chạy thử trên BBB:**

```bash
insmod hello.ko
dmesg | tail -5
```

```
[12345.678] Hello from kernel space!
```

```bash
rmmod hello
dmesg | tail -5
```

```
[12350.123] Goodbye from kernel space!
```

### Giải thích từng thành phần

**Header files** — không có `<stdio.h>`, `<stdlib.h>`. Chỉ dùng kernel headers từ `/include/linux/`:
- `linux/module.h` — bắt buộc cho mọi kernel module
- `linux/kernel.h` — chứa `printk()` và các macro cơ bản
- `linux/init.h` — chứa `__init`, `__exit`

**`__init` và `__exit`** — đây là compiler attribute, không phải tên hàm tùy ý. Chúng báo cho kernel biết:
- `__init`: hàm này chỉ dùng một lần khi load — kernel sẽ **giải phóng vùng nhớ chứa hàm này sau khi gọi xong** để tiết kiệm RAM
- `__exit`: hàm này chỉ chạy khi unload — nếu module được compile tĩnh vào kernel (không phải `.ko`), kernel sẽ bỏ qua hàm này hoàn toàn

**`module_init()` / `module_exit()`** — macro đăng ký hàm init và exit với kernel. Kernel không gọi trực tiếp theo tên hàm — nó tìm entry point qua các symbol được đăng ký bởi hai macro này.

**`MODULE_LICENSE("GPL")`** — không chỉ là metadata. Nếu module không khai báo GPL, kernel sẽ đánh dấu kernel là "tainted" (bị nhiễm) và một số kernel API sẽ bị từ chối export cho module đó.

**`printk()` vs `printf()`** — `printk()` không in thẳng ra terminal. Nó ghi vào **kernel ring buffer**. Anh đọc buffer đó bằng `dmesg`. `KERN_INFO` là log level — kernel có 8 level từ `KERN_EMERG` (nghiêm trọng nhất) đến `KERN_DEBUG`.

---

## Phần 3: Kernel Space vs User Space — Mindset Shift (15 phút)

### Bắt đầu từ những gì anh quen thuộc

Khi lập trình userspace, anh có một môi trường rất "thoải mái" mà thường không để ý:

- Gọi `malloc()` để xin bộ nhớ — kernel sẽ cấp phát và map page vào process của anh
- Gọi `printf()` để in log — bên dưới là `write()` syscall gọi vào kernel
- Truy cập sai địa chỉ nhớ — kernel bắt lỗi, gửi `SIGSEGV`, process bị kill, **hệ thống vẫn sống**
- Stack thiếu chỗ — kernel detect stack overflow, kill process, **hệ thống vẫn sống**

Tất cả những cơ chế bảo vệ đó — chính là kernel đang bảo vệ anh.

Bây giờ anh **đang viết code bên trong kernel**. Không còn ai bảo vệ nữa.

### Những thứ không tồn tại trong kernel space

**Không có libc.** `printf()`, `malloc()`, `free()` từ libc — không dùng được. Kernel có API riêng:

| Userspace | Kernel space |
|---|---|
| `printf()` | `printk()` |
| `malloc()` / `free()` | `kmalloc()` / `kfree()` |
| `sleep()` | `msleep()` / `usleep_range()` |

**`kmalloc()` không giống `malloc()`.** Anh phải chỉ định **GFP flags** — cho kernel biết anh đang xin bộ nhớ trong ngữ cảnh nào:

```c
/* Có thể sleep để chờ bộ nhớ — dùng trong process context */
ptr = kmalloc(size, GFP_KERNEL);

/* Không được sleep — dùng trong interrupt handler */
ptr = kmalloc(size, GFP_ATOMIC);
```

Dùng sai flag — ví dụ dùng `GFP_KERNEL` trong interrupt handler — kernel sẽ phát hiện và panic ngay lập tức.

### Stack cố định và rất nhỏ

Ở userspace, khi stack của process sắp đầy, kernel tự động mở rộng stack bằng cách map thêm page mới — anh hầu như không bao giờ phải nghĩ đến giới hạn stack.

Trong kernel space, mỗi process/thread có một **kernel stack cố định, không thể mở rộng** — thường chỉ **4KB hoặc 8KB** tùy kiến trúc. Không có cơ chế tự động mở rộng.

Hệ quả thực tế:
- Không khai báo array lớn trên stack trong kernel code
- Không gọi hàm đệ quy sâu
- Stack overflow trong kernel = **kernel panic**, không có cơ hội recover

```c
/* NGUY HIỂM trong kernel — 4KB array trên stack có thể overflow */
void my_kernel_func(void)
{
    char buffer[4096];  /* Đừng làm thế này */
    ...
}

/* ĐÚNG — xin bộ nhớ từ heap */
void my_kernel_func(void)
{
    char *buffer = kmalloc(4096, GFP_KERNEL);
    if (!buffer)
        return;
    ...
    kfree(buffer);
}
```

### Không có exception handler — crash là crash

Ở userspace, khi anh dereference NULL pointer, kernel bắt page fault, gửi `SIGSEGV` cho process, process bị kill. Hệ thống vẫn chạy bình thường.

Trong kernel space, khi code của anh dereference NULL pointer:

```c
struct device *dev = NULL;
dev->driver->probe(dev);  /* Kernel oops — hoặc panic */
```

Kernel sẽ in **oops message** với call stack ra `dmesg`, sau đó tùy cấu hình có thể tiếp tục chạy (bất ổn định) hoặc **panic và reboot**. Không có cơ chế nào bắt lỗi và recover một cách sạch sẽ như userspace.

> **Mindset cần có**: Trong kernel space, mọi pointer đều phải được kiểm tra trước khi dereference. Không có ngoại lệ.

---

## Phần 4: Lập trình Phần cứng — ioremap + Blink LED (20 phút)

### Pain point quen thuộc với embedded developer

Trên MCU, anh truy cập thanh ghi ngoại vi trực tiếp qua địa chỉ vật lý:

```c
/* Trên STM32 — hoàn toàn bình thường */
#define GPIOA_ODR  (*(volatile uint32_t*)0x40020014)
GPIOA_ODR |= (1 << 5);  /* Bật LED */
```

Trên Linux, điều này **không thể làm được từ userspace** vì MMU đã bật — mọi địa chỉ trong code đều là địa chỉ ảo. Truy cập thẳng địa chỉ vật lý sẽ bị kernel chặn ngay.

Nhưng anh đang viết kernel module — anh có quyền làm điều này, nhưng phải **xin kernel map địa chỉ vật lý thành địa chỉ ảo** trước khi dùng.

### ioremap — cầu nối giữa địa chỉ vật lý và ảo

```c
#include <linux/io.h>

void __iomem *base;

/* Xin map 4 byte tại địa chỉ vật lý 0x44E07000 */
base = ioremap(0x44E07000, 4);
if (!base) {
    printk(KERN_ERR "ioremap failed!\n");
    return -ENOMEM;
}

/* Đọc/ghi thông qua địa chỉ ảo vừa được cấp */
writel(0x1 << 20, base + 0x134);  /* Set output */

/* Giải phóng mapping khi không dùng nữa */
iounmap(base);
```

`__iomem` là annotation cho compiler và các static analysis tool biết đây là con trỏ trỏ vào vùng I/O — không được dereference trực tiếp bằng `*ptr`, phải dùng `readl()`/`writel()`.

### Tại sao không dùng `*ptr` trực tiếp?

```c
/* SAI — undefined behavior trên nhiều kiến trúc */
*base = 0x100000;

/* ĐÚNG — đảm bảo memory barrier và byte order đúng */
writel(0x100000, base);
```

`writel()`/`readl()` không chỉ là wrapper đơn giản. Chúng đảm bảo:
- **Memory barrier** — đảm bảo thứ tự ghi đến hardware đúng như code
- **Byte order** — tự động xử lý endianness giữa CPU và peripheral

### Blink LED trên BBB — GPIO0_20

BeagleBone Black, GPIO0:
- Base address: `0x44E07000`
- `GPIO_OE` (Output Enable) offset: `0x134` — bit = 0 là output
- `GPIO_DATAOUT` offset: `0x13C` — ghi giá trị ra chân GPIO

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO0_BASE    0x44E07000
#define GPIO_OE       0x134
#define GPIO_DATAOUT  0x13C
#define LED_PIN       (1 << 20)

static void __iomem *gpio0_base;

static int __init led_init(void)
{
    uint32_t val;

    gpio0_base = ioremap(GPIO0_BASE, SZ_4K);
    if (!gpio0_base)
        return -ENOMEM;

    /* Cấu hình GPIO0_20 là output */
    val = readl(gpio0_base + GPIO_OE);
    val &= ~LED_PIN;
    writel(val, gpio0_base + GPIO_OE);

    /* Bật LED */
    val = readl(gpio0_base + GPIO_DATAOUT);
    val |= LED_PIN;
    writel(val, gpio0_base + GPIO_DATAOUT);

    printk(KERN_INFO "LED ON\n");
    return 0;
}

static void __exit led_exit(void)
{
    uint32_t val;

    /* Tắt LED */
    val = readl(gpio0_base + GPIO_DATAOUT);
    val &= ~LED_PIN;
    writel(val, gpio0_base + GPIO_DATAOUT);

    iounmap(gpio0_base);
    printk(KERN_INFO "LED OFF\n");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luu An Phu - Vinalinux");
```

**Kết quả khi chạy:**

```bash
insmod led.ko
# LED sáng
dmesg | tail -3
```

```
[1234.567] LED ON
```

```bash
rmmod led
# LED tắt
dmesg | tail -3
```

```
[1240.123] LED OFF
```

### Điểm cần nhớ

- Luôn kiểm tra return value của `ioremap()` — nếu NULL thì không được dùng
- Luôn gọi `iounmap()` trong `module_exit()` — nếu quên sẽ leak mapping cho đến khi reboot
- Dùng `readl()`/`writel()` thay vì dereference pointer trực tiếp

---

## Phần 5: Interrupt Handling + Button GPIO (20 phút)

### Tại sao cần interrupt trong kernel module?

Cách tiếp cận polling — liên tục đọc trạng thái GPIO trong vòng lặp — hoàn toàn không phù hợp trong kernel space. Không có process riêng để chạy vòng lặp, và chiếm CPU trong kernel sẽ ảnh hưởng toàn bộ hệ thống. Interrupt là cách đúng đắn duy nhất.

### Đăng ký interrupt handler

```c
#include <linux/interrupt.h>

int request_irq(unsigned int irq,
                irq_handler_t handler,
                unsigned long flags,
                const char *name,
                void *dev_id);
```

- **`irq`** — IRQ number, không phải GPIO number. Phải convert từ GPIO number sang IRQ number
- **`handler`** — hàm xử lý interrupt, có prototype cố định
- **`flags`** — điều kiện trigger: `IRQF_TRIGGER_RISING`, `IRQF_TRIGGER_FALLING`, `IRQF_TRIGGER_BOTH`
- **`dev_id`** — pointer tùy ý, thường dùng để pass context vào handler — quan trọng khi dùng với `container_of`

### Interrupt handler — những ràng buộc cứng

Interrupt handler chạy trong **atomic context** — đây là ràng buộc quan trọng nhất cần ghi nhớ:

- **Không được sleep** — không gọi `msleep()`, `kmalloc(GFP_KERNEL)`, hay bất kỳ hàm nào có thể block
- **Không được gọi scheduler** — không `schedule()`, không `mutex_lock()`
- **Chạy càng nhanh càng tốt** — handler đang chiếm CPU, mọi interrupt khác có thể bị delay

```c
/* ĐÚNG — handler làm việc tối thiểu */
static irqreturn_t button_handler(int irq, void *dev_id)
{
    /* Chỉ set flag, việc nặng để tasklet/workqueue xử lý */
    got_interrupt = 1;
    return IRQ_HANDLED;
}

/* SAI — sleep trong interrupt handler = kernel panic */
static irqreturn_t bad_handler(int irq, void *dev_id)
{
    msleep(100);              /* KHÔNG BAO GIỜ làm thế này */
    kmalloc(64, GFP_KERNEL);  /* GFP_KERNEL có thể sleep — SAI */
    return IRQ_HANDLED;
}
```

### Button + LED trên BBB — GPIO0_31

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

#define GPIO0_BASE    0x44E07000
#define GPIO_OE       0x134
#define GPIO_DATAOUT  0x13C
#define GPIO_DATAIN   0x138
#define LED_PIN       (1 << 20)
#define BUTTON_GPIO   31

static void __iomem *gpio0_base;
static int button_irq;

static irqreturn_t button_handler(int irq, void *dev_id)
{
    uint32_t val;

    /* Toggle LED */
    val = readl(gpio0_base + GPIO_DATAOUT);
    val ^= LED_PIN;
    writel(val, gpio0_base + GPIO_DATAOUT);

    printk(KERN_INFO "Button pressed — LED toggled\n");
    return IRQ_HANDLED;
}

static int __init btn_led_init(void)
{
    uint32_t val;
    int ret;

    /* Map GPIO0 */
    gpio0_base = ioremap(GPIO0_BASE, SZ_4K);
    if (!gpio0_base)
        return -ENOMEM;

    /* Cấu hình GPIO0_20 là output (LED) */
    val = readl(gpio0_base + GPIO_OE);
    val &= ~LED_PIN;
    writel(val, gpio0_base + GPIO_OE);

    /* Lấy IRQ number từ GPIO number */
    button_irq = gpio_to_irq(BUTTON_GPIO);
    if (button_irq < 0) {
        printk(KERN_ERR "Failed to get IRQ for GPIO%d\n", BUTTON_GPIO);
        iounmap(gpio0_base);
        return button_irq;
    }

    /* Đăng ký interrupt handler */
    ret = request_irq(button_irq,
                      button_handler,
                      IRQF_TRIGGER_FALLING,
                      "button_irq",
                      NULL);
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ %d\n", button_irq);
        iounmap(gpio0_base);
        return ret;
    }

    printk(KERN_INFO "Button-LED module loaded. IRQ=%d\n", button_irq);
    return 0;
}

static void __exit btn_led_exit(void)
{
    free_irq(button_irq, NULL);
    iounmap(gpio0_base);
    printk(KERN_INFO "Button-LED module unloaded\n");
}

module_init(btn_led_init);
module_exit(btn_led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luu An Phu - Vinalinux");
```

**Kết quả:**

```bash
insmod btn_led.ko
dmesg | tail -3
```

```
[1234.567] Button-LED module loaded. IRQ=200
```

```bash
# Nhấn button
dmesg | tail -3
```

```
[1240.123] Button pressed — LED toggled
```

### Kiểm tra IRQ đã đăng ký

```bash
cat /proc/interrupts | grep button_irq
```

```
200:    3    gpio    button_irq
```

Cột số 3 cho biết interrupt đã được trigger 3 lần — hữu ích để debug khi không chắc hardware có gửi interrupt không.

### Điểm cần nhớ

- Luôn gọi `free_irq()` trong `module_exit()` — nếu quên, handler vẫn được gọi sau khi module bị unload, trỏ vào vùng nhớ không còn hợp lệ → kernel panic
- `dev_id` trong `request_irq()` và `free_irq()` phải khớp nhau
- Dùng `GFP_ATOMIC` nếu bắt buộc phải `kmalloc` trong handler

---

## Phần 6: Kernel Timer + Macro `container_of` (15 phút)

### Kernel Timer — đặt lịch công việc trong tương lai

Trong userspace, anh dùng `sleep()` hoặc `setitimer()` để delay. Trong kernel, cách đúng là dùng **kernel timer** — một cơ chế cho phép đăng ký một callback sẽ được gọi sau một khoảng thời gian nhất định.

```c
#include <linux/timer.h>

struct timer_list my_timer;

/* Callback — chạy trong softirq context, ràng buộc tương tự interrupt handler */
static void timer_callback(struct timer_list *t)
{
    printk(KERN_INFO "Timer fired!\n");

    /* Tự schedule lại sau 1 giây — tạo periodic timer */
    mod_timer(&my_timer, jiffies + HZ);
}

static int __init timer_init(void)
{
    /* Khởi tạo timer và gán callback */
    timer_setup(&my_timer, timer_callback, 0);

    /* Kích hoạt lần đầu — HZ = số jiffies trong 1 giây */
    mod_timer(&my_timer, jiffies + HZ);

    printk(KERN_INFO "Timer started\n");
    return 0;
}

static void __exit timer_exit(void)
{
    /* Hủy timer trước khi unload — bắt buộc */
    del_timer_sync(&my_timer);
    printk(KERN_INFO "Timer stopped\n");
}
```

**`jiffies`** là bộ đếm tick của kernel — tăng lên 1 mỗi timer interrupt. **`HZ`** là số tick trong 1 giây (thường 100, 250, hoặc 1000 tùy config). `jiffies + HZ` có nghĩa là "1 giây kể từ bây giờ".

Timer callback chạy trong **softirq context** — ràng buộc tương tự interrupt handler: không được sleep, không được dùng `GFP_KERNEL`.

### `container_of` — tư duy thiết kế data structure của Linux kernel

Đây là một trong những macro quan trọng nhất trong toàn bộ Linux kernel source. Hiểu `container_of` là hiểu một phần lớn tư duy thiết kế của kernel.

### Vấn đề: callback chỉ nhận được một pointer

Khi timer callback được gọi, signature cố định:

```c
static void timer_callback(struct timer_list *t);
```

Chỉ nhận được `struct timer_list *t`. Nhưng trong thực tế, anh cần truy cập vào toàn bộ context của driver — ví dụ GPIO base address, IRQ number, trạng thái LED. Làm thế nào?

### Giải pháp của kernel: embed struct vào struct lớn hơn

```c
/* Định nghĩa struct chứa toàn bộ context của driver */
struct led_dev {
    void __iomem     *gpio_base;
    int               irq;
    int               led_state;
    struct timer_list timer;   /* Nhúng timer vào trong struct */
};

static struct led_dev my_dev;
```

Khi timer callback được gọi với `struct timer_list *t`, anh biết `t` chính là trường `timer` bên trong `struct led_dev`. `container_of` cho phép tính ngược địa chỉ của struct cha từ địa chỉ của member:

```c
static void timer_callback(struct timer_list *t)
{
    /* Từ pointer đến member timer, tìm ngược ra pointer đến struct led_dev */
    struct led_dev *dev = container_of(t, struct led_dev, timer);

    /* Giờ có thể truy cập toàn bộ context */
    uint32_t val = readl(dev->gpio_base + GPIO_DATAOUT);
    val ^= LED_PIN;
    writel(val, dev->gpio_base + GPIO_DATAOUT);

    mod_timer(&dev->timer, jiffies + HZ);
}
```

### Cơ chế hoạt động của `container_of`

```c
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
```

`offsetof(type, member)` trả về khoảng cách byte từ đầu struct đến member. Trừ offset đó khỏi địa chỉ của member → thu được địa chỉ đầu struct.

Minh họa trong bộ nhớ:

```
Địa chỉ thấp
┌─────────────────────────┐  ← &my_dev  (container_of trả về đây)
│ gpio_base   (8 bytes)   │
├─────────────────────────┤
│ irq         (4 bytes)   │
├─────────────────────────┤
│ led_state   (4 bytes)   │
├─────────────────────────┤
│ timer       (N bytes)   │  ← t  (kernel truyền vào đây)
└─────────────────────────┘
Địa chỉ cao

container_of(t, struct led_dev, timer)
= t - offsetof(struct led_dev, timer)
= &my_dev
```

### `container_of` xuất hiện ở khắp nơi trong kernel

Pattern này không chỉ dùng cho timer. Bất cứ khi nào kernel callback chỉ truyền vào một pointer đến một member cụ thể, `container_of` là cách lấy lại context:

- `struct list_head` — linked list của kernel
- `struct work_struct` — workqueue
- `struct kobject` — device model
- `struct file_operations` — character device

---

## Phần 7: Tư duy Debugging ở Kernel Space (15 phút)

### Tại sao debugging trong kernel khó hơn userspace?

Ở userspace, workflow debug rất quen thuộc: attach GDB, đặt breakpoint, chạy từng dòng, inspect biến. Workflow này gần như không áp dụng được trong kernel vì một lý do căn bản:

**Khi GDB dừng execution tại breakpoint, toàn bộ kernel dừng theo** — scheduler không chạy, interrupt không được xử lý, watchdog timer không được reset. Sau vài giây, hệ thống treo hoặc reboot.

### Các lựa chọn debug kernel — từ mạnh đến yếu

**Trace32** — công cụ mạnh nhất, dùng trong môi trường professional. Kết nối qua JTAG, có hardware debug unit riêng nên có thể dừng CPU mà không làm treo hệ thống. Nhược điểm: cần mua cả hardware lẫn software license, chi phí cao.

**JTAG thông thường (OpenOCD + GDB)** — đi qua hardware debug module của SoC, ổn định hơn software GDB thuần túy. Thường dùng trong giai đoạn bringup board — debug bootloader, early kernel init. Vẫn có giới hạn khi kernel đang chạy đầy đủ.

**printk** — công cụ thực tế nhất cho driver development hàng ngày. Không cần hardware đặc biệt, luôn hoạt động, không làm thay đổi timing của hệ thống.

### Chiến lược dùng printk hiệu quả

Nguyên tắc cơ bản: **in vào kernel ring buffer, đọc khi cần — không in liên tục ra terminal.**

```c
/* In với log level phù hợp */
printk(KERN_ERR  "Lỗi nghiêm trọng: ioremap failed\n");   /* Luôn hiện */
printk(KERN_INFO "Module loaded successfully\n");           /* Thông tin */
printk(KERN_DEBUG "GPIO val = 0x%08x\n", val);             /* Chỉ khi debug */
```

Chỉ bật `KERN_DEBUG` khi cần — log nhiều quá làm chậm hệ thống và che lấp log quan trọng:

```bash
# Bật log level debug cho module cụ thể
echo "module my_driver +p" > /sys/kernel/debug/dynamic_debug/control

# Đọc kernel ring buffer
dmesg | tail -20

# Theo dõi log real-time
dmesg -w
```

### Heisenbug trong kernel — bug biến mất khi thêm log

Đây là hiện tượng anh sẽ gặp sớm hay muộn trong kernel development. Một bug race condition hoặc timing-sensitive chỉ xuất hiện khi **không có** log — vừa thêm `printk()` vào để debug, bug biến mất.

Lý do: `printk()` không phải là hàm nhanh. Nó acquire lock để ghi vào ring buffer, tạo ra **memory barrier**, và thay đổi timing của toàn bộ đoạn code xung quanh. Race condition vốn phụ thuộc vào timing chính xác — một `printk()` có thể làm thay đổi timing đủ để race không còn xảy ra nữa.

Khi gặp tình huống này:

```c
/* Thay vì printk trực tiếp, dùng trace_printk — nhanh hơn nhiều */
trace_printk("GPIO val = 0x%08x\n", val);
```

```bash
# Đọc trace buffer
cat /sys/kernel/debug/tracing/trace
```

`trace_printk()` ghi vào **ftrace buffer** thay vì ring buffer — nhanh hơn đáng kể, ít ảnh hưởng timing hơn.

### Đọc kernel oops message

Khi kernel module crash, đây là thông tin quan trọng nhất anh có. Ví dụ một oops thực tế:

```
BUG: unable to handle kernel NULL pointer dereference at 00000000
IP: [<bf000018>] button_handler+0x18/0x30 [my_driver]
Call Trace:
 [<c0521234>] handle_irq_event+0x44/0x60
 [<c0522abc>] generic_handle_irq+0x2c/0x40
 [<c0523def>] irq_exit+0x8c/0xa0
```

Cách đọc:
- **`NULL pointer dereference`** — dereference con trỏ NULL
- **`button_handler+0x18`** — crash xảy ra tại offset `0x18` trong hàm `button_handler`
- **Call Trace** — chuỗi hàm dẫn đến crash, đọc từ trên xuống

Tìm chính xác dòng code bị crash:

```bash
# Dùng addr2line với file .ko có debug symbol
arm-none-linux-gnueabihf-addr2line -e my_driver.ko 0x18
# Output: /home/user/my_driver.c:42
```

---

## Phần 8: Bài Lab Thực Hành

### Lab 1 — Blink LED bằng Kernel Timer

**Mục tiêu:** Viết kernel module blink LED GPIO0_20 trên BBB với chu kỳ 500ms sử dụng kernel timer.

**Yêu cầu:**
- Dùng `ioremap` để map GPIO0
- Dùng `timer_setup` + `mod_timer` để tạo periodic timer
- Dùng `container_of` để truy cập GPIO base từ trong timer callback
- LED toggle mỗi 500ms
- `rmmod` phải tắt LED và hủy timer sạch sẽ

**Câu hỏi tư duy:**
- Tại sao không dùng `msleep(500)` trong một vòng lặp thay vì timer?
- Nếu quên `del_timer_sync()` trong `module_exit()`, điều gì xảy ra?

---

### Lab 2 — Button Interrupt + Debounce bằng Timer

**Mục tiêu:** Xử lý nút nhấn GPIO0_31 qua interrupt, dùng kernel timer để debounce.

**Yêu cầu:**
- Đăng ký interrupt handler cho GPIO0_31 trigger `IRQF_TRIGGER_FALLING`
- Trong interrupt handler: không xử lý LED trực tiếp — chỉ `mod_timer` để kích hoạt debounce timer 50ms
- Trong timer callback: đọc trạng thái GPIO, nếu button vẫn còn được nhấn thì mới toggle LED
- Dùng `container_of` để chia sẻ context giữa interrupt handler và timer callback thông qua một struct duy nhất

**Câu hỏi tư duy:**
- Tại sao phải đọc lại trạng thái GPIO trong timer callback thay vì toggle LED ngay trong interrupt handler?
- `dev_id` trong `request_irq` và `free_irq` phải khớp nhau — nếu không khớp, điều gì xảy ra khi `rmmod`?

---

### Lab 3 — Đọc Kernel Oops

**Mục tiêu:** Nhận diện và phân tích lỗi từ kernel oops message — không cần board, chỉ cần tư duy.

**Cho oops message sau:**

```
BUG: unable to handle kernel NULL pointer dereference at 00000000
IP: [<bf000024>] led_toggle+0x24/0x3c [blink_mod]
Call Trace:
 [<bf000048>] timer_callback+0x18/0x2c [blink_mod]
 [<c0521234>] call_timer_fn+0x44/0x110
 [<c0522abc>] run_timer_softirq+0x1f4/0x280
 [<c0523def>] __do_softirq+0xd8/0x2b0
```

**Yêu cầu phân tích:**
1. Crash xảy ra ở hàm nào, trong module nào?
2. Crash xảy ra trong context nào — process context hay interrupt context? Căn cứ vào đâu trong call trace?
3. Nguyên nhân khả năng nhất là gì?
4. Nếu có file `blink_mod.ko` với debug symbol, lệnh nào dùng để tìm chính xác dòng code bị crash?
