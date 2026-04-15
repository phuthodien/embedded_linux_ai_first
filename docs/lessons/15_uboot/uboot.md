# Bài giảng: U-Boot — Bootloader cho Embedded Linux

**Giảng viên:** Lưu An Phú — FPT Software Corporate Training Center  
**Khóa học:** Lập trình Embedded Linux  
**Thời lượng:** 6 giờ (lý thuyết + thực hành)  
**Target board:** BeagleBone Black (AM335x)  
**U-Boot source:** `/home/phula/work/bbb/u-boot`

---

## MỤC LỤC

1. [Boot flow trên BeagleBone Black](#1-boot-flow-trên-beaglebone-black)
2. [Kiến trúc tổng quan của U-Boot](#2-kiến-trúc-tổng-quan-của-u-boot)
3. [Cách tra cứu và sử dụng command](#3-cách-tra-cứu-và-sử-dụng-command)
4. [Cách viết device tree cho U-Boot](#4-cách-viết-device-tree-cho-u-boot)
5. [Cách viết driver cho U-Boot](#5-cách-viết-driver-cho-u-boot)
6. [Cách viết command cho U-Boot](#6-cách-viết-command-cho-u-boot)
7. [Porting U-Boot cho board mới](#7-porting-u-boot-cho-board-mới)
8. [Các nhóm công việc thường gặp](#8-các-nhóm-công-việc-thường-gặp)
9. [Secure boot trên BBB](#9-secure-boot-trên-bbb)

---

## 1. Boot flow trên BeagleBone Black

### 1.1 Toàn cảnh quá trình boot

Từ khi bấm nút nguồn đến khi Linux kernel chạy, BBB trải qua **4 giai đoạn**:

```
Power ON
   │
   ▼
┌─────────────────────────────────┐
│  ROM Code (Boot ROM)            │  ← ghi cố định trong chip, không đổi được
│  địa chỉ: 0x20000 (internal)   │
│  kích thước: ~176KB             │
└────────────────┬────────────────┘
                 │ load MLO vào SRAM
                 ▼
┌─────────────────────────────────┐
│  MLO / SPL                      │  ← Second stage bootloader
│  chạy trong: internal SRAM      │    (~46KB available)
│  kích thước: ~104KB             │
└────────────────┬────────────────┘
                 │ khởi tạo DDR, load u-boot.img vào DDR
                 ▼
┌─────────────────────────────────┐
│  u-boot.img                     │  ← Third stage bootloader
│  chạy trong: DDR RAM            │
│  kích thước: ~968KB             │
└────────────────┬────────────────┘
                 │ load kernel + DTB vào DDR, jump vào kernel
                 ▼
┌─────────────────────────────────┐
│  Linux Kernel                   │
└─────────────────────────────────┘
```

### 1.2 Giai đoạn 1 — ROM Code (First stage bootloader)

ROM Code là chương trình **TI lập trình cố định vào chip**, tồn tại ngay cả khi flash trống rỗng. CPU nhảy vào đây ngay sau khi power on.

**Nhiệm vụ:**
- Khởi tạo clock tối thiểu, disable watchdog, cấu hình pinmux cơ bản
- Đọc **SYSBOOT pins** (15 chân) để biết boot từ thiết bị nào và theo thứ tự nào
- Load MLO từ thiết bị đó vào **internal SRAM** (46KB)
- Nhảy vào MLO

**SYSBOOT pins** — 15 chân GPIO trên SoC, trạng thái điện áp tại power-on quyết định:

| Nhóm chân | Thông tin |
|-----------|-----------|
| SYSBOOT[14:13] | Crystal frequency |
| SYSBOOT[12:11] | Pinmuxing mode |
| SYSBOOT[10:0]  | Boot device sequence |

Ví dụ boot sequence mặc định của BBB: **MMC1 (eMMC) → MMC0 (SD card) → UART0 → USB0**. Muốn boot từ SD card trước eMMC, phải thay đổi trạng thái chân SYSBOOT trước khi power on (thường thông qua một button hoặc jumper trên board).

> **Ràng buộc phần cứng quan trọng**: Vì ROM Code không thay đổi được, các thiết bị lưu MLO (SD card, eMMC, SPI Flash...) **phải nối đúng các chân pin mà ROM Code biết**. Khi thiết kế board mới, schematic phải tuân theo quy định này từ datasheet AM335x.

### 1.3 Giai đoạn 2 — MLO / SPL (Second stage bootloader)

**MLO** (Memory LOader) là tên file trên FAT partition. Bên trong là **SPL** (Secondary Program Loader) — một phiên bản rút gọn của U-Boot, build riêng với flag `CONFIG_SPL_BUILD`.

Tại sao cần SPL riêng? Vì khi ROM Code chạy, **DDR chưa được khởi tạo** — chỉ có 46KB internal SRAM. U-Boot proper (~968KB) không thể fit vào đó. SPL làm cầu nối: khởi tạo DDR trước, sau đó load U-Boot proper vào DDR.

**Luồng thực thi SPL:**

```
arch/arm/cpu/armv7/start.S   ← entry point (reset vector)
        │
        ▼
lowlevel_init()              ← tắt watchdog, khởi tạo PLL clock cơ bản
        │
        ▼
board_init_f()               ← [SPL version] khởi tạo UART để debug,
        │                       cấu hình SRAM layout
        ▼
board_init_r()               ← [SPL version] khởi tạo DDR SDRAM,
        │                       đọc u-boot.img từ MMC/UART/USB
        ▼
jump_to_image_no_args()      ← nhảy vào u-boot.img trong DDR
```

> **Note kỹ thuật**: `board_init_f` trong SPL và trong U-Boot proper là **hai hàm khác nhau** dù cùng tên. Macro `CONFIG_SPL_BUILD` được dùng để phân biệt lúc compile.

### 1.4 Giai đoạn 3 — u-boot.img (Third stage bootloader)

U-Boot proper chạy trong DDR với đầy đủ tài nguyên. Nhiệm vụ là chuẩn bị mọi thứ để boot Linux.

**Luồng thực thi U-Boot proper:**

```
arch/arm/cpu/armv7/start.S
        │
        ▼
board_init_f()               ← khởi tạo toàn bộ ngoại vi: UART, Ethernet,
        │                       USB, MMC, graphic...
        │                       khởi tạo malloc, stdio, filesystem
        ▼
relocate_code()              ← copy U-Boot từ địa chỉ load về cuối DDR
        │                       (nhường vùng RAM thấp cho kernel)
        ▼
board_init_r()               ← khởi tạo driver model, scan devices,
        │                       đọc environment variables, khởi tạo network
        ▼
main_loop()                  ← thực thi bootcmd hoặc vào interactive CLI
```

**Address relocation** — tại sao U-Boot tự di chuyển bản thân?

Kernel cần một vùng RAM liên tục lớn từ địa chỉ thấp. Nếu U-Boot nằm ở giữa sẽ cản trở kernel. Sau khi relocation:

```
DDR RAM layout (512MB BBB):
0x80000000  ┌────────────────┐
            │  Kernel space  │  ← kernel, initrd, DTB được load vào đây
            │                │
0x8FA00000  ├────────────────┤
            │  U-Boot        │  ← sau relocation, U-Boot thu mình về cuối DDR
0x8FFFFFFF  └────────────────┘
```

### 1.5 Giai đoạn 4 — U-Boot gọi Linux kernel

Sau khi `main_loop()` thực thi `bootcmd`, U-Boot:

1. Load `zImage` vào `0x82000000`
2. Load `am335x-boneblack.dtb` vào `0x88000000`
3. Gọi `bootz 0x82000000 - 0x88000000`

Bên trong `bootz`, U-Boot thiết lập **ARM boot protocol** rồi jump vào kernel:

```c
/* Trước khi jump, U-Boot set các register: */
r0 = 0                   /* phải bằng 0 theo convention */
r1 = machine type ID     /* MACH_TYPE_AM335XEVM */
r2 = địa chỉ DTB         /* 0x88000000 */

/* Sau đó jump vào entry point của kernel: */
kernel_entry(0, machid, r2)  /* → head.S của kernel */
```

Kernel `head.S` đọc `r2` để tìm DTB, parse DTB để biết phần cứng, rồi gọi `start_kernel()`.

---

## 2. Kiến trúc tổng quan của U-Boot

### 2.1 Mô hình phân lớp

```
┌──────────────────────────────────────────┐
│  Boot Script / uEnv.txt                  │  Lớp automation
├──────────────────────────────────────────┤
│  Command Line (cmd/)                     │  Lớp application
│  bootm, bootz, ext4load, env, mmc...     │
├──────────────────────────────────────────┤
│  Libraries (lib/, fs/)                   │  Lớp middleware
│  printf, malloc, FAT, EXT4, zlib...      │
├──────────────────────────────────────────┤
│  Driver Model — UCLASS (drivers/)        │  Lớp abstraction
│  SERIAL, MMC, GPIO, I2C, LED, ETH...     │
├──────────────────────────────────────────┤
│  Board / SoC specific (board/, arch/)    │  Lớp hardware
│  clock init, DDR, pinmux, EEPROM...      │
└──────────────────────────────────────────┘
```

### 2.2 Các thành phần chính

**Board specific code** — `board/ti/am335x/`

Code khởi tạo đặc thù của board: clock PLL, DDR timing, đọc EEPROM để biết board revision, cấu hình ethernet PHY... Đây là phần phải viết khi porting sang board mới.

**Device Tree** — `arch/arm/dts/am335x-boneblack.dts`

Mô tả phần cứng board. U-Boot dùng DT để driver model probe đúng device. DTB được **đính kèm trực tiếp vào `u-boot.img`** khi build — không load từ file riêng như Linux kernel.

**Driver** — `drivers/`

Điều khiển ngoại vi. U-Boot dùng **Driver Model (DM)** — framework tương tự Linux với UCLASS, probe/remove lifecycle. Ví dụ: `drivers/led/led_gpio.c` điều khiển LED qua GPIO.

**Command** — `cmd/`

Mỗi lệnh CLI là một file `.c` độc lập, khai báo bằng macro `U_BOOT_CMD`. Ví dụ `cmd/led.c` cung cấp lệnh `led on/off/toggle`.

**Environment variable** — `env/`

Cơ chế cấu hình linh hoạt. Lưu vào flash (eMMC, SPI) để tồn tại qua reboot. Các biến quan trọng:

| Biến | Ý nghĩa |
|------|---------|
| `bootdelay` | Số giây countdown trước autoboot |
| `bootcmd` | Lệnh thực thi khi autoboot |
| `bootargs` | Tham số truyền cho kernel (`console=`, `root=`...) |
| `ipaddr`, `serverip` | Cấu hình mạng cho TFTP boot |
| `fdtfile` | Tên file DTB |

**Boot script và uEnv.txt trên BBB — cơ chế thực sự**

Trên BBB, `bootcmd` được hardcode trong U-Boot binary (định nghĩa tại `include/configs/am335x_evm.h`):

```
bootcmd = run findfdt; run init_console; run envboot; run distro_bootcmd
```

Trong đó `envboot` — cũng là script hardcode trong `include/environment/ti/mmc.h` — làm việc như sau:

```
envboot:
  1. Load uEnv.txt từ SD/eMMC vào RAM
  2. Import các biến trong uEnv.txt vào U-Boot environment
  3. Nếu có biến "uenvcmd" trong uEnv.txt → chạy nó (user override hoàn toàn)
  4. Nếu không có → fall through, tiếp tục chạy distro_bootcmd
```

**`uEnv.txt` trên BBB thực tế chỉ chứa `uname_r`:**

```
uname_r=5.10.168-ti-r61
```

Đây là vì toàn bộ logic boot (`EEWIKI_UNAME_BOOT`) đã được hardcode trong U-Boot — nó tự xây dựng đường dẫn `vmlinuz-${uname_r}`, tìm DTB trong `/boot/dtbs/${uname_r}/`... `uEnv.txt` chỉ cần cung cấp kernel version để điền vào biến đó.

Người dùng có thể override hoàn toàn bằng cách thêm `uenvcmd` vào `uEnv.txt`:

```bash
# uEnv.txt — override boot hoàn toàn
uname_r=5.10.168-ti-r61
uenvcmd=load mmc 0:1 ${loadaddr} /boot/zImage; load mmc 0:1 ${fdtaddr} /boot/my-custom.dtb; bootz ${loadaddr} - ${fdtaddr}
```

### 2.3 Demo một số command cơ bản

Kết nối serial console 115200 8N1, nhấn phím bất kỳ trong countdown để vào shell:

```
U-Boot 2021.xx (...)

Hit any key to stop autoboot: 3
=>
```

```bash
# Xem thông tin board và memory layout
=> bdinfo

# Xem tất cả environment variable
=> printenv

# Xem danh sách file trên SD card (device 0, partition 1)
=> ls mmc 0:1 /boot

# Đọc thanh ghi (memory dump)
=> md.l 0x44E10040 4

# Ghi thanh ghi
=> mw.l 0x44E10040 0x00000031

# Probe I2C bus 0
=> i2c probe

# Ping kiểm tra network
=> setenv ipaddr 192.168.1.100
=> setenv serverip 192.168.1.1
=> ping 192.168.1.1

# Xem danh sách LED được khai báo trong DT
=> led list
```

---

## 3. Cách tra cứu và sử dụng command

### 3.1 Help system tích hợp

U-Boot có help system đầy đủ ngay trên command line:

```bash
# Liệt kê tất cả command có sẵn
=> help

# Xem cú pháp chi tiết của một command
=> help led
led - manage LEDs

Usage:
led <led_label> on|off|toggle   Change LED state
led <led_label>                  Get LED state
led list                         show a list of LEDs

# Viết tắt: ? tương đương help
=> ? mmc
```

### 3.2 Tra cứu trong source code

Khi cần hiểu sâu hơn hoặc tìm các option ẩn, tìm trong `cmd/`:

```bash
# Tìm file implement command 'mmc'
ls /home/phula/work/bbb/u-boot/cmd/ | grep mmc

# Xem định nghĩa U_BOOT_CMD để biết đầy đủ syntax và subcmd
grep -n "U_BOOT_CMD" /home/phula/work/bbb/u-boot/cmd/mmc.c
```

### 3.3 Tra cứu CONFIG để bật/tắt command

Mỗi command được kiểm soát bởi một `CONFIG_CMD_*`:

```bash
# Xem command nào đang được bật trong config hiện tại
grep "CONFIG_CMD_" /home/phula/work/bbb/u-boot/.config | grep "=y"

# Bật command qua menuconfig
make ARCH=arm menuconfig
# → Command line interface → Individual command configuration
```

### 3.4 Các command thường dùng nhất

| Nhóm | Command | Mô tả |
|------|---------|-------|
| **Storage** | `mmc list`, `mmc dev`, `mmc info` | Quản lý MMC/SD |
| | `ls mmc 0:1 /boot` | Liệt kê file |
| | `load mmc 0:1 <addr> <file>` | Load file vào RAM |
| **Memory** | `md.l <addr> <count>` | Đọc memory (32-bit) |
| | `mw.l <addr> <val>` | Ghi memory |
| | `cp.l <src> <dst> <count>` | Copy memory |
| **Network** | `dhcp` | Lấy IP qua DHCP |
| | `tftp <addr> <file>` | Load file từ TFTP server |
| **Boot** | `bootz <zImage_addr> - <dtb_addr>` | Boot zImage |
| | `bootm <uImage_addr>` | Boot uImage |
| | `run <var>` | Thực thi script trong biến env |
| **Env** | `printenv [var]` | In environment |
| | `setenv <var> <val>` | Đặt biến |
| | `saveenv` | Lưu env vào flash |
| **GPIO** | `gpio set <pin>`, `gpio clear <pin>` | Điều khiển GPIO |
| | `gpio input <pin>` | Đọc GPIO |
| **I2C** | `i2c probe` | Scan I2C bus |
| | `i2c md <chip> <addr>` | Đọc I2C register |

---

## 4. Cách viết device tree cho U-Boot

### 4.1 DTS trong U-Boot vs Linux

U-Boot dùng device tree cú pháp giống hệt Linux, nhưng có một số điểm khác:

| | Linux | U-Boot |
|--|-------|--------|
| File DTB | Build riêng, load qua bootloader | **Nhúng vào `u-boot.img`** |
| File `.dtsi` overlay | `arch/arm/boot/dts/` | `arch/arm/dts/` + thêm file `*-u-boot.dtsi` |
| Property đặc biệt | Không | `u-boot,dm-pre-reloc` — probe trước relocation |
| Ngoại vi hardcode | Không | UART debug phải hardcode trước khi DT init |

**File `-u-boot.dtsi`** — overlay chỉ dành riêng cho U-Boot, thêm các property U-Boot cần nhưng Linux không dùng:

```
arch/arm/dts/
├── am335x-boneblack.dts         ← file chính, include cả Linux lẫn U-Boot node
├── am335x-bone-common.dtsi      ← shared giữa bone và boneblack
├── am335x-boneblack-common.dtsi ← đặc thù boneblack
└── am33xx-u-boot.dtsi           ← U-Boot specific properties
```

### 4.2 Property `u-boot,dm-pre-reloc`

Driver cần hoạt động **trước khi U-Boot relocation** (ví dụ UART debug console) phải được đánh dấu:

```dts
&uart0 {
    u-boot,dm-pre-reloc;  /* probe UART ngay từ đầu, không chờ relocation */
    status = "okay";
};
```

Không có property này, driver sẽ chỉ được probe sau relocation — quá trễ cho debug console.

### 4.3 Ví dụ thực tế: khai báo LED trong DTS

Đây là thay đổi thực tế được thực hiện trên source BBB (`git diff arch/arm/dts/am335x-boneblack.dts`):

```diff
--- a/arch/arm/dts/am335x-boneblack.dts
+++ b/arch/arm/dts/am335x-boneblack.dts
@@ -11,6 +11,27 @@
 / {
 	model = "TI AM335x BeagleBone Black";
 	compatible = "ti,am335x-bone-black", "ti,am335x-bone", "ti,am33xx";
+
+	demo_leds {
+		pinctrl-names = "default";
+		pinctrl-0 = <&demo_leds>;
+
+		compatible = "demo-uboot-leds";   /* khớp với driver demo_led.c */
+
+		used_led@1 {
+				label = "led-1_14";                   /* tên dùng trong command */
+				gpios = <&gpio1 14 GPIO_ACTIVE_HIGH>; /* gpio1, pin 14 */
+				default-state = "off";
+		};
+	};
+};
+
+&am33xx_pinmux {
+	demo_leds: demo_leds {
+		pinctrl-single,pins = <
+			0x38 (PIN_OUTPUT_PULLDOWN | MUX_MODE7) /* gpmc_ad14.gpio1_14 */
+		>;
+	};
 };
```

**Giải thích từng phần:**

- `compatible = "demo-uboot-leds"` — chuỗi này phải khớp với `udevice_id` trong driver (`drivers/demo_led/demo_led.c`). U-Boot dùng đây để biết driver nào sẽ handle node này.
- `pinctrl-0 = <&demo_leds>` — trỏ đến pinmux group bên dưới, đảm bảo chân GPIO được set đúng mode trước khi driver probe.
- `0x38` — offset của pad control register cho chân `gpmc_ad14`. Tra trong TRM AM335x, chương "Control Module" → "Pad Control Registers". Với `MUX_MODE7` → chân hoạt động ở GPIO mode → `gpio1[14]`.
- `gpios = <&gpio1 14 GPIO_ACTIVE_HIGH>` — khai báo GPIO controller (`gpio1`), số pin (`14`), và active level. Driver đọc property này qua `gpio_request_by_name()`.

**Sau khi build và flash, kiểm tra trên U-Boot shell:**

```
=> demo_led show
led-1_14

=> demo_led led-1_14 on
=> demo_led led-1_14 off
=> demo_led led-1_14 toggle
```

### 4.4 Quy trình chỉnh sửa DTS và build

```bash
cd /home/phula/work/bbb/u-boot

# Chỉnh sửa DTS
vim arch/arm/dts/am335x-boneblack.dts

# Build lại (chỉ cần build phần DTS, nhanh hơn full build)
export CROSS_COMPILE=/home/phula/work/bbb/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
make ARCH=arm -j$(nproc)

# Copy lên board
scp MLO u-boot.img debian@192.168.137.2:/home/debian/
```

---

## 5. Cách viết driver cho U-Boot

### 5.1 Driver Model (DM) — Khung xương của mọi driver

U-Boot Driver Model có 3 khái niệm cốt lõi:

- **UCLASS**: Giao diện chuẩn cho một loại thiết bị (`UCLASS_LED`, `UCLASS_SERIAL`, `UCLASS_MMC`...)
- **UDEVICE**: Một instance của device cụ thể (ví dụ LED gpio1_14)
- **Driver**: Code điều khiển phần cứng, implement các ops của UCLASS

```
UCLASS_LED (interface)
    │
    ├── led_gpio driver  ←─ led_gpio.c (hardware agnostic: bất kỳ GPIO LED nào)
    │       │
    │       ├── device: "led-1_14"  (gpio1 pin 14)
    │       └── device: "led-1_15"  (gpio1 pin 15)
    │
    └── led_bcm6328 driver  ←─ led_bcm6328.c (BCM6328 specific)
```

### 5.2 Phân tích driver `led_gpio.c`

Đây là driver LED đơn giản nhất, phù hợp để học cấu trúc DM driver. File: `drivers/led/led_gpio.c`

**Struct private data:**

```c
struct led_gpio_priv {
    struct gpio_desc gpio;  /* mô tả GPIO pin: controller + số pin + flags */
};
```

**Probe function** — gọi khi device được activate:

```c
static int led_gpio_probe(struct udevice *dev)
{
    struct led_uc_plat *uc_plat = dev_get_uclass_plat(dev);
    struct led_gpio_priv *priv = dev_get_priv(dev);

    /* Bỏ qua node gốc "gpio-leds", chỉ xử lý child node */
    if (!uc_plat->label)
        return 0;

    /* Lấy GPIO từ device tree property "gpios = <&gpio1 14 ...>" */
    return gpio_request_by_name(dev, "gpios", 0, &priv->gpio, GPIOD_IS_OUT);
}
```

**Ops functions** — implement interface của UCLASS_LED:

```c
static int gpio_led_set_state(struct udevice *dev, enum led_state_t state)
{
    struct led_gpio_priv *priv = dev_get_priv(dev);

    switch (state) {
    case LEDST_TOGGLE:
        /* đọc state hiện tại rồi đảo */
        state = !dm_gpio_get_value(&priv->gpio);
        break;
    default:
        break;
    }
    return dm_gpio_set_value(&priv->gpio, state);
}
```

**Bind function** — tạo child device cho mỗi LED node trong DT:

```c
static int led_gpio_bind(struct udevice *parent)
{
    struct udevice *dev;
    ofnode node;

    /* Duyệt tất cả child node (mỗi node = một LED) */
    dev_for_each_subnode(node, parent) {
        const char *label = ofnode_read_string(node, "label");
        device_bind_driver_to_node(parent, "gpio_led",
                                   ofnode_get_name(node), node, &dev);
        dev_get_uclass_plat(dev)->label = label;
    }
    return 0;
}
```

**Khai báo driver với U-Boot:**

```c
/* compatible phải khớp với DTS: compatible = "gpio-leds" */
static const struct udevice_id led_gpio_ids[] = {
    { .compatible = "gpio-leds" },
    { }  /* sentinel */
};

U_BOOT_DRIVER(led_gpio) = {
    .name          = "gpio_led",
    .id            = UCLASS_LED,         /* thuộc UCLASS_LED */
    .of_match      = led_gpio_ids,       /* match với DTS compatible */
    .ops           = &gpio_led_ops,      /* implement set/get state */
    .priv_auto     = sizeof(struct led_gpio_priv),
    .bind          = led_gpio_bind,      /* tạo child device */
    .probe         = led_gpio_probe,     /* khởi tạo hardware */
    .remove        = led_gpio_remove,    /* cleanup */
};
```

### 5.3 Viết driver LED mới từ đầu (ví dụ: LED điều khiển qua I2C)

Giả sử có LED controller I2C tại địa chỉ 0x60. Cấu trúc driver:

**Bước 1 — Tạo file `drivers/led/led_i2c_demo.c`:**

```c
// SPDX-License-Identifier: GPL-2.0+
#include <common.h>
#include <dm.h>
#include <led.h>
#include <i2c.h>
#include <asm/gpio.h>

/* Register map của LED controller giả định */
#define LED_CTRL_REG    0x00
#define LED_ON          0x01
#define LED_OFF         0x00

struct led_i2c_priv {
    struct udevice *i2c_dev;  /* I2C device handle */
    u8 channel;               /* kênh LED trên controller */
};

static int led_i2c_set_state(struct udevice *dev, enum led_state_t state)
{
    struct led_i2c_priv *priv = dev_get_priv(dev);
    u8 val = (state == LEDST_ON) ? LED_ON : LED_OFF;

    return dm_i2c_write(priv->i2c_dev, LED_CTRL_REG + priv->channel, &val, 1);
}

static enum led_state_t led_i2c_get_state(struct udevice *dev)
{
    struct led_i2c_priv *priv = dev_get_priv(dev);
    u8 val;

    dm_i2c_read(priv->i2c_dev, LED_CTRL_REG + priv->channel, &val, 1);
    return val ? LEDST_ON : LEDST_OFF;
}

static int led_i2c_probe(struct udevice *dev)
{
    struct led_uc_plat *uc_plat = dev_get_uclass_plat(dev);
    struct led_i2c_priv *priv = dev_get_priv(dev);

    if (!uc_plat->label)
        return 0;

    /* Lấy I2C device từ parent node trong DT */
    priv->i2c_dev = dev_get_parent(dev);
    priv->channel = dev_read_u32_default(dev, "channel", 0);
    return 0;
}

static const struct led_ops led_i2c_ops = {
    .set_state = led_i2c_set_state,
    .get_state = led_i2c_get_state,
};

static const struct udevice_id led_i2c_ids[] = {
    { .compatible = "demo,i2c-leds" },
    { }
};

U_BOOT_DRIVER(led_i2c_demo) = {
    .name      = "led_i2c_demo",
    .id        = UCLASS_LED,
    .of_match  = led_i2c_ids,
    .ops       = &led_i2c_ops,
    .priv_auto = sizeof(struct led_i2c_priv),
    .probe     = led_i2c_probe,
};
```

**Bước 2 — Thêm vào `drivers/led/Makefile`:**

```makefile
obj-$(CONFIG_LED_I2C_DEMO) += led_i2c_demo.o
```

**Bước 3 — Thêm vào `drivers/led/Kconfig`:**

```kconfig
config LED_I2C_DEMO
    bool "Demo I2C LED driver"
    depends on DM_LED && DM_I2C
    help
      Driver for demo I2C LED controller.
```

**Bước 4 — Khai báo DTS:**

```dts
&i2c1 {
    led_ctrl: led-controller@60 {
        compatible = "demo,i2c-leds";
        reg = <0x60>;

        led_red {
            label = "red";
            channel = <0>;
        };
        led_green {
            label = "green";
            channel = <1>;
        };
    };
};
```

---

## 6. Cách viết command cho U-Boot

### 6.1 Cấu trúc một command

Mọi U-Boot command đều có cấu trúc giống nhau. Phân tích `cmd/demo_led.c` — file có sẵn trong source:

```c
/* Hàm thực thi chính của command */
int do_demo_led(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
    /* argc: số argument (bao gồm tên command)
     * argv[0]: tên command ("demo_led")
     * argv[1]: argument đầu tiên
     * argv[2]: argument thứ hai
     */

    if (argc < 2)
        return CMD_RET_USAGE;  /* in usage string và return */

    /* ... xử lý logic ... */

    return 0;             /* thành công */
    /* return CMD_RET_FAILURE;  khi thất bại */
}

/* Khai báo command với U-Boot */
U_BOOT_CMD(
    demo_led,   /* tên command — gõ trên CLI */
    4,          /* số argument tối đa (bao gồm tên command) */
    1,          /* repeatable: 1 = có thể giữ Enter để lặp lại */
    do_demo_led,/* handler function */
    "manage DEMO LEDs",                                    /* short help */
    "<led_label> on|off|toggle\tChange LED state\n"        /* long help */
    "demo_led <led_label>\tGet LED state\n"
    "demo_led show\t\tShow LED labels"
);
```

### 6.2 Ví dụ: Viết command `myled` điều khiển LED

Tạo file `cmd/myled.c` — command đơn giản để bật/tắt LED theo label:

```c
// SPDX-License-Identifier: GPL-2.0+
/*
 * myled command — simple LED control for training purpose
 */
#include <common.h>
#include <command.h>
#include <dm.h>
#include <led.h>

static int do_myled(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
    struct udevice *dev;
    const char *label;
    int ret;

    if (argc < 3)
        return CMD_RET_USAGE;

    label = argv[1];

    /* Tìm LED device theo label khai báo trong DT */
    ret = led_get_by_label(label, &dev);
    if (ret) {
        printf("LED '%s' not found\n", label);
        return CMD_RET_FAILURE;
    }

    /* Xử lý subcmd */
    if (!strcmp(argv[2], "on")) {
        ret = led_set_state(dev, LEDST_ON);
    } else if (!strcmp(argv[2], "off")) {
        ret = led_set_state(dev, LEDST_OFF);
    } else if (!strcmp(argv[2], "toggle")) {
        ret = led_set_state(dev, LEDST_TOGGLE);
    } else {
        printf("Unknown action: %s\n", argv[2]);
        return CMD_RET_USAGE;
    }

    if (ret) {
        printf("Failed to set LED state (err=%d)\n", ret);
        return CMD_RET_FAILURE;
    }

    return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
    myled, 3, 0, do_myled,
    "control LED by label",
    "<label> on|off|toggle\n"
    "  label  - LED label from device tree\n"
    "  on     - turn LED on\n"
    "  off    - turn LED off\n"
    "  toggle - toggle LED state"
);
```

### 6.3 Kích hoạt command trong build

**Bước 1 — Thêm vào `cmd/Makefile`:**

```makefile
obj-$(CONFIG_CMD_MYLED) += myled.o
```

**Bước 2 — Thêm vào `cmd/Kconfig`:**

```kconfig
config CMD_MYLED
    bool "myled command"
    depends on LED
    help
      Command to control LEDs by label for training purpose.
```

**Bước 3 — Bật trong defconfig hoặc menuconfig:**

```bash
# Thêm vào configs/am335x_evm_defconfig
echo "CONFIG_CMD_MYLED=y" >> configs/am335x_evm_defconfig

# Hoặc dùng menuconfig:
make ARCH=arm menuconfig
# → Command line interface → myled command
```

**Bước 4 — Build và test:**

```bash
make ARCH=arm -j$(nproc)
scp u-boot.img MLO debian@192.168.137.2:/home/debian/

# Trên board, sau khi flash và reboot:
=> myled led-1_14 on
=> myled led-1_14 toggle
=> myled led-1_14 off
```

### 6.4 Subcmd pattern — Command có nhiều sub-command

Khi command có nhiều subcmd phức tạp (như `mmc`, `i2c`), dùng pattern `MK_CMD_TBL`:

```c
static struct cmd_tbl myled_subcmds[] = {
    U_BOOT_CMD_MKENT(on,     2, 0, do_myled_on,     "", ""),
    U_BOOT_CMD_MKENT(off,    2, 0, do_myled_off,    "", ""),
    U_BOOT_CMD_MKENT(toggle, 2, 0, do_myled_toggle, "", ""),
    U_BOOT_CMD_MKENT(status, 2, 0, do_myled_status, "", ""),
};

static int do_myled(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
    struct cmd_tbl *cp;

    if (argc < 2)
        return CMD_RET_USAGE;

    /* Dispatch đến subcmd tương ứng */
    cp = find_cmd_tbl(argv[1], myled_subcmds, ARRAY_SIZE(myled_subcmds));
    if (!cp)
        return CMD_RET_USAGE;

    return cp->cmd(cmdtp, flag, argc - 1, argv + 1);
}
```

---

## 7. Porting U-Boot cho board mới

### 7.1 Khi nào cần porting

- Board custom dùng SoC AM335x (hoặc AM57xx, i.MX8...) nhưng layout phần cứng khác BBB
- DDR khác loại / dung lượng / timing
- Peripheral kết nối ở chân khác
- Thêm/bớt ngoại vi so với board reference

**Nguyên tắc**: Luôn bắt đầu từ board gần nhất (cùng SoC), đừng tạo từ đầu.

### 7.2 Quy trình từng bước

**Bước 1 — Tạo board directory**

```bash
mkdir -p /home/phula/work/bbb/u-boot/board/mycompany/myboard/
```

File cần tạo:
```
board/mycompany/myboard/
├── Makefile
├── Kconfig
├── myboard.c     ← board init code (copy từ board/ti/am335x/board.c rồi sửa)
└── myboard.h
```

Nội dung `Makefile`:
```makefile
obj-y = myboard.o
```

**Bước 2 — Tạo defconfig**

```bash
cp configs/am335x_evm_defconfig configs/myboard_defconfig
```

Chỉnh sửa `configs/myboard_defconfig`:
```
CONFIG_DEFAULT_DEVICE_TREE="myboard"   ← tên file DTS
CONFIG_TARGET_MYBOARD=y                ← target mới
# Bỏ CONFIG_TARGET_AM335X_EVM=y
```

**Bước 3 — Khai báo target trong Kconfig**

Trong `arch/arm/mach-omap2/Kconfig`, thêm:
```kconfig
config TARGET_MYBOARD
    bool "My custom board"
    depends on AM33XX
    select BOARD_LATE_INIT
    help
      My custom board based on TI AM335x SoC.
```

**Bước 4 — Viết device tree**

```bash
# Tạo file DTS
vim /home/phula/work/bbb/u-boot/arch/arm/dts/myboard.dts
```

```dts
/dts-v1/;
#include "am33xx.dtsi"
#include "am335x-bone-common.dtsi"  /* tái sử dụng common BBB config */

/ {
    model = "My Custom Board";
    compatible = "mycompany,myboard", "ti,am33xx";

    memory@80000000 {
        device_type = "memory";
        reg = <0x80000000 0x20000000>;  /* 512MB */
    };
};

/* Khai báo ngoại vi board cần */
&uart0 {
    u-boot,dm-pre-reloc;
    status = "okay";
};

&mmc1 {
    status = "okay";
    bus-width = <4>;
    pinctrl-0 = <&mmc1_pins>;
    pinctrl-names = "default";
    cd-gpios = <&gpio0 6 GPIO_ACTIVE_LOW>;
};
```

Thêm vào `arch/arm/dts/Makefile`:
```makefile
dtb-$(CONFIG_TARGET_MYBOARD) += myboard.dtb
```

**Bước 5 — Chỉnh sửa board init code**

Trong `board/mycompany/myboard/myboard.c`, các hàm cần implement:

```c
/* Gọi rất sớm trong SPL — khởi tạo clock và DDR */
void am33xx_spl_board_init(void)
{
    /* Cấu hình VDD_CORE và VDD_MPU voltage qua PMIC */
    /* Khởi tạo DDR timing cho loại DDR trên board */
}

/* Khởi tạo sau khi DDR ready */
int board_init(void)
{
    gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
    return 0;
}

/* Khởi tạo muộn — sau khi driver model ready */
int board_late_init(void)
{
    /* Set environment variables dựa theo hardware detect */
    env_set("board_name", "myboard");
    return 0;
}
```

**Bước 6 — Phần khó nhất: DDR timing**

DDR là phần phức tạp nhất khi porting. Sai timing → board không boot hoặc memory error ngẫu nhiên.

Quy trình:
1. Lấy datasheet DDR chip đang dùng
2. Dùng **TI EMIF Tools** để tính timing registers từ datasheet
3. Điền vào struct `ddr3_data` trong board code:

```c
static const struct ddr3_data ddr3 = {
    .ioctrl_value   = 0x18B,
    .datawdsratio0  = K4B2G1646EBIH9_RD_DQS,
    .datatec        = K4B2G1646EBIH9_PHY_FIFO_WE,
    /* ... nhiều register khác ... */
};
```

### 7.3 Checklist porting

- [ ] UART console hoạt động (đây là bước đầu tiên phải xong)
- [ ] SPL boot thành công (MLO load được u-boot.img)
- [ ] DDR khởi tạo đúng (chạy `memtest` để verify)
- [ ] MMC/SD card đọc được
- [ ] Environment lưu/đọc được
- [ ] Network hoạt động (nếu board có Ethernet)
- [ ] Kernel boot thành công

---

## 8. Các nhóm công việc thường gặp

### 8.1 Rescue board bị brick

**Tình huống**: Flash u-boot.img bị hỏng, board không boot.

**Giải pháp — Boot qua UART (xmodem):**

```bash
# Trên host — chuẩn bị
apt install lrzsz minicom

# SYSBOOT pin set về UART boot mode
# Kết nối UART0, mở minicom 115200 8N1
minicom -b 115200 -D /dev/ttyUSB0

# ROM Code sẽ gửi ký tự 'C' liên tục khi sẵn sàng nhận
# Gửi MLO qua xmodem:
Ctrl+A → S → xmodem → chọn file MLO

# Sau khi MLO chạy, gửi tiếp u-boot.img
# Trong U-Boot shell, flash lại eMMC:
=> mmc dev 1
=> fatload uart 0 0x82000000 u-boot.img  /* hoặc dùng tftp */
=> mmc write 0x82000000 0x300 0x400
```

**Giải pháp — Boot từ SD card:**

```bash
# Chuẩn bị SD card với MLO + u-boot.img
dd if=/dev/zero of=/dev/sdX bs=1M count=1
# Tạo FAT partition, copy MLO và u-boot.img vào
# Cắm SD card + boot BBB (giữ boot button để force SD boot)
```

### 8.2 Tăng tốc vòng lặp development

**Tình huống**: Đang phát triển kernel driver, phải flash kernel mới liên tục — rất chậm.

**Giải pháp — Boot kernel qua TFTP:**

```bash
# Trên host: cài và cấu hình TFTP server
apt install tftpd-hpa
cp zImage /srv/tftp/
cp am335x-boneblack.dtb /srv/tftp/

# Trên U-Boot:
=> setenv serverip 192.168.137.1
=> setenv ipaddr 192.168.137.2
=> dhcp
=> tftp 0x82000000 zImage
=> tftp 0x88000000 am335x-boneblack.dtb
=> setenv bootargs console=ttyO0,115200n8 root=/dev/mmcblk0p2 rw
=> bootz 0x82000000 - 0x88000000

# Lưu thành script để chạy 1 lệnh:
=> setenv netboot 'dhcp; tftp ${loadaddr} zImage; tftp ${fdtaddr} am335x-boneblack.dtb; bootz ${loadaddr} - ${fdtaddr}'
=> setenv bootcmd 'run netboot'
=> saveenv
```

### 8.3 Flash firmware lên eMMC

**Tình huống**: Cần burn image lên eMMC cho sản phẩm.

```bash
# Boot từ SD card trước
# Trong U-Boot shell:

# Kiểm tra eMMC
=> mmc list
=> mmc dev 1  /* eMMC thường là device 1 */
=> mmc info

# Load image từ SD (mmc 0) lên RAM
=> load mmc 0:1 0x82000000 /boot/image.img

# Ghi vào eMMC (sector 0x300 = offset 384KB, tránh partition table)
=> mmc dev 1
=> mmc write 0x82000000 0x300 <số_sector>
```

### 8.4 A/B boot — Fallback khi update firmware thất bại

**Tình huống**: OTA update thất bại → board brick → không thể remote recover.

**Giải pháp — A/B partition với fallback:**

```bash
# uEnv.txt implement A/B boot logic
uboot_overlay_addr0=/lib/firmware/BB-BONE-EMMC-2GB-00A0.dtbo

# Boot script kiểm tra boot_count
bootcmd=\
  if test ${boot_count} -gt 3; then \
    echo "Boot failed ${boot_count} times, falling back to slot B"; \
    setenv active_slot b; \
    saveenv; \
    reset; \
  fi; \
  setexpr boot_count ${boot_count} + 1; \
  saveenv; \
  run boot_slot_${active_slot}
```

### 8.5 Debug hardware sớm

**Tình huống**: Board mới, không rõ lỗi là hardware hay software.

```bash
# Đọc/ghi register trực tiếp để test
=> md.l 0x44E10810 1    /* đọc UART0 THR register */
=> mw.l 0x44E10810 0x41 /* ghi ký tự 'A' trực tiếp vào UART */

# Probe I2C để kiểm tra sensor có respond không
=> i2c dev 0
=> i2c probe
0x1C: 0x38: 0x50:  /* địa chỉ các device đang respond */

# Test GPIO
=> gpio set 53     /* gpio1_21 = 32*1 + 21 = 53 → bật LED USR0 */
=> gpio clear 53
```

---

## 9. Secure boot trên BBB

### 9.1 Bài toán secure boot giải quyết gì

Không có secure boot: attacker cắm SD card với bootloader giả → chạy code tùy ý → truy cập toàn bộ hệ thống.

Secure boot đảm bảo **chain of trust**: mỗi giai đoạn chỉ chạy nếu được xác nhận bởi giai đoạn trước.

```
ROM Code (trust anchor)
    │ verify chữ ký
    ▼
MLO/SPL (signed)
    │ verify chữ ký
    ▼
u-boot.img (signed)
    │ verify chữ ký
    ▼
Linux kernel (signed)
```

### 9.2 Cơ chế secure boot trên AM335x

AM335x hỗ trợ hai chế độ:
- **GP (General Purpose)**: Không secure, dùng trong development — BBB mặc định ở chế độ này
- **HS (High Security)**: Secure boot enabled, ROM Code chỉ chạy image đã ký

> **Thực tế**: TI yêu cầu ký NDA để nhận tài liệu và tool cho HS device. Phần này trình bày cơ chế conceptual và cách dùng **FIT image** — một cơ chế verify khả dụng ngay trên GP device.

### 9.3 FIT Image — Secure boot ở tầng U-Boot

**FIT (Flattened Image Tree)** là format image của U-Boot cho phép:
- Đóng gói kernel + DTB + initrd vào một file
- Ký và verify bằng RSA
- Không cần HS device — chạy được trên BBB thông thường

**Tạo RSA key pair:**

```bash
mkdir -p /tmp/keys
openssl genrsa -F4 -out /tmp/keys/dev.key 2048
openssl req -batch -new -x509 -key /tmp/keys/dev.key \
    -out /tmp/keys/dev.crt
```

**Tạo FIT image descriptor (`.its` file):**

```
/dts-v1/;
/ {
    description = "BBB FIT Image";
    #address-cells = <1>;

    images {
        kernel {
            description = "Linux kernel";
            data = /incbin/("zImage");
            type = "kernel";
            arch = "arm";
            os = "linux";
            compression = "none";
            load = <0x82000000>;
            entry = <0x82000000>;
            hash-1 {
                algo = "sha256";
            };
            signature-1 {
                algo = "sha256,rsa2048";
                key-name-hint = "dev";
            };
        };

        fdt {
            description = "BBB device tree";
            data = /incbin/("am335x-boneblack.dtb");
            type = "flat_dt";
            arch = "arm";
            compression = "none";
            hash-1 {
                algo = "sha256";
            };
        };
    };

    configurations {
        default = "conf-1";
        conf-1 {
            description = "BBB config";
            kernel = "kernel";
            fdt = "fdt";
            signature-1 {
                algo = "sha256,rsa2048";
                key-name-hint = "dev";
                sign-images = "kernel", "fdt";
            };
        };
    };
};
```

**Ký image:**

```bash
# Tạo FIT image chưa ký
mkimage -f bbb.its bbb.itb

# Ký FIT image, nhúng public key vào U-Boot DTB
mkimage -F -k /tmp/keys -K /path/to/u-boot.dtb -r bbb.itb
```

**Rebuild U-Boot với public key nhúng vào:**

```bash
make ARCH=arm EXT_DTB=/path/to/u-boot.dtb -j$(nproc)
```

**Boot với FIT image:**

```bash
=> load mmc 0:1 0x82000000 bbb.itb
=> bootm 0x82000000
# U-Boot sẽ verify chữ ký trước khi boot
# Nếu chữ ký sai → boot bị chặn
```

### 9.4 ARM Trusted Firmware — Bảo vệ runtime

Secure boot đảm bảo **khởi động đúng**. Nhưng sau khi boot, hệ thống vẫn có thể bị tấn công trong runtime. **ARM Trusted Firmware (ATF)** giải quyết vấn đề này bằng cách:

- Chạy ở **EL3** (Exception Level cao nhất của ARM)
- Phân quyền truy cập tài nguyên: memory, crypto key, secure peripheral
- Ngay cả khi attacker leo quyền root trong Linux → không thể truy cập tài nguyên EL3

```
EL3: ARM Trusted Firmware  ← kiểm soát toàn bộ tài nguyên nhạy cảm
EL1: Linux Kernel           ← chỉ thấy tài nguyên được ATF cho phép
EL0: User space
```

> **BBB thực tế**: Cortex-A8 của BBB hỗ trợ TrustZone nhưng không có full ATF implementation. ATF phổ biến trên Cortex-A53/A72 (Raspberry Pi 4, i.MX8, Rockchip RK3399...).

---

## Build U-Boot cho BBB

```bash
cd /home/phula/work/bbb/u-boot
export CROSS_COMPILE=/home/phula/work/bbb/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

# Áp defconfig
make ARCH=arm am335x_evm_defconfig

# (Tùy chọn) Tùy chỉnh
make ARCH=arm menuconfig

# Build
make ARCH=arm -j$(nproc)

# Deploy lên board
scp MLO u-boot.img debian@192.168.137.2:/home/debian/
```

Output:
- `MLO` — SPL (~104KB), Boot ROM load đầu tiên
- `u-boot.img` — U-Boot proper (~968KB), SPL load tiếp theo

---

## Tài liệu tham khảo trong repo

- `docs/lessons/15_uboot/Bai_18_Uboot/uboot_flow/uboot_flow.pdf` — Sơ đồ luồng chi tiết
- `docs/lessons/15_uboot/Bai_18_Uboot/porting_uboot/U_boot_Porting_guide.pdf` — Hướng dẫn porting
- `docs/lessons/15_uboot/Bai_18_Uboot/porting_uboot/sitara_boot_camp_03_giving_linux_the_boot.pptx` — TI Sitara Boot Camp
- `docs/beaglebone_black_docs/hw-docs/` — TRM AM335x, schematic BBB
