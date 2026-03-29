# hw-docs — Index

> File này giúp AI hiểu cấu trúc và mục đích của từng tài liệu trong thư mục `hw-docs/`.
> Đọc file này TRƯỚC KHI đọc bất kỳ file nào khác trong thư mục.

## 1. AM335x Technical Reference Manual (trích)

Các file dưới đây được trích xuất từ **TI AM335x Technical Reference Manual (SPRUH73Q)**.
Mỗi file tương ứng với một chapter, mô tả chi tiết về một peripheral/subsystem của SoC AM335x.

| File | Chapter | Nội dung |
|------|---------|----------|
| `am335x_ch2_memory_map.md` | Ch.2 | Memory map của ARM Cortex-A8: địa chỉ L4_WKUP, L4_PER, L4_FAST |
| `am335x_ch6_interrupt_controller.md` | Ch.6 | Interrupt Controller (INTC): cấu trúc, programming model, bảng interrupt number |
| `am335x_ch8_prcm.md` | Ch.8 | Power, Reset and Clock Management: clock domains, module enable/disable |
| `am335x_ch9_pin_mux_control.md` | Ch.9 | Control Module — Pin Mux: thanh ghi conf_\<pin\>, mode 0-7, pull-up/down |
| `am335x_ch20_dmtimer.md` | Ch.20 | DM Timer: hardware timer, prescaler, compare, capture, interrupt |
| `am335x_ch25_gpio.md` | Ch.25 | GPIO: 4 instances × 32 pins, input/output, interrupt, debounce |
| `am335x_ch26_rom_code.md` | Ch.26 | ROM Code: boot process, SYSBOOT pins, boot sources (NAND/MMC/SPI/UART/USB/EMAC), image format, FAT filesystem, wakeup |

**Thứ tự đọc khuyến nghị** khi cần hiểu một peripheral:
`ch2 (memory map)` → `ch8 (clock)` → `ch9 (pin mux)` → chapter của peripheral cần dùng (ch20, ch25, ch6...)

## 2. Device Tree source files (Linux kernel)

Các file `.dtsi` và `.dts` lấy từ Linux kernel source. Trong quá trình compile kernel,
chúng được **include lồng nhau** và gộp thành một file `.dtb` duy nhất.

Thứ tự include (từ thấp → cao):

```
am33xx.dtsi                    ← SoC-level: định nghĩa tất cả peripheral của AM33xx
  └── am335x-bone-common.dtsi  ← Board-level: config chung cho mọi BeagleBone (UART, I2C, LED, ...)
        └── am335x-boneblack-common.dtsi  ← Variant: config riêng BBB (eMMC, HDMI)
              └── am335x-boneblack.dts    ← Top-level DTS: file cuối cùng được compile thành .dtb
```

| File | Vai trò |
|------|---------|
| `am33xx.dtsi` | Định nghĩa SoC: CPU, interrupt controller, GPIO, UART, I2C, SPI, timer, clock, pin mux, ... Mọi board dùng AM33xx đều include file này |
| `am335x-bone-common.dtsi` | Config chung cho dòng BeagleBone: enable UART0, I2C0/2, MMC, USB, LED heartbeat, ... |
| `am335x-boneblack-common.dtsi` | Config riêng BeagleBone Black: enable eMMC (MMC1), HDMI (nxp,tda998x), ... |
| `am335x-boneblack.dts` | Top-level entry point. Include 3 file trên, set `model` và `compatible` string |

## 3. Device Tree header — Pin mux constants

| File | Nội dung |
|------|----------|
| `am33xx.h` | Header file (`dt-bindings/pinctrl/am33xx.h`) định nghĩa các macro dùng trong device tree: `PULL_DISABLE`, `INPUT_EN`, `MUX_MODE0`-`MUX_MODE7`, ... Được `#include` trong các file `.dtsi` |

Khi đọc pin mux config trong device tree (ví dụ `AM33XX_PADCONF(0x0950, PIN_OUTPUT, MUX_MODE7)`),
tra giá trị các macro trong file này.

## 4. Board pinout — Sơ đồ chân header

Hai file dưới đây mô tả mapping giữa **chân vật lý trên header** ↔ **tên signal** ↔ **thanh ghi pin mux**
của board BeagleBone Black.

| File | Nội dung |
|------|----------|
| `bbb_p8_header_pinout.md` | Sơ đồ chân header P8 (46 pin): tên pin, mode 0-7, offset thanh ghi control module |
| `bbb_p9_header_pinout.md` | Sơ đồ chân header P9 (46 pin): tương tự P8, bao gồm cả chân power (VDD, GND) |

**Cách sử dụng**: Khi cần điều khiển một chân GPIO cụ thể, tra pinout để biết:
- Chân đó nằm ở header nào, vị trí nào
- Offset thanh ghi pin mux (dùng với `ch9`)
- Mode nào ứng với GPIO function

## 5. Hướng dẫn Device Tree binding (Linux)

| File | Nội dung |
|------|----------|
| `devicetree_pinctrl_bindings_guide.md` | Hướng dẫn chuẩn pinctrl binding trong Linux device tree: cách khai báo `pinctrl-0`, `pinctrl-names`, pin group, pin config. Áp dụng chung cho mọi SoC, không riêng AM335x |

## 6. Schematic — Sơ đồ nguyên lý board

| File | Nội dung |
|------|----------|
| `beagle_bone_black_schematic.json` | Schematic đầy đủ của BeagleBone Black ở dạng JSON. Chứa thông tin tất cả linh kiện (IC, resistor, capacitor, connector, ...), pin connections, net names, power specs, và design rationale |

**Cấu trúc JSON**:
- `_definitions`: giải thích ý nghĩa các trường (id, refdes, value, net, pin, ...)
- `_refdes_prefixes`: quy ước ký hiệu linh kiện (R = Resistor, U = IC, P = Connector, ...)
- `components[]`: danh sách linh kiện với pins và net connections
- `nets_info`: thông tin chi tiết các net (power, signal), bao gồm connected pins

**Cách sử dụng**: Khi cần biết một tín hiệu được nối đến đâu, IC nào kết nối với chân nào,
hoặc tra cứu mạch nguồn / decoupling / pull-up, tìm trong file này theo `net` name hoặc `refdes`.

---

## Tóm tắt workflow tra cứu

Khi cần **viết driver điều khiển một chân GPIO** trên BeagleBone Black:

1. **Pinout** (`bbb_p8/p9_header_pinout.md`) → xác định chân vật lý, tìm offset pin mux
2. **Pin mux** (`am335x_ch9`) → hiểu thanh ghi control module, set mode + pull + direction
3. **Clock** (`am335x_ch8`) → enable clock cho GPIO module tương ứng
4. **GPIO** (`am335x_ch25`) → đọc/ghi GPIO register: OE, DATAOUT, DATAIN, interrupt
5. **Device tree** (`am33xx.dtsi` + `am335x-bone*.dtsi`) → xem node GPIO đã được khai báo thế nào
6. **DT binding** (`devicetree_pinctrl_bindings_guide.md` + `am33xx.h`) → khai báo pin mux trong DT overlay
