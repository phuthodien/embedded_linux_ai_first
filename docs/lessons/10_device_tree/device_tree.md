# Bài giảng: Device Tree trong Embedded Linux

**Tác giả:** Lưu An Phú — Vinalinux CEO  
**Khóa học:** Lập trình trên Linux  
**Thời lượng:** 4 giờ

---

## Phần 1 — Tại sao cần Device Tree?

### 1.1 Hardware enumerable vs non-enumerable

Trên x86, khi Linux boot, kernel không cần ai nói trước "máy này có card mạng Intel ở địa chỉ nào" — vì **PCI bus tự enumerate**: kernel hỏi bus, bus trả lời danh sách thiết bị, địa chỉ, IRQ. ACPI đảm nhiệm phần còn lại (power, thermal...). Kernel x86 hoàn toàn generic, chạy được trên mọi máy tính mà không cần biết trước cấu hình phần cứng.

ARM embedded thì khác. Các peripheral (UART, I2C, GPIO...) được nối trực tiếp lên **memory-mapped bus** — không có cơ chế tự announce, không có enumeration. Kernel không thể hỏi "bus ơi, mày có gì?" mà không ai trả lời cả. Ai đó phải nói cho kernel biết hardware là gì, ở địa chỉ nào, dùng IRQ nào.

Sự khác biệt cốt lõi:
- **x86**: Bus là PCI/PCIe — tự enumerate, kernel không cần biết trước
- **ARM embedded**: Bus là memory-mapped (AHB, APB) — không enumerate được, kernel **phải được nói trước**

Đây là nguồn gốc của toàn bộ vấn đề.

---

### 1.2 Giải pháp đầu tiên: Hard-code trong board file — và tại sao nó thất bại

Giải pháp tự nhiên nhất: viết thẳng thông tin hardware vào code C, dưới dạng `board-xxx.c` trong kernel. Mỗi board có một file riêng khai báo địa chỉ base register, số IRQ, clock frequency... dưới dạng struct C.

Cách này hoạt động — nhưng không scale. Đến năm 2011, ARM Linux có **hơn 1000 file board-xxx.c**, mỗi board một file, phần lớn là copy-paste với vài con số khác nhau. Linus Torvalds gọi đây là *"a disaster"* và yêu cầu ARM phải tìm giải pháp khác.

Vấn đề cốt lõi: **kernel code đang mô tả hardware thay vì điều khiển hardware**.

---

### 1.3 Giải pháp: Device Tree

Device Tree giải quyết vấn đề bằng cách tách mô tả hardware ra khỏi kernel thành một file riêng — **Device Tree Source (.dts)** — compile thành binary **Device Tree Blob (.dtb)**. Bootloader (U-Boot) load DTB vào RAM và truyền pointer cho kernel khi boot. Kernel đọc DTB, tự cấu hình theo hardware hiện tại.

Flow tổng quát:

```
[board.dts]
    │
    ▼ dtc (compiler)
[board.dtb]
    │
    ▼ U-Boot load vào RAM
[Kernel] ← pointer đến DTB
    │
    ▼
Parse DTB → probe drivers
```

**Lợi ích 1 — Tránh hard-code, một kernel chạy nhiều board:**

Cùng một kernel binary, chỉ cần thay DTB là chạy được trên board khác. Không cần rebuild kernel.

**Lợi ích 2 — BSP modular, kế thừa theo dòng board:**

DTS hỗ trợ include và override — cho phép tổ chức BSP theo tầng. Đây chính xác là cấu trúc của BeagleBone trong tài liệu của khóa học này:

```
am33xx.dtsi
  └── am335x-bone-common.dtsi
        └── am335x-boneblack-common.dtsi
              └── am335x-boneblack.dts
```

Mỗi tầng chỉ mô tả những gì mới hoặc khác biệt so với tầng trên. Muốn support BeagleBone Blue hay BeagleBone WiFi? Chỉ cần viết thêm một file `.dts` mới, kế thừa từ `am335x-bone-common.dtsi`, override đúng những peripheral khác biệt. Không đụng đến kernel code.

**Lợi ích 3 — Ngôn ngữ khai báo, mô tả được topology phần cứng:**

DTS là ngôn ngữ **declarative** — mô tả *cái gì*, không mô tả *làm thế nào*. Điều này cho phép diễn đạt quan hệ giữa các thiết bị mà C rất khó biểu đạt mà không kèm logic điều kiện.

Ví dụ: mô tả UART1 kết nối vào interrupt controller nào. Trong C, driver phải tự biết số IRQ theo từng board — dẫn đến `if (board_type == BOARD_BBB)` rải rác khắp nơi. Trong DTS, chỉ cần khai báo quan hệ:

```dts
uart1: serial@48022000 {
    interrupt-parent = <&intc>;
    interrupts = <73 IRQ_TYPE_LEVEL_HIGH>;
};
```

Kernel tự resolve quan hệ này — driver không cần biết board là gì.

---

## Phần 2 — DT trong kernel boot flow

### 2.1 Bootloader và DTB

Trước khi kernel chạy, **bootloader (U-Boot)** là người chịu trách nhiệm chuẩn bị DTB. U-Boot thực hiện các bước sau:

- Load kernel image (`zImage` hoặc `Image`) vào RAM
- Load DTB vào RAM tại một địa chỉ cố định, thường ngay sau kernel image
- Truyền địa chỉ DTB cho kernel thông qua register `r2` (ARM 32-bit) hoặc `x0` (ARM64)

Lệnh boot thông thường trong U-Boot:

```
bootz 0x80008000 - 0x80F00000
         │             │
    kernel addr     DTB addr
```

Tại thời điểm này, kernel chưa chạy gì cả — DTB chỉ là một vùng binary nằm trong RAM, chờ được parse.

---

### 2.2 Kernel nhận DTB và unflatten

Ngay khi kernel bắt đầu chạy, một trong những việc đầu tiên nó làm là xử lý DTB. Quá trình này diễn ra theo thứ tự:

**Bước 1 — Validate DTB:**
Kernel kiểm tra magic number `0xd00dfeed` ở đầu DTB để xác nhận đây là DTB hợp lệ. Nếu không khớp, kernel panic ngay lập tức.

**Bước 2 — `early_init_dt_scan()`:**
Kernel scan qua DTB ở dạng flat (binary) để lấy một số thông tin tối thiểu cần thiết trước khi memory management khởi động — cụ thể là thông tin về RAM từ node `/memory` và boot arguments từ node `/chosen`.

**Bước 3 — `unflatten_device_tree()`:**
Sau khi memory management sẵn sàng, kernel convert DTB từ dạng flat binary thành cây `device_node` trong bộ nhớ — đây là cấu trúc dữ liệu mà toàn bộ kernel và driver sẽ dùng để query thông tin hardware sau này.

```
DTB (flat binary)              device_node tree (in RAM)
─────────────────              ─────────────────────────
0xd00dfeed...      ──────►     root
[FDT_BEGIN_NODE]               ├── cpus
[FDT_PROP]                     │     └── cpu@0
[FDT_BEGIN_NODE]               ├── memory@80000000
...                            ├── chosen
                               └── ocp
                                     ├── uart@48022000
                                     ├── gpio@44e07000
                                     └── ...
```

---

### 2.3 `of_platform_populate()` — từ DT node đến platform device

Sau khi cây `device_node` đã được dựng xong, kernel cần chuyển các node trong cây thành **platform device** — đối tượng mà driver framework có thể làm việc. Hàm `of_platform_populate()` đảm nhiệm việc này.

Nó duyệt qua cây `device_node`, với mỗi node có `compatible` string, tạo ra một `platform_device` tương ứng và đăng ký vào kernel. Từ đây, driver matching có thể bắt đầu.

---

### 2.4 Driver matching và probe

Khi một `platform_device` được đăng ký, kernel so sánh `compatible` string của nó với danh sách `of_match_table` mà các driver đã đăng ký. Nếu tìm thấy match, kernel gọi hàm `probe()` của driver đó.

Flow đầy đủ từ đầu đến cuối:

```
U-Boot
  │  load DTB vào RAM, truyền địa chỉ cho kernel
  ▼
early_init_dt_scan()
  │  đọc /memory, /chosen từ flat DTB
  ▼
unflatten_device_tree()
  │  flat DTB → cây device_node trong RAM
  ▼
of_platform_populate()
  │  device_node → platform_device
  ▼
Driver matching
  │  so sánh compatible string
  ▼
driver->probe()
  │  driver đọc thông tin từ DT, map register, request IRQ...
  ▼
Device sẵn sàng hoạt động
```

---

### 2.5 Ý nghĩa thực tế

Hiểu flow này giúp developer debug được những vấn đề thường gặp:

- **Kernel panic ngay khi boot**: thường do DTB address sai hoặc DTB corrupt — magic number check thất bại
- **Driver không probe**: `compatible` string trong DTS không khớp với `of_match_table` trong driver
- **Device probe nhưng không hoạt động**: driver `probe()` thành công nhưng đọc sai thông tin từ DT node

---

## Phần 3 — DTS Syntax & Cấu trúc cơ bản

### 3.1 Cấu trúc tổng thể một file DTS

Một file DTS có cấu trúc như sau:

```dts
/dts-v1/;

#include "am33xx.dtsi"

/ {
    model = "TI AM335x BeagleBone Black";
    compatible = "ti,am335x-bone-black", "ti,am33xx";
    #address-cells = <1>;
    #size-cells = <1>;

    memory@80000000 {
        device_type = "memory";
        reg = <0x80000000 0x20000000>;
    };

    chosen {
        bootargs = "console=ttyO0,115200n8";
    };
};
```

Các thành phần cơ bản:

- `/dts-v1/;` — khai báo version DTS, bắt buộc phải có ở đầu file
- `#include` — include file khác, hoạt động giống C preprocessor
- `/ { }` — root node, cha của toàn bộ cây
- Bên trong root node là các child node và property

---

### 3.2 Node

Mỗi node đại diện cho một hardware component. Cú pháp:

```dts
node-name@unit-address {
    /* properties */
    /* child nodes */
};
```

`unit-address` là địa chỉ của thiết bị trong address space của node cha — thường là địa chỉ base register đầu tiên trong `reg`. Ví dụ:

```dts
serial@48022000 {
    reg = <0x48022000 0x1000>;
};
```

Nếu node không có `reg`, có thể bỏ `@unit-address`:

```dts
chosen {
    bootargs = "console=ttyO0,115200n8";
};
```

---

### 3.3 Property và các kiểu dữ liệu

Property là cặp name-value mô tả đặc tính của node. DTS hỗ trợ các kiểu dữ liệu sau:

**u32 — số nguyên 32-bit, viết trong `< >`:**

```dts
clock-frequency = <48000000>;
```

**Array of u32:**

```dts
reg = <0x48022000 0x1000>;
interrupts = <73 4>;
```

**String:**

```dts
compatible = "ti,omap3-uart";
```

**String list — nhiều string ngăn cách bằng dấu phẩy:**

```dts
compatible = "ti,am335x-bone-black", "ti,am335x-bone", "ti,am33xx";
```

**Byte array — viết trong `[ ]`:**

```dts
local-mac-address = [00 11 22 33 44 55];
```

**Empty property — chỉ có tên, không có value, ý nghĩa nằm ở sự hiện diện:**

```dts
interrupt-controller;
gpio-controller;
```

---

### 3.4 Các property cốt lõi

**`compatible`** — chuỗi định danh thiết bị, dùng để match driver. Viết từ specific nhất đến generic nhất:

```dts
compatible = "ti,am335x-uart", "ti,omap3-uart", "ns16550";
```

Kernel thử match driver theo thứ tự từ trái sang phải. Nếu không có driver cho `ti,am335x-uart`, thử tiếp `ti,omap3-uart`, rồi `ns16550`.

**`reg`** — mô tả vùng địa chỉ của thiết bị. Format phụ thuộc vào `#address-cells` và `#size-cells` của node cha:

```dts
/* #address-cells = <1>, #size-cells = <1> */
reg = <0x48022000 0x1000>;
/*     base addr   size  */
```

**`#address-cells` và `#size-cells`** — quy định số lượng cell (u32) dùng để encode địa chỉ và size trong `reg` của các child node:

```dts
soc {
    #address-cells = <1>;
    #size-cells = <1>;

    uart1: serial@48022000 {
        reg = <0x48022000 0x1000>;
    };
};
```

**`status`** — trạng thái hoạt động của thiết bị:

```dts
status = "okay";      /* đang hoạt động */
status = "disabled";  /* tắt, kernel bỏ qua */
```

---

### 3.5 Label và phandle

**Label** là alias đặt trước một node, dùng để tham chiếu từ chỗ khác:

```dts
intc: interrupt-controller@48200000 {
    interrupt-controller;
    #interrupt-cells = <1>;
};
```

Từ bất kỳ đâu trong DTS, có thể tham chiếu đến node này bằng `&intc`:

```dts
uart1: serial@48022000 {
    interrupt-parent = <&intc>;
    interrupts = <73>;
};
```

`<&intc>` là **phandle reference** — kernel sẽ resolve thành pointer đến node `interrupt-controller@48200000` trong cây `device_node`. Đây là cơ chế để mô tả **quan hệ giữa các thiết bị** trong DTS.

---

### 3.6 .dts vs .dtsi

`.dts` là file top-level, được compile trực tiếp thành `.dtb`. Mỗi board có đúng một file `.dts`.

`.dtsi` là file include, không compile độc lập mà được include bởi `.dts` hoặc `.dtsi` khác. Dùng để chia sẻ mô tả chung giữa nhiều board.

Cú pháp include:

```dts
/dts-v1/;
#include "am33xx.dtsi"
#include "am335x-bone-common.dtsi"
```

`#include` hoạt động giống C preprocessor — nội dung file được include sẽ được chèn trực tiếp vào vị trí đó trước khi `dtc` compile.

---

### 3.7 Compile DTS thành DTB

Công cụ compile là `dtc` — Device Tree Compiler:

```bash
# Compile DTS → DTB
dtc -I dts -O dtb -o board.dtb board.dts

# Decompile DTB → DTS (dùng khi debug)
dtc -I dtb -O dts -o board.dts board.dtb
```

Trong kernel build system, DTB được build bằng:

```bash
make dtbs
```

File `.dtb` output nằm tại `arch/arm/boot/dts/`.

---

## Phần 4 — Interrupt trong DTS

### 4.1 Đặt vấn đề

Trên MCU, developer thường hard-code số IRQ từ datasheet trực tiếp vào firmware. Driver biết trực tiếp mình dùng IRQ nào. Cách này ổn với MCU vì firmware và hardware gắn chặt với nhau.

Trên Linux, driver phải hoạt động được trên nhiều board khác nhau — cùng một UART controller nhưng có thể được nối vào interrupt controller khác nhau, với số IRQ khác nhau tùy board. Driver không thể tự biết — thông tin này phải đến từ DTS.

---

### 4.2 Interrupt controller trong DTS

Thiết bị nhận và xử lý interrupt được gọi là **interrupt controller**. Nó được khai báo trong DTS với hai property đặc trưng:

- `interrupt-controller;` — empty property, đánh dấu node này là interrupt controller
- `#interrupt-cells = <N>;` — khai báo số lượng cell cần dùng để mô tả một interrupt

Ví dụ từ `am33xx.dtsi`, INTC của AM335x:

```dts
intc: interrupt-controller@48200000 {
    compatible = "ti,am33xx-intc";
    interrupt-controller;
    #interrupt-cells = <1>;
    reg = <0x48200000 0x1000>;
};
```

`#interrupt-cells = <1>` nghĩa là mỗi interrupt chỉ cần 1 cell để mô tả — đó là số IRQ. Một số interrupt controller phức tạp hơn cần 2 hoặc 3 cell — ví dụ GIC của ARM dùng 3 cell (loại interrupt, số IRQ, trigger type).

---

### 4.3 Thiết bị sinh interrupt

Thiết bị muốn khai báo interrupt cần hai property:

- `interrupt-parent = <&label>` — trỏ đến interrupt controller mà thiết bị này kết nối vào
- `interrupts = <...>` — mô tả interrupt, số lượng cell phải khớp với `#interrupt-cells` của controller

Ví dụ UART1 trên BBB:

```dts
uart1: serial@48022000 {
    compatible = "ti,am335x-uart";
    reg = <0x48022000 0x1000>;
    interrupt-parent = <&intc>;
    interrupts = <73>;
};
```

Vì `intc` có `#interrupt-cells = <1>`, nên `interrupts` chỉ cần 1 giá trị — số IRQ 73.

---

### 4.4 Interrupt controller phân cấp

Thực tế hardware thường có nhiều tầng interrupt controller. Trên AM335x, GPIO controller vừa là **thiết bị sinh interrupt** (kết nối vào INTC), vừa là **interrupt controller** cho các GPIO pin bên dưới nó.

Từ `am33xx.dtsi`:

```dts
gpio0: gpio@44e07000 {
    compatible = "ti,omap4-gpio";
    reg = <0x44e07000 0x1000>;

    /* gpio0 là thiết bị, kết nối vào intc */
    interrupt-parent = <&intc>;
    interrupts = <96>;

    /* gpio0 đồng thời là interrupt controller cho các pin */
    interrupt-controller;
    #interrupt-cells = <2>;
};
```

Khi một button nối vào GPIO0_31 muốn dùng interrupt:

```dts
button {
    interrupt-parent = <&gpio0>;
    interrupts = <31 IRQ_TYPE_EDGE_FALLING>;
};
```

Ở đây `#interrupt-cells = <2>` — cell đầu là GPIO pin number, cell thứ hai là trigger type.

---

### 4.5 `interrupt-parent` mặc định

Việc khai báo `interrupt-parent` trên từng node riêng lẻ sẽ rất lặp lại nếu hầu hết thiết bị đều kết nối vào cùng một controller. DTS cho phép khai báo `interrupt-parent` ở node cha — các node con sẽ kế thừa nếu không tự khai báo:

```dts
/ {
    interrupt-parent = <&intc>;  /* mặc định cho toàn bộ cây */

    serial@48022000 {
        interrupts = <73>;  /* kế thừa interrupt-parent = &intc */
    };

    i2c@44e0b000 {
        interrupts = <70>;  /* kế thừa interrupt-parent = &intc */
    };
};
```

---

## Phần 5 — Tính năng nâng cao của DTS

### 5.1 Include và kế thừa

`#include` trong DTS hoạt động giống hệt C preprocessor — nội dung file được chèn trực tiếp vào vị trí include trước khi `dtc` xử lý. Toàn bộ node và property từ file cha đều có mặt trong file con, như thể được viết trực tiếp ở đó.

Ví dụ thực tế từ BBB:

```dts
/* am335x-boneblack.dts */
/dts-v1/;
#include "am33xx.dtsi"
#include "am335x-bone-common.dtsi"
#include "am335x-boneblack-common.dtsi"

/ {
    model = "TI AM335x BeagleBone Black";
    compatible = "ti,am335x-bone-black", "ti,am335x-bone", "ti,am33xx";
};
```

Sau khi preprocessor xử lý, `dtc` nhìn thấy một file duy nhất chứa toàn bộ nội dung của cả 4 file — rồi mới compile thành DTB.

---

### 5.2 Override property

Khi file con include file cha, nếu cùng một property được khai báo ở cả hai nơi trong cùng một node, **giá trị ở file con sẽ thắng**.

Ví dụ: `am33xx.dtsi` định nghĩa `uart0` với `status = "disabled"` theo mặc định. `am335x-bone-common.dtsi` override lại để enable:

```dts
/* am33xx.dtsi — SoC level, disable mặc định */
uart0: serial@44e09000 {
    compatible = "ti,am335x-uart";
    status = "disabled";
};

/* am335x-bone-common.dtsi — board level, enable lại */
&uart0 {
    status = "okay";
};
```

Pattern `&label { }` — dùng label để mở lại một node đã được định nghĩa trước đó và thêm hoặc ghi đè property. Đây là cú pháp quan trọng nhất trong DTS vì nó là nền tảng của toàn bộ cơ chế override.

---

### 5.3 Node splitting — một node, nhiều file

Cơ chế `&label { }` không chỉ dùng để override — nó còn cho phép **chia một node ra viết ở nhiều file**, mỗi file đóng góp một phần thông tin. `dtc` sẽ merge tất cả lại thành một node duy nhất trong DTB.

Ví dụ thực tế: node `pinctrl` trên BBB được định nghĩa ở `am33xx.dtsi`, nhưng các pin group cụ thể được thêm vào ở `am335x-bone-common.dtsi`:

```dts
/* am33xx.dtsi — định nghĩa controller */
am33xx_pinmux: pinmux@44e10800 {
    compatible = "pinctrl-single";
    reg = <0x44e10800 0x0238>;
};

/* am335x-bone-common.dtsi — thêm pin groups */
&am33xx_pinmux {
    uart0_pins: pinmux_uart0_pins {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_UART0_RXD, PIN_INPUT_PULLUP, MUX_MODE0)
            AM33XX_PADCONF(AM335X_PIN_UART0_TXD, PIN_OUTPUT_PULLDOWN, MUX_MODE0)
        >;
    };
};
```

---

### 5.4 Sử dụng macro từ C header

DTS hỗ trợ `#include` file `.h` và sử dụng macro C preprocessor. Đây là cách các SoC vendor cung cấp các constant có tên thay vì magic number.

Ví dụ từ `am33xx.h`:

```c
#define AM335X_PIN_UART0_RXD    0x970
#define AM335X_PIN_UART0_TXD    0x974
#define PIN_INPUT_PULLUP        (INPUT_EN | PULL_UP)
#define MUX_MODE0               0
```

Nhờ đó, thay vì viết magic number:

```dts
pinctrl-single,pins = <0x970 0x37  0x974 0x07>;
```

Developer viết được:

```dts
pinctrl-single,pins = <
    AM33XX_PADCONF(AM335X_PIN_UART0_RXD, PIN_INPUT_PULLUP, MUX_MODE0)
    AM33XX_PADCONF(AM335X_PIN_UART0_TXD, PIN_OUTPUT_PULLDOWN, MUX_MODE0)
>;
```

---

### 5.5 `/delete-node/` và `/delete-property/`

Đôi khi file con cần xóa node hoặc property đã được khai báo ở file cha — ví dụ một peripheral có trên SoC nhưng không được nối ra trên board cụ thể này.

Xóa toàn bộ một node:

```dts
/delete-node/ &uart2;
```

Xóa một property trong node:

```dts
&uart0 {
    /delete-property/ dma-names;
};
```

---

### 5.6 `status = "disabled"` pattern

Một pattern phổ biến hơn `/delete-node/` để quản lý peripheral là dùng `status`. Thay vì xóa node, chỉ cần disable:

```dts
&uart2 {
    status = "disabled";
};
```

Kernel sẽ bỏ qua node này khi `of_platform_populate()` — không tạo `platform_device`, không probe driver. Pattern này được ưu tiên hơn `/delete-node/` vì:

- Dễ revert — chỉ cần đổi lại `"okay"`
- Node vẫn visible trong `/sys/firmware/devicetree` để debug
- Phù hợp hơn với mô hình BSP: SoC file liệt kê tất cả peripheral, board file chọn cái nào enable

---

## Phần 6 — Implicit vs Explicit — Viết node & Driver đọc DT

### 6.1 Implicit vs Explicit handling

Khi driver được probe, không phải mọi thông tin trong DT node đều do driver tự đọc. Có một ranh giới quan trọng cần hiểu rõ:

**Implicit** — thông tin được kernel core hoặc subsystem đọc và xử lý hoàn toàn, driver không cần biết đến sự tồn tại của chúng trong DT:

- `compatible` — kernel dùng để match driver, driver không bao giờ tự đọc property này
- `pinctrl-0`, `pinctrl-names` — pinctrl subsystem tự apply trước khi `probe()` được gọi
- `clocks` — clock framework cấp phát và enable clock
- `power-domains` — PM core quản lý power state
- `#address-cells`, `#size-cells` — kernel dùng để parse cây, driver không quan tâm

**Explicit** — thông tin kernel có thể giúp *lấy ra*, nhưng ý nghĩa và cách sử dụng hoàn toàn do driver hãng quyết định:

- `reg` — kernel giúp lấy địa chỉ qua `platform_get_resource()`, nhưng chỉ driver hãng biết offset `0x04` là register gì, `0x08` là register gì
- `interrupts` — kernel giúp lấy IRQ number qua `platform_get_irq()`, nhưng chỉ driver hãng biết IRQ đó xử lý event gì
- Vendor-specific properties như `vinalinux,blink-period-ms` — hoàn toàn do driver hãng tự đọc và tự hiểu

Ranh giới này quan trọng vì nó giải thích tại sao hai driver khác nhau dùng cùng một DT node có thể hoạt động rất khác nhau — cùng một `reg`, nhưng mỗi driver hiểu các register bên trong theo cách riêng của mình.

---

### 6.2 Anatomy của một custom DT node

DT node cho LED (GPIO0_20) và button (GPIO0_31) trên BBB, sử dụng địa chỉ tuyệt đối:

```dts
vinalinux_leds: vinalinux-leds@44e07000 {
    compatible = "vinalinux,gpio-led-button";
    reg = <0x44e07000 0x1000>;
    interrupt-parent = <&intc>;
    interrupts = <96>;
    vinalinux,led-pin = <20>;
    vinalinux,button-pin = <31>;
    vinalinux,blink-period-ms = <500>;
    status = "okay";
};
```

Phân tích từng thành phần:

- `compatible` — kernel dùng để tìm driver `vinalinux,gpio-led-button`
- `reg` — địa chỉ base của GPIO0, kernel giúp lấy ra, driver tự dùng để `ioremap` và truy cập register
- `interrupts` — IRQ number của GPIO0 controller, kernel giúp lấy ra, driver tự xử lý
- `vinalinux,led-pin` và `vinalinux,button-pin` — vendor property, chỉ driver này biết dùng để shift bitmask vào register
- `vinalinux,blink-period-ms` — vendor property, driver đọc để biết tần số nhấp nháy LED
- Prefix `vinalinux,` là convention bắt buộc cho vendor-specific property — tránh đụng chạm với các property chuẩn của kernel

Node này không tham chiếu `&gpio0` — driver không dùng GPIO subsystem mà truy cập register trực tiếp. Chỉ driver này biết rằng `base + 0x134` là `GPIO_OE`, `base + 0x13C` là `GPIO_DATAOUT`.

---

### 6.3 Driver đọc thông tin Explicit từ DT

Khi `probe()` được gọi, driver nhận vào một `platform_device`. Từ đó, driver có thể lấy thông tin từ DT node theo nhiều cách:

**Lấy `reg` — địa chỉ register:**

```c
struct resource *res;
void __iomem *base;

res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
base = devm_ioremap_resource(&pdev->dev, res);
```

**Lấy IRQ number:**

```c
int irq;
irq = platform_get_irq(pdev, 0);
devm_request_irq(&pdev->dev, irq, my_handler, 0, "vinalinux-led", priv);
```

**Đọc vendor property — số nguyên:**

```c
u32 period_ms;
of_property_read_u32(pdev->dev.of_node,
                     "vinalinux,blink-period-ms",
                     &period_ms);
```

**Đọc vendor property — array:**

```c
u32 values[4];
of_property_read_u32_array(pdev->dev.of_node,
                           "vinalinux,config-values",
                           values, 4);
```

**Kiểm tra empty property:**

```c
bool active_high;
active_high = of_property_read_bool(pdev->dev.of_node,
                                    "vinalinux,led-active-high");
```

---

### 6.4 Driver hoàn chỉnh

```c
// vinalinux_led_button.c
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/timer.h>

/* AM335x GPIO register offsets — AM335x TRM Table 25-5 */
#define GPIO_IRQSTATUS_0    0x02C
#define GPIO_IRQENABLE_0    0x034
#define GPIO_OE             0x134
#define GPIO_DATAIN         0x138
#define GPIO_DATAOUT        0x13C

struct vinalinux_priv {
    void __iomem        *base;
    u32                 led_pin;
    u32                 button_pin;
    u32                 blink_period_ms;
    struct timer_list   blink_timer;
    bool                blinking;
};

static void led_set(struct vinalinux_priv *priv, int val)
{
    u32 reg = readl(priv->base + GPIO_DATAOUT);
    if (val)
        reg |=  (1 << priv->led_pin);
    else
        reg &= ~(1 << priv->led_pin);
    writel(reg, priv->base + GPIO_DATAOUT);
}

static void blink_timer_cb(struct timer_list *t)
{
    struct vinalinux_priv *priv =
        from_timer(priv, t, blink_timer);

    u32 reg = readl(priv->base + GPIO_DATAOUT);
    int val = (reg >> priv->led_pin) & 1;
    led_set(priv, !val);

    if (priv->blinking)
        mod_timer(&priv->blink_timer,
                  jiffies + msecs_to_jiffies(priv->blink_period_ms));
}

static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    struct vinalinux_priv *priv = dev_id;

    /* Xóa interrupt status của GPIO0 */
    writel((1 << priv->button_pin),
           priv->base + GPIO_IRQSTATUS_0);

    priv->blinking = !priv->blinking;

    if (priv->blinking)
        mod_timer(&priv->blink_timer,
                  jiffies + msecs_to_jiffies(priv->blink_period_ms));
    else
        led_set(priv, 0);

    return IRQ_HANDLED;
}

static int vinalinux_probe(struct platform_device *pdev)
{
    struct vinalinux_priv *priv;
    struct resource *res;
    int irq, ret;
    u32 reg;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    /* --- Explicit: lấy reg từ DT, tự ioremap --- */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    priv->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(priv->base))
        return PTR_ERR(priv->base);

    /* --- Explicit: đọc vendor properties --- */
    ret = of_property_read_u32(pdev->dev.of_node,
                               "vinalinux,led-pin", &priv->led_pin);
    if (ret)
        return dev_err_probe(&pdev->dev, ret,
                             "Thiếu vinalinux,led-pin\n");

    ret = of_property_read_u32(pdev->dev.of_node,
                               "vinalinux,button-pin", &priv->button_pin);
    if (ret)
        return dev_err_probe(&pdev->dev, ret,
                             "Thiếu vinalinux,button-pin\n");

    ret = of_property_read_u32(pdev->dev.of_node,
                               "vinalinux,blink-period-ms",
                               &priv->blink_period_ms);
    if (ret)
        priv->blink_period_ms = 500;

    /* --- Config GPIO register trực tiếp --- */

    /* LED pin (GPIO0_20): set OE = 0 → output */
    reg = readl(priv->base + GPIO_OE);
    reg &= ~(1 << priv->led_pin);
    writel(reg, priv->base + GPIO_OE);

    /* Button pin (GPIO0_31): set OE = 1 → input */
    reg = readl(priv->base + GPIO_OE);
    reg |= (1 << priv->button_pin);
    writel(reg, priv->base + GPIO_OE);

    /* Enable interrupt cho button pin */
    reg = readl(priv->base + GPIO_IRQENABLE_0);
    reg |= (1 << priv->button_pin);
    writel(reg, priv->base + GPIO_IRQENABLE_0);

    /* --- Explicit: lấy IRQ từ DT --- */
    irq = platform_get_irq(pdev, 0);
    if (irq < 0)
        return dev_err_probe(&pdev->dev, irq,
                             "Không lấy được IRQ\n");

    ret = devm_request_irq(&pdev->dev, irq, button_irq_handler,
                           IRQF_TRIGGER_FALLING,
                           "vinalinux-button", priv);
    if (ret)
        return dev_err_probe(&pdev->dev, ret,
                             "Không đăng ký được IRQ\n");

    /* --- Setup timer --- */
    timer_setup(&priv->blink_timer, blink_timer_cb, 0);

    platform_set_drvdata(pdev, priv);

    dev_info(&pdev->dev,
             "probe OK — base: 0x%08x, LED: pin%d, Button: pin%d, period: %dms\n",
             (u32)res->start,
             priv->led_pin, priv->button_pin,
             priv->blink_period_ms);

    return 0;
}

static int vinalinux_remove(struct platform_device *pdev)
{
    struct vinalinux_priv *priv = platform_get_drvdata(pdev);

    del_timer_sync(&priv->blink_timer);
    led_set(priv, 0);

    return 0;
}

static const struct of_device_id vinalinux_of_match[] = {
    { .compatible = "vinalinux,gpio-led-button" },
    { }
};
MODULE_DEVICE_TABLE(of, vinalinux_of_match);

static struct platform_driver vinalinux_driver = {
    .probe  = vinalinux_probe,
    .remove = vinalinux_remove,
    .driver = {
        .name           = "vinalinux-led-button",
        .of_match_table = vinalinux_of_match,
    },
};
module_platform_driver(vinalinux_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lưu An Phú — Vinalinux CEO");
MODULE_DESCRIPTION("LED Button driver — truy cập GPIO register trực tiếp qua DT reg");
```

---

### 6.5 Flow đầy đủ từ DT node đến driver hoạt động

```
DT node (vinalinux_leds)
    │
    ▼ of_platform_populate()
platform_device được tạo
    │
    ▼ compatible matching: "vinalinux,gpio-led-button"
driver->probe(pdev) được gọi
    │
    ├── platform_get_resource()     → lấy địa chỉ reg
    ├── devm_ioremap_resource()     → map vào vmem
    ├── platform_get_irq()          → lấy IRQ number
    ├── of_property_read_u32()      → đọc led-pin, button-pin
    ├── of_property_read_u32()      → đọc blink-period-ms
    └── config GPIO_OE, GPIO_IRQENABLE trực tiếp
    │
    ▼
Driver hoạt động — nhấn button → LED nhấp nháy
```

---

### 6.6 Tại sao phân biệt Implicit vs Explicit quan trọng khi debug

Khi driver không hoạt động, biết được ranh giới này giúp thu hẹp vùng tìm kiếm:

- Clock không chạy, pin không đúng function → vấn đề ở phần Implicit — kiểm tra `clocks`, `pinctrl-0` trong DT
- Driver probe thành công nhưng hoạt động sai → vấn đề ở phần Explicit — kiểm tra `reg`, vendor properties
- Driver không probe → kiểm tra `compatible` string và `status`

---

## Phần 7 — Debug Device Tree

### 7.1 Kiểm tra warning khi compile với `dtc -W`

Trước khi flash DTB lên board, nên compile với flag kiểm tra warning để phát hiện lỗi sớm:

```bash
dtc -W no-unit_address_vs_reg \
    -I dts -O dtb \
    -o board.dtb board.dts
```

Các warning phổ biến mà `dtc` có thể phát hiện:

- `unit_address_vs_reg` — unit-address trong tên node không khớp với giá trị đầu tiên trong `reg`
- `node_name_chars` — tên node chứa ký tự không hợp lệ
- `property_name_chars` — tên property chứa ký tự không hợp lệ
- `interrupt_provider` — node có `interrupt-controller` nhưng thiếu `#interrupt-cells`

Trong kernel build system:

```bash
make dtbs W=1
```

---

### 7.2 Dịch ngược DTB để kiểm tra kết quả compile

Sau khi compile, cách nhanh nhất để kiểm tra DTB có đúng như mong muốn không là dịch ngược lại thành DTS:

```bash
dtc -I dtb -O dts -o result.dts board.dtb
```

File `result.dts` là kết quả sau khi `dtc` đã merge toàn bộ include, override, và node splitting — đây là thứ kernel thực sự nhìn thấy. Kiểm tra file này giúp phát hiện:

- Override không có tác dụng — property vẫn giữ giá trị cũ từ file cha
- Node splitting bị sai — property bị duplicate thay vì merge
- Include bị thiếu — node không xuất hiện trong output

---

### 7.3 Inspect DTB binary với `fdtdump` và `fdtget`

`fdtdump` — dump toàn bộ nội dung DTB ra dạng text:

```bash
fdtdump board.dtb | grep -A 10 "vinalinux-leds"
```

`fdtget` — đọc một property cụ thể từ DTB:

```bash
# Đọc compatible string của node
fdtget board.dtb /vinalinux-leds@44e07000 compatible

# Đọc giá trị reg
fdtget -t u board.dtb /vinalinux-leds@44e07000 reg
```

Output ví dụ:

```
vinalinux,gpio-led-button
0x44e07000 0x1000
```

---

### 7.4 Kiểm tra DT tại runtime qua sysfs

`/sys/firmware/devicetree/base/` — cây thư mục, mỗi node là một thư mục, mỗi property là một file:

```bash
# Xem cấu trúc tổng thể
ls /sys/firmware/devicetree/base/

# Đọc model của board
cat /sys/firmware/devicetree/base/model

# Kiểm tra node vinalinux-leds
ls /sys/firmware/devicetree/base/vinalinux-leds@44e07000/

# Đọc compatible
cat /sys/firmware/devicetree/base/vinalinux-leds@44e07000/compatible
```

Điểm quan trọng: đây là DTB mà bootloader đã nạp vào RAM — không phải file `.dts` trên máy tính. Nếu quên flash DTB mới sau khi sửa DTS, runtime sẽ vẫn hiển thị DTB cũ.

---

### 7.5 Kiểm tra driver probe qua sysfs và dmesg

```bash
# Liệt kê device mà driver đã bind
ls /sys/bus/platform/drivers/vinalinux-led-button/

# Kiểm tra of_node của device
ls -la /sys/bus/platform/devices/vinalinux-leds/of_node

# Lọc log liên quan đến OF
dmesg | grep "of:"

# Lọc log của driver cụ thể
dmesg | grep "vinalinux"
```

Output khi probe thành công:

```
[    2.314] vinalinux-led-button vinalinux-leds: probe OK — base: 0x44e07000, LED: pin20, Button: pin31, period: 500ms
```

Output khi compatible không match:

```
[    2.314] platform vinalinux-leds: Driver vinalinux-led-button does not match
```

Output khi `status = "disabled"` — không có log, kiểm tra bằng:

```bash
cat /sys/firmware/devicetree/base/vinalinux-leds@44e07000/status
# disabled
```

---

## Phần 8 — Lab BBB

### 8.1 Mục tiêu

Sau lab này, học viên có thể:

- Đọc hiểu cây DT đang chạy trên BBB thực tế
- Thêm một custom node vào DTS, compile và flash DTB mới
- Nạp kernel module điều khiển LED và button thông qua thông tin từ DT
- Debug khi driver không probe hoặc hoạt động sai

---

### 8.2 Chuẩn bị

Yêu cầu:

- BeagleBone Black với Debian image
- Kết nối SSH qua static IP `192.168.137.2`
- Kernel source đã clone và configured cho BBB
- Cross-compiler `arm-linux-gnueabihf-gcc` đã cài trên máy host

Kiểm tra kết nối:

```bash
ssh debian@192.168.137.2
uname -r
```

---

### 8.3 Lab 1 — Đọc hiểu DT đang chạy trên BBB

**Bước 1 — Xem cấu trúc tổng thể:**

```bash
ls /sys/firmware/devicetree/base/
```

**Bước 2 — Đọc thông tin board:**

```bash
cat /sys/firmware/devicetree/base/model
cat /sys/firmware/devicetree/base/compatible
```

**Bước 3 — Trace GPIO0 node:**

```bash
ls /sys/firmware/devicetree/base/ocp/gpio@44e07000/

cat /sys/firmware/devicetree/base/ocp/gpio@44e07000/compatible

# reg là binary — dùng xxd để đọc
xxd /sys/firmware/devicetree/base/ocp/gpio@44e07000/reg
```

**Bước 4 — Kiểm tra GPIO0 driver đã probe:**

```bash
ls /sys/bus/platform/drivers/gpio-omap/
dmesg | grep "44e07000"
```

**Câu hỏi thảo luận:**

Tại sao `reg` đọc ra dạng binary thay vì số thập phân? Điều này nói lên điều gì về cách DTB lưu trữ dữ liệu?

---

### 8.4 Lab 2 — Thêm custom node, compile và flash DTB

**Bước 1 — Mở file DTS trên máy host:**

```bash
cd linux/arch/arm/boot/dts/
vim am335x-boneblack.dts
```

**Bước 2 — Thêm node vào cuối file, trước dấu `}` cuối cùng:**

```dts
vinalinux_leds: vinalinux-leds@44e07000 {
    compatible = "vinalinux,gpio-led-button";
    reg = <0x44e07000 0x1000>;
    interrupt-parent = <&intc>;
    interrupts = <96>;
    vinalinux,led-pin = <20>;
    vinalinux,button-pin = <31>;
    vinalinux,blink-period-ms = <500>;
    status = "okay";
};
```

**Bước 3 — Compile DTB:**

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
     am335x-boneblack.dtb
```

**Bước 4 — Copy DTB lên board:**

```bash
scp arch/arm/boot/dts/am335x-boneblack.dtb \
    debian@192.168.137.2:/boot/dtbs/
```

**Bước 5 — Reboot và kiểm tra:**

```bash
ssh debian@192.168.137.2 sudo reboot

# Sau khi boot lại
ssh debian@192.168.137.2
ls /sys/firmware/devicetree/base/vinalinux-leds@44e07000/
cat /sys/firmware/devicetree/base/vinalinux-leds@44e07000/compatible
```

---

### 8.5 Lab 3 — Build và nạp driver

**Bước 1 — Tạo Makefile:**

```makefile
obj-m := vinalinux_led_button.o

KDIR := /path/to/linux
CROSS := arm-linux-gnueabihf-

all:
	make -C $(KDIR) M=$(PWD) \
	     ARCH=arm CROSS_COMPILE=$(CROSS) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
```

**Bước 2 — Build module:**

```bash
make
```

**Bước 3 — Copy module lên board và nạp:**

```bash
scp vinalinux_led_button.ko debian@192.168.137.2:~

ssh debian@192.168.137.2
sudo insmod vinalinux_led_button.ko
```

**Bước 4 — Kiểm tra probe thành công:**

```bash
dmesg | tail -5
ls /sys/bus/platform/drivers/vinalinux-led-button/
```

**Bước 5 — Test thực tế:**

Nhấn button ở GPIO0_31 — LED ở GPIO0_20 bắt đầu nhấp nháy với chu kỳ 500ms. Nhấn lần nữa — LED tắt.

**Bước 6 — Gỡ module:**

```bash
sudo rmmod vinalinux_led_button
```

---

### 8.6 Bài tập take-home

Thay đổi `vinalinux,blink-period-ms` trong DTS thành `200`, compile lại DTB, flash, nạp lại driver — **không sửa một dòng code C nào** — và quan sát LED nhấp nháy nhanh hơn.

Đây là điểm mấu chốt của bài lab: **thay đổi behavior của driver thông qua DT mà không cần recompile driver**.
