# Bài giảng: Character Device Driver

**Khóa học:** Lập trình trên Linux  
**Tác giả:** Lưu An Phú — CEO Vinalinux  
**Thời lượng:** ~2 giờ 20 phút  

---

## Agenda

1. Tại sao cần character driver? (15 phút)
2. Device number & device file (20 phút)
3. VFS và file_operations (25 phút)
4. Implement với Misc Device (25 phút)
5. ioctl (15 phút)
6. Linux Driver Template Pattern (15 phút)
7. Quy tắc tư duy viết driver trên Linux (10 phút)
8. Lab thực hành (15 phút)

---

## Phần 1 — Tại sao cần character driver? (15 phút)

### 1.1 Pain point: từ MCU sang Linux

Trên MCU, khi muốn điều khiển một ngoại vi — ví dụ UART — developer làm thẳng:

```c
// STM32 HAL
HAL_UART_Transmit(&huart1, data, len, HAL_MAX_DELAY);
```

Không có lớp trung gian. Code chạy trực tiếp trên phần cứng, không có process khác tranh chấp, không có memory protection. Đây là mô hình **bare-metal**: một chương trình, một CPU, toàn quyền.

Khi chuyển sang Linux, developer thường hỏi ngay: *"Tại sao không làm vậy được?"*

Câu trả lời nằm ở hai điểm:

**Thứ nhất — Linux là hệ thống đa tiến trình.** Có thể có 5 process cùng muốn ghi UART một lúc. Nếu mỗi process tự gọi thẳng vào hardware register, dữ liệu sẽ xen lẫn nhau và không process nào nhận được output đúng.

**Thứ hai — Linux có memory protection.** User space process không được phép truy cập physical address của hardware register trực tiếp. Kernel chặn điều này thông qua MMU — đây là cơ chế học viên đã biết từ bài Virtual Memory.

### 1.2 Triết lý "Everything is a file"

Linux đưa ra một abstraction cực kỳ mạnh: **mọi thứ đều là file**.

UART, GPIO, màn hình, bàn phím, socket mạng — tất cả đều được truy cập qua cùng một interface: `open()`, `read()`, `write()`, `close()`.

```
User application
      │
      │  open("/dev/ttyS0")
      │  write(fd, data, len)
      ▼
   Kernel (VFS)
      │
      ▼
  UART Driver  ──►  Hardware Register
```

Lợi ích thực tế:

- **Đồng nhất**: application không cần biết đang nói chuyện với UART, GPIO hay thiết bị ảo — interface giống nhau hoàn toàn.
- **Kiểm soát truy cập**: kernel đứng giữa, phân xử khi nhiều process cùng mở một device.
- **Testability**: có thể dùng `cat`, `echo`, `dd` để test driver mà không cần viết application riêng.

Đây chính là lý do character driver tồn tại: **driver là cầu nối giữa file interface của kernel và hardware register của thiết bị**.

### 1.3 Ba loại device trong Linux

Linux chia device thành ba loại, mỗi loại có cách truy cập khác nhau:

| Loại | Đặc điểm | Ví dụ |
|---|---|---|
| **Character device** | Truy cập tuần tự, từng byte | UART, GPIO, sensor |
| **Block device** | Truy cập theo block, có buffer | eMMC, SD card |
| **Network device** | Không có file trong `/dev`, dùng socket | eth0, wlan0 |

Với embedded developer, **character device là loại phổ biến nhất** — hầu hết ngoại vi giao tiếp tuần tự (UART, I2C, SPI, GPIO) đều được implement dưới dạng character device.

---

## Phần 2 — Device number & device file (20 phút)

### 2.1 Device number là gì?

Khi kernel nhìn vào một device file, nó không quan tâm tên file — nó quan tâm **device number**. Đây là một cặp số nguyên:

- **Major number**: xác định *driver* nào chịu trách nhiệm cho device này.
- **Minor number**: xác định *instance* cụ thể trong driver đó.

Analogy: Major number giống như **số tòa nhà**, minor number giống như **số phòng**. Kernel gõ đúng tòa, driver mở đúng phòng.

Ví dụ thực tế trên Linux:

```bash
$ ls -l /dev/tty*
crw-rw-rw- 1 root tty  5, 0 Mar 17 08:00 /dev/tty
crw--w---- 1 root tty  4, 0 Mar 17 08:00 /dev/tty0
crw--w---- 1 root tty  4, 1 Mar 17 08:00 /dev/tty1
```

Cột `5, 0` và `4, 0` chính là cặp `(major, minor)`. Tất cả `/dev/ttyN` đều dùng chung major number 4 — cùng một driver — nhưng minor number khác nhau để phân biệt từng cổng.

Xem danh sách major number đã được kernel cấp phát:

```bash
$ cat /proc/devices
Character devices:
  1 mem
  4 tty
  5 /dev/tty
 10 misc
 89 i2c
...
```

### 2.2 Device file — `/dev` không phải folder thường

Embedded developer lần đầu nhìn vào `/dev` thường nghĩ đây là thư mục chứa file bình thường. Thực ra không phải.

`/dev` được mount bởi **devtmpfs** — một filesystem ảo nằm hoàn toàn trong RAM, được kernel tạo ra lúc boot. Không có file nào trong `/dev` tồn tại trên disk.

```bash
$ mount | grep dev
devtmpfs on /dev type devtmpfs (rw,nosuid,size=65536k,nr_inodes=4096,mode=755)
```

Khi kernel phát hiện một driver đăng ký device mới, **udev** daemon nhận sự kiện từ kernel qua netlink socket, sau đó tạo ra file tương ứng trong `/dev`. Đây là lý do khi cắm USB, `/dev/sda1` tự xuất hiện mà không cần reboot.

### 2.3 Ba cách tạo device file

**Cách 1 — `mknod` (thủ công)**

```bash
$ mknod /dev/mydevice c 100 0
# c = character device, 100 = major, 0 = minor
```

Dùng để test nhanh hoặc trong môi trường embedded tối giản không có udev. Nhược điểm: phải tạo tay mỗi lần, không tự động mất đi khi driver unload.

**Cách 2 — udev (tự động, production)**

Driver gọi `device_create()` → kernel phát sự kiện → udev tạo file trong `/dev` tự động. Đây là cách chuẩn trên desktop Linux và các embedded distro đầy đủ như Yocto.

**Cách 3 — Driver tự tạo (embedded tối giản)**

Trong môi trường không có udev (busybox init, initramfs đơn giản), driver phải tự gọi `device_create()` và đảm bảo `/dev` đã được mount. Ít phổ biến hơn nhưng hay gặp trong firmware nhỏ.

**Khi nào dùng cái nào?**

| Tình huống | Cách dùng |
|---|---|
| Debug nhanh, test driver mới | `mknod` |
| Production Linux (Yocto, Debian) | udev + `device_create()` |
| Embedded không có udev | Driver tự tạo |

### 2.4 Cấp phát device number trong driver

Có hai cách cấp phát major/minor number:

**Tĩnh** — tự chọn major number:
```c
register_chrdev_region(MKDEV(100, 0), 1, "mydevice");
```
Rủi ro: xung đột nếu số đó đã được driver khác dùng.

**Động** — để kernel chọn (khuyến nghị):
```c
alloc_chrdev_region(&dev_num, 0, 1, "mydevice");
// dev_num sẽ chứa major/minor kernel vừa cấp
```

Với misc device (sẽ dùng ở Phần 4), bước này được ẩn hoàn toàn — major number luôn là **10**, minor number được kernel cấp tự động.

---

## Phần 3 — VFS và file_operations (25 phút)

### 3.1 Luồng từ `open()` đến driver

Khi user space gọi:

```c
int fd = open("/dev/mydevice", O_RDWR);
```

Kernel không nhảy thẳng vào driver. Nó đi qua một lớp trung gian gọi là **VFS — Virtual File System**.

```
User space
    │
    │  open("/dev/mydevice")
    ▼
  VFS (fs/open.c)
    │  1. Tìm inode của /dev/mydevice
    │  2. Đọc major/minor number từ inode
    │  3. Tra bảng chrdevs[] → tìm đúng driver
    │  4. Gọi driver->fops->open()
    ▼
  Driver của anh
    │
    ▼
  Hardware
```

VFS là lý do tại sao application không cần biết đang nói chuyện với driver nào — nó chỉ gọi `open()`, VFS lo phần còn lại.

Điều này cũng có nghĩa: **nếu driver không đăng ký đúng major/minor number, VFS sẽ không tìm thấy driver và `open()` trả về `-ENODEV`**.

### 3.2 `struct file_operations` — bảng hàm con trỏ

Đây là trái tim của character driver. `struct file_operations` là một struct chứa toàn bộ con trỏ hàm tương ứng với các system call mà user space có thể gọi trên file descriptor.

```c
static struct file_operations my_fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .release = dev_close,
    .read    = dev_read,
    .write   = dev_write,
    .unlocked_ioctl = dev_ioctl,
};
```

Kernel định nghĩa struct này trong `<linux/fs.h>` với hơn 30 trường — driver chỉ cần điền những trường mình quan tâm, các trường còn lại để `NULL`. Khi user space gọi một system call mà driver không implement, kernel trả về `-EINVAL` hoặc `-ENOTTY` tùy trường hợp.

Signature của từng hàm:

```c
// open / close
int (*open)(struct inode *, struct file *);
int (*release)(struct inode *, struct file *);

// read: trả về số byte đọc được, âm nếu lỗi
ssize_t (*read)(struct file *, char __user *, size_t, loff_t *);

// write: trả về số byte ghi được, âm nếu lỗi
ssize_t (*write)(struct file *, const char __user *, size_t, loff_t *);

// ioctl
long (*unlocked_ioctl)(struct file *, unsigned int, unsigned long);
```

Lưu ý `__user` — đây là annotation cho compiler và static analyzer (sparse) biết rằng con trỏ này trỏ vào **user space memory**, không được dereference trực tiếp trong kernel. Đây là lý do phải dùng `copy_to_user` / `copy_from_user` thay vì `memcpy` — sẽ giải thích kỹ ở Phần 4.

---

## Phần 4 — Implement với Misc Device (25 phút)

### 4.1 Misc device là gì?

Thay vì đi qua toàn bộ quy trình đăng ký character driver thủ công — `alloc_chrdev_region`, `cdev_init`, `cdev_add`, `class_create`, `device_create` — kernel cung cấp một shortcut gọi là **misc device**.

Misc device là một wrapper của character driver với một số đặc điểm cố định:
- Major number luôn là **10** (xem trong `/proc/devices` với tên `misc`)
- Minor number được kernel cấp động
- Device file được tạo tự động trong `/dev`

Toàn bộ boilerplate đăng ký rút gọn xuống còn hai bước: khai báo `struct miscdevice` và gọi `misc_register()`.

```c
#include <linux/miscdevice.h>

static struct miscdevice my_misc = {
    .minor = MISC_DYNAMIC_MINOR,  // kernel tự cấp minor number
    .name  = "mydevice",          // tạo ra /dev/mydevice
    .fops  = &my_fops,            // file_operations đã định nghĩa
};
```

### 4.2 Sequence khởi tạo và cleanup

**init:**

```c
static int __init my_init(void)
{
    int ret;

    ret = misc_register(&my_misc);
    if (ret) {
        pr_err("misc_register failed: %d\n", ret);
        return ret;
    }

    pr_info("mydevice registered at /dev/%s\n", my_misc.name);
    return 0;
}
```

**exit:**

```c
static void __exit my_exit(void)
{
    misc_deregister(&my_misc);
    pr_info("mydevice unregistered\n");
}
```

So sánh với character driver truyền thống — driver dùng misc tiết kiệm khoảng 20 dòng boilerplate. Phần còn lại — implement `file_operations` — hoàn toàn giống nhau.

### 4.3 `copy_to_user` / `copy_from_user` — tại sao không dùng `memcpy`?

Trên MCU, khi muốn copy dữ liệu từ buffer này sang buffer khác, dùng `memcpy` là chuyện bình thường. Nhưng trong Linux kernel, copy giữa kernel space và user space **không thể dùng `memcpy`** vì ba lý do:

**Thứ nhất — con trỏ user space có thể không hợp lệ.** User space application có thể truyền vào một con trỏ NULL, một địa chỉ chưa được map, hoặc cố tình truyền địa chỉ của kernel space. Nếu kernel dereference thẳng con trỏ đó, kết quả là kernel panic.

**Thứ hai — page có thể chưa có trong RAM.** Linux dùng lazy allocation — page của user space có thể đang bị swap ra disk. `copy_from_user` xử lý page fault một cách an toàn; `memcpy` thì không.

**Thứ ba — kiến trúc có thể yêu cầu cơ chế đặc biệt.** Trên một số kiến trúc, user space và kernel space dùng segment register khác nhau — copy trực tiếp bằng `memcpy` sẽ copy sai vùng nhớ.

`copy_from_user` và `copy_to_user` giải quyết cả ba vấn đề này:

```c
// Đọc dữ liệu từ user space vào kernel buffer
unsigned long copy_from_user(void *to,
                              const void __user *from,
                              unsigned long n);

// Ghi dữ liệu từ kernel buffer ra user space
unsigned long copy_to_user(void __user *to,
                            const void *from,
                            unsigned long n);

// Cả hai trả về số byte CHƯA copy được (0 = thành công hoàn toàn)
```

### 4.4 Code mẫu: driver "echo" dùng misc

Driver đơn giản nhất có thể viết: ghi gì đọc lại nấy.

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUF_SIZE 256

static char kbuf[BUF_SIZE];
static size_t kbuf_len = 0;

static ssize_t echo_read(struct file *filep, char __user *buf,
                          size_t len, loff_t *offset)
{
    if (len > kbuf_len)
        len = kbuf_len;

    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    return len;
}

static ssize_t echo_write(struct file *filep, const char __user *buf,
                           size_t len, loff_t *offset)
{
    if (len > max_size)
        len = max_size;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf_len = len;
    return len;
}

static struct file_operations echo_fops = {
    .owner = THIS_MODULE,
    .read  = echo_read,
    .write = echo_write,
};

static struct miscdevice echo_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "echo_demo",
    .fops  = &echo_fops,
};

static int __init echo_init(void)
{
    return misc_register(&echo_misc);
}

static void __exit echo_exit(void)
{
    misc_deregister(&echo_misc);
}

module_init(echo_init);
module_exit(echo_exit);
MODULE_LICENSE("GPL");
```

Test ngay không cần viết application:

```bash
$ insmod echo_demo.ko
$ echo "hello vinalinux" > /dev/echo_demo
$ cat /dev/echo_demo
hello vinalinux
```

---

## Phần 5 — ioctl (15 phút)

### 5.1 Khi nào cần ioctl thay vì read/write?

`read()` và `write()` phù hợp để truyền **dữ liệu**. Nhưng có những tình huống driver cần nhận **lệnh điều khiển** từ user space — không phải dữ liệu thuần túy:

- Set baud rate cho UART
- Set độ phân giải cho camera
- Bật/tắt một tính năng của device
- Query trạng thái hardware

Dùng `write()` để truyền lệnh điều khiển là có thể, nhưng phải tự định nghĩa protocol, tự parse — dễ lỗi và khó maintain. `ioctl` giải quyết bài toán này bằng cách cung cấp một kênh giao tiếp **có cấu trúc** giữa user space và driver.

### 5.2 `cmd` encode — ý nghĩa từng bit

Mỗi ioctl command là một số nguyên 32-bit được encode theo quy ước của kernel:

```
 31      30-29    28-16      15-8     7-0
┌──────┬────────┬─────────┬────────┬────────┐
│ (1b) │  dir   │  size   │  type  │ number │
│unused│ (2 bit)│ (13 bit)│ (8 bit)│ (8 bit)│
└──────┴────────┴─────────┴────────┴────────┘
```

- **type**: một ký tự ASCII định danh cho driver, ví dụ `'E'` cho echo_demo
- **number**: số thứ tự của command trong driver đó
- **size**: kích thước data truyền kèm
- **dir**: hướng truyền dữ liệu

Kernel cung cấp 4 macro để tạo command:

```c
_IO(type, nr)              // không truyền data
_IOR(type, nr, datatype)   // driver → user (read)
_IOW(type, nr, datatype)   // user → driver (write)
_IOWR(type, nr, datatype)  // hai chiều
```

### 5.3 Code mẫu — driver và userspace

Mở rộng driver "echo_demo" ở Phần 4: thêm biến config `max_size` kiểm soát số byte tối đa mỗi lần write. User space có thể đọc và ghi biến này qua ioctl.

**echo_demo.h** — dùng chung giữa driver và application:

```c
#define ECHO_MAGIC          'E'
#define ECHO_GET_MAX_SIZE   _IOR(ECHO_MAGIC, 0, unsigned int)
#define ECHO_SET_MAX_SIZE   _IOW(ECHO_MAGIC, 1, unsigned int)
```

**Driver side:**

```c
static unsigned int max_size = BUF_SIZE; // mặc định 256

static long echo_ioctl(struct file *filep, unsigned int cmd,
                        unsigned long arg)
{
    unsigned int val;

    switch (cmd) {
    case ECHO_GET_MAX_SIZE:
        if (copy_to_user((unsigned int __user *)arg, &max_size,
                         sizeof(max_size)))
            return -EFAULT;
        break;

    case ECHO_SET_MAX_SIZE:
        if (copy_from_user(&val, (unsigned int __user *)arg,
                           sizeof(val)))
            return -EFAULT;
        if (val == 0 || val > BUF_SIZE)
            return -EINVAL;
        max_size = val;
        pr_info("echo_demo: max_size set to %u\n", max_size);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}
```

Hàm `echo_write` cập nhật lại để tôn trọng `max_size`:

```c
static ssize_t echo_write(struct file *filep, const char __user *buf,
                           size_t len, loff_t *offset)
{
    if (len > max_size)
        len = max_size;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf_len = len;
    return len;
}
```

**User space side:**

```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "echo_demo.h"

int main(void)
{
    int fd = open("/dev/echo_demo", O_RDWR);

    unsigned int size;
    ioctl(fd, ECHO_GET_MAX_SIZE, &size);
    printf("max_size hiện tại: %u\n", size);

    size = 64;
    ioctl(fd, ECHO_SET_MAX_SIZE, &size);
    printf("max_size sau khi set: %u\n", size);

    close(fd);
    return 0;
}
```

Output khi chạy:

```
max_size hiện tại: 256
max_size sau khi set: 64
```

---

## Phần 6 — Linux Driver Template Pattern (15 phút)

### 6.1 Kernel định nghĩa "mẫu đơn" cho từng loại device

Nhìn lại những gì vừa làm ở Phần 3-5: để viết một character driver, có một danh sách việc bắt buộc phải làm theo đúng thứ tự — khai báo `struct file_operations`, điền các hàm vào, đăng ký với kernel. Không thể làm khác đi.

Đây không phải ngẫu nhiên. Kernel **quy định trước** một "mẫu đơn" cho từng loại device. Driver chỉ có một việc: **điền vào mẫu đó rồi nộp cho kernel**.

Analogy: giống như thủ tục hành chính — mỗi loại yêu cầu có một mẫu đơn riêng. Đăng ký kết hôn dùng mẫu khác đăng ký xe máy. Người nộp đơn không tự chọn format — cơ quan nhà nước đã quy định sẵn. Driver developer cũng vậy: kernel đã quy định format, developer điền vào.

### 6.2 Mỗi loại device — một mẫu đơn khác nhau

| Loại device | Struct template | Đăng ký với kernel |
|---|---|---|
| Character device | `struct file_operations` | `misc_register()` / `cdev_add()` |
| I2C device | `struct i2c_driver` | `i2c_add_driver()` |
| SPI device | `struct spi_driver` | `spi_register_driver()` |
| Platform device | `struct platform_driver` | `platform_driver_register()` |
| USB device | `struct usb_driver` | `usb_register()` |
| Network device | `struct net_device_ops` | `register_netdev()` |

Nhìn vào cột giữa: tất cả đều là **struct chứa con trỏ hàm**. Kernel gọi những hàm đó khi cần — driver không chủ động gọi kernel, kernel chủ động gọi driver. Đây là mô hình **inversion of control** — quen thuộc với ai đã làm callback trên MCU.

### 6.3 Tại sao kernel thiết kế vậy?

Có hai lý do cốt lõi:

**Tách biệt policy và mechanism.** Kernel quyết định *khi nào* gọi driver (policy). Driver quyết định *làm gì* với hardware (mechanism). Hai bên không phụ thuộc vào implementation của nhau — chỉ phụ thuộc vào interface được định nghĩa bởi struct template.

**Cho phép thay driver mà không sửa kernel.** Vì kernel chỉ biết đến struct template, có thể load một driver mới vào lúc runtime — kernel không cần biết driver đó làm gì bên trong, chỉ cần nó điền đúng mẫu đơn.

Đây chính là lý do Linux hỗ trợ hàng nghìn loại hardware khác nhau mà không cần rebuild kernel mỗi khi có hardware mới.

### 6.4 Ý nghĩa thực tế khi đọc driver mới

Khi gặp một driver lạ trong kernel source — ví dụ một I2C sensor driver — thay vì đọc từ đầu đến cuối, embedded developer có thể:

1. Xác định loại driver: I2C? SPI? Platform?
2. Tìm struct template tương ứng: `struct i2c_driver`
3. Tìm hàm đăng ký: `i2c_add_driver()`
4. Đọc các hàm được điền vào template: `probe`, `remove`, `id_table`

Bốn bước này cho phép nắm được skeleton của bất kỳ driver nào trong vài phút — trước khi đọc một dòng logic thực sự.

---

## Phần 7 — Quy tắc tư duy viết driver trên Linux (10 phút)

### 7.1 Vấn đề: embedded developer hay nhảy thẳng vào code

Thói quen từ MCU development là nhận datasheet, mở IDE, bắt đầu gõ. Trên MCU điều này hoạt động vì môi trường đơn giản — một chương trình, một CPU, không có lớp trung gian.

Trên Linux, nhảy thẳng vào code driver mà không hiểu đủ context dẫn đến một vòng lặp quen thuộc: viết → build lỗi → sửa → kernel panic → reboot → lặp lại. Mỗi vòng lặp mất nhiều thời gian hơn MCU vì debug kernel khó hơn debug bare-metal.

Quy tắc dưới đây giúp phá vỡ vòng lặp đó.

### 7.2 Bốn bước tư duy trước khi viết driver

**Bước 1 — Đọc hiểu tài liệu phần cứng**

Output cần đạt: hiểu hardware đến mức có thể code được trên MCU nếu cần.

Cụ thể: biết hardware giao tiếp qua giao thức gì (I2C, SPI, UART, memory-mapped register), biết các thanh ghi quan trọng và ý nghĩa từng bit, biết timing constraint nếu có, biết interrupt hay polling.

Nếu chưa đạt output này, mọi code driver viết ra chỉ là đoán mò.

**Bước 2 — Tìm hiểu quy tắc giao tiếp giữa app và hardware**

Output cần đạt: biết cách test driver sau khi code xong — không cần viết application phức tạp.

Cụ thể: hardware này thường được user space truy cập qua interface nào? `/dev/i2c-X` và `ioctl` với `I2C_RDWR`? `/dev/spidevX.Y` và `SPI_IOC_MESSAGE`? Hay một device file custom với read/write đơn giản?

Biết interface trước giúp developer viết driver với đúng API ngay từ đầu, đồng thời biết ngay câu lệnh test sau khi `insmod`.

**Bước 3 — Tìm template driver cho loại device đó**

Output cần đạt: có một driver hoạt động được làm điểm xuất phát — không bắt đầu từ trang trắng.

Cụ thể: tìm trong kernel source một driver của device khác cùng loại — cùng bus, cùng loại sensor, cùng class. Copy skeleton đó, giữ nguyên phần đăng ký template, chỉ thay phần logic hardware. Kernel source trên GitHub là nguồn tham khảo tốt nhất: `drivers/i2c/`, `drivers/spi/`, `drivers/char/`.

Đây là cách kernel developer thực tế làm — không ai viết driver từ đầu nếu đã có driver tương tự.

**Bước 4 — Code driver**

Chỉ đến bước này mới bắt đầu viết code thực sự. Lúc này developer đã có:
- Hiểu biết đầy đủ về hardware
- Biết interface cần implement
- Có skeleton làm điểm xuất phát
- Biết cách verify kết quả

Code viết ra ở bước này thường đúng ngay lần đầu — hoặc ít nhất fail theo cách có thể debug được.

### 7.3 Áp dụng vào bài thực hành hôm nay

| Bước | Áp dụng cho lab hôm nay |
|---|---|
| 1. Đọc tài liệu hardware | BBB GPIO register map — đã có từ bài Kernel Module |
| 2. Giao tiếp app ↔ hardware | Ghi `0`/`1` vào `/dev/vinalinux`, đọc về trạng thái — test bằng `echo` và `cat` |
| 3. Tìm template | Dùng misc device echo_demo vừa viết làm skeleton |
| 4. Code driver | Thêm GPIO init và toggle vào `write()`, đọc trạng thái trong `read()` |

---

## Phần 8 — Lab thực hành (15 phút)

### 8.1 Mục tiêu

Viết một misc device driver cho BeagleBone Black. Driver tạo ra `/dev/vinalinux` — user space ghi `1` để bật LED, ghi `0` để tắt LED, đọc về trạng thái hiện tại.

Áp dụng đúng 4 bước tư duy từ Phần 7.

### 8.2 Bước 1 — Hardware

LED trên BBB nối với **GPIO0_20**:
- GPIO0 base address: `0x44E07000`
- `GPIO_OE` offset `0x134` — cấu hình input/output (bit = 0 → output)
- `GPIO_DATAOUT` offset `0x13C` — set mức logic output

### 8.3 Bước 2 — Interface app ↔ driver

```bash
# Bật LED
echo "1" > /dev/vinalinux

# Tắt LED
echo "0" > /dev/vinalinux

# Đọc trạng thái
cat /dev/vinalinux
```

Không cần viết application — `echo` và `cat` là đủ để verify.

### 8.4 Bước 3 — Template

Dùng lại skeleton `echo_demo` từ Phần 4. Thay phần logic buffer bằng GPIO toggle.

### 8.5 Bước 4 — Code driver

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define GPIO0_BASE    0x44E07000
#define GPIO_OE       0x134
#define GPIO_DATAOUT  0x13C
#define GPIO0_20      (1 << 20)

static void __iomem *gpio_base;
static int led_state = 0;

static ssize_t vinalinux_read(struct file *filep, char __user *buf,
                               size_t len, loff_t *offset)
{
    char state = led_state ? '1' : '0';

    if (copy_to_user(buf, &state, 1))
        return -EFAULT;

    return 1;
}

static ssize_t vinalinux_write(struct file *filep, const char __user *buf,
                                size_t len, loff_t *offset)
{
    char val;

    if (copy_from_user(&val, buf, 1))
        return -EFAULT;

    if (val == '1') {
        led_state = 1;
        writel(readl(gpio_base + GPIO_DATAOUT) | GPIO0_20,
               gpio_base + GPIO_DATAOUT);
    } else if (val == '0') {
        led_state = 0;
        writel(readl(gpio_base + GPIO_DATAOUT) & ~GPIO0_20,
               gpio_base + GPIO_DATAOUT);
    } else {
        return -EINVAL;
    }

    return len;
}

static struct file_operations vinalinux_fops = {
    .owner = THIS_MODULE,
    .read  = vinalinux_read,
    .write = vinalinux_write,
};

static struct miscdevice vinalinux_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "vinalinux",
    .fops  = &vinalinux_fops,
};

static int __init vinalinux_init(void)
{
    gpio_base = ioremap(GPIO0_BASE, 0x1000);
    if (!gpio_base)
        return -ENOMEM;

    /* Cấu hình GPIO0_20 là output */
    writel(readl(gpio_base + GPIO_OE) & ~GPIO0_20,
           gpio_base + GPIO_OE);

    return misc_register(&vinalinux_misc);
}

static void __exit vinalinux_exit(void)
{
    misc_deregister(&vinalinux_misc);
    iounmap(gpio_base);
}

module_init(vinalinux_init);
module_exit(vinalinux_exit);
MODULE_LICENSE("GPL");
```

### 8.6 Build và test

```bash
# Build trên host
make -C /path/to/kernel M=$(pwd) modules

# Copy sang BBB và load
scp vinalinux.ko root@192.168.137.2:~
ssh root@192.168.137.2

insmod vinalinux.ko
ls -l /dev/vinalinux        # xác nhận device file đã xuất hiện

echo "1" > /dev/vinalinux   # LED sáng
cat /dev/vinalinux           # output: 1

echo "0" > /dev/vinalinux   # LED tắt
cat /dev/vinalinux           # output: 0

rmmod vinalinux
```

### 8.7 Take-home exercise

Thêm một cặp ioctl vào driver vừa viết:

- `VINALINUX_SET_BLINK` — nhận interval tính bằng millisecond, driver tự động nhấp nháy LED theo interval đó (gợi ý: dùng `kernel_thread` hoặc `timer_list`)
- `VINALINUX_STOP_BLINK` — dừng nhấp nháy, giữ nguyên trạng thái LED hiện tại
