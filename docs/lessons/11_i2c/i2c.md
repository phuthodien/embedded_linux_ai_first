# Bài giảng: I2C trong Linux

**Tác giả:** Lưu An Phú — Vinalinux CEO  
**Khóa học:** Lập trình trên Linux  
**Thời lượng:** 4 giờ

---

## Phần 1 — Tổng quan giao thức I2C

### 1.1 I2C là gì?

I2C (Inter-Integrated Circuit) là giao thức truyền thông nối tiếp đồng bộ, chỉ sử dụng **2 dây** để kết nối nhiều thiết bị trên cùng một bus:

- **SDA** (Serial Data) — đường truyền dữ liệu
- **SCL** (Serial Clock) — đường truyền clock

Đặc điểm chính:

- Không cần thỏa thuận trước về tốc độ giữa các thiết bị (khác với UART)
- Sử dụng **7 bit** để định danh địa chỉ thiết bị (tối đa 128 thiết bị trên 1 bus)
- Dễ dàng thêm/bớt thiết bị trên bus
- Mỗi xung clock trên SCL tương ứng với **1 bit** dữ liệu trên SDA được gửi đi

---

### 1.2 Phần cứng — Open-drain và Pull-up

Cả SDA và SCL đều được cấu hình ở chế độ **open-drain** và được kéo lên mức HIGH thông qua điện trở pull-up.

```
       VCC
        │
       ┌┴┐ R_pull-up
       └┬┘
        ├───────── SDA (hoặc SCL)
        │
   ───┤ │  (MOSFET open-drain)
      │ │
      GND
```

**Tại sao dùng open-drain?**

Để tránh hiện tượng **ngắn mạch** khi một thiết bị kéo đường dây lên HIGH trong khi thiết bị khác kéo xuống LOW. Với open-drain, thiết bị chỉ có thể kéo xuống LOW hoặc thả ra (để pull-up kéo lên HIGH), không bao giờ chủ động đẩy lên HIGH.

---

### 1.3 Mô hình Master - Slave

I2C hoạt động theo mô hình **Master - Slave**:

| Vai trò | Chức năng |
|---------|-----------|
| **Master** | Tạo xung clock trên SCL, điều khiển tín hiệu trên SDA, khởi tạo giao tiếp |
| **Slave** | Đáp ứng yêu cầu của Master, được định danh bởi **7 bits địa chỉ** |

Khi Master muốn giao tiếp với một Slave cụ thể, nó sẽ gửi 7 bits địa chỉ của thiết bị đó lên đường SDA. Tất cả Slave trên bus đều nhận được, nhưng chỉ Slave có địa chỉ trùng khớp mới phản hồi.

---

### 1.4 Giao thức truyền dữ liệu

Một frame I2C gồm các thành phần:

```
┌───────┬─────────────┬─────┬──────────┬──────────┬───────┐
│ START │ Address (7b) │ R/W │ ACK/NACK │ Data (8b)│ STOP  │
└───────┴─────────────┴─────┴──────────┴──────────┴───────┘
```

| Thành phần | Mô tả |
|------------|-------|
| **Start Condition** | Master kéo SDA xuống LOW trong khi SCL vẫn HIGH — bắt đầu frame |
| **Address** | 7 bits địa chỉ của slave mà master muốn giao tiếp |
| **Read/Write** | 1 bit xác định hướng truyền dữ liệu (0 = Write, 1 = Read) |
| **ACK/NACK** | Slave kéo SDA xuống LOW (ACK = 0) nếu nhận được, hoặc để HIGH (NACK = 1) |
| **Stop Condition** | Master thả SDA lên HIGH trong khi SCL đang HIGH — kết thúc frame |

---

### 1.5 Trình tự gửi nhận dữ liệu

**Step 1:** Master gửi **Start Condition** đến tất cả slave trên bus.

**Step 2:** Master gửi **7 bits address** + **1 bit R/W**.

**Step 3:** Mỗi slave so sánh address nhận được với address của mình:
- Trùng khớp → kéo SDA xuống LOW → **ACK (0)**
- Không trùng → SDA giữ HIGH → **NACK (1)**

**Step 4 (Write — R/W = 0):** Master gửi 8 bits dữ liệu đến slave. Slave gửi ACK/NACK sau mỗi byte nhận được.

**Step 4 (Read — R/W = 1):** Slave gửi 8 bits dữ liệu đến master. Master gửi ACK/NACK sau mỗi byte nhận được.

**Step 5:** Sau khi truyền nhận xong, master tạo **Stop Condition** để kết thúc frame.

> **Lưu ý:** Master có thể gửi/nhận nhiều bytes trong cùng một frame mà không cần tạo Start/Stop mới cho mỗi byte. Chỉ cần duy trì ACK sau mỗi byte là đủ.

---

### 1.6 Câu hỏi tự kiểm tra

1. Làm sao để 2 thiết bị slave trao đổi được dữ liệu với nhau?
2. Master muốn gửi 5 bytes dữ liệu cho slave thì có phải tạo ra 5 frames không?
3. Có được phép 2 slaves có địa chỉ giống nhau ở trong mạng không?

---

## Phần 2 — I2C trong Linux

### 2.1 Kiến trúc I2C subsystem

Linux kernel tổ chức I2C theo 3 tầng:

```
┌──────────────────────────────────────────────────┐
│                  User Space                      │
│          (Application / i2c-tools)               │
├──────────────────────────────────────────────────┤
│              I2C Client Driver                   │
│     (OLED driver, sensor driver, ...)            │
├──────────────────────────────────────────────────┤
│                I2C Core                          │
│      (drivers/i2c/i2c-core.c)                    │
├──────────────────────────────────────────────────┤
│            I2C Bus Adapter Driver                │
│   (SoC-specific: i2c-omap, i2c-imx, ...)        │
├──────────────────────────────────────────────────┤
│               Hardware (I2C bus)                 │
└──────────────────────────────────────────────────┘
```

- **I2C Adapter Driver** (bus driver): Điều khiển hardware I2C controller trên SoC (ví dụ: AM335x có 3 I2C controller). Driver này do vendor SoC cung cấp.
- **I2C Core**: Tầng trung gian trong kernel, cung cấp API thống nhất cho các I2C client driver.
- **I2C Client Driver**: Driver cho thiết bị cụ thể trên bus I2C (OLED, accelerometer, EEPROM...).

---

### 2.2 Xác định I2C bus và device trên board

Trước khi viết driver, cần xác định rõ device nằm ở bus nào, địa chỉ gì.

**Bước 1 — Kiểm tra bus I2C có được enable không:**

```bash
ls /sys/class/i2c-adapter/
# Kết quả: i2c-0  i2c-1  i2c-2
```

**Bước 2 — Kiểm tra pin mux trong Device Tree:**

Đảm bảo pins SDA/SCL đã được map đúng chức năng I2C trong device tree overlay hoặc DTS chính.

**Bước 3 — Xác định address của device:**

Cách 1: Đọc datasheet + kiểm tra hardware (xem các pin address select).

Cách 2: Dùng `i2cdetect`:

```bash
i2cdetect -y -r <bus_number>
```

Ví dụ với bus 2:

```bash
$ i2cdetect -y -r 2
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- UU UU UU UU -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```

Ở đây `3c` là địa chỉ của SSD1306 OLED, `UU` là các địa chỉ đã có driver đang sử dụng.

---

### 2.3 Đăng ký I2C device trong Device Tree

I2C device được khai báo là **node con** của I2C bus adapter node trong device tree:

```dts
&i2c2 {
	status = "okay";
	clock-frequency = <100000>;

	oled: ssd1306@3c {
		compatible = "solomon,ssd1306";
		reg = <0x3c>;
	};
};
```

Giải thích:

- `&i2c2`: Tham chiếu đến I2C bus adapter thứ 2 (đã được định nghĩa trong DTSI của SoC)
- `ssd1306@3c`: Tên node, `3c` là địa chỉ I2C 7-bit
- `compatible`: Chuỗi dùng để match với I2C client driver
- `reg`: Địa chỉ I2C của device (trường **bắt buộc** cho I2C device node)

---

### 2.4 Triển khai I2C Client Driver

#### Cấu trúc cơ bản

```c
#include <linux/module.h>
#include <linux/i2c.h>

static int oled_probe(struct i2c_client *client,
		      const struct i2c_device_id *id)
{
	/* Cấu hình tốc độ, chế độ hoạt động, khởi tạo device */
	pr_info("OLED probed, addr=0x%02x\n", client->addr);
	return 0;
}

static int oled_remove(struct i2c_client *client)
{
	/* Giải phóng tài nguyên (RAM, I/O, ...) */
	pr_info("OLED removed\n");
	return 0;
}

static const struct of_device_id oled_of_match[] = {
	{ .compatible = "solomon,ssd1306" },
	{ }
};
MODULE_DEVICE_TABLE(of, oled_of_match);

static struct i2c_driver oled_driver = {
	.driver = {
		.name = "ssd1306",
		.of_match_table = oled_of_match,
	},
	.probe = oled_probe,
	.remove = oled_remove,
};

module_i2c_driver(oled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SSD1306 OLED I2C Driver");
```

Giải thích:

| Thành phần | Vai trò |
|------------|---------|
| `struct i2c_driver` | Cấu trúc đăng ký I2C client driver với kernel |
| `probe()` | Được gọi khi device tree match — thực hiện khởi tạo device |
| `remove()` | Được gọi khi driver bị unload — giải phóng tài nguyên |
| `of_match_table` | Bảng matching với `compatible` string trong device tree |
| `module_i2c_driver()` | Macro đăng ký driver, thay thế `module_init()` + `module_exit()` |

---

### 2.5 Gửi nhận dữ liệu — i2c_master_send / i2c_master_recv

Kernel cung cấp 2 hàm cơ bản để truyền nhận dữ liệu I2C:

```c
int i2c_master_send(struct i2c_client *client, const char *buf, int count);
int i2c_master_recv(struct i2c_client *client, char *buf, int count);
```

| Tham số | Mô tả |
|---------|-------|
| `client` | Con trỏ `i2c_client` nhận được từ hàm `probe()` |
| `buf` | Buffer chứa dữ liệu gửi/nhận |
| `count` | Số lượng byte gửi/nhận |
| **Return** | Số byte thực tế đã gửi/nhận, hoặc giá trị âm nếu lỗi |

Ví dụ gửi command tắt màn hình SSD1306:

```c
static int ssd1306_send_cmd(struct i2c_client *client, u8 cmd)
{
	u8 buf[2];

	buf[0] = 0x00;  /* Control byte: command mode */
	buf[1] = cmd;
	return i2c_master_send(client, buf, 2);
}

/* Tắt display */
ssd1306_send_cmd(client, 0xAE);
```

Ví dụ gửi data (ghi vào GDDRAM):

```c
static int ssd1306_send_data(struct i2c_client *client, u8 data)
{
	u8 buf[2];

	buf[0] = 0x40;  /* Control byte: data mode */
	buf[1] = data;
	return i2c_master_send(client, buf, 2);
}
```

---

### 2.6 Đăng ký device bằng sysfs (runtime)

Ngoài device tree, có thể đăng ký/hủy I2C device tại runtime thông qua sysfs — hữu ích khi debug hoặc prototype:

**Đăng ký:**

```bash
echo ssd1306 0x3c > /sys/bus/i2c/devices/i2c-2/new_device
```

**Hủy đăng ký:**

```bash
echo 0x3c > /sys/bus/i2c/devices/i2c-2/delete_device
```

> **Lưu ý:** Cách này chỉ nên dùng trong quá trình phát triển. Production nên khai báo device trong device tree.

---

### 2.7 Detect device tự động

I2C driver có thể cấu hình để tự động detect device trên bus thông qua 3 trường trong `struct i2c_driver`:

```c
static const unsigned short address_list[] = {
	0x3c, 0x3d, I2C_CLIENT_END
};

static int oled_detect(struct i2c_client *client,
		       struct i2c_board_info *info)
{
	/* Xác nhận device tồn tại, khởi tạo info->type */
	strlcpy(info->type, "ssd1306", I2C_NAME_SIZE);
	return 0;
}

static struct i2c_driver oled_driver = {
	.driver = { .name = "ssd1306" },
	.probe = oled_probe,
	.remove = oled_remove,
	.detect = oled_detect,
	.address_list = address_list,
	.class = I2C_CLASS_HWMON,
};
```

| Trường | Mô tả |
|--------|-------|
| `address_list` | Danh sách địa chỉ sẽ được thăm dò (probe) trên bus |
| `detect()` | Callback được gọi khi phát hiện device — cần khởi tạo `type` trong `i2c_board_info` |
| `class` | Được AND với class của I2C adapter để xác định bus nào được scan |

---

## Phần 3 — I2C từ User Space (i2c-dev)

### 3.1 Tổng quan i2c-dev

`i2c-dev` là module cho phép truy cập I2C device trực tiếp từ **user space** thông qua device file, không cần viết kernel driver.

- Mỗi I2C bus được đăng ký sẽ tạo ra một device file: `/dev/i2c-0`, `/dev/i2c-1`, ...
- Tên device file: `i2c-%d`, trong đó `%d` là id của I2C bus
- Cần include: `#include <linux/i2c-dev.h>`

---

### 3.2 Quy trình sử dụng i2c-dev

```
open() → ioctl(I2C_SLAVE) → read()/write() → close()
```

**Bước 1 — Open I2C bus:**

```c
int fd = open("/dev/i2c-2", O_RDWR);
if (fd < 0) {
	perror("open i2c bus");
	return -1;
}
```

**Bước 2 — Thiết lập địa chỉ slave:**

```c
if (ioctl(fd, I2C_SLAVE, 0x3c) < 0) {
	perror("ioctl I2C_SLAVE");
	close(fd);
	return -1;
}
```

Dùng `I2C_SLAVE_FORCE` nếu device đã có kernel driver đang sử dụng (cẩn thận khi dùng).

**Bước 3 — Gửi/nhận dữ liệu:**

```c
/* Gửi command tắt display */
u8 buf[2] = {0x00, 0xAE};
write(fd, buf, 2);

/* Đọc dữ liệu */
u8 recv[4];
read(fd, recv, sizeof(recv));
```

**Bước 4 — Đóng file:**

```c
close(fd);
```

---

## Phần 4 — Thực hành: Porting LCD SSD1306 trên BeagleBone Black

### 4.1 Thông số SSD1306

| Thông số | Giá trị |
|----------|---------|
| Display Technology | OLED (Organic LED) |
| MCU Interface | I2C / SPI |
| Screen Size | 0.96 inch |
| Resolution | 128x64 pixels |
| Operating Voltage | 3.3V – 5V |
| Operating Current | 20mA max |
| Viewing Angle | 160 degrees |

---

### 4.2 Tổ chức bộ nhớ GDDRAM

SSD1306 có built-in **1KB Graphic Display Data RAM (GDDRAM)** để lưu trữ bit pattern hiển thị trên màn hình.

Bộ nhớ được chia thành:
- **8 pages** (page 0–7)
- Mỗi page chứa **128 columns** (segment 0–127)
- Mỗi column chứa **8 bits** (tương ứng 8 pixel theo chiều dọc)

```
         Column 0   Column 1   ...   Column 127
Page 0:  [8 bits]   [8 bits]   ...   [8 bits]
Page 1:  [8 bits]   [8 bits]   ...   [8 bits]
  ...
Page 7:  [8 bits]   [8 bits]   ...   [8 bits]
```

Tổng cộng: 8 pages x 128 columns x 8 bits = 8192 pixels = 128 x 64.

---

### 4.3 Chế độ gửi nhận dữ liệu

SSD1306 phân biệt 2 loại dữ liệu qua **control byte**:

| Control Byte | Chế độ | Mô tả |
|-------------|--------|-------|
| `0x00` | Command | Byte tiếp theo là lệnh điều khiển |
| `0x40` | Data | Byte tiếp theo được ghi vào GDDRAM |

**Gửi command (ví dụ: Display OFF = 0xAE):**

```
START → Slave Addr + W → ACK → 0x00 (control) → ACK → 0xAE (cmd) → ACK → STOP
```

**Gửi data (ví dụ: ghi 0xFF vào GDDRAM):**

```
START → Slave Addr + W → ACK → 0x40 (control) → ACK → 0xFF (data) → ACK → STOP
```

---

### 4.4 Ví dụ: In ký tự lên màn hình

Để hiển thị ký tự **'A'** (font 5x8) từ vị trí page 0, column 0:

1. Di chuyển cursor đến vị trí (page 0, column 0) bằng command
2. Gửi lần lượt 5 byte data: `0x7C, 0x12, 0x11, 0x12, 0x7C`

Mỗi byte tương ứng 1 column, 8 bits trong mỗi byte tương ứng 8 pixel dọc.

---

### 4.5 Bảng lệnh SSD1306 thường dùng

| Command | Tên | Mô tả |
|---------|-----|-------|
| `0x00` | COMMAND | Byte tiếp theo là command |
| `0x40`–`0x7F` | Display Start Line | Đặt dòng bắt đầu hiển thị trong RAM |
| `0xAE` | Display OFF | Tắt màn hình |
| `0xAF` | Display ON | Bật màn hình |
| `0x20` | Memory Addressing Mode | Chọn chế độ addressing: horizontal, vertical, page |
| `0x21` | Column Address | Đặt column address hiện tại |
| `0x22` | Page Address | Đặt page address hiện tại |
| `0x81` | Contrast Control | Đặt độ tương phản (0x00–0xFF, 256 mức) |
| `0xA0` | Segment Re-map | Column 0 → SEG0 |
| `0xA1` | Segment Re-map | Column 127 → SEG0 (lật ngang) |
| `0xA6` | Normal Display | RAM data 1 = pixel ON |
| `0xA7` | Inverse Display | RAM data 0 = pixel ON |
| `0xA8` | Multiplex Ratio | Đặt tỷ lệ multiplex |
| `0xC0` | COM Scan Direction | Scan COM0 → COM[N-1] (normal) |
| `0xC8` | COM Scan Direction | Scan COM[N-1] → COM0 (lật dọc) |
| `0xD3` | Display Offset | Dịch hiển thị theo chiều dọc (0–63) |
| `0xD5` | Clock Divide Ratio | Đặt clock divider và tần số oscillator |
| `0x2E` | Deactivate Scroll | Tắt scroll |
| `0x2F` | Activate Scroll | Bật scroll (phải cấu hình trước) |
| `0x26` / `0x27` | Horizontal Scroll | Scroll phải (0x26) / trái (0x27) |
| `0x29` / `0x2A` | Vertical + Horizontal Scroll | Scroll dọc + phải (0x29) / trái (0x2A) |

---

### 4.6 Quy trình thực hành trên BeagleBone Black

**Bước 1 — Xác định I2C bus và pins:**

```bash
# Kiểm tra bus I2C available
ls /sys/class/i2c-adapter/

# Kiểm tra pin mux trong device tree
```

**Bước 2 — Xác định address của SSD1306:**

```bash
# Đọc datasheet: SSD1306 mặc định là 0x3C (SA0 = 0) hoặc 0x3D (SA0 = 1)
# Xác nhận bằng i2cdetect
i2cdetect -y -r 2
```

**Bước 3 — Đăng ký device trong device tree:**

Thêm node SSD1306 vào device tree overlay của BBB.

**Bước 4 — Triển khai I2C driver:**

Viết kernel module sử dụng `struct i2c_driver` với `probe()` khởi tạo SSD1306 và `remove()` giải phóng tài nguyên.

---

## Phần 5 — SSD1306 Kernel Driver hoàn chỉnh

Phần này trình bày driver hoàn chỉnh cho SSD1306 OLED, cho phép hiển thị ký tự thông qua device file `/dev/ssd1306`.

Source code: `examples/lessons/11_i2c/ssd1306_drv.c`

### 5.1 Cách sử dụng

```bash
# Build module
make

# Copy lên board
scp ssd1306_drv.ko debian@192.168.137.2:/home/debian/

# Trên board: load module
sudo insmod ssd1306_drv.ko

# Đăng ký device (nếu không dùng device tree)
echo training_ssd1306 0x3c > /sys/bus/i2c/devices/i2c-2/new_device

# Hiển thị ký tự
echo "A" > /dev/ssd1306
echo "Hello World" > /dev/ssd1306

# Xóa màn hình
echo "" > /dev/ssd1306

# Gỡ device và module
echo 0x3c > /sys/bus/i2c/devices/i2c-2/delete_device
sudo rmmod ssd1306_drv
```

Hoặc dùng device tree thay vì sysfs:

```dts
&i2c2 {
	status = "okay";
	clock-frequency = <100000>;

	ssd1306@3c {
		compatible = "training,ssd1306";
		reg = <0x3c>;
	};
};
```

### 5.2 Kiến trúc driver

```
┌─────────────────────────────────────────────────────┐
│ User Space:  echo "Hi" > /dev/ssd1306               │
├─────────────────────────────────────────────────────┤
│ VFS layer → file_operations.write()                  │
├─────────────────────────────────────────────────────┤
│ ssd1306_drv.c                                        │
│  ├── misc device  (/dev/ssd1306)                     │
│  ├── font table   (5x8 ASCII, 95 glyphs)            │
│  ├── ssd1306_putchar() → set cursor + write glyph   │
│  └── i2c_master_send() → I2C core                   │
├─────────────────────────────────────────────────────┤
│ I2C Adapter Driver (i2c-omap)  →  Hardware I2C bus   │
└─────────────────────────────────────────────────────┘
```

### 5.3 Phân tích code

#### a) Gửi command và data qua I2C

SSD1306 phân biệt command/data bằng **control byte** — byte đầu tiên trong mỗi I2C message:

```c
/* Control byte 0x00 = command mode */
static int ssd1306_write_cmd(struct i2c_client *client, u8 cmd)
{
	u8 buf[2] = {0x00, cmd};
	return i2c_master_send(client, buf, 2);
}

/* Control byte 0x40 = data mode (ghi vào GDDRAM) */
static int ssd1306_write_data(struct i2c_client *client,
			      const u8 *data, int len)
{
	u8 *buf = kmalloc(len + 1, GFP_KERNEL);
	buf[0] = 0x40;
	memcpy(buf + 1, data, len);
	i2c_master_send(client, buf, len + 1);
	kfree(buf);
}
```

#### b) Khởi tạo display

Chuỗi command khởi tạo SSD1306 theo datasheet:

```c
static const u8 ssd1306_init_cmds[] = {
	0xAE,         /* Display OFF */
	0xD5, 0x80,   /* Clock div ratio */
	0xA8, 0x3F,   /* Multiplex ratio: 64 lines */
	0xD3, 0x00,   /* Display offset: 0 */
	0x40,         /* Start line: 0 */
	0x8D, 0x14,   /* Enable charge pump */
	0x20, 0x02,   /* Page addressing mode */
	0xA1,         /* Segment re-map (lật ngang) */
	0xC8,         /* COM scan remapped (lật dọc) */
	0xDA, 0x12,   /* COM pins: alternative config */
	0x81, 0xCF,   /* Contrast: 0xCF */
	0xD9, 0xF1,   /* Pre-charge period */
	0xDB, 0x40,   /* VCOMH deselect level */
	0xA4,         /* Display from RAM */
	0xA6,         /* Normal display */
	0x2E,         /* Deactivate scroll */
	0xAF,         /* Display ON */
};
```

#### c) Di chuyển cursor (Page Addressing Mode)

Trong page addressing mode, vị trí ghi được xác định bởi **page** (dòng) và **column** (cột):

```c
static int ssd1306_set_cursor(struct ssd1306_dev *dev, u8 page, u8 col)
{
	ssd1306_write_cmd(client, 0xB0 | (page & 0x07));     /* Set page */
	ssd1306_write_cmd(client, col & 0x0F);                /* Col low nibble */
	ssd1306_write_cmd(client, 0x10 | ((col >> 4) & 0x0F));/* Col high nibble */
}
```

#### d) Font 5x8 và hàm putchar

Mỗi ký tự ASCII được biểu diễn bằng 5 bytes (5 columns x 8 rows). Ví dụ ký tự 'A':

```
Column:  0     1     2     3     4
Byte:   0x7E  0x11  0x11  0x11  0x7E

Bit 0:   0     1     1     1     0      ─┐
Bit 1:   1     0     0     0     1       │
Bit 2:   1     0     0     0     1       │
Bit 3:   1     0     0     0     1       │ 8 pixels
Bit 4:   1     1     1     1     1       │ dọc
Bit 5:   1     0     0     0     1       │
Bit 6:   1     0     0     0     1       │
Bit 7:   0     0     0     0     0      ─┘
```

Hàm `ssd1306_putchar()` ghi 5 byte font + 1 byte spacing (0x00) vào GDDRAM. Khi hết dòng (column > 128), tự động xuống page tiếp theo.

#### e) File operation — write handler

Khi user `echo "Hello" > /dev/ssd1306`:

1. Kernel gọi `ssd1306_fop_write()`
2. Driver copy dữ liệu từ user space, strip trailing `\n`
3. Nếu chuỗi rỗng → clear screen
4. Ngược lại → clear screen, rồi gọi `ssd1306_putchar()` cho từng ký tự

#### f) I2C driver registration

```c
static const struct of_device_id ssd1306_of_match[] = {
	{ .compatible = "training,ssd1306" },
	{ }
};

static const struct i2c_device_id ssd1306_id[] = {
	{ "training_ssd1306", 0 },   /* match khi dùng sysfs new_device */
	{ }
};

static struct i2c_driver ssd1306_driver = {
	.driver = {
		.name           = "ssd1306",
		.of_match_table = ssd1306_of_match,
	},
	.probe    = ssd1306_probe,
	.remove   = ssd1306_remove,
	.id_table = ssd1306_id,
};
module_i2c_driver(ssd1306_driver);
```

Driver hỗ trợ 2 cách matching:
- **Device tree**: match qua `compatible = "training,ssd1306"`
- **Sysfs**: match qua `i2c_device_id` khi dùng `echo training_ssd1306 0x3c > .../new_device`

---

## Tổng kết

| Khái niệm | Điểm chính |
|------------|------------|
| I2C protocol | 2 dây (SDA, SCL), master-slave, 7-bit address |
| Open-drain | Tránh ngắn mạch, cần pull-up resistor |
| Linux I2C stack | Adapter driver → I2C core → Client driver |
| Device Tree | I2C device là node con của bus adapter, trường `reg` = address |
| Kernel API | `i2c_master_send()` / `i2c_master_recv()` |
| User space | `/dev/i2c-X` + `ioctl(I2C_SLAVE)` + `read()`/`write()` |
| SSD1306 | Control byte `0x00` = command, `0x40` = data, GDDRAM 128x64 |
