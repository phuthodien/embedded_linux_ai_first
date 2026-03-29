# 1 Introduction

## **1.1 AM335x Family**

### *1.1.1 Device Features*

This architecture is configured with different sets of features in different devices. This technical reference manual details all of the features available in current and future AM335x devices. Some features may not be available or supported in your particular device. The features supported across different AM335x devices are detailed in your device-specific data manual.

### *1.1.2 Device Identification*

Several registers help identify the type and available features of the device. The DEV\_FEATURE register in the control module is summarized in the Device Comparison section of your device-specific data manual, and [Table](#page-0-0) 1-1 summarizes the Device\_ID register.

**Table 1-1. Device\_ID (Address 0x44E10600) Bit Field Descriptions**


| Bit   | Field    | Value | Description                                                                                                                                                                                                                                   |
|-------|----------|-------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 31-28 | DEVREV   |       | Device revision<br>0000b - Silicon Revision 1.0<br>0001b - Silicon Revision 2.0<br>0010b - Silicon Revision 2.1<br>See device errata for detailed information on functionality in each device revision.<br>Reset value is revision-dependent. |
| 27-12 | PARTNUM  |       | Device part number<br>0xB944                                                                                                                                                                                                                  |
| 11-1  | MFGR     |       | Manufacturer's ID<br>0x017                                                                                                                                                                                                                    |
| 0     | Reserved |       | Read always as 0<br>0x0                                                                                                                                                                                                                       |

## **1.2 Silicon Revision Functional Differences and Enhancements**

This section describes the differences in functionality among different silicon revisions of AM335x. Enhancements introduced in silicon revisions are also described. For a description of silicon bugs that were fixed in later revisions of the device, see *AM335x ARM Cortex-A8 Microprocessors (MPUs) Silicon Errata* (literature number [SPRZ360\)](http://www.ti.com/lit/pdf/SPRZ360). Errata items that were fixed may or may not show up in this section.

### *1.2.1 Added RTC Alarm Wakeup for DeepSleep Modes*

See Section 8.1.4.5, *Wakeup Sources/Events*.

PG1.0: RTC alarm will not wake up device from DeepSleep0 and RTC-only modes.

PG2.x: RTC alarm was added as a wake-up source from DeepSleep modes and RTC-only modes.

### *1.2.2 Changed BOOTP Identifier*

See Section 26.1.9.4.2, *BOOTP (RFC 951)* and Errata Advisory 1.0.8.


PG1.0: BOOTP Identifier string is "DM814x ROM v1.0".

PG2.x: BOOTP Identifier string is "AM335x ROM".

### *1.2.3 Changed Product String in USB Descriptor*

See Section 26.1.9.6.1.2, *Enumeration Descriptors*.

PG1.0: Product string in USB descriptor is "Subarctic".

PG2.x: Product string in USB descriptor is "AM335x USB".

### *1.2.4 Added DPLL Power Switch Control and Status Registers*

See Section 9.3.1.14, *dpll\_pwr\_sw\_status Register*, and Section 9.3.1.75, *dpll\_pwr\_sw\_ctrl Register*.

PG1.0: DPLL Power Switch Control and Status registers do not exist.

PG2.x: Added DPLL Power Switch Control and DPLL Power Switch Status registers in the Control Module to facilitate power optimization.

### *1.2.5 Added Control for CORE SRAM LDO Retention Mode*

See newly added SMA2 register, Section 9.3.1.77.

PG1.0: SMA2 register does not exist.

PG2.x: Added SMA2.VSLDO\_CORE\_AUTO\_RAMP\_EN.

### *1.2.6 Added Pin Mux Options for GPMC\_A9 to Facilitate RMII Pin Muxing*

See newly added SMA2 register, Section 9.3.1.77.

PG1.0: SMA2 register does not exist.

PG2.x: Added SMA2.RMII2\_CRS\_DV\_MODE\_SEL.

### *1.2.7 Changed Polarity of Input Signal nNMI (Pin EXTINTn)*

See Section 6.3, *ARM Cortex-A8 Interrupts* and Errata Advisory 1.0.6.

PG1.0: nNMI input signal is active high.

PG2.x: nNMI input signal is active low.


### *1.2.8 Changed Default Value of ncin and pcin Bits in vtp\_ctrl Register*

See Section 9.3.1.53, *vtp\_ctrl Register*.

PG1.0: VTP\_CTRL.NCIN and VTP\_CTRL.PCIN reset value is 0.

PG2.x: VTP\_CTRL.NCIN and VTP\_CTRL.PCIN reset value is 1.

### *1.2.9 Changed Default Value of RGMII Mode to No Internal Delay*

See Section 9.3.1.30, *gmii\_sel Register* and Errata Advisory 1.0.10.

PG1.0: RGMII1\_IDMODE And RGMII2\_IDMODE reset value is 0.

PG2.x: RGMII1\_IDMODE And RGMII2\_IDMODE reset value is 1.

### *1.2.10 Changed Default Value of RMII Clock Source*

See Section 9.3.1.30, *gmii\_sel Register* and Errata Advisory 1.0.18.

PG1.0: RMII1\_IO\_CLK\_EN and RMII2\_IO\_CLK\_EN reset value is 0.

PG2.x: RMII1\_IO\_CLK\_EN and RMII2\_IO\_CLK\_EN reset value is 1.

### *1.2.11 Changed the Method of Determining Speed of Operation During EMAC Boot*

See Section 26.1.9.4, *EMAC Boot Procedure* and Errata Advisory 1.0.7.

PG1.0: Link speed is determined by CONTROL register bit 6 in the external ethernet PHY. Note that some PHYs may not update this bit, as it is not necessary as described in the 802.3 specification.

PG2.x: Link speed is determined by reading the Auto-Negotiation Advertisement and Auto-Negotiation Link Partner Base Page Ability registers in the external ethernet PHY.

### *1.2.12 Added EFUSE\_SMA Register for Help Identifying Different Device Variants*

See Section 9.3.1.49, *efuse\_sma Register*.

PG1.0: EFUSE\_SMA register value is not applicable. Value is always 0.

PG2.x: Added EFUSE\_SMA description to distinguish package type and maximum ARM frequency of the device.
