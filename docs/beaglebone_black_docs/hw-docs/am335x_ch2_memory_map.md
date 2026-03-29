# Chapter 2: AM335x Memory Map
**TI SPRUH73Q - October 2011 - Revised December 2019**

---

## 2.1 ARM Cortex-A8 Memory Map

### Table 2-1: L3 Memory Map

| Block Name | Start Address (hex) | End Address (hex) | Size | Description |
|-----------|-------------------|------------------|------|-------------|
| GPMC (External Memory) | 0x0000_0000 | 0x1FFF_FFFF | 512MB | 8/16-bit External Memory (EXRAM) |
| Reserved | 0x2000_0000 | 0x3FFF_FFFF | 512MB | Reserved |
| Boot ROM | 0x4000_0000 | 0x4001_FFFF | 128KB | Boot ROM |
| Reserved | 0x4002_0000 | 0x4002_BFFF | 48KB | 32-bit EXRAM - Public |
| Reserved | 0x4002_C000 | 0x400F_FFFF | 848KB | Reserved |
| Reserved | 0x4010_0000 | 0x401F_FFFF | 1MB | Reserved |
| Reserved | 0x4020_0000 | 0x402E_FFFF | 960KB | Reserved |
| Reserved | 0x402F_0000 | 0x402F_03FF | 64KB | Reserved |
| SRAM Internal | 0x402F_0400 | 0x402F_FFFF | - | 32-bit EXRAM |
| L3 OCMC0 | 0x4030_0000 | 0x4030_FFFF | 64KB | 32-bit EXRAM OCMC SRAM |
| Reserved | 0x4031_0000 | 0x403F_FFFF | 960KB | Reserved |
| Reserved | 0x4040_0000 | 0x404F_FFFF | 128KB | Reserved |
| Reserved | 0x4042_0000 | 0x404F_FFFF | 896KB | Reserved |
| Reserved | 0x4050_0000 | 0x405F_FFFF | 1MB | Reserved |
| Reserved | 0x4060_0000 | 0x407F_FFFF | 2MB | Reserved |
| Reserved | 0x4080_0000 | 0x408B_FFFF | 256KB | Reserved |
| Reserved | 0x4084_0000 | 0x40DF_FFFF | 5888KB | Reserved |
| Reserved | 0x40E0_0000 | 0x40E0_7FFF | 32KB | Reserved |
| Reserved | 0x40E0_8000 | 0x40EF_FFFF | 992KB | Reserved |
| Reserved | 0x40F0_0000 | 0x40F0_7FFF | 32KB | Reserved |
| Reserved | 0x4010_8000 | 0x40FF_FFFF | 992KB | Reserved |
| Reserved | 0x4100_0000 | 0x41FF_FFFF | 16MB | Reserved |
| Reserved | 0x4200_0000 | 0x43FF_FFFF | 32MB | Reserved |
| L3F Fast configuration registers | 0x4400_0000 | 0x443F_FFFF | 4MB | L3Fast configuration registers |
| Reserved | 0x4440_0000 | 0x447F_FFFF | 4MB | Reserved |
| L3S Slow Regs | 0x4480_0000 | 0x44BF_FFFF | 4MB | L3Slow configuration registers |
| L4_WKUP | 0x44C0_0000 | 0x44FF_FFFF | 4MB | L4_WKUP |
| Reserved | 0x4500_0000 | 0x45FF_FFFF | 16MB | Reserved |
| MCASP0 Data | 0x4600_0000 | 0x463F_FFFF | 4MB | MCASP0 Data Registers |
| MCASP1 Data | 0x4640_0000 | 0x467F_FFFF | 4MB | MCASP1 Data Registers |
| Reserved | 0x4680_0000 | 0x46FF_FFFF | 8MB | Reserved |
| Reserved | 0x4700_0000 | 0x473F_FFFF | 4MB | Reserved |

### Table 2-1: L3 Memory Map (continued)

| Block Name | Start Address (hex) | End Address (hex) | Size | Description |
|-----------|-------------------|------------------|------|-------------|
| USBSS | 0x4740_0000 | 0x4740_0FFF | 2KB | USB Subsystem Registers |
| USB0 | 0x4740_1000 | 0x4740_12FF | - | USB0 Controller Registers |
| USB0_PHY | 0x4740_1300 | 0x4740_13FF | - | USB0 PHY Registers |
| USB0 Core | 0x4740_1400 | 0x4740_17FF | - | USB0 Core Registers |
| USB1 | 0x4740_1800 | 0x4740_1AFF | - | USB1 Controller Registers |
| USB1_PHY | 0x4740_1B00 | 0x4740_1BFF | - | USB1 PHY Registers |
| USB1 Core | 0x4740_1C00 | 0x4740_1FFF | - | USB1 Core Registers |
| USB CPPI DMA Controller | 0x4740_2000 | 0x4740_2FFF | - | USB CPPI DMA Controller Registers |
| USB CPPI DMA Scheduler | 0x4740_3000 | 0x4740_3FFF | - | USB CPPI DMA Scheduler Registers |
| USB Queue Manager | 0x4740_4000 | 0x4740_7FFF | - | USB Queue Manager Registers |
| Reserved | 0x4740_8000 | 0x477F_FFFF | 4MB-32KB | Reserved |
| Reserved | 0x4780_0000 | 0x478D_FFFF | 64KB | Reserved |
| MMCHS2 | 0x4781_0000 | 0x4781_FFFF | 64KB | MMCHS2 |
| Reserved | 0x4782_0000 | 0x478F_FFFF | 4MB-128KB | Reserved |
| Reserved | 0x47C0_0000 | 0x47FF_FFFF | 4MB | Reserved |
| L4 PER | 0x4800_0000 | 0x48FF_FFFF | 16MB | L4 Peripheral (see L4_PER table) |
| TPCC (EDMA3CC) | 0x4900_0000 | 0x490F_FFFF | 1MB | EDMA3 Channel Controller Registers |
| Reserved | 0x4910_0000 | 0x497F_FFFF | 7MB | Reserved |
| TPTC0 (EDMA3TC0) | 0x4980_0000 | 0x498F_FFFF | 1MB | EDMA3 Transfer Controller 0 Registers |
| Reserved | 0x4990_0000 | 0x499F_FFFF | 1MB | Reserved |
| TPTC1 (EDMA3TC1) | 0x49A0_0000 | 0x49AF_FFFF | 1MB | EDMA3 Transfer Controller 1 Registers |
| TPTC2 (EDMA3TC2) | 0x49C0_0000 | 0x49CF_FFFF | 1MB | EDMA3 Transfer Controller 2 Registers |
| Reserved | 0x49D0_0000 | 0x49BF_FFFF | 1MB | Reserved |
| Reserved | 0x49E0_0000 | 0x49FF_FFFF | 4MB | Reserved |
| L4_FAST | 0x4A00_0000 | 0x4AFF_FFFF | 16MB | L4_FAST |
| Reserved | 0x4B00_0000 | 0x4B13_FFFF | 1280KB | Reserved |
| DebugSS_ETM | 0x4B14_0000 | 0x4B14_0FFF | 4KB | CortexA8 ETM Unit |
| DebugSS_Debug | 0x4B14_1000 | 0x4B14_1FFF | 4KB | CortexA8 Debug Unit |
| DebugSS_CTI | 0x4B14_2000 | 0x4B14_2FFF | 4KB | CortexA8 CTI Unit |
| DebugSS_ICE | 0x4B14_3000 | 0x4B14_3FFF | 4KB | ICECrusher-CS APB |
| Reserved | 0x4B14_4000 | 0x4B15_FFFF | 128KB | Reserved |
| DebugSS_DAP | 0x4B16_0000 | 0x4B16_0FFF | 4KB | Debug Subsystem: Debug Resource Manager |
| DebugSS_ETB | 0x4B16_2000 | 0x4B16_2FFF | 4KB | Debug Subsystem: Embedded Trace Buffer |
| Reserved | 0x4B16_3000 | 0x4BFF_FFFF | 15MB-396KB | Reserved |
| EMIF0 | 0x4C00_0000 | 0x4CFF_FFFF | 16MB | EMIF0 Configuration registers |
| Reserved | 0x4D00_0000 | 0x4DFF_FFFF | 16MB | Reserved |
| Reserved | 0x4E00_0000 | 0x4EFF_FFFF | 32MB | Reserved |
| GPMC | 0x5000_0000 | 0x50FF_FFFF | 16MB | GPMC Configuration registers |
| Reserved | 0x5100_0000 | 0x52FF_FFFF | 32MB | Reserved |
| Reserved | 0x5300_0000 | 0x530F_FFFF | 1MB | Reserved |
| Reserved | 0x5310_0000 | 0x531F_FFFF | 1MB | Reserved |
| Reserved | 0x5320_0000 | 0x533F_FFFF | 2MB | Reserved |
| Reserved | 0x5340_0000 | 0x534F_FFFF | 1MB | Reserved |

### Table 2-1: L3 Memory Map (continued)

| Block Name | Start Address (hex) | End Address (hex) | Size | Description |
|-----------|-------------------|------------------|------|-------------|
| Reserved | 0x5350_0000 | 0x535F_FFFF | 1MB | Reserved |
| Reserved | 0x5360_0000 | 0x54BF_FFFF | 22MB | Reserved |
| ADC_TSC Regs | 0x54C0_0000 | 0x54FF_FFFF | 4MB | ADC_TSC Main Port |
| Reserved | 0x5500_0000 | 0x55FF_FFFF | 16MB | Reserved |
| SGXS30 | 0x5600_0000 | 0x56FF_FFFF | 16MB | SGXS30 Slave Port |
| Reserved | 0x5700_0000 | 0x57FF_FFFF | 16MB | Reserved |
| Reserved | 0x5800_0000 | 0x58FF_FFFF | 16MB | Reserved |
| Reserved | 0x5900_0000 | 0x59FF_FFFF | 16MB | Reserved |
| Reserved | 0x5A00_0000 | 0x5AFF_FFFF | 16MB | Reserved |
| Reserved | 0x5B00_0000 | 0x5BFF_FFFF | 16MB | Reserved |
| Reserved | 0x5C00_0000 | 0x5DFF_FFFF | 32MB | Reserved |
| Reserved | 0x5E00_0000 | 0x5FFF_FFFF | 32MB | Reserved |
| Reserved | 0x6000_0000 | 0x7FFF_FFFF | 512MB | Reserved |
| EMIF0 SDRAM | 0x8000_0000 | 0x9FFF_FFFF | 1GB | 8/16-bit External Memory (EXRAM) |
| Reserved | 0xC000_0000 | 0xFFFF_FFFF | 1GB | Reserved |

---

## 2.2 L4_WKUP Peripheral Memory Map

### Table 2-2: L4_WKUP Peripheral Memory Map

| Region Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| L4_WKUP Configuration | 0x44C0_0000 | 0x44C0_07FF | 2KB | Address/Protection (AP) |
| - | 0x44C0_0800 | 0x44C0_0FFF | 2KB | Link Agent (LA) |
| - | 0x44C0_1000 | 0x44C0_13FF | 1KB | Initiator Port (IP0) |
| - | 0x44C0_1400 | 0x44C0_17FF | 1KB | Initiator Port (IP1) |
| Reserved | 0x44C0_1800 | 0x44C0_1FFF | 2KB | Reserved (IP2 = IP3) |
| Reserved | 0x44C0_2000 | 0x44CF_FFFF | 1MB-8KB | Reserved |
| Reserved | 0x44D0_0000 | 0x44D0_3FFF | 16KB | Reserved |
| - | 0x44D0_4000 | 0x44D0_4FFF | 4KB | Reserved |
| Reserved | 0x44D0_5000 | 0x44D7_FFFF | 492KB | Reserved |
| Reserved | 0x44D8_0000 | 0x44D8_1FFF | 8KB | Reserved |
| - | 0x44D8_2000 | 0x44D8_2FFF | 4KB | Reserved |
| Reserved | 0x44D8_3000 | 0x44DF_FFFF | 500KB | Reserved |
| CM_WKUP | 0x44E0_0000 | 0x44E0_03FF | 1KB | Clock Module Peripheral Registers |
| CM_WKUP | 0x44E0_0400 | 0x44E0_04FF | 256 Bytes | Clock Module Wakeup Registers |
| CM_PLL | 0x44E0_0500 | 0x44E0_05FF | 256 Bytes | Clock Module PLL Registers |
| CM_MPU | 0x44E0_0600 | 0x44E0_06FF | 256 Bytes | Clock Module MPU Registers |
| CM_DEVICE | 0x44E0_0700 | 0x44E0_07FF | 256 Bytes | Clock Module Device Registers |
| CM_RTC | 0x44E0_0800 | 0x44E0_08FF | 256 Bytes | Clock Module RTC Registers |
| CM_GFX | 0x44E0_0900 | 0x44E0_09FF | 256 Bytes | Clock Module Graphics Registers |
| CM_CEFUSE | 0x44E0_0A00 | 0x44E0_0AFF | 256 Bytes | Clock Module Efuse Registers |
| PRM_IRQ | 0x44E0_0B00 | 0x44E0_0BFF | 256 Bytes | Power Reset Module Device Registers |
| PRM_RTC | 0x44E0_1000 | 0x44E0_10FF | 256 Bytes | Power Reset Module RTC Registers |
| PRM_GFX | 0x44E0_1100 | 0x44E0_11FF | 256 Bytes | Power Reset Module Graphics Registers |
| PRM_CEFUSE | 0x44E0_1200 | 0x44E0_12FF | 256 Bytes | Power Reset Module Efuse Controller Registers |
| Reserved | 0x44E0_3000 | 0x44E0_3FFF | 4KB | Reserved |
| - | 0x44E0_4000 | 0x44E0_4FFF | 4KB | Reserved |
| DMTIMER0 | 0x44E0_5000 | 0x44E0_5FFF | 4KB | DMTimer0 Registers |
| - | 0x44E0_6000 | 0x44E0_6FFF | 4KB | Reserved |
| GPIO0 | 0x44E0_7000 | 0x44E0_7FFF | 4KB | GPIO Registers |
| - | 0x44E0_8000 | 0x44E0_8FFF | 4KB | Reserved |
| UART0 | 0x44E0_9000 | 0x44E0_9FFF | 4KB | UART0 Registers |
| - | 0x44E0_A000 | 0x44E0_AFFF | 4KB | Reserved |
| I2C0 | 0x44E0_B000 | 0x44E0_BFFF | 4KB | I2C Registers |
| - | 0x44E0_C000 | 0x44E0_CFFF | 4KB | Reserved |
| ADC_TSC | 0x44E0_D000 | 0x44E0_EFFF | 8KB | ADC_TSC Registers |
| - | 0x44E0_F000 | 0x44E1_0FFF | 8KB | Reserved |
| Control Module | 0x44E1_0000 | 0x44E1_1FFF | 128KB | Control Module Registers |
| DDR2/mDDR PHY | 0x44E1_2000 | 0x44E1_23FF | - | DDR2/mDDR PHY Registers |
| Reserved | 0x44E1_2400 | 0x44E3_0FFF | 4KB | Reserved |
| DMTIMER1_1ms (Accurate Timers) | 0x44E3_1000 | 0x44E3_1FFF | 4KB | DMTIMER1 1ms Registers |
| Reserved | 0x44E3_2000 | 0x44E3_2FFF | 4KB | Reserved |
| Reserved | 0x44E3_3000 | 0x44E3_3FFF | 4KB | Reserved |
| - | 0x44E3_4000 | 0x44E3_4FFF | 4KB | Reserved |
| WDT1 | 0x44E3_5000 | 0x44E3_5FFF | 4KB | Watchdog Timer Registers |
| - | 0x44E3_6000 | 0x44E3_6FFF | 4KB | Reserved |
| SmartReflex0 | 0x44E3_7000 | 0x44E3_7FFF | 4KB | L3 Registers |
| - | 0x44E3_8000 | 0x44E3_8FFF | 4KB | Reserved |
| SmartReflex1 | 0x44E3_9000 | 0x44E3_9FFF | 4KB | L3 Registers |
| - | 0x44E3_A000 | 0x44E3_AFFF | 4KB | Reserved |
| Reserved | 0x44E3_B000 | 0x44E3_DFFF | 12KB | Reserved |
| RTCSS | 0x44E3_E000 | 0x44E3_EFFF | 4KB | RTC Registers |
| - | 0x44E3_F000 | 0x44E3_FFFF | 4KB | Reserved |
| DebugSS Instrumentation ROM Agent Port | 0x44E4_0000 | 0x44E7_FFFF | 256KB | Debug Registers |
| Reserved | 0x44E8_0000 | 0x44E8_0FFF | 4KB | Reserved |
| Reserved | 0x44E8_1000 | 0x44FF_FFFF | 608KB | Reserved |
| Reserved | 0x44F0_0000 | 0x44FF_FFFF | 1MB | Reserved |

---

## 2.3 L4_PER Peripheral Memory Map

### Table 2-3: L4_PER Peripheral Memory Map

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| - | 0x4800_0000 | 0x4800_07FF | 2KB | Reserved |
| - | 0x4800_0800 | 0x4800_0FFF | 2KB | Reserved |
| Reserved | 0x4800_1000 | 0x4800_17FF | 1KB | Reserved |

### Table 2-3: L4_PER Peripheral Memory Map (continued)

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| - | 0x4800_1000 | 0x4800_17FF | 1KB | Reserved |
| - | 0x4800_1800 | 0x4800_1BFF | 1KB | Reserved |
| - | 0x4800_1C00 | 0x4800_1FFF | 1KB | Reserved |
| Reserved | 0x4800_2000 | 0x4800_3FFF | 8KB | Reserved |
| Reserved | 0x4800_4000 | 0x4800_7FFF | 16KB | Reserved |
| Reserved | 0x4800_8000 | 0x4800_8FFF | 4KB | Reserved |
| - | 0x4800_9000 | 0x4800_9FFF | 4KB | Reserved |
| - | 0x4800_A000 | 0x4800_FFFF | 24KB | Reserved |
| Reserved | 0x4801_0000 | 0x4801_0FFF | 4KB | Reserved |
| - | 0x4801_1000 | 0x4801_1FFF | 4KB | Reserved |
| Reserved | 0x4802_2000 | 0x4801_3FFF | 8KB | Reserved |
| Reserved | 0x4801_4000 | 0x4801_FFFF | 48KB | Reserved |
| Reserved | 0x4802_0000 | 0x4802_0FFF | 4KB | Reserved |
| UART1 | 0x4802_2000 | 0x4802_2FFF | 4KB | UART1 Registers |
| - | 0x4802_3000 | 0x4802_3FFF | 4KB | Reserved |
| UART2 | 0x4802_4000 | 0x4802_4FFF | 4KB | UART2 Registers |
| - | 0x4802_5000 | 0x4802_5FFF | 4KB | Reserved |
| Reserved | 0x4802_6000 | 0x4802_7FFF | 8KB | Reserved |
| Reserved | 0x4802_8000 | 0x4802_8FFF | 4KB | Reserved |
| Reserved | 0x4802_9000 | 0x4802_9FFF | 4KB | Reserved |
| I2C1 | 0x4802_A000 | 0x4802_AFFF | 4KB | I2C1 Registers |
| - | 0x4802_B000 | 0x4802_BFFF | 4KB | Reserved |
| Reserved | 0x4802_C000 | 0x4802_CFFF | 4KB | Reserved |
| - | 0x4802_D000 | 0x4802_DFFF | 4KB | Reserved |
| Reserved | 0x4802_F000 | 0x4802_FFFF | 4KB | Reserved |
| McSPI0 | 0x4803_0000 | 0x4803_0FFF | 4KB | McSPI0 Registers |
| - | 0x4803_1000 | 0x4803_1FFF | 4KB | Reserved |
| Reserved | 0x4803_2000 | 0x4803_2FFF | 4KB | Reserved |
| - | 0x4803_3000 | 0x4803_3FFF | 4KB | Reserved |
| Reserved | 0x4803_4000 | 0x4803_4FFF | 4KB | Reserved |
| - | 0x4803_5000 | 0x4803_5FFF | 4KB | Reserved |
| Reserved | 0x4803_6000 | 0x4803_6FFF | 4KB | Reserved |
| - | 0x4803_7000 | 0x4803_7FFF | 4KB | Reserved |
| McASP0 CFG | 0x4803_8000 | 0x4803_9FFF | 8KB | McASP0 CFG Registers |
| - | 0x4803_A000 | 0x4803_AFFF | 4KB | Reserved |
| Reserved | 0x4803_B000 | 0x4803_BFFF | 4KB | Reserved |
| McASP1 CFG | 0x4803_C000 | 0x4803_DFFF | 8KB | McASP1 CFG Registers |
| - | 0x4803_E000 | 0x4803_EFFF | 4KB | Reserved |
| Reserved | 0x4803_F000 | 0x4803_FFFF | 4KB | Reserved |
| DMTIMER2 | 0x4804_0000 | 0x4804_0FFF | 4KB | DMTimer2 Registers |
| - | 0x4804_1000 | 0x4804_1FFF | 4KB | Reserved |
| DMTIMER3 | 0x4804_2000 | 0x4804_2FFF | 4KB | DMTimer3 Registers |
| - | 0x4804_3000 | 0x4804_3FFF | 4KB | Reserved |
| DMTIMER4 | 0x4804_4000 | 0x4804_4FFF | 4KB | DMTimer4 Registers |
| - | 0x4804_5000 | 0x4804_5FFF | 4KB | Reserved |

### Table 2-3: L4_PER Peripheral Memory Map (continued)

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| DMTIMER5 | 0x4804_6000 | 0x4804_6FFF | 4KB | DMTimer5 Registers |
| - | 0x4804_7000 | 0x4804_7FFF | 4KB | Reserved |
| DMTIMER6 | 0x4804_8000 | 0x4804_8FFF | 4KB | DMTimer6 Registers |
| - | 0x4804_9000 | 0x4804_9FFF | 4KB | Reserved |
| DMTIMER7 | 0x4804_A000 | 0x4804_AFFF | 4KB | DMTimer7 Registers |
| - | 0x4804_B000 | 0x4804_BFFF | 4KB | Reserved |
| GPIO1 | 0x4804_C000 | 0x4804_CFFF | 4KB | GPIO1 Registers |
| - | 0x4804_D000 | 0x4804_DFFF | 4KB | Reserved |
| Reserved | 0x4804_E000 | 0x4804_EFFF | 8KB | Reserved |
| Reserved | 0x4805_0000 | 0x4805_FFFF | 64KB | Reserved |
| MMCHS0 | 0x4806_0000 | 0x4806_0FFF | 4KB | MMCHS0 Registers |
| - | 0x4806_1000 | 0x4806_1FFF | 4KB | Reserved |
| Reserved | 0x4806_2000 | 0x4807_FFFF | 120KB | Reserved |
| ELM | 0x4808_0000 | 0x4808_FFFF | 64KB | ELM Registers |
| - | 0x4809_0000 | 0x4809_0FFF | 4KB | Reserved |
| Reserved | 0x4809_1000 | 0x4809_FFFF | 60KB | Reserved |
| Reserved | 0x480A_0000 | 0x480A_FFFF | 64KB | Reserved |
| - | 0x480B_0000 | 0x480B_0FFF | 4KB | Reserved |
| Reserved | 0x480B_1000 | 0x480B_FFFF | 60KB | Reserved |
| Reserved | 0x480C_0000 | 0x480C_0FFF | 4KB | Reserved |
| - | 0x480C_1000 | 0x480C_1FFF | 4KB | Reserved |
| - | 0x480C_2000 | 0x480C_2FFF | 4KB | Reserved |
| Reserved | 0x480C_3000 | 0x480C_3FFF | 4KB | Reserved |
| Reserved | 0x480C_4000 | 0x480C_7FFF | 16KB | Reserved |
| Mailbox 0 | 0x480C_8000 | 0x480C_8FFF | 4KB | Mailbox Registers |
| - | 0x480C_9000 | 0x480C_9FFF | 4KB | Reserved |
| Spinlock | 0x480C_A000 | 0x480C_AFFF | 4KB | Spinlock Registers |
| - | 0x480C_B000 | 0x480C_BFFF | 4KB | Reserved |
| Reserved | 0x480F_0000 | 0x480F_FFFF | 208KB | Reserved |
| Reserved | 0x4810_0000 | 0x4811_FFFF | 128KB | Reserved |
| - | 0x4812_0000 | 0x4812_0FFF | 4KB | Reserved |
| Reserved | 0x4812_1000 | 0x4812_1FFF | 4KB | Reserved |
| - | 0x4812_2000 | 0x4812_2FFF | 4KB | Reserved |
| Reserved | 0x4812_3000 | 0x4812_3FFF | 4KB | Reserved |
| Reserved | 0x4812_4000 | 0x4812_7FFF | 112KB | Reserved |
| Reserved | 0x4814_0000 | 0x4815_FFFF | 128KB | Reserved |
| - | 0x4816_0000 | 0x4816_0FFF | 4K | Reserved |
| Reserved | 0x4817_0000 | 0x4817_FFFF | 124KB | Reserved |
| Reserved | 0x4818_0000 | 0x4818_2FFF | 12KB | Reserved |
| Reserved | 0x4818_3000 | 0x4818_3FFF | 4KB | Reserved |
| Reserved | 0x4818_4000 | 0x4818_7FFF | 16KB | Reserved |
| Reserved | 0x4818_8000 | 0x4818_8FFF | 4KB | Reserved |
| Reserved | 0x4818_9000 | 0x4818_9FFF | 4KB | Reserved |
| Reserved | 0x4818_A000 | 0x4818_AFFF | 4KB | Reserved |
| OCP Watchpoint | 0x4818_C000 | 0x4818_CFFF | 4KB | OCP Watchpoint Registers |
| - | 0x4818_D000 | 0x4818_EFFF | 4KB | Reserved |

### Table 2-3: L4_PER Peripheral Memory Map (continued)

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| Reserved | 0x4818_E000 | 0x4818_EFFF | 4KB | Reserved |
| - | 0x4818_F000 | 0x4818_FFFF | 4KB | Reserved |
| Reserved | 0x4819_0000 | 0x4819_0FFF | 4KB | Reserved |
| - | 0x4819_1000 | 0x4819_1FFF | 4KB | Reserved |
| Reserved | 0x4819_2000 | 0x4819_2FFF | 4KB | Reserved |
| - | 0x4819_3000 | 0x4819_3FFF | 4KB | Reserved |
| Reserved | 0x4819_4000 | 0x4819_BFFF | 32KB | Reserved |
| I2C2 | 0x4819_C000 | 0x4819_CFFF | 4KB | I2C2 Registers |
| - | 0x4819_D000 | 0x4819_DFFF | 4KB | Reserved |
| Reserved | 0x4819_E000 | 0x4819_EFFF | 4KB | Reserved |
| - | 0x4819_F000 | 0x4819_FFFF | 4KB | Reserved |
| McSPI1 | 0x481A_0000 | 0x481A_0FFF | 4KB | McSPI1 Registers |
| - | 0x481A_1000 | 0x481A_1FFF | 4KB | Reserved |
| Reserved | 0x481A_2000 | 0x481A_5FFF | 16KB | Reserved |
| UART3 | 0x481A_6000 | 0x481A_6FFF | 4KB | UART3 Registers |
| - | 0x481A_7000 | 0x481A_7FFF | 4KB | Reserved |
| UART4 | 0x481A_8000 | 0x481A_8FFF | 4KB | UART4 Registers |
| - | 0x481A_9000 | 0x481A_9FFF | 4KB | Reserved |
| UART5 | 0x481A_A000 | 0x481A_AFFF | 4KB | UART5 Registers |
| - | 0x481A_B000 | 0x481A_BFFF | 4KB | Reserved |
| GPIO2 | 0x481A_C000 | 0x481A_CFFF | 4KB | GPIO2 Registers |
| - | 0x481A_D000 | 0x481A_DFFF | 4KB | Reserved |
| GPIO3 | 0x481A_E000 | 0x481A_EFFF | 4KB | GPIO3 Registers |
| - | 0x481A_F000 | 0x481A_FFFF | 4KB | Reserved |
| Reserved | 0x481B_0000 | 0x481B_FFFF | 64KB | Reserved |
| - | 0x481C_0000 | 0x481C_0FFF | 4KB | Reserved |
| Reserved | 0x481C_1000 | 0x481C_1FFF | 4KB | Reserved |
| - | 0x481C_2000 | 0x481C_2FFF | 4KB | Reserved |
| Reserved | 0x481C_3000 | 0x481C_0FFF | 28KB | Reserved |
| Reserved | 0x481C_4000 | 0x481C_AFFF | 4KB | Reserved |
| - | 0x481C_B000 | 0x481C_BFFF | 4KB | Reserved |
| DCAN0 | 0x481C_C000 | 0x481C_DFFF | 8KB | DCAN0 Registers |
| - | 0x481C_E000 | 0x481C_FFFF | 8KB | Reserved |
| DCAN1 | 0x481D_0000 | 0x481D_1FFF | 8KB | DCAN1 Registers |
| - | 0x481D_2000 | 0x481D_3FFF | 8KB | Reserved |
| Reserved | 0x481D_4000 | 0x481D_4FFF | 4KB | Reserved |
| - | 0x481D_5000 | 0x481D_5FFF | 4KB | Reserved |
| Reserved | 0x481D_6000 | 0x481D_6FFF | 4KB | Reserved |
| - | 0x481D_7000 | 0x481D_7FFF | 4KB | Reserved |
| MMC1 | 0x481D_8000 | 0x481D_8FFF | 4KB | MMC1 Registers |
| - | 0x481D_9000 | 0x481D_9FFF | 4KB | Reserved |
| Reserved | 0x481D_A000 | 0x481F_FFFF | 152KB | Reserved |
| Interrupt controller (INTCPS) | 0x4820_0000 | 0x4820_0FFF | 4KB | Interrupt Controller Registers |
| Reserved | 0x4820_1000 | 0x4823_FFFF | 252KB | Reserved |
| MPUSS config register | 0x4824_0000 | 0x4824_0FFF | 4KB | Host ARM non-shared device mapping |
| Reserved | 0x4824_1000 | 0x4827_FFFF | 252KB | Reserved |

### Table 2-3: L4_PER Peripheral Memory Map (continued)

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| Reserved | 0x4828_0000 | 0x4828_0FFF | 4KB | Reserved |
| Reserved | 0x4828_1000 | 0x482F_FFFF | 508KB | Reserved |
| PWM Subsystem 0 eCAP0 | 0x4830_0000 | 0x4830_00FF | - | PWMSS0 eCAP0 Registers |
| - | 0x4830_0100 | 0x4830_017F | - | PWMSS eCAP0 Registers |
| eQEP0 | 0x4830_0180 | 0x4830_01FF | - | PWMSS eQEP0 Registers |
| ePWM0 | 0x4830_0200 | 0x4830_025F | - | PWMSS ePWM0 Registers |
| - | 0x4830_0260 | 0x4830_1FFF | 4KB | Reserved |
| PWM Subsystem 1 | 0x4830_2000 | 0x4830_20FF | - | PWMSS1 Configuration Registers |
| eCAP1 | 0x4830_2100 | 0x4830_217F | 4KB | PWMSS eCAP1 Registers |
| eQEP1 | 0x4830_2180 | 0x4830_21FF | - | PWMSS eQEP1 Registers |
| ePWM1 | 0x4830_2200 | 0x4830_225F | - | PWMSS ePWM1 Registers |
| - | 0x4830_2260 | 0x4830_3FFF | 4KB | Reserved |
| PWM Subsystem 2 | 0x4830_4000 | 0x4830_40FF | - | PWMSS2 Configuration Registers |
| eCAP2 | 0x4830_4100 | 0x4830_417F | 4KB | PWMSS eCAP2 Registers |
| eQEP2 | 0x4830_4180 | 0x4830_41FF | - | PWMSS eQEP2 Registers |
| ePWM2 | 0x4830_4200 | 0x4830_425F | - | PWMSS ePWM2 Registers |
| - | 0x4830_5000 | 0x4830_5FFF | 4KB | Reserved |
| Reserved | 0x4830_6000 | 0x4830_DFFF | 32KB | Reserved |
| LCD Controller | 0x4830_E000 | 0x4830_EFFF | 4KB | LCD Registers |
| - | 0x4830_F000 | 0x4830_FFFF | 4KB | Reserved |
| Reserved | 0x4831_0000 | 0x4831_1FFF | 8KB | Reserved |
| - | 0x4831_2000 | 0x4831_2FFF | 4KB | Reserved |
| Reserved | 0x4831_3000 | 0x4831_7FFF | 20KB | Reserved |
| Reserved | 0x4831_8000 | 0x4831_8FFF | 16KB | Reserved |
| - | 0x4831_C000 | 0x4831_CFFF | 4KB | Reserved |
| Reserved | 0x4832_0000 | 0x4832_5FFF | 16KB | Reserved |
| Reserved | 0x4832_6000 | 0x48FF_FFFF | 13MB-152KB | Reserved |

---

## 2.4 L4_FAST Peripheral Memory Map

### Table 2-4: L4_FAST Peripheral Memory Map

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| L4_Fast configuration | 0x4A00_007F | 0x4A00_007F | 2KB | Address/Protection (AP) |
| - | 0x4A00_0800 | 0x4A00_0FFF | 2KB | Link Agent (LA) |
| - | 0x4A00_1000 | 0x4A00_13FF | 1KB | Initiator Port (IP0) |
| - | 0x4A00_1400 | 0x4A00_17FF | 1KB | Reserved |
| - | 0x4A00_1800 | 0x4A00_1FFF | 2KB | Reserved (IP2 = IP3) |
| Reserved | 0x4A00_2000 | 0x4A07_FFFF | 504KB | Reserved |
| Reserved | 0x4A08_0000 | 0x4A0A_0FFF | 128KB | Reserved |
| - | 0x4A0A_1000 | 0x4A0A_0FFF | 4KB | Reserved |
| Reserved | 0x4A0A_1000 | 0x4A0F_FFFF | 380KB | Reserved |
| CPSW_SS | 0x4A10_0000 | 0x4A10_7FFF | 32KB | Ethernet Switch Subsystem |
| CPSW_PORT | 0x4A10_0100 | 0x4A10_07FF | - | Ethernet Switch Port Control |
| CPSW_CPDMA | 0x4A10_0800 | 0x4A10_08FF | - | CPPI DMA Controller Module |
| CPSW_STATS | 0x4A10_0900 | 0x4A10_09FF | - | Ethernet Statistics |

### Table 2-4: L4_FAST Peripheral Memory Map (continued)

| Device Name | Start Address (hex) | End Address (hex) | Size | Description |
|------------|-------------------|------------------|------|-------------|
| CPSW_STATERAM | 0x4A10_0A00 | 0x4A10_0BFF | - | CPPI DMA State RAM |
| CPSW_CPTS | 0x4A10_0C00 | 0x4A10_0CFF | - | Ethernet Time Sync Module |
| CPSW_ALE | 0x4A10_0D00 | 0x4A10_0D7F | - | Ethernet Address Lookup Engine |
| CPSW_SL1 | 0x4A10_0D80 | 0x4A10_0DBF | - | Ethernet Sliver for Port 1 |
| CPSW_SL2 | 0x4A10_0DC0 | 0x4A10_0DFF | - | Ethernet Sliver for Port 2 |
| Reserved | 0x4A10_0E00 | 0x4A10_0FFF | - | Reserved |
| MDIO | 0x4A10_1000 | 0x4A10_10FF | - | Ethernet MDIO Controller |
| - | 0x4A10_1100 | 0x4A10_11FF | - | Reserved |
| CPSW_WR | 0x4A10_1200 | 0x4A10_1FFF | - | Ethernet Subsystem Wrapper for RMII/RGMII |
| CPPI_RAM | 0x4A10_2000 | 0x4A10_3FFF | - | Communications Port Programming Interface RAM |
| Reserved | 0x4A10_9000 | 0x4A13_FFFF | 220KB | Reserved |
| Reserved | 0x4A14_0000 | 0x4A14_FFFF | 64KB | Reserved |
| Reserved | 0x4A15_0000 | 0x4A15_0FFF | 4KB | Reserved |
| Reserved | 0x4A15_1000 | 0x4A17_FFFF | 188KB | Reserved |
| Reserved | 0x4A18_0000 | 0x4A1A_1FFF | 136KB | Reserved |
| Reserved | 0x4A1A_2000 | 0x4A1A_3FFF | 8KB | Reserved |
| - | 0x4A1A_4000 | 0x4A1A_4FFF | 4KB | Reserved |
| Reserved | 0x4A1A_5000 | 0x4A1A_5FFF | 4KB | Reserved |
| - | 0x4A1A_6000 | 0x4A1A_6FFF | 4KB | Reserved |
| Reserved | 0x4A1A_7000 | 0x4A1A_7FFF | 4KB | Reserved |
| Reserved | 0x4A1A_8000 | 0x4A1A_9FFF | 8KB | Reserved |
| - | 0x4A1A_A000 | 0x4A1A_AFFF | 4KB | Reserved |
| Reserved | 0x4A1A_B000 | 0x4A1A_BFFF | 4KB | Reserved |
| Reserved | 0x4A1B_0000 | 0x4A1B_1FFF | 4KB | Reserved |
| - | 0x4A1B_2000 | 0x4A1B_2FFF | 4KB | Reserved |
| Reserved | 0x4A1B_3000 | 0x4A1B_3FFF | 4KB | Reserved |
| - | 0x4A1B_4000 | 0x4A1B_4FFF | 4KB | Reserved |
| Reserved | 0x4A1B_5000 | 0x4A1B_8FFF | 4KB | Reserved |
| - | 0x4A1B_9000 | 0x4A1B_9FFF | 4KB | Reserved |
| Reserved | 0x4A1E_0000 | 0x4A1E_FFFF | 304KB | Reserved |
| Reserved | 0x4A20_0000 | 0x4A2F_FFFF | 1MB | Reserved |
| PRU_ICSS | 0x4A30_0000 | 0x4A37_FFFF | 512KB | PRU-ICSS Instruction/Data/Control Space |
| - | 0x4A38_0000 | 0x4A38_0FFF | 4KB | Reserved |
| Reserved | 0x4A38_1000 | 0x4A3F_FFFF | 508KB | Reserved |
| Reserved | 0x4A40_0000 | 0x4AFF_FFFF | 12MB | Reserved |

---

## Ghi chú

1. **EXRAM**: External RAM - Bộ nhớ ngoài
2. **OCMC**: On-Chip Memory Controller
3. **GPMC**: General Purpose Memory Controller
4. **L3/L4**: Các bus kết nối nội bộ theo kiến trúc OMAP
5. **Peripheral**: Các thiết bị ngoại vi như UART, I2C, SPI, GPIO, Timer, v.v.

---

## Mục đích sử dụng cho Training AI

Tài liệu này cung cấp thông tin chi tiết về:
- Địa chỉ bộ nhớ của các khối chức năng
- Kích thước từng vùng nhớ
- Mô tả chức năng của từng peripheral
- Cấu trúc phân cấp bộ nhớ trong AM335x

Thông tin này rất hữu ích cho:
- Lập trình bare-metal
- Phát triển device driver
- Debug và phân tích hệ thống
- Tối ưu hóa hiệu năng bộ nhớ
