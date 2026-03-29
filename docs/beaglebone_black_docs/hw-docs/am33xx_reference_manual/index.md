# AM335x Technical Reference Manual - Index

Thư mục này chứa **AM335x Technical Reference Manual** (TI SPRUH73Q) đã được chia thành 27 chapter.
Mỗi file tương ứng với một module/subsystem của chip AM335x (SoC trên BeagleBone Black).

## Cách sử dụng

1. Xác định module cần tra cứu từ bảng bên dưới
2. Đọc file chapter tương ứng để tìm thông tin chi tiết (registers, cấu hình, hoạt động)
3. Với các file lớn (>5000 dòng), đọc phần đầu để nắm cấu trúc rồi đọc section cụ thể

## Danh sách chapters

| File | Chủ đề | Mô tả ngắn |
|------|--------|-------------|
| [ch01_introduction.md](ch01_introduction.md) | Introduction | Tổng quan AM335x family, features, block diagram |
| [ch02_memory_map.md](ch02_memory_map.md) | Memory Map | Bản đồ bộ nhớ toàn chip — địa chỉ base của mọi peripheral, L3/L4 interconnect mapping |
| [ch03_arm_mpu_subsystem.md](ch03_arm_mpu_subsystem.md) | ARM MPU Subsystem | Cortex-A8 core, cache (L1/L2), MMU, NEON, exception handling |
| [ch04_programmable_real_time_unit_subsystem_and_industrial_communication_subsystem_pru_icss.md](ch04_programmable_real_time_unit_subsystem_and_industrial_communication_subsystem_pru_icss.md) | PRU-ICSS | Programmable Real-Time Unit — 2 PRU cores 200MHz, shared memory, industrial I/O |
| [ch05_graphics_accelerator_sgx.md](ch05_graphics_accelerator_sgx.md) | Graphics (SGX) | PowerVR SGX530 GPU — OpenGL ES 2.0, registers |
| [ch06_interrupts.md](ch06_interrupts.md) | Interrupts | ARM interrupt controller (INTC) — interrupt mapping, priority, routing cho tất cả peripherals |
| [ch07_memory_subsystem.md](ch07_memory_subsystem.md) | Memory Subsystem | EMIF (DDR2/DDR3 controller), GPMC (NAND/NOR flash), OCMC RAM, ELM error correction |
| [ch08_power_reset_and_clock_management_prcm.md](ch08_power_reset_and_clock_management_prcm.md) | PRCM | Power domains, reset sources, clock tree — CM (Clock Module) & PRM (Power/Reset Module) registers |
| [ch09_control_module.md](ch09_control_module.md) | Control Module | Pin mux configuration, pad control registers, device ID, boot config |
| [ch10_interconnects.md](ch10_interconnects.md) | Interconnects | L3/L4 bus fabric — bandwidth, arbitration, firewall |
| [ch11_enhanced_direct_memory_access_edma.md](ch11_enhanced_direct_memory_access_edma.md) | EDMA | Enhanced DMA controller — channels, PaRAM sets, transfer types, event triggers, linking |
| [ch12_touchscreen_controller.md](ch12_touchscreen_controller.md) | Touchscreen / ADC | TSC_ADC_SS — touchscreen controller + 8-channel ADC, step sequencer, FIFO |
| [ch13_lcd_controller.md](ch13_lcd_controller.md) | LCD Controller | LCDC — raster mode, DMA, timing parameters, palette, display output |
| [ch14_ethernet_subsystem.md](ch14_ethernet_subsystem.md) | Ethernet (CPSW) | 3-port Ethernet switch (CPSW) — MDIO, ALE, SGMII, RMII/RGMII, DMA (CPDMA) |
| [ch15_pulse_width_modulation_subsystem_pwmss.md](ch15_pulse_width_modulation_subsystem_pwmss.md) | PWMSS | PWM subsystem — eHRPWM (high-res PWM), eCAP (capture), eQEP (quadrature encoder) |
| [ch16_universal_serial_bus_usb.md](ch16_universal_serial_bus_usb.md) | USB | USB 2.0 OTG subsystem — Mentor Graphics controller, host/device/OTG modes, DMA (CPPI) |
| [ch17_interprocessor_communication.md](ch17_interprocessor_communication.md) | IPC | Mailbox & spinlock modules cho giao tiếp giữa ARM và PRU/other processors |
| [ch18_multimedia_card_mmc.md](ch18_multimedia_card_mmc.md) | MMC/SD/SDIO | MMC host controller — SD card, eMMC, SDIO interface, DMA |
| [ch19_universal_asynchronous_receiver_transmitter_uart.md](ch19_universal_asynchronous_receiver_transmitter_uart.md) | UART | UART module — baud rate, FIFO, flow control, IrDA, RS-485 |
| [ch20_timers.md](ch20_timers.md) | Timers | DMTimer (general-purpose timer) & WDT (watchdog timer) — compare, capture, overflow |
| [ch21_i2c.md](ch21_i2c.md) | I2C | I2C master/slave — clock config, FIFO, addressing, multi-master |
| [ch22_multichannel_audio_serial_port_mcasp.md](ch22_multichannel_audio_serial_port_mcasp.md) | McASP | Multichannel Audio Serial Port — I2S/TDM, serializers, clock, DMA integration |
| [ch23_controller_area_network_can.md](ch23_controller_area_network_can.md) | CAN (DCAN) | CAN 2.0B controller — message objects, filtering, error handling, bit timing |
| [ch24_multichannel_serial_port_interface_mcspi.md](ch24_multichannel_serial_port_interface_mcspi.md) | McSPI | SPI master/slave — multi-channel, FIFO, DMA, configurable word length |
| [ch25_general_purpose_input_output.md](ch25_general_purpose_input_output.md) | GPIO | GPIO modules — input/output, interrupt (edge/level), debounce, wakeup |
| [ch26_initialization.md](ch26_initialization.md) | Initialization / Boot | Boot process — ROM code, boot devices (NAND/MMC/SPI/UART/USB), boot sequence, image format |
| [ch27_debug_subsystem.md](ch27_debug_subsystem.md) | Debug | JTAG, ETB (Embedded Trace Buffer), debug access port |

## Tra cứu nhanh theo use-case

| Bạn cần... | Đọc chapter |
|-------------|-------------|
| Địa chỉ base register của peripheral X | ch02 (Memory Map) |
| Cấu hình pin mux cho GPIO/UART/SPI/I2C | ch09 (Control Module) |
| Bật clock cho peripheral | ch08 (PRCM) |
| Cấu hình GPIO input/output, interrupt | ch25 (GPIO) |
| Giao tiếp UART | ch19 (UART) |
| Giao tiếp I2C | ch21 (I2C) |
| Giao tiếp SPI | ch24 (McSPI) |
| Điều khiển PWM / đọc encoder | ch15 (PWMSS) |
| Đọc ADC | ch12 (Touchscreen/ADC) |
| Cấu hình timer / watchdog | ch20 (Timers) |
| Interrupt mapping, số IRQ | ch06 (Interrupts) |
| Boot process, ROM bootloader | ch26 (Initialization) |
| DMA transfers | ch11 (EDMA) |
| Ethernet driver | ch14 (Ethernet/CPSW) |
| USB driver | ch16 (USB) |
| SD/eMMC driver | ch18 (MMC) |

## Ghi chú

- File gốc đầy đủ: [AM335x_RM_Complete.md](AM335x_RM_Complete.md) (~105K dòng)
- Nguồn: Texas Instruments SPRUH73Q - AM335x ARM Cortex-A8 Microprocessors Technical Reference Manual
- Một số chapter rất lớn (ch16 USB: ~38K dòng, ch11 EDMA: ~11K dòng) — nên đọc theo section cụ thể thay vì toàn bộ
