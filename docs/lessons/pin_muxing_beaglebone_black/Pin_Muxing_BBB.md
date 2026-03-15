# PIN MUXING TRÊN BEAGLEBONE BLACK
*Từ thanh ghi phần cứng đến Linux Kernel Driver*

**Giảng viên:** Lưu An Phú — CEO, Vinalinux

---

## MỤC LỤC

### PHẦN I: NỘI DUNG BÀI GIẢNG
- [1. Giới thiệu Pin Muxing](#1-giới-thiệu-pin-muxing)
- [2. Tầng phần cứng — AM335x Pad Control Register](#2-tầng-phần-cứng--am335x-pad-control-register)
- [3. Tầng Device Tree — Mô tả cấu hình pin mux](#3-tầng-device-tree--mô-tả-cấu-hình-pin-mux)
- [4. Tầng Kernel Driver — pinctrl-single](#4-tầng-kernel-driver--pinctrl-single)
- [5. Runtime Pin Muxing — bone-pinmux-helper](#5-runtime-pin-muxing--bone-pinmux-helper)
- [6. Tổng kết luồng End-to-End](#6-tổng-kết-luồng-end-to-end)

### PHẦN II: BÀI THỰC HÀNH
- [Lab 1: Đọc hiểu Device Tree pin mux](#lab-1-đọc-hiểu-device-tree-pin-mux)
- [Lab 2: Thêm pin mux cho một peripheral mới](#lab-2-thêm-pin-mux-cho-một-peripheral-mới)
- [Lab 3: Trace luồng xử lý trong kernel](#lab-3-trace-luồng-xử-lý-trong-kernel)

---

# PHẦN I: NỘI DUNG BÀI GIẢNG

## 1. Giới thiệu Pin Muxing

### Vấn đề
SoC AM335x có khoảng **142 pin I/O** nhưng bên trong chứa hàng trăm peripheral (UART, SPI, I2C, GPIO, PWM, PRU...). Không đủ chân vật lý cho tất cả peripheral cùng lúc.

### Giải pháp: Pin Multiplexing
Mỗi pin vật lý được kết nối đến một **multiplexer**, cho phép chọn 1 trong **8 chức năng** (Mode 0 – Mode 7) tại thời điểm cấu hình.

```
                    ┌─── Mode 0: uart1_rtsn
                    ├─── Mode 1: timer5
Pin D17 ──── MUX ──├─── Mode 2: dcan0_rx
(P9_19)             ├─── Mode 3: I2C2_SCL     ◄── BBB chọn mode này
                    ├─── Mode 4: spi1_cs1
                    ├─── Mode 5: pr1_uart0_rts_n
                    ├─── Mode 6: (reserved)
                    └─── Mode 7: gpio0_13
```

### Các thành phần trong hệ thống pin mux

| Tầng | Thành phần | Vai trò |
|------|-----------|---------|
| Hardware | Pad Control Register | Thanh ghi vật lý điều khiển mux |
| Device Tree | `pinctrl-single,pins` | Mô tả cấu hình mong muốn |
| Header files | `am33xx.h`, `omap.h` | Macro cho pin offset và config flags |
| Kernel driver | `pinctrl-single.c` | Parse DT, ghi thanh ghi phần cứng |
| BBB extension | `bone-pinmux-helper.c` | Thay đổi mux lúc runtime qua sysfs |

---

## 2. Tầng phần cứng — AM335x Control Module

### 2.1. Tổng quan Control Module

Control Module là khối quản lý trung tâm của SoC AM335x, base address **0x44E1_0000**. Nó chứa tất cả thanh ghi điều khiển và trạng thái không thuộc về peripheral cụ thể nào, bao gồm: pin muxing, boot config, power management, DDR PHY, Ethernet mode...

Tổng cộng **91 thanh ghi**, chia thành **7 nhóm chức năng**:

```
Control Module (0x44E1_0000)
│
├── Nhóm 1: System Identity (0h–110h)        ← ID chip, boot status
├── Nhóm 2: Power & Clock (428h–50Ch)        ← LDO, PLL, sleep, thermal
├── Nhóm 3: Device ID & Bus (600h–614h)      ← Part number, bus priority
├── Nhóm 4: Peripheral Control (620h–7FCh)   ← USB, ETH mode, PWM, OPP
├── Nhóm 5: Pad Control (800h–A34h)          ← PIN MUX ★ Quan trọng nhất
├── Nhóm 6: DDR PHY (E00h–1444h)             ← DDR impedance, voltage ref
└── Nhóm 7: EDMA & Misc (F90h–1344h)         ← DMA event, IPC với Cortex-M3
```

### 2.2. Nhóm 5: Pad Control — Thanh ghi cấu hình Pin Mux

Đây là nhóm thanh ghi **duy nhất** mà developer cần trực tiếp làm việc khi lập trình pin mux trên Linux.

**Vùng địa chỉ:**
```
Base: 0x44E1_0800
End:  0x44E1_0A34
Size: 0x238 bytes (~142 thanh ghi × 4 bytes)
```

Mỗi pin vật lý trên SoC có **1 thanh ghi 32-bit** riêng (`conf_<module>_<pin>`). Ví dụ:

| Offset | Tên thanh ghi | Pin Mode 0 | Trên BBB |
|--------|---------------|------------|----------|
| 800h | `conf_gpmc_ad0` | GPMC data | P8_25 (eMMC) |
| 854h | `conf_gpmc_a5` | GPMC addr | GPIO1_21 (User LED USR0) |
| 8A0h | `conf_lcd_data0` | LCD data | P8_45 (HDMI) |
| 970h | `conf_uart0_rxd` | UART0 RX | On-board debug console |
| 97Ch | `conf_uart1_rtsn` | UART1 RTS | P9_19 (I2C2_SCL ở Mode 3) |
| 988h | `conf_i2c0_sda` | I2C0 SDA | On-board (PMIC, EEPROM) |

**Cấu trúc thanh ghi:**

```
 31                    7   6        5        4          3        2  1  0
┌────────────────────┬────┬────────┬────────┬──────────┬────────┬────────┐
│     Reserved       │SLEW│RXACTIVE│PULLTYPE│ PULLUDEN │ MUXMODE        │
│     (all 0)        │CTRL│        │  SEL   │          │ [2:0]          │
└────────────────────┴────┴────────┴────────┴──────────┴────────────────┘
```

| Bit | Tên | Giá trị | Ý nghĩa |
|-----|-----|---------|---------|
| 6 | SLEWCTRL | 0 / 1 | Fast / Slow slew rate |
| 5 | RXACTIVE | 0 / 1 | Receiver disabled / enabled |
| 4 | PULLTYPESEL | 0 / 1 | Pulldown / Pullup |
| 3 | PULLUDEN | 0 / 1 | Pull enabled / Pull disabled |
| 2:0 | MUXMODE | 0-7 | Chọn chức năng Mode 0 đến Mode 7 |

> **Lưu ý bit 3 (PULLUDEN):** Logic đảo — giá trị 0 nghĩa là pull **enabled**, giá trị 1 nghĩa là pull **disabled**.

**Các cấu hình phổ biến:**

| Cấu hình | Giá trị hex | Bit pattern | Giải thích |
|-----------|-------------|-------------|------------|
| Output, pulldown, Mode 7 (GPIO) | `0x07` | `000_0_0_0_111` | MUXMODE=7, pull enabled, pulldown |
| Output, pullup, Mode 7 (GPIO) | `0x17` | `001_0_1_0_111` | MUXMODE=7, pull enabled, pullup |
| Output, no pull, Mode 0 | `0x08` | `000_1_0_0_000` | MUXMODE=0, pull disabled |
| Input, pulldown, Mode 7 (GPIO) | `0x27` | `010_0_0_0_111` | MUXMODE=7, RX enabled, pulldown |
| Input, pullup, Mode 7 (GPIO) | `0x37` | `010_1_1_0_111` | MUXMODE=7, RX enabled, pullup |
| Input, pullup, Mode 0 | `0x30` | `011_0_0_0_000` | MUXMODE=0, RX enabled, pullup |

**Ví dụ: Cấu hình pin P9_19 làm I2C2_SCL**

Pin P9_19 trên header BBB = chân D17 của SoC = thanh ghi `conf_uart1_rtsn` tại offset **0x97C**.

```
Giá trị = RXACTIVE | PULLUP | MUX_MODE3
        = (1 << 5) | (1 << 4) | 3
        = 0x20     | 0x10     | 0x03
        = 0x33

→ Ghi 0x33 vào thanh ghi 0x44E1_097C
→ Kết quả: Pin hoạt động ở chức năng I2C2_SCL, input enabled, pullup active
```

### 2.3. Các nhóm thanh ghi khác — Developer có cần quan tâm không?

Khi lập trình pin mux trên Linux, **nhóm 5 là đủ cho hầu hết trường hợp**. Các nhóm khác đã được kernel driver tự xử lý. Tuy nhiên, cần hiểu tại sao một số peripheral cần thêm thanh ghi ngoài pad mux.

**Nguyên tắc chung:** Pad mux chỉ giải quyết vấn đề **"chân pin nào nối đến module nào"** (nối dây). Nhưng một số module cần thêm thông tin **"hoạt động theo chế độ nào"** — thông tin này nằm ở nhóm 4 (Peripheral Control).

#### Ví dụ: Ethernet và thanh ghi `gmii_sel` (offset 650h, nhóm 4)

Module Ethernet (CPSW) của AM335x hỗ trợ 3 giao thức vật lý khác nhau:

| Giao thức | Tốc độ | Số chân | Ghi chú |
|-----------|--------|---------|---------|
| MII | 10/100 Mbps | ~16 chân | BBB dùng cái này |
| RMII | 10/100 Mbps | ~9 chân | Tiết kiệm pin |
| RGMII | 10/100/1000 Mbps | ~12 chân | Gigabit |

Pad mux (nhóm 5) nối pin vật lý đến module Ethernet, nhưng module CPSW **không biết** nên đọc tín hiệu theo chuẩn MII, RMII hay RGMII. Thanh ghi `gmii_sel` mới là nơi cho CPSW biết điều này:

```
gmii_sel (0x44E1_0650):

Bit [1:0] gmii1_sel       → 00: MII, 01: RMII, 10: RGMII
Bit [3:2] gmii2_sel       → 00: MII, 01: RMII, 10: RGMII
Bit [4]   rgmii1_idmode   → Internal delay cho RGMII1
Bit [5]   rgmii2_idmode   → Internal delay cho RGMII2
Bit [6]   rmii1_io_clk_en → Nguồn clock RMII1: từ PLL hay chip pin
Bit [7]   rmii2_io_clk_en → Nguồn clock RMII2: từ PLL hay chip pin
```

**So sánh vai trò 2 nhóm thanh ghi cho Ethernet:**

```
Pad Mux (nhóm 5)                          gmii_sel (nhóm 4)
─────────────────                          ─────────────────
conf_mii1_tx_en  = 0x00 (Mode 0, output)  gmii1_sel = 00 (MII mode)
conf_mii1_rxd0   = 0x20 (Mode 0, input)
conf_mii1_rxd1   = 0x20 (Mode 0, input)
... (13 pin nữa)

Tác dụng: NỐI DÂY                         Tác dụng: CHỌN GIAO THỨC
"Pin nào → module Ethernet"                "Module Ethernet đọc dây theo chuẩn nào"
```

Nếu chỉ cấu hình pad mux mà quên `gmii_sel` → pin đã nối đúng, nhưng CPSW không biết parse tín hiệu theo chuẩn nào → Ethernet không hoạt động.

#### Trên Linux, developer không cần ghi các thanh ghi nhóm khác thủ công

Kernel đã có driver riêng cho từng thanh ghi đặc biệt:

| Thanh ghi | Nhóm | Driver xử lý | Developer chỉ cần |
|-----------|------|--------------|-------------------|
| `gmii_sel` (650h) | 4 | `ti,am3352-phy-gmii-sel` | Set `phy-mode = "mii"` trong DT |
| `pwmss_ctrl` (664h) | 4 | PWMSS driver tự bật khi probe | Không cần làm gì |
| DDR IO ctrl (1404h–1444h) | 6 | U-Boot SPL cấu hình | Kernel không chạm |
| `tpcc_evt_mux` (F90h–FCCh) | 7 | EDMA driver | Cấu hình qua DT |

Ví dụ cho Ethernet, trong Device Tree chỉ cần:
```dts
/* Pad mux (nhóm 5) — developer tự viết */
&am33xx_pinmux {
    cpsw_default: cpsw_default {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_MII1_TX_EN, PIN_OUTPUT_PULLDOWN, MUX_MODE0)
            /* ... các pin MII khác ... */
        >;
    };
};

/* gmii_sel (nhóm 4) — driver tự ghi dựa vào phy-mode */
&cpsw_emac0 {
    phy-mode = "mii";   /* ← Driver đọc property này → ghi gmii1_sel = 00 */
};
```

> **Kết luận:** Khi lập trình pin mux trên Linux, bạn chỉ cần tập trung vào **nhóm 5 (Pad Control)**. Các nhóm thanh ghi khác đã được Linux kernel abstraction layer che giấu — driver tương ứng sẽ tự xử lý dựa vào Device Tree properties.

### 2.4. Flow cấu hình Pin Mux từ POR đến Linux

```
┌─────────────────────────────────────────────────────────┐
│ BƯỚC 1: Power-On Reset (POR)                            │
│ • Hầu hết pin: MUXMODE = 111 (Mode 7 = GPIO)           │
│ • Ngoại lệ: boot pins giữ Mode 0 để ROM đọc SYSBOOT    │
│ • Receiver disabled, pull tùy từng pad                   │
└─────────────────────┬───────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────────────┐
│ BƯỚC 2: ROM Boot                                        │
│ • ROM đọc SYS_BOOT → xác định boot từ MMC/SPI/UART...  │
│ • Cấu hình pin mux tối thiểu để load SPL                │
└─────────────────────┬───────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────────────┐
│ BƯỚC 3: U-Boot (SPL + full)                             │
│ • SPL: cấu hình DDR pins (nhóm 6)                      │
│ • U-Boot: cấu hình UART console, I2C (PMIC), MMC, ETH  │
│ • Ghi trực tiếp vào thanh ghi conf_*                    │
└─────────────────────┬───────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────────────┐
│ BƯỚC 4: Linux Kernel                                    │
│ • pinctrl-single driver parse Device Tree               │
│ • Cấu hình lại TẤT CẢ pin theo DT (không phụ thuộc     │
│   U-Boot, có thể khác hoàn toàn)                        │
│ • Mỗi device probe → request pinctrl state → ghi HW    │
│ • Có thể thay đổi runtime (sleep/active states)         │
└─────────────────────────────────────────────────────────┘
```

### 2.5. Lưu ý quan trọng

**a) Mux switching có glitch:**
Multiplexer **không phải glitch-free**. Khi thay đổi MUXMODE, pin có thể xuất hiện xung nhiễu vài nanosecond. Phải đảm bảo không gây hại cho thiết bị ngoài.

**b) Pull resistor khi output:**
Nếu pin luôn ở chế độ output, nên disable pull (PULLUDEN = 1) để tránh tiêu thụ dòng không cần thiết.

**c) Receiver cho input:**
Pin ở chế độ input **bắt buộc** RXACTIVE = 1. Nếu quên → peripheral không nhận được dữ liệu từ bên ngoài.

**d) Quyền truy cập:**
Ghi Control Module yêu cầu privileged mode. Trong Linux, kernel driver chạy ở privileged mode nên không gặp vấn đề. User mode không ghi được.

---

## 3. Tầng Device Tree — Mô tả cấu hình pin mux

### 3.1. Khai báo pinmux controller

Trong file `arch/arm/boot/dts/am33xx-l4.dtsi`, node pinmux controller được khai báo:

```dts
am33xx_pinmux: pinmux@800 {
    compatible = "pinctrl-single";          // Driver sẽ match string này
    reg = <0x800 0x238>;                    // Vùng thanh ghi: offset 0x800, size 0x238
    #pinctrl-cells = <1>;
    pinctrl-single,register-width = <32>;   // Mỗi thanh ghi 32-bit
    pinctrl-single,function-mask = <0x7f>;  // Mask 7 bit thấp (bit 0-6)
};
```

**Ý nghĩa `function-mask = 0x7f`:** Driver chỉ ghi đè 7 bit thấp (SLEWCTRL + RXACTIVE + PULLTYPESEL + PULLUDEN + MUXMODE), giữ nguyên các bit khác.

### 3.2. Hệ thống macro trong header files

Hai file header cung cấp macro cho device tree:

**File `include/dt-bindings/pinctrl/omap.h`** — Macro chung cho OMAP/AM:
```c
#define MUX_MODE0    0
#define MUX_MODE1    1
...
#define MUX_MODE7    7

// Macro tính offset từ base
#define AM33XX_PADCONF(pa, dir, mux)   OMAP_IOPAD_OFFSET((pa), 0x0800) ((dir) | (mux))
//                                     = ((pa) & 0xFFFF) - 0x0800      = config value
```

**File `include/dt-bindings/pinctrl/am33xx.h`** — Macro riêng cho AM335x:
```c
// Pin offset (địa chỉ tuyệt đối trong Control Module)
#define AM335X_PIN_I2C0_SDA         0x988
#define AM335X_PIN_I2C0_SCL         0x98c
#define AM335X_PIN_UART1_RTSN       0x97c   // = P9_19
...

// Config flags (đã override từ omap.h cho AM335x)
#define PULL_DISABLE    (1 << 3)    // bit 3 = 1: pull disabled
#define INPUT_EN        (1 << 5)    // bit 5 = 1: receiver enabled

#define PIN_OUTPUT          (PULL_DISABLE)              // = 0x08
#define PIN_OUTPUT_PULLUP   (PULL_UP)                   // = 0x10
#define PIN_OUTPUT_PULLDOWN 0                           // = 0x00
#define PIN_INPUT           (INPUT_EN | PULL_DISABLE)   // = 0x28
#define PIN_INPUT_PULLUP    (INPUT_EN | PULL_UP)        // = 0x30
#define PIN_INPUT_PULLDOWN  (INPUT_EN)                  // = 0x20
```

### 3.3. Cách macro mở ra thành giá trị

Xem xét ví dụ cấu hình I2C0:

```dts
AM33XX_PADCONF(AM335X_PIN_I2C0_SDA, PIN_INPUT_PULLUP, MUX_MODE0)
```

Bước mở macro:
```
① AM335X_PIN_I2C0_SDA = 0x988

② AM33XX_PADCONF(0x988, PIN_INPUT_PULLUP, MUX_MODE0)
   = OMAP_IOPAD_OFFSET(0x988, 0x800)   (PIN_INPUT_PULLUP | MUX_MODE0)

③ OMAP_IOPAD_OFFSET(0x988, 0x800)
   = (0x988 & 0xFFFF) - 0x800
   = 0x188                              ← offset từ base của pinmux

④ PIN_INPUT_PULLUP | MUX_MODE0
   = 0x30 | 0x00
   = 0x30                               ← giá trị config

⑤ Kết quả cuối: 0x188 0x30
   → Ghi 0x30 vào thanh ghi tại (base + 0x188)
   → Tức là 0x44E1_0800 + 0x188 = 0x44E1_0988
```

### 3.4. Định nghĩa pin group trong board DTS

Các pin mux group được định nghĩa bên trong node `&am33xx_pinmux`:

**File `am335x-bone-common.dtsi`** (dùng chung cho tất cả Bone board):
```dts
&am33xx_pinmux {
    /* LED pins → GPIO mode */
    user_leds_s0: user_leds_s0 {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_GPMC_A5, PIN_OUTPUT_PULLDOWN, MUX_MODE7)  /* gpio1_21 */
            AM33XX_PADCONF(AM335X_PIN_GPMC_A6, PIN_OUTPUT_PULLUP, MUX_MODE7)    /* gpio1_22 */
            AM33XX_PADCONF(AM335X_PIN_GPMC_A7, PIN_OUTPUT_PULLDOWN, MUX_MODE7)  /* gpio1_23 */
            AM33XX_PADCONF(AM335X_PIN_GPMC_A8, PIN_OUTPUT_PULLUP, MUX_MODE7)    /* gpio1_24 */
        >;
    };

    /* I2C0 pins → I2C mode */
    i2c0_pins: pinmux_i2c0_pins {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_I2C0_SDA, PIN_INPUT_PULLUP, MUX_MODE0)
            AM33XX_PADCONF(AM335X_PIN_I2C0_SCL, PIN_INPUT_PULLUP, MUX_MODE0)
        >;
    };

    /* UART0 pins */
    uart0_pins: pinmux_uart0_pins {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_UART0_RXD, PIN_INPUT_PULLUP, MUX_MODE0)
            AM33XX_PADCONF(AM335X_PIN_UART0_TXD, PIN_OUTPUT_PULLDOWN, MUX_MODE0)
        >;
    };
};
```

**File `am335x-boneblack-common.dtsi`** (riêng cho BBB):
```dts
&am33xx_pinmux {
    /* HDMI pins */
    nxp_hdmi_bonelt_pins: nxp_hdmi_bonelt_pins {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_LCD_DATA0, PIN_OUTPUT, MUX_MODE0)
            AM33XX_PADCONF(AM335X_PIN_LCD_DATA1, PIN_OUTPUT, MUX_MODE0)
            /* ... 16 LCD data pins + vsync, hsync, pclk, bias ... */
        >;
    };

    /* eMMC pins */
    /* (định nghĩa trong emmc_pins ở bone-common.dtsi) */

    /* McASP0 pins (audio) */
    mcasp0_pins: mcasp0_pins {
        pinctrl-single,pins = <
            AM33XX_PADCONF(AM335X_PIN_MCASP0_AHCLKX, PIN_INPUT_PULLUP, MUX_MODE0)
            AM33XX_PADCONF(AM335X_PIN_MCASP0_AHCLKR, PIN_OUTPUT_PULLDOWN, MUX_MODE2)
            /* ... */
        >;
    };
};
```

### 3.5. Gán pin group cho device

Mỗi device node tham chiếu đến pin group qua property `pinctrl-0`, `pinctrl-1`, ...:

```dts
&i2c0 {
    pinctrl-names = "default";      // Tên state
    pinctrl-0 = <&i2c0_pins>;       // State "default" → dùng i2c0_pins
    status = "okay";
};

&uart0 {
    pinctrl-names = "default";
    pinctrl-0 = <&uart0_pins>;
    status = "okay";
};

/* Một số device có nhiều state (ví dụ sleep) */
&mac {
    pinctrl-names = "default", "sleep";
    pinctrl-0 = <&cpsw_default>;    // Khi hoạt động
    pinctrl-1 = <&cpsw_sleep>;      // Khi sleep (chuyển tất cả pin về GPIO mode)
    status = "okay";
};
```

### 3.6. Chuỗi include của BBB Device Tree

```
am335x-boneblack.dts               ← Top-level, chọn board
  #include "am33xx.dtsi"            ← SoC: CPU, interrupt, clock
    #include "am33xx-l4.dtsi"       ← L4 bus: am33xx_pinmux node (pinctrl-single)
  #include "am335x-bone-common.dtsi"← Pin mux cho LED, I2C, UART, ETH, MMC
  #include "am335x-boneblack-common.dtsi" ← Pin mux cho HDMI, eMMC, McASP
```

---

## 4. Tầng Kernel Driver — pinctrl-single

Driver `drivers/pinctrl/pinctrl-single.c` xử lý toàn bộ việc parse DT và ghi thanh ghi.

### 4.1. Probe — Khởi tạo driver

Khi kernel phát hiện node có `compatible = "pinctrl-single"`, hàm `pcs_probe()` được gọi:

```c
static int pcs_probe(struct platform_device *pdev)
{
    // 1. Đọc property từ DT
    of_property_read_u32(np, "pinctrl-single,register-width", &pcs->width);
    // → width = 32

    of_property_read_u32(np, "pinctrl-single,function-mask", &pcs->fmask);
    // → fmask = 0x7f

    // 2. Map vùng thanh ghi vào virtual memory
    pcs->base = devm_ioremap(pcs->dev, res->start, resource_size(res));
    // → base = ioremap(0x44E1_0800, 0x238)

    // 3. Đăng ký với pinctrl framework
    pcs->pctl = devm_pinctrl_register(&pdev->dev, ...);
}
```

### 4.2. Parse DT — Đọc cấu hình pin

Khi một device (ví dụ i2c0) probe và request pinctrl state, framework gọi `pcs_parse_one_pinctrl_entry()`:

```c
static int pcs_parse_one_pinctrl_entry(struct pcs_device *pcs,
                                        struct device_node *np, ...)
{
    // Đọc property "pinctrl-single,pins"
    rows = pinctrl_count_index_with_args(np, "pinctrl-single,pins");

    for (i = 0; i < rows; i++) {
        // Parse từng cặp (offset, value)
        pinctrl_parse_index_with_args(np, name, i, &pinctrl_spec);

        offset = pinctrl_spec.args[0];     // VD: 0x188
        vals[found].reg = pcs->base + offset;  // VD: base + 0x188
        vals[found].val = pinctrl_spec.args[1]; // VD: 0x30
    }

    // Tạo pin group và function mapping
    pcs_add_function(pcs, &function, np->name, vals, found, ...);
    pinctrl_generic_add_group(pcs->pctl, np->name, pins, found, pcs);
}
```

### 4.3. Set Mux — Ghi thanh ghi phần cứng

Khi framework kích hoạt một pin state (ví dụ "default"), hàm `pcs_set_mux()` thực hiện ghi thanh ghi:

```c
static int pcs_set_mux(struct pinctrl_dev *pctldev,
                        unsigned fselector, unsigned group)
{
    for (i = 0; i < func->nvals; i++) {
        vals = &func->vals[i];

        val = pcs->read(vals->reg);      // Đọc giá trị hiện tại
        val &= ~mask;                     // Xóa 7 bit thấp (mask = 0x7f)
        val |= (vals->val & mask);        // Set giá trị mới
        pcs->write(val, vals->reg);       // GHI VÀO THANH GHI PHẦN CỨNG
    }
}
```

**Ví dụ cụ thể cho I2C0_SDA:**
```
read(base + 0x188)    → đọc giá trị hiện tại, giả sử = 0x07 (reset default: GPIO mode)
val &= ~0x7f          → val = 0x00 (xóa 7 bit thấp)
val |= (0x30 & 0x7f)  → val = 0x30 (set: input, pullup, mode 0)
write(0x30, base + 0x188) → pin I2C0_SDA giờ ở I2C mode, input, pullup
```

### 4.4. Sơ đồ gọi hàm trong kernel

```
Kernel boot
  │
  ├─ pcs_probe()                    ← pinctrl-single driver khởi tạo
  │    └─ ioremap + đăng ký pinctrl
  │
  ├─ i2c_omap_probe()               ← i2c0 driver bắt đầu probe
  │    └─ pinctrl_select_state("default")
  │         └─ pcs_parse_one_pinctrl_entry()  ← parse i2c0_pins từ DT
  │              └─ offset=0x188, val=0x30
  │         └─ pcs_set_mux()                  ← ghi thanh ghi
  │              └─ writel(0x30, base+0x188)   ← PIN ĐƯỢC CẤU HÌNH
  │
  ├─ serial8250_probe()              ← uart0 driver probe
  │    └─ pinctrl_select_state("default")
  │         └─ ... parse uart0_pins → ghi thanh ghi
  │
  └─ (tương tự cho mỗi device có pinctrl-0)
```

---

## 5. Runtime Pin Muxing — bone-pinmux-helper

### 5.1. Vấn đề

Cấu hình pin mux ở trên chỉ xảy ra **1 lần khi boot**. Trên BBB, nhiều pin trên header P8/P9 cần thay đổi chức năng lúc runtime (ví dụ: chuyển P9_19 từ I2C sang GPIO).

### 5.2. Giải pháp: bone-pinmux-helper driver

Driver `drivers/misc/cape/beaglebone/bone-pinmux-helper.c` cung cấp giao diện sysfs để thay đổi pin mux.

**Cấu hình trong DT** — mỗi pin có nhiều state được định nghĩa sẵn:

```dts
/* Định nghĩa tất cả mode khả dụng cho P9_19 */
&am33xx_pinmux {
    P9_19_default_pin: ... { pinctrl-single,pins = <
        AM33XX_IOPAD(0x097c, PIN_INPUT_PULLUP | MUX_MODE3) >; };  /* I2C2_SCL */
    P9_19_gpio_pin: ... { pinctrl-single,pins = <
        AM33XX_IOPAD(0x097c, PIN_OUTPUT | INPUT_EN | MUX_MODE7) >; };  /* GPIO */
    P9_19_can_pin: ... { pinctrl-single,pins = <
        AM33XX_IOPAD(0x097c, PIN_INPUT_PULLUP | MUX_MODE2) >; };  /* DCAN0_RX */
    P9_19_spi_cs_pin: ... { pinctrl-single,pins = <
        AM33XX_IOPAD(0x097c, PIN_OUTPUT_PULLUP | INPUT_EN | MUX_MODE4) >; }; /* SPI1_CS1 */
    /* ... thêm các mode khác ... */
};

/* Tạo pinmux-helper node cho P9_19 */
&ocp {
    P9_19_pinmux {
        compatible = "bone-pinmux-helper";
        status = "okay";
        pinctrl-names = "default", "gpio", "gpio_pu", "gpio_pd",
                        "gpio_input", "spi_cs", "can", "i2c",
                        "pru_uart", "timer";
        pinctrl-0 = <&P9_19_default_pin>;
        pinctrl-1 = <&P9_19_gpio_pin>;
        pinctrl-2 = <&P9_19_gpio_pu_pin>;
        /* ... */
        pinctrl-7 = <&P9_19_i2c_pin>;
    };
};
```

### 5.3. Sử dụng từ userspace

```bash
# Xem state hiện tại
cat /sys/devices/platform/ocp/ocp:P9_19_pinmux/state
# → default

# Chuyển P9_19 sang GPIO mode
echo gpio > /sys/devices/platform/ocp/ocp:P9_19_pinmux/state

# Chuyển P9_19 sang CAN mode
echo can > /sys/devices/platform/ocp/ocp:P9_19_pinmux/state
```

### 5.4. Luồng xử lý khi thay đổi state

```
Userspace: echo "gpio" > .../state
  │
  └─ bone_pinmux_helper_store()         ← sysfs write handler
       └─ pinctrl_select_state("gpio")  ← kernel pinctrl framework
            └─ pcs_set_mux()            ← pinctrl-single driver
                 └─ writel(0x2f, base + offset)  ← ghi thanh ghi HW
```

---

## 6. Tổng kết luồng End-to-End

### 6.1. Sơ đồ tổng quan

```
┌─────────────────────────────────────────────────────────────────────┐
│                        DEVICE TREE                                  │
│                                                                     │
│  am33xx-l4.dtsi          am335x-bone-common.dtsi                    │
│  ┌──────────────┐        ┌───────────────────────┐                  │
│  │ am33xx_pinmux │        │ &am33xx_pinmux {      │                  │
│  │ compatible =  │        │   i2c0_pins: ... {    │                  │
│  │ "pinctrl-     │        │     pinctrl-single,   │                  │
│  │  single"      │        │     pins = <          │                  │
│  │ reg = <0x800  │        │       0x188 0x30      │ ◄── AM33XX_     │
│  │        0x238> │        │       0x18c 0x30      │     PADCONF()   │
│  │ function-mask │        │     >;                │                  │
│  │   = <0x7f>    │        │   };                  │                  │
│  └──────┬───────┘        │ };                    │                  │
│         │                 │                       │                  │
│         │                 │ &i2c0 {               │                  │
│         │                 │   pinctrl-0 =         │                  │
│         │                 │     <&i2c0_pins>;     │                  │
│         │                 │ };                    │                  │
│         │                 └───────────┬───────────┘                  │
└─────────┼─────────────────────────────┼─────────────────────────────┘
          │                             │
          ▼                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│                     KERNEL (pinctrl-single.c)                       │
│                                                                     │
│  pcs_probe()                                                        │
│    ├─ ioremap(0x44E1_0800, 0x238)  → pcs->base                     │
│    ├─ fmask = 0x7f                                                  │
│    └─ register pinctrl device                                       │
│                                                                     │
│  pcs_parse_one_pinctrl_entry()                                      │
│    └─ parse "pinctrl-single,pins" → offset=0x188, val=0x30         │
│                                                                     │
│  pcs_set_mux()                                                      │
│    └─ for each pin:                                                 │
│         val = readl(base + 0x188)                                   │
│         val = (val & ~0x7f) | (0x30 & 0x7f)                        │
│         writel(val, base + 0x188)  ─────────────────────┐           │
└─────────────────────────────────────────────────────────┼───────────┘
                                                          │
                                                          ▼
┌─────────────────────────────────────────────────────────────────────┐
│                     PHẦN CỨNG (AM335x SoC)                          │
│                                                                     │
│  Thanh ghi 0x44E1_0988 (= 0x44E1_0800 + 0x188)                     │
│  ┌──────────────────────────────────────────────────┐               │
│  │ Bit:  31..7  │  6   │  5  │  4  │  3  │ 2  1  0 │               │
│  │       0      │  0   │  1  │  1  │  0  │ 0  0  0 │  = 0x30       │
│  │              │ Fast │ RX  │Pull │Pull │ Mode 0   │               │
│  │              │      │ ON  │ UP  │ EN  │ (I2C)    │               │
│  └──────────────────────────────────────────────────┘               │
│                                                                     │
│  Pin I2C0_SDA → Mode 0, Input enabled, Pullup active                │
└─────────────────────────────────────────────────────────────────────┘
```

### 6.2. Bảng tham chiếu nhanh — File liên quan

| Loại | File | Mô tả |
|------|------|-------|
| DT Controller | `arch/arm/boot/dts/am33xx-l4.dtsi` | Khai báo `am33xx_pinmux` node |
| DT Board | `arch/arm/boot/dts/am335x-bone-common.dtsi` | Pin mux cho LED, I2C, UART, ETH, MMC + runtime mux |
| DT Board | `arch/arm/boot/dts/am335x-boneblack-common.dtsi` | Pin mux cho HDMI, eMMC, McASP |
| DT Top | `arch/arm/boot/dts/am335x-boneblack.dts` | Top-level DTS, include các file trên |
| Header | `include/dt-bindings/pinctrl/am33xx.h` | `AM335X_PIN_*` offset, `PIN_INPUT_PULLUP`... |
| Header | `include/dt-bindings/pinctrl/omap.h` | `MUX_MODE*`, `AM33XX_PADCONF()` macro |
| Driver | `drivers/pinctrl/pinctrl-single.c` | Driver chính: parse DT → ghi thanh ghi |
| Driver | `drivers/misc/cape/beaglebone/bone-pinmux-helper.c` | Runtime mux qua sysfs |
| Driver | `drivers/gpio/gpio-of-helper.c` | Export GPIO pins cho cape-universal |
| HW Doc | `hardware_doc/am335x_ch9_pin_mux_control.md` | TRM Ch.9: pad control register spec |
| HW Doc | `hardware_doc/bbb_p8_header_pinout.md` | P8 header: pin → GPIO → mode mapping |
| HW Doc | `hardware_doc/bbb_p9_header_pinout.md` | P9 header: pin → GPIO → mode mapping |

---

# PHẦN II: BÀI THỰC HÀNH

## Lab 1: Đọc hiểu Device Tree pin mux

### Mục tiêu
Hiểu cách đọc và giải mã cấu hình pin mux trong device tree.

### Bài tập

**1.1.** Mở file `am335x-bone-common.dtsi`, tìm pin group `cpsw_default` (Ethernet). Liệt kê tất cả pin trong group, giải mã từng dòng `AM33XX_PADCONF(...)` ra:
- Tên pin
- Offset thanh ghi
- Giá trị config (hex)
- Ý nghĩa: mode mấy, input/output, pull gì

**1.2.** Dựa vào bảng pinout P8/P9 trong `hardware_doc/`, xác định pin Ethernet `MII1_TX_EN` nằm ở đâu trên board (header pin nào? hay là on-board?).

**1.3.** Giải thích tại sao Ethernet có 2 state `"default"` và `"sleep"`. State `"sleep"` cấu hình tất cả pin sang Mode 7 — điều này có ý nghĩa gì?

---

## Lab 2: Thêm pin mux cho một peripheral mới

### Mục tiêu
Thực hành thêm cấu hình pin mux cho UART1 trên header P9.

### Bài tập

**2.1.** Tra bảng pinout P9 header, xác định:
- UART1_TXD = pin nào? Offset thanh ghi?
- UART1_RXD = pin nào? Offset thanh ghi?
- Cần chọn Mode mấy cho UART1?

**2.2.** Viết pin group DTS cho UART1:
```dts
&am33xx_pinmux {
    uart1_pins: pinmux_uart1_pins {
        pinctrl-single,pins = <
            /* Điền macro AM33XX_PADCONF cho TXD và RXD */
        >;
    };
};
```

**2.3.** Viết node kích hoạt UART1 với pin group vừa tạo.

**2.4.** (Nâng cao) TXD nên cấu hình `PIN_OUTPUT` hay `PIN_OUTPUT_PULLUP`? RXD nên cấu hình `PIN_INPUT_PULLUP` hay `PIN_INPUT`? Giải thích.

---

## Lab 3: Trace luồng xử lý trong kernel

### Mục tiêu
Theo dõi luồng gọi hàm trong kernel source code.

### Bài tập

**3.1.** Mở file `drivers/pinctrl/pinctrl-single.c`. Tìm hàm `pcs_set_mux()`. Giải thích từng dòng code trong vòng `for` loop.

**3.2.** Trong hàm `pcs_probe()`, driver đọc `function-mask` như thế nào? Giá trị `0x7f` được sử dụng ở đâu trong quá trình set mux?

**3.3.** (Nâng cao) Nếu thay `function-mask` từ `0x7f` thành `0x3f` (6 bit), bit SLEWCTRL có còn được driver kiểm soát không? Giải thích.

---

*Tài liệu tham khảo:*
- *AM335x Technical Reference Manual (SPRUH73P) — Chapter 9*
- *BeagleBone Black System Reference Manual*
- *Kernel source: `drivers/pinctrl/pinctrl-single.c`*
