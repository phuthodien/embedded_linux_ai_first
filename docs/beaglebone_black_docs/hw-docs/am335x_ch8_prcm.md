# AM335x Power, Reset, and Clock Management (PRCM)

## Chapter 8: Power Management Architecture

---

## 8.1 Introduction

The AM335x device power-management architecture ensures maximum performance and operation time while offering versatile power-management techniques for maximum design flexibility. The architecture is built on three fundamental levels:

1. **Clock Management** - Control of clock gating and distribution
2. **Power Management** - Control of power domain switching
3. **Voltage Management** - Control of operating voltages

These management levels are enforced through **domains** - groups of modules that share common resources (clock source, voltage source, or power switch).

---

## 8.2 Device Power-Management Architecture Building Blocks

### 8.2.1 Domain Concept

A **domain** is a group of modules or subsections that share a common entity:
- **Clock Domain** - Modules sharing a common clock source
- **Power Domain** - Modules sharing a common power switch
- **Voltage Domain** - Modules sharing a common voltage source

Each domain is managed by a dedicated policy manager. For example, a clock domain's clocks are managed by a clock manager within the PRCM module.

---

## 8.3 Clock Management

The PRCM module manages the gating (switching off) and enabling of clocks to device modules based on module requirements.

### 8.3.1 Module Clock Types

Each module requires specific clock inputs divided into two categories:

#### 8.3.1.1 Interface Clock (ICLK)

**Characteristics:**
- Ensures proper communication between module/subsystem and interconnect
- Supplies the system interconnect interface and registers of the module
- Typically one interface clock per module (some may have multiple)
- Managed at device level
- Identified by `_ICLK` suffix in PRCM

#### 8.3.1.2 Functional Clock (FCLK)

**Characteristics:**
- Supplies the functional part of a module or subsystem
- A module can have one or more functional clocks:
  - **Mandatory clocks**: Required for module operation
  - **Optional clocks**: Used for specific features, can be shut down without stopping module
- Distributed directly to modules through dedicated clock tree
- Identified by `_FCLK` suffix in PRCM

### 8.3.2 Module-Level Clock Management

The PRCM module differentiates clock-management behavior based on whether a module can:
- **Initiate transactions** (master/initiator modules) → Master Standby Protocol
- **Only respond to transactions** (slave/target modules) → Slave Idle Protocol

#### 8.3.2.1 Master Standby Protocol

Used for master modules that can initiate transactions on the device interconnect.

**Standby Mode Configuration:**
Set via `<Module>_SYSCONFIG.MIDLEMODE` or `<Module>_SYSCONFIG.STANDBYMODE`

| Mode Value | Mode Name | Description |
|------------|-----------|-------------|
| 0x0 | Force-standby | Module unconditionally asserts standby request regardless of internal operations. PRCM may gate clocks immediately. **Risk of data loss** |
| 0x1 | No-standby | Module never asserts standby request. Clocks remain active. Safe but not power-efficient |
| 0x2 | Smart-standby | Module asserts standby based on internal activity. Standby signal asserted only when all transactions complete and module idle. PRCM can then gate clocks |
| 0x3 | Smart-standby wakeup-capable | Same as smart-standby but module can generate master-related wakeup events when in STANDBY state |

**Standby Status:**
Read from `CM_<Power_domain>_<Module>_CLKCTRL[x].STBYST`
- 0x0 = Module is functional
- 0x1 = Module is in standby mode

#### 8.3.2.2 Slave Idle Protocol

Allows PRCM module to control the state of a slave module through idle request/acknowledge handshake.

**Idle Mode Configuration:**
Set via `<Module>_SYSCONFIG.SIDLEMODE` or `<Module>_SYSCONFIG.IDLEMODE`

| Mode Value | Mode Name | Description |
|------------|-----------|-------------|
| 0x0 | Force-idle | Module unconditionally acknowledges idle request regardless of internal operations. **Risk of data loss** |
| 0x1 | No-idle | Module never acknowledges idle request. Clocks remain active. Safe but not power-efficient |
| 0x2 | Smart-idle | Module acknowledges idle based on internal activity. Only acknowledges when internal operations complete |
| 0x3 | Smart-idle wakeup-capable | Same as smart-idle but module can generate slave-related wakeup events (interrupt or DMA request) |

**Idle Status:**
Read from `CM_<Power_domain>_<Module>_CLKCTRL[x].IDLEST`
- 0x0 = Functional (fully functional, clocks running)
- 0x1 = In transition (between functional and idle)
- 0x2 = Idle (module idle, clocks may be gated)
- 0x3 = Disabled (module disabled by software)

#### 8.3.2.3 Module Mode Control

Software controls module operational state via `CM_<Power_domain>_<Module>_CLKCTRL[x].MODULEMODE`

| Mode Value | Mode Name | Description |
|------------|-----------|-------------|
| 0x0 | Disabled | Module is disabled. PRCM does not manage module clock and power states. Both interface and functional clocks gated |
| 0x1 | Reserved | - |
| 0x2 | Enabled | PRCM manages interface and functional clocks. Functional clock remains active unconditionally. Interface clock automatically asserted/deasserted based on clock-domain transitions |
| 0x3 | Reserved | - |

**Optional Clock Control:**
PRCM offers direct software control of optional clocks through `OptFclken` bit in programming registers.

#### 8.3.2.4 Clock Enabling Conditions

| Clock Type | Enabling Condition |
|------------|-------------------|
| Clock with STANDBY protocol | Clock Domain ready **AND** (MStandby de-asserted **OR** Mwakeup asserted) |
| Clock with IDLE protocol (interface clock) | Clock Domain ready **AND** (Idle status = FUNCT **OR** Idle status = TRANS **OR** SWakeup asserted) |
| Clock with IDLE protocol (functional clock) | Clock Domain ready **AND** (Idle status = FUNCT **OR** Idle status = IDLE **OR** Idle status = TRANS **OR** SWakeup asserted) |
| Optional clock | Clock domain ready **AND** OptFclken = Enabled ('1') |

### 8.3.3 Clock Domain

A **clock domain** is a group of modules fed by clock signals controlled by the same clock manager in PRCM.

**Purpose:**
Allows control of dynamic power consumption by gating clocks in a domain, cutting clocks to all modules in that domain.

#### 8.3.3.1 Clock Domain States

Clock domains transition between three states:

```
        Sleep condition
ACTIVE ─────────────────→ IDLE_TRANSITION
  ↑                              ↓
  │      All modules IDLE/STANDBY
  │      All domain clocks gated
  │                              ↓
  │                          INACTIVE
  │                              │
  └──────────────────────────────┘
   Domain sleep conditions not satisfied
   OR wake-up request received
```

**State Descriptions:**

| State | Description |
|-------|-------------|
| **ACTIVE** | • Every nondisabled slave module put out of IDLE state<br>• All interface clocks to nondisabled slave modules provided<br>• All functional and interface clocks to active master modules provided<br>• All enabled optional clocks provided |
| **IDLE_TRANSITION** | • Transitory state<br>• Every master module in STANDBY<br>• Idle request asserted to all slave modules<br>• Functional clocks to enabled slave modules remain active<br>• All enabled optional clocks provided |
| **INACTIVE** | • All clocks within clock domain gated<br>• Every slave module in IDLE state and disabled<br>• Every optional functional clock gated |

#### 8.3.3.2 Clock Transition Control

Controlled via `CM_<Clock_domain>_CLKSTCTRL[x].CLKTRCTRL`

| Value | Mode | Description |
|-------|------|-------------|
| 0x0 | NO_SLEEP | Sleep transition cannot be initiated. Wakeup transition may occur |
| 0x1 | SW_SLEEP | Software-forced sleep transition. Transition initiated when associated hardware conditions satisfied |
| 0x2 | SW_WKUP | Software-forced clock domain wake-up transition initiated |
| 0x3 | Reserved | - |

#### 8.3.3.3 Clock Domain Status

**Functional Clock Activity:**
Read from `CM_<Clock_domain>_CLKSTCTRL[x].CLKACTIVITY_<FCLK/Clock_name_FCLK>`

| Value | Status | Description |
|-------|--------|-------------|
| 0x0 | Gated | Functional clock of clock domain is inactive |
| 0x1 | Active | Functional clock of clock domain is running |

---

## 8.4 Power Management

The PRCM module manages switching on/off of power supply to device modules. Independent power control allows turning on/off specific sections without affecting others.

### 8.4.1 Power Domain

A **power domain** is a section (group of modules) with independent and dedicated power manager. Can be turned on/off without affecting other device parts.

Each power domain can be split into:

#### 8.4.1.1 Memory Area States

| State | Description |
|-------|-------------|
| ON | Memory array powered and fully functional |
| RETENTION | Memory array powered at reduced voltage, contents preserved |
| OFF | Memory array powered down, contents lost |

#### 8.4.1.2 Logic Area States

| State | Description |
|-------|-------------|
| ON | Logic fully powered |
| OFF | Logic power switches off. All logic (DFF) lost |

### 8.4.2 Power Domain Management

The power manager ensures all hardware conditions satisfied before initiating power domain transition.

**Control and Status Registers:**

| Register/Bit Field | Type | Description |
|-------------------|------|-------------|
| `PM_<Power_domain>_PWRSTCTRL[1:0].POWERSTATE` | Control | Selects target power state: OFF, ON, or RETENTION |
| `PM_<Power_domain>_PWRSTST[1:0].POWERSTATEST` | Status | Identifies current state of power domain |
| `PM_<Power_domain>_PWRSTST[2].LOGICSTATEST` | Status | Identifies current state of logic area: OFF or ON |
| `PM_<Power_domain>_PWRSTST[5:4].MEMSTATEST` | Status | Identifies current state of memory area: OFF, ON, or RETENTION |

#### 8.4.2.1 Power-Management Techniques

##### Adaptive Voltage Scaling (AVS)

AVS is based on Smart Reflex technology for automatic control of operating voltages to reduce active power consumption.

**Operation:**
- Power-supply voltage adapted to silicon performance
- Can be static (based on predefined performance points) or dynamic (based on real-time temperature performance)
- Comparison of predefined performance points to real-time on-chip measured performance determines voltage adjustment
- Achieves optimal performance/power trade-off across technology process spectrum and temperature variation

### 8.4.3 AM335x Power Domains

The device supports four functional power domains:

1. **PD_WKUP** - Wakeup domain (always ON)
2. **PD_MPU** - MPU subsystem domain
3. **PD_PER** - Peripheral domain
4. **PD_RTC** - RTC domain
5. **PD_GFX** - Graphics domain (optional)

---

## 8.5 Power Modes

AM335x supports five power modes, ordered from highest power consumption/lowest wakeup latency to lowest power consumption/highest wakeup latency.

### 8.5.1 Active Mode

**Application State:** All features operational

**Power, Clock, and Voltage Configuration:**
- **Power supplies:** All ON
  - VDD_MPU = 1.1V (nominal)
  - VDD_CORE = 1.1V (nominal)
- **Clocks:**
  - Main Oscillator (OSC0) = ON
  - All DPLLs locked
- **Power domains:**
  - PD_PER = ON
  - PD_MPU = ON
  - PD_GFX = ON or OFF (depending on use case)
  - PD_WKUP = ON
- **DDR:** Active

**Use Case:** Normal full-performance operation

### 8.5.2 Standby Mode

**Application State:**
DDR memory in self-refresh, contents preserved. Wakeup from any GPIO. Cortex-A8 context/register contents lost - must be saved before entering standby. On exit, context restored from DDR. Boot ROM executes and branches to system resume.

**Power, Clock, and Voltage Configuration:**
- **Power supplies:** All ON
  - VDD_MPU = 0.95V (nominal)
  - VDD_CORE = 0.95V (nominal)
- **Clocks:**
  - Main Oscillator (OSC0) = ON
  - All DPLLs in bypass
- **Power domains:**
  - PD_PER = ON
  - PD_MPU = OFF
  - PD_GFX = OFF
  - PD_WKUP = ON
- **DDR:** In self-refresh

**Key Characteristics:**
- All modules clock gated except GPIOs
- PLLs in bypass mode
- Voltage levels reduced to OPP50
- MPU context lost (save to OCMC RAM or DDR)
- Internal SRAM contents lost (PD_MPU OFF)
- Wakeup via GPIO or configured peripheral interrupts

**Power:** Lower than Active
**Latency:** Low - fast resume

### 8.5.3 DeepSleep1 Mode

**Application State:**
On-chip peripheral registers preserved. Wakeup from configured wakeup sources. Lowest sleep mode required for certain USB wakeup scenarios. On exit, boot ROM executes, Cortex-M3 performs peripheral context restore and system resume.

**Power, Clock, and Voltage Configuration:**
- **Power supplies:** All ON
  - VDD_MPU = 0.95V (nominal)
  - VDD_CORE = 0.95V (nominal)
- **Clocks:**
  - Main Oscillator (OSC0) = **OFF**
  - All DPLLs in bypass
- **Power domains:**
  - PD_PER = ON
  - PD_MPU = OFF
  - PD_GFX = OFF
  - PD_WKUP = ON
- **DDR:** In self-refresh

**Key Difference from Standby:**
- Main oscillator disabled
- Oscillator re-enabled by wakeup events via oscillator control circuit

**Power:** Lower than Standby
**Latency:** Higher than Standby (oscillator restart)

### 8.5.4 DeepSleep0 Mode

**Application State:**
All on-chip peripheral registers lost. DDR memory in self-refresh, contents preserved. Wakeup from configured wakeup sources. On exit, boot ROM executes, checks resume state, and redirects to DDR. Cortex-M3 performs peripheral context restore followed by system resume.

**Power, Clock, and Voltage Configuration:**
- **Power supplies:** All ON
  - VDD_MPU = 0.95V (nominal)
  - VDD_CORE = 0.95V (nominal)
- **Clocks:**
  - Main Oscillator (OSC0) = OFF
  - All DPLLs in bypass
- **Power domains:**
  - PD_PER = **OFF**
  - PD_MPU = OFF
  - PD_GFX = OFF
  - PD_WKUP = ON
- **DDR:** In self-refresh

**Key Characteristics:**
- All on-chip power domains shut off (except PD_WKUP and PD_RTC)
- VDD_CORE power to DPLLs turned OFF via dpll_pwr_sw_ctrl (PG 2.x only)
- VDDS_SRAM_CORE_BG in retention using SMA2.vsldo_core_auto_ramp_en (PG 2.x only)
- Internal SRAM contents lost
- Peripheral and MPU context must be saved to DDR before sleep
- OCMC RAM powered to preserve internal information
- Boot ROM checks DeepSleep0 resume state on wakeup

**Use Case:** Very low power during inactivity while maintaining DDR contents. Lowest power mode with DDR retention - avoids full cold boot.

**Power:** Very low
**Latency:** High (context restore from DDR)

### 8.5.5 RTC-Only Mode

**Application State:**
RTC timer remains active, all other device functionality disabled.

**Power, Clock, and Voltage Configuration:**
- **Power supplies:**
  - All OFF except VDDS_RTC
  - VDD_MPU = 0V
  - VDD_CORE = 0V
- **Clocks:**
  - Main Oscillator (OSC0) = OFF
- **Power domains:**
  - All OFF

**Key Characteristics:**
- Ultra-low power mode
- Only RTC subsystem operational
- All context and memories lost
- Only RTC power supply required
- RTC battery backup domain includes:
  - RTCSS (RTC subsystem)
  - Dedicated 32.768 kHz crystal oscillator
  - pmic_power_en I/O
  - ext_wakeup I/O

**Wakeup Sources:**
- ext_wakeup0 signal only
- RTC Alarm (ALARM) only

**Wakeup Process:**
- Device drives pmic_pwr_enable to initiate PMIC power-up sequence
- Device must go through full cold boot

**Use Case:** Ultra-low power battery-backed operation with RTC functionality only

**Power:** Extremely low
**Latency:** Extremely high (full cold boot)

### 8.5.6 Power Mode Comparison Table

| Power Mode | Power Consumption | Wakeup Latency | Main Oscillator | PD_PER | PD_MPU | DDR State | Context Loss |
|-----------|-------------------|----------------|-----------------|---------|---------|-----------|--------------|
| Active | Highest | N/A | ON | ON | ON | Active | None |
| Standby | High | Low | ON | ON | OFF | Self-refresh | MPU only |
| DeepSleep1 | Medium | Medium | **OFF** | ON | OFF | Self-refresh | MPU only |
| DeepSleep0 | Very Low | High | OFF | **OFF** | OFF | Self-refresh | All except DDR |
| RTC-Only | Extremely Low | Extremely High | OFF | OFF | OFF | **OFF** | All |

### 8.5.7 Internal RTC LDO

The device contains an internal LDO regulator powering the RTC digital core. Can be disabled in certain configurations to save power.

**Configuration Scenarios:**

#### Scenario 1: RTC Functionality Not Used

**Connections:**
- RTC_KALDO_ENn → VDDS_RTC
- CAP_VDD_RTC → VDD_CORE
- RTC_PWRONRSTn → GND

**Result:** Internal RTC LDO disabled, external VDD_CORE supplies RTC digital core, RTC stays in reset, achieves lower power in all low power modes.

#### Scenario 2: RTC Used, RTC-Only Mode Not Required

**Connections:**
- Same as Scenario 1, but RTC_PWRONRSTn → PWRONRSTn (may require level shifting)

**Result:** Full RTC functionality without internal LDO consuming power.

#### Scenario 3: RTC Used with RTC-Only Mode Required

**Connections:**
- RTC_KALDO_ENn → GND
- CAP_VDD_RTC → 1µF decoupling capacitor to GND
- RTC_PWRONRSTn → 1.8V RTC power-on reset
- PMIC_POWER_EN → PMIC power input
- EXT_WAKEUP0 → wakeup source

**Result:** Internal LDO required for proper wakeup signaling from RTC domain.

---

## 8.6 Wakeup Management

### 8.6.1 Wakeup Sources/Events

The following events wake the device from deep sleep (low power) modes. These are part of the Wakeup Power domain and remain always ON:

- **GPIO0 bank** - General purpose I/O wakeup
- **dmtimer1_1ms** - Timer-based wakeup
- **USB2PHY** - USB resume signaling from suspend (both USB ports supported)
- **TSC** - Touch screen controller, ADC monitor functions
- **UART0** - Infrared support
- **I2C0** - I2C interface
- **RTC alarm** - Real-time clock alarm

**Note:** These wake events apply to all deep sleep modes and standby mode.

### 8.6.2 Main Oscillator Control During Deep Sleep

The **DeepSleep oscillator circuit** controls the main oscillator:

**Configuration:**
- Set `DEEPSLEEP_CTRL.DSENABLE = 1` to activate oscillator control circuit for deep sleep
- Configure `DEEPSLEEP_CTRL.DSCOUNT` for delay period before re-enabling oscillator

**Operation:**
1. When oscillator control is activated and Wake M3 enters standby:
   - Oscillator control disables the oscillator
   - Clock shuts OFF
2. Any async event from wakeup sources:
   - Oscillator control re-enables oscillator
   - After DSCOUNT configured period

### 8.6.3 USB Wakeup Scenarios

#### USB Wakeup Event Types

Two possible wakeup events generated:

1. **PHY WKUP:** Internal wakeup signal to Cortex-M3 generated by USB PHY based on USB signaling
2. **VBUS2GPIO:** External wakeup from level change on VBUS voltage (requires external board solution routing VBUS to GPIO with level shifting)

#### USB Wakeup Use Cases

**USB Connect Use Cases:**

| System Sleep State | USB Controller State | USB Mode | Supported | Wakeup Event |
|-------------------|---------------------|----------|-----------|--------------|
| DS0 | POWER OFF | Host | No | N/A |
| DS0 | POWER OFF | Device | Yes | VBUS2GPIO |
| DS1/Standby | Clock Gated | Host | Yes | PHY WKUP |
| DS1/Standby | Clock Gated | Device | Yes | VBUS2GPIO |

**USB Suspend/Resume Use Cases:**

| System Sleep State | USB Controller State | USB Mode | Supported | Wakeup Event |
|-------------------|---------------------|----------|-----------|--------------|
| DS0 | POWER OFF | Host | No | N/A |
| DS0 | POWER OFF | Device | No | N/A |
| DS1/Standby | Clock Gated | Host | Yes | PHY WKUP |
| DS1/Standby | Clock Gated | Device | Yes | PHY WKUP |

**USB Disconnect Use Cases:**

| System Sleep State | USB Controller State | USB Mode | Supported | Wakeup Event |
|-------------------|---------------------|----------|-----------|--------------|
| DS0 | POWER OFF | Host | No | N/A |
| DS0 | POWER OFF | Device | No | N/A |
| DS1/Standby | Clock Gated | Host | Yes | PHY WKUP |
| DS1/Standby | Clock Gated | Device | Yes | VBUS2GPIO |

**Note:** DeepSleep1 is the lowest sleep mode required for certain USB wakeup scenarios.

---

## 8.7 Power Management Sequencing with Cortex-M3

### 8.7.1 Overview

AM335x contains a dedicated **Cortex-M3 processor** to handle power management transitions. Located in Wake up Power Domain (PD_WKUP).

**Architecture:**
- **Cortex-A8 MPU:** Implements power modes, executes application
- **Cortex-M3:** Handles low-level power management control
- **Inter-Processor Communication (IPC):** Registers in Control Module for communication

**General Principle:** Cortex-A8 and Cortex-M3 are not expected to be active simultaneously. Cortex-M3 along with PRCM is the power manager primarily for PD_MPU and PD_PER. Other power domains (e.g., PD_GFX) may be handled directly by Cortex-A8 MPU software.

### 8.7.2 Power Management Sequence

**Basic Flow:**

1. During Active power mode: Cortex-A8 MPU executes WFI instruction to enter IDLE mode
2. Cortex-M3 gets interrupt and becomes active
3. Cortex-M3 powers down MPU power domain (if required)
4. Cortex-M3 registers interrupt for wakeup peripheral
5. Cortex-M3 executes WFI and goes into idle state
6. Wakeup event triggers interrupt to Cortex-M3 system
7. Cortex-M3 wakes up Cortex-A8 MPU

### 8.7.3 IPC Mechanism

**IPC Register Mapping:**

| Register | Bits | Field | Direction | Purpose |
|----------|------|-------|-----------|---------|
| IPC_MSG_REG0 | [15:0] | CMD_STAT | MPU→CM3 | Command status |
| | [31:16] | CMD_ID | MPU→CM3 | Command ID |
| IPC_MSG_REG1 | [31:0] | CMD param1 | MPU→CM3 | Command parameter 1 |
| IPC_MSG_REG2 | [31:0] | CMD param2 | MPU→CM3 | Command parameter 2 |
| IPC_MSG_REG3 | - | - | CM3→MPU | Response/status from CM3 |
| IPC_MSG_REG4-6 | - | Reserved | - | Reserved for future use |
| IPC_MSG_REG7 | [31:0] | Customer Use | Both | Available for customer use |

**CMD_STAT Field Values:**

| Value | Name | Description |
|-------|------|-------------|
| 0x0 | PASS | In initialization phase, CM3 successfully initialized. For other tasks, task completed successfully |
| 0x1 | FAIL | In initialization phase, CM3 could not initialize. For other tasks, error occurred. Check trace vector for details |
| 0x2 | WAIT4OK | CM3 INTC will catch next WFI of A8 and continue with pre-defined sequence |

**CMD_ID Field Values:**

| Value | Name | Description |
|-------|------|-------------|
| 0x1 | CMD_RTC | 1. Initiates force_sleep on interconnect clocks<br>2. Turns off MPU and PER power domains<br>3. Programs RTC alarm register for deasserting pmic_pwr_enable |
| 0x2 | CMD_RTC_FAST | Programs RTC alarm register for deasserting pmic_pwr_enable |
| 0x3 | CMD_DS0 | 1. Initiates force_sleep on interconnect clocks<br>2. Turns off MPU and PER power domains<br>3. Configures system for disabling MOSC when CM3 executes WFI |
| 0x5 | CMD_DS1 | 1. Initiates force_sleep on interconnect clocks<br>2. Turns off MPU power domain<br>3. Configures system for disabling MOSC when CM3 executes WFI |

### 8.7.4 Sleep Sequencing Guidelines

**Recommended Sleep Sequence:**

1. Application saves context of peripherals to memories supporting retention and DDR (required for DeepSleep0)
2. MPU OCMC_RAM remains in retention
3. Unused power domains turned OFF - program clock/power domain PWRSTCTRL, save contexts
4. Software populates L3_OCMC_RAM for wakeup restoration:
   - Save EMIF settings
   - Public/secure restoration pointers
5. Execute WFI from SRAM
6. Any peripheral interrupt triggers wake interrupt to Cortex-M3 via Cortex-A8 MPU's WKUP signal
7. After MPU power domain clock gated, PRCM provides interrupt to Cortex-M3
8. Cortex-M3 starts execution and performs low-level power sequencing:
   - Turns off certain power domains
   - Eventually executes WFI
9. Hardware oscillator control circuit disables oscillator once Cortex-M3 goes into WFI

### 8.7.5 Wakeup Sequencing Guidelines

**Recommended Wakeup Sequence:**

1. Configured wakeup event triggers wakeup sequence
2. Wakeup event switches ON oscillator (if configured OFF during sleep)
3. Wakeup event triggers interrupt to Cortex-M3
4. Cortex-M3 executes following on wakeup:
   - Restores voltages to normal operating voltage
   - Enables PLL locking
   - Switches ON power domains and/or enables clocks for PD_PER
   - Switches ON power domains and/or enables clocks for PD_MPU
   - Executes WFI
5. Cortex-A8 MPU starts executing from ROM reset vector
6. Restores application context (only for DeepSleep0)

### 8.7.6 Periodic Idling of Cortex-A8 MPU

For periodic ON/OFF of Cortex-A8 MPU:

1. Cortex-A8 MPU executes WFI instruction
2. Any peripheral interrupt triggers wake interrupt to Cortex-M3 via MPU Subsystem's WKUP signal
3. Cortex-M3 powers down MPU (PD_MPU)
4. On receiving interrupt, Cortex-M3 switches ON MPU power domain
5. Cortex-M3 goes into idle mode using WFI instruction

---

## 8.8 PRCM Module Overview

The PRCM is structured using architectural concepts providing:
- Set of modular, re-usable FSM blocks for clock and power management
- Register set and associated programming model
- Functional sub-block definitions for clock, power, system clock, and master clock generation

### 8.8.1 Functional Power Domains

**Generic Domains:**
- **WAKEUP** - Always-on domain for wakeup functionality
- **MPU** - MPU subsystem (Cortex-A8 processor)
- **PER** - Peripheral domain
- **RTC** - Real-time clock domain

### 8.8.2 PRCM Functional Features

- Software configurable for direct, automatic, or combination power domain state transition control
- Device power-up sequence control
- Device sleep/wake-up sequence control
- Centralized reset generation and management
- Centralized clock generation and management

### 8.8.3 PRCM Interface Overview

**Key Interfaces:**

1. **OCP Configuration Ports** - OCP/IP2.0 compliant 32-bit target interface
2. **Power Control Interface** - Controls power domain switches, receives switch status, controls isolation signals
3. **Device Control Interface** - Device-level feature management:
   - Device type coding
   - IOs isolation control
4. **Clocks Interface** - All clock inputs and outputs
5. **Resets Interface** - All reset inputs and outputs
6. **Modules Power Management Control Interface:**
   - **Initiator modules:** MStandby signal, MWait signal
   - **Target modules:** SIdleReq signal, SIdleAck signal, FCLKEN signal

---

## 8.9 Clock Generation and Management

PRCM provides centralized control for generation, distribution, and gating of most clocks in the device.

### 8.9.1 Clock Terminology

**Two types of clocks:**

1. **Interface Clocks:**
   - Provide clocking for system interconnect modules
   - Supply functional module's system interconnect interface and registers
   - In some cases, also used as functional clock

2. **Functional Clocks:**
   - Supply functional part of module or subsystem
   - May require several functional clocks:
     - One or several main functional clock(s)
     - One or several optional clock(s)
   - Main clocks required for module operation
   - Optional clocks for specific features, can shutdown without stopping module

### 8.9.2 Clock Structure

**DPLL Types:**

The device supports multiple on-chip DPLLs:

1. **ADPLLS:** Used for Core, Display, ARM Subsystem, and DDR PLLs
2. **ADPLLLJ:** Used for peripheral functional clocks

**Reference Clocks:**

Two reference clocks generated by on-chip oscillators or externally:
- **Main clock tree** - From main oscillator (CLK_M_OSC)
- **RTC block** - From 32 kHz crystal oscillator (controlled by RTC IP)
- **RC oscillator** - On-chip RC oscillator (always on, not configurable)

**Note:** All PLLs come up in bypass mode at reset. Software must program all PLL settings and wait for PLL lock.

### 8.9.3 ADPLLS Architecture

High resolution frequency synthesizer PLL with built-in level shifters, allows generation of PLL-locked frequencies up to 2 GHz.

**ADPLLS PLLs:**
- MPU PLL
- Core PLL
- Display PLL
- DDR PLL

**Input Clocks:**
- **CLKINP:** Reference input clock
- **CLKINPULOW:** Low frequency input clock for bypass mode only
- **CLKINPHIF:** High frequency input clock for post-divider M3

**Output Clocks:**
- **CLKOUTHIF:** High frequency output clock from post divider M3
- **CLKOUTX2:** Secondary 2x output
- **CLKOUT:** Primary output clock
- **CLKDCOLDO:** Oscillator (DCO) output clock with no bypass

**Internal Clocks:**
- **REFCLK:** Generated by dividing CLKINP by (N+1). REFCLK = CLKINP/(N+1)
- **BCLK:** Bus clock for programming registers

**Lock Frequency:** fDPLL = CLKDCOLDO

#### ADPLLS Output Clock Frequencies

**In Locked Condition (REGM4XEN='0'):**

| Clock | Frequency Formula |
|-------|------------------|
| CLKOUT | [M / (N+1)] * CLKINP * [1/M2] |
| CLKOUTX2 | 2 * [M / (N+1)] * CLKINP * [1/M2] |
| CLKDCOLDO | 2 * [M / (N+1)] * CLKINP |
| CLKOUTHIF (CLKINPHIFSEL='1') | CLKINPHIF / M3 |
| CLKOUTHIF (CLKINPHIFSEL='0') | 2 * [M / (N+1)] * CLKINP * [1/M3] |

**In Locked Condition (REGM4XEN='1'):**

| Clock | Frequency Formula |
|-------|------------------|
| CLKOUT | [4M / (N+1)] * CLKINP * [1/M2] |
| CLKOUTX2 | 2 * [4M / (N+1)] * CLKINP * [1/M2] |
| CLKDCOLDO | 2 * [4M / (N+1)] * CLKINP |
| CLKOUTHIF (CLKINPHIFSEL='1') | CLKINPHIF / M3 |
| CLKOUTHIF (CLKINPHIFSEL='0') | 2 * [4M / (N+1)] * CLKINP * [1/M3] |

**Before Lock and During Relock Modes:**

| Clock | Frequency (ULOWCLKEN='0') | Frequency (ULOWCLKEN='1') |
|-------|-------------------------|-------------------------|
| CLKOUT | CLKINP / (N2+1) | CLKINPULOW |
| CLKOUTX2 | CLKINP / (N2+1) | CLKINPULOW |
| CLKDCOLDO | Low | Low |
| CLKOUTHIF | CLKINPHIF/M3 (ULOWCLKEN='1') | Low (ULOWCLKEN='0') |

### 8.9.4 ADPLLLJ (Low Jitter DPLL)

Low jitter PLL with 2 GHz maximum output, used for peripheral functional clocks.

**Features:**
- Predivide feature allows dividing reference clock (e.g., 24/26 MHz) to 1 MHz
- Then multiply up to 2 GHz maximum
- Similar architecture to ADPLLS but optimized for low jitter

### 8.9.5 Spread Spectrum Clocking (SSC)

**Purpose:** Reduce electromagnetic interference (EMI) by spreading clock signal energy across frequency spectrum.

**Principle:**
- Modulates clock frequency in triangular pattern
- Spreads energy instead of concentrating at single frequency
- Reduces power of peaks but increases global noise

**EMI Reduction Estimation:**

Peak_power_reduction (dB) = 10 * log((Deviation * fc) / fm)

Where:
- Deviation: % of initial clock frequency (Δf / fc)
- fc: Original clock frequency (MHz)
- fm: Spreading frequency (MHz)

**Example:** For fc=400 MHz, deviation=1% (Δf=4 MHz), fm=400 kHz → peak power reduction ≈ 10 dB

**SSC Configuration:**

Enabled/disabled via `CM_CLKMODE_DPLL_xxx.DPLL_SSC_EN` where xxx = MPU, DDR, DISP, CORE, or PER.

**Key Parameters:**
- **Modulation Frequency (fm):** Programmed via MODFREQDEV_MANTISSA and MODFREQDEV_EXPONENT
  - ModFreqDivider = Fref / (4*fm)
  - ModFreqDivider = MODFREQDEV_MANTISSA * 2^MODFREQDEV_EXPONENT
- **Frequency Spread (Δf):** Controlled via DELTAMSTEP_INTEGER and DELTAMSTEP_FRACTION
- **Downspread Mode:** If DPLL_SSC_DOWNSPREAD=1, frequency spread on lower side is 2x programmed value, upper side is 0

**Restrictions:**
- M - ΔM ≥ 20
- M + ΔM ≤ 2045
- If downspread enabled: M - 2*ΔM ≥ 20 and M ≤ 2045
- Modulation frequency must be within DPLL loop bandwidth (fm < Fref/70)

---

## 8.10 Summary of Key Concepts for AI Training

### Clock Management Hierarchy

```
PRCM Module
├── Clock Domains (groups of modules with common clock source)
│   ├── Clock Managers (control clock gating per domain)
│   ├── Interface Clocks (_ICLK) → interconnect/registers
│   └── Functional Clocks (_FCLK) → module functionality
│       ├── Mandatory clocks (required for operation)
│       └── Optional clocks (feature-specific)
└── Module Clock Protocols
    ├── Master Standby Protocol (for initiator modules)
    │   ├── Force-standby (immediate, risk of data loss)
    │   ├── No-standby (always active, not power-efficient)
    │   ├── Smart-standby (activity-based, safe)
    │   └── Smart-standby wakeup (with wakeup generation)
    └── Slave Idle Protocol (for target modules)
        ├── Force-idle (immediate, risk of data loss)
        ├── No-idle (always active, not power-efficient)
        ├── Smart-idle (activity-based, safe)
        └── Smart-idle wakeup (with wakeup generation)
```

### Power Management Hierarchy

```
PRCM Module
├── Power Domains (groups with independent power control)
│   ├── Logic Area (ON/OFF states)
│   └── Memory Area (ON/RETENTION/OFF states)
├── Power States Control
│   ├── PWRSTCTRL (target state configuration)
│   └── PWRSTST (current state status)
└── Voltage Management
    └── AVS (Adaptive Voltage Scaling via Smart Reflex)
```

### Power Modes Spectrum

```
Active Mode (Highest Power, Zero Latency)
  ↓ All supplies ON, all DPLLs locked
Standby Mode
  ↓ Main osc ON, DPLLs bypass, PD_MPU OFF, DDR self-refresh
DeepSleep1 Mode
  ↓ Main osc OFF, DDR self-refresh, USB wakeup capable
DeepSleep0 Mode
  ↓ PD_PER OFF, DDR self-refresh maintained
RTC-Only Mode (Lowest Power, Highest Latency)
  All OFF except RTC domain, full cold boot on wakeup
```

### Wakeup Management

```
Wakeup Sources (PD_WKUP domain, always ON):
├── GPIO0 bank
├── dmtimer1_1ms
├── USB2PHY (both ports)
├── TSC
├── UART0
├── I2C0
└── RTC alarm

Wakeup Path:
Event → Oscillator Enable → Cortex-M3 Interrupt →
Power Domain Restore → Clock Enable → Cortex-A8 Resume
```

### Cortex-M3 Power Management Role

```
Power Transition Flow:
1. Cortex-A8 executes WFI
2. Cortex-M3 wakes, receives IPC command (CMD_DS0/DS1/RTC)
3. Cortex-M3 executes power sequencing:
   - Forces sleep on interconnect clocks
   - Powers down MPU/PER domains
   - Configures oscillator control
4. Cortex-M3 executes WFI (enters idle)
5. Wakeup event arrives
6. Cortex-M3 wakes, executes restore sequence:
   - Restores voltages
   - Locks PLLs
   - Enables power domains
   - Enables clocks
7. Cortex-A8 resumes from ROM reset vector
```

### Critical Register Fields

**Clock Control:**
- `MODULEMODE`: Module enable/disable control
- `CLKTRCTRL`: Clock domain transition control
- `CLKACTIVITY`: Clock activity status
- `IDLEST`: Module idle status
- `STBYST`: Module standby status

**Power Control:**
- `POWERSTATE`: Target power state configuration
- `POWERSTATEST`: Current power state status
- `LOGICSTATEST`: Logic area state status
- `MEMSTATEST`: Memory area state status

**IPC Communication:**
- `CMD_STAT`: Command status (PASS/FAIL/WAIT4OK)
- `CMD_ID`: Command identifier (CMD_RTC/CMD_DS0/CMD_DS1)
- `CMD param1/param2`: Command parameters

---

## Key Takeaways

1. **Hierarchical Management:** Three-level architecture (clock/power/voltage) with domain-based organization

2. **Hardware Protocols:** Automatic clock gating via Master Standby and Slave Idle protocols with smart modes for safety

3. **Power Mode Trade-offs:** Five power modes providing spectrum from full performance to ultra-low power with corresponding latency trade-offs

4. **Cortex-M3 Role:** Dedicated processor handles low-level power transitions, allowing Cortex-A8 to focus on application

5. **Flexible Wakeup:** Multiple wakeup sources in always-on domain enable application-specific low-power scenarios

6. **Context Management:** Critical requirement to save/restore context appropriately for each power mode

7. **USB Considerations:** DeepSleep1 minimum for USB wakeup scenarios, different events for host vs device mode

8. **RTC Domain:** Can operate independently with battery backup for ultra-low power timekeeping

9. **Clock Generation:** ADPLLS and ADPLLLJ DPLLs with SSC capability for EMI reduction

10. **Software Control:** Extensive register set provides fine-grained control while hardware protocols ensure safety

---

## 8.11 Clock Module Registers

### 8.11.1 Register Groups Overview

The Clock Module provides the following register groups:

| Register Group | Base Address | Purpose |
|---------------|--------------|---------|
| **CM_PER** | 0x44E00000 | Peripheral clock management |
| **CM_WKUP** | 0x44E00400 | Wakeup domain clock management |
| **CM_DPLL** | 0x44E00500 | DPLL configuration and control |
| **CM_MPU** | 0x44E00600 | MPU clock management |
| **CM_DEVICE** | 0x44E00700 | Device-level clock control |
| **CM_RTC** | 0x44E00800 | RTC clock management |
| **CM_GFX** | 0x44E00900 | Graphics clock management |
| **CM_CEFUSE** | 0x44E00A00 | eFuse clock management |

### 8.11.2 Common Register Types

#### 8.11.2.1 CLKSTCTRL Registers (Clock State Control)

Control clock domain state transitions and monitor clock activity.

**Common Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [31:8] | CLKACTIVITY_* | R | Activity status bits for various clocks in domain (0=Gated, 1=Active) |
| [1:0] | CLKTRCTRL | R/W | Clock transition control:<br>0x0 = NO_SLEEP (sleep transition cannot be initiated)<br>0x1 = SW_SLEEP (software forced sleep)<br>0x2 = SW_WKUP (software forced wakeup)<br>0x3 = Reserved |

### 8.11.2.2 CLKCTRL Registers (Clock Control)

Control individual module clocks and monitor module state.

**Common Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [18] | STBYST | R | Standby status:<br>0x0 = Module functional (not in standby)<br>0x1 = Module in standby |
| [17:16] | IDLEST | R | Idle status:<br>0x0 = Functional (fully functional including OCP)<br>0x1 = Transition (wakeup, sleep, or sleep abortion)<br>0x2 = Idle (OCP idle, functional if using separate functional clock)<br>0x3 = Disabled (module disabled, cannot be accessed) |
| [1:0] | MODULEMODE | R/W | Module mode:<br>0x0 = DISABLED (module disabled by SW, OCP access causes error)<br>0x1 = Reserved<br>0x2 = ENABLE (module explicitly enabled, clocks guaranteed)<br>0x3 = Reserved |

### 8.11.3 Key CM_PER Registers

Peripheral domain clock management registers.

| Offset | Register Name | Purpose |
|--------|--------------|---------|
| 0x00 | CM_PER_L4LS_CLKSTCTRL | L4LS clock domain state control |
| 0x04 | CM_PER_L3S_CLKSTCTRL | L3S clock domain state control |
| 0x0C | CM_PER_L3_CLKSTCTRL | L3 clock domain state control |
| 0x14 | CM_PER_CPGMAC0_CLKCTRL | Ethernet switch (CPSW) clock control |
| 0x18 | CM_PER_LCDC_CLKCTRL | LCD controller clock control |
| 0x1C | CM_PER_USB0_CLKCTRL | USB0 clock control |
| 0x28 | CM_PER_EMIF_CLKCTRL | EMIF (DDR) clock control |
| 0x2C | CM_PER_OCMCRAM_CLKCTRL | On-chip RAM clock control |
| 0x30 | CM_PER_GPMC_CLKCTRL | GPMC (parallel interface) clock control |
| 0x3C | CM_PER_MMC0_CLKCTRL | MMC0 clock control |
| 0x44 | CM_PER_I2C2_CLKCTRL | I2C2 clock control |
| 0x48 | CM_PER_I2C1_CLKCTRL | I2C1 clock control |
| 0x4C | CM_PER_SPI0_CLKCTRL | SPI0 clock control |
| 0x50 | CM_PER_SPI1_CLKCTRL | SPI1 clock control |
| 0x6C | CM_PER_UART1_CLKCTRL | UART1 clock control |
| 0x70 | CM_PER_UART2_CLKCTRL | UART2 clock control |
| 0x74 | CM_PER_UART3_CLKCTRL | UART3 clock control |
| 0xAC | CM_PER_GPIO1_CLKCTRL | GPIO1 clock control |
| 0xB0 | CM_PER_GPIO2_CLKCTRL | GPIO2 clock control |
| 0xB4 | CM_PER_GPIO3_CLKCTRL | GPIO3 clock control |

### 8.11.4 Key CM_WKUP Registers

Wakeup domain clock management registers (Base: 0x44E00400).

| Offset | Register Name | Purpose |
|--------|--------------|---------|
| 0x00 | CM_WKUP_CLKSTCTRL | Wakeup clock domain state control |
| 0x04 | CM_WKUP_CONTROL_CLKCTRL | Control module clock control |
| 0x08 | CM_WKUP_GPIO0_CLKCTRL | GPIO0 clock control |
| 0x0C | CM_WKUP_L4WKUP_CLKCTRL | L4 wakeup interconnect clock control |
| 0x10 | CM_WKUP_TIMER0_CLKCTRL | Timer0 clock control |
| 0x14 | CM_WKUP_DEBUGSS_CLKCTRL | Debug subsystem clock control |
| 0x2C | CM_L3_AON_CLKSTCTRL | L3 always-on clock domain state control |
| 0x34 | CM_WKUP_UART0_CLKCTRL | UART0 clock control |
| 0x38 | CM_WKUP_I2C0_CLKCTRL | I2C0 clock control |
| 0x3C | CM_WKUP_ADC_TSC_CLKCTRL | ADC/Touchscreen clock control |

### 8.11.5 CM_DPLL Registers

DPLL configuration and control registers (Base: 0x44E00500).

#### 8.11.5.1 DPLL Control Registers

Each DPLL has the following register set pattern:

| Register | Purpose |
|----------|---------|
| CM_CLKMODE_DPLL_xxx | DPLL mode control (bypass, lock, etc.) |
| CM_IDLEST_DPLL_xxx | DPLL lock status |
| CM_CLKSEL_DPLL_xxx | DPLL multiply/divide configuration |
| CM_DIV_M2_DPLL_xxx | M2 divider configuration |
| CM_DIV_M4_DPLL_xxx | M4 divider configuration (if available) |
| CM_DIV_M5_DPLL_xxx | M5 divider configuration (if available) |
| CM_DIV_M6_DPLL_xxx | M6 divider configuration (if available) |

Where xxx = MPU, DDR, DISP, CORE, or PER.

#### 8.11.5.2 CM_CLKMODE_DPLL_xxx (DPLL Mode Control)

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [23] | DPLL_SSC_ACK | R | SSC acknowledgment (0=Disabled, 1=Enabled) |
| [22] | DPLL_SSC_DOWNSPREAD | R/W | Downspread enable (0=Center spread, 1=Downspread) |
| [12] | DPLL_SSC_EN | R/W | Spread Spectrum Clocking enable |
| [8] | DPLL_LPMODE_EN | R/W | Low power mode enable |
| [7] | DPLL_RELOCK_RAMP_EN | R/W | Relock ramp enable |
| [6] | DPLL_DRIFTGUARD_EN | R/W | Drift guard enable (recalibration) |
| [5:4] | DPLL_RAMP_LEVEL | R/W | Ramp level control |
| [3] | DPLL_RAMP_RATE | R/W | Ramp rate control |
| [2:0] | DPLL_EN | R/W | DPLL enable mode:<br>0x0 = Reserved<br>0x1 = Reserved<br>0x2 = Reserved<br>0x3 = Reserved<br>0x4 = MN bypass mode<br>0x5 = Idle bypass low-power mode<br>0x6 = Reserved<br>0x7 = Lock mode |

#### 8.11.5.3 CM_IDLEST_DPLL_xxx (DPLL Status)

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [8] | ST_MN_BYPASS | R | MN bypass status (0=Not in bypass, 1=In bypass) |
| [0] | ST_DPLL_CLK | R | DPLL lock status (0=Unlocked, 1=Locked) |

#### 8.11.5.4 CM_CLKSEL_DPLL_xxx (DPLL Multiply/Divide)

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [22:8] | DPLL_MULT | R/W | DPLL multiplier factor (M) [2-2047] |
| [6:0] | DPLL_DIV | R/W | DPLL divider factor (N) [0-127] |

**Frequency Calculation:**
- Fref = Finp / (N + 1)
- Fdpll = Fref * M = Finp * M / (N + 1)
- Fout = Fdpll / M2

#### 8.11.5.5 CM_DIV_M2_DPLL_xxx (M2 Divider)

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [5] | DPLL_CLKOUT_DIVCHACK | R | Divider change acknowledgment (toggles on change) |
| [4:0] | DPLL_CLKOUT_DIV | R/W | M2 divider value [1-31], actual divisor = value + 1 |

**Note:** M2 divider can be changed on-the-fly without putting DPLL in bypass mode.

#### 8.11.5.6 SSC Configuration Registers

For DPLL with Spread Spectrum Clocking support:

**CM_SSC_DELTAMSTEP_DPLL_xxx:**

| Bits | Field | Description |
|------|-------|-------------|
| [19:18] | DELTAMSTEP_INTEGER | Integer part of delta M step |
| [17:0] | DELTAMSTEP_FRACTION | Fractional part of delta M step (18-bit) |

**CM_SSC_MODFREQDIV_DPLL_xxx:**

| Bits | Field | Description |
|------|-------|-------------|
| [9:7] | MODFREQDIV_EXPONENT | Modulation frequency divider exponent (3-bit) |
| [6:0] | MODFREQDIV_MANTISSA | Modulation frequency divider mantissa (7-bit) |

### 8.11.6 CM_MPU Registers

MPU subsystem clock management (Base: 0x44E00600).

| Offset | Register Name | Purpose |
|--------|--------------|---------|
| 0x00 | CM_MPU_CLKSTCTRL | MPU clock domain state control |
| 0x04 | CM_MPU_MPU_CLKCTRL | MPU clock control |

---

## 8.12 Power Management Registers

### 8.12.1 Register Groups Overview

The Power Management module provides the following register groups:

| Register Group | Base Address | Purpose |
|---------------|--------------|---------|
| **PRM_PER** | 0x44E00C00 | Peripheral power domain management |
| **PRM_WKUP** | 0x44E00D00 | Wakeup power domain management |
| **PRM_MPU** | 0x44E00E00 | MPU power domain management |
| **PRM_DEVICE** | 0x44E00F00 | Device-level power management |
| **PRM_RTC** | 0x44E01000 | RTC power domain management |
| **PRM_GFX** | 0x44E01100 | Graphics power domain management |
| **PRM_CEFUSE** | 0x44E01200 | eFuse power domain management |

### 8.12.2 Common Register Types

#### 8.12.2.1 PM_xxx_PWRSTCTRL (Power State Control)

Controls target power state for power domain.

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [11:10] | LOWPOWERSTATECHANGE | R/W | Low power state change request |
| [9:8] | LOGICRETSTATE | R/W | Logic retention state:<br>0x0 = Logic off<br>0x1 = Logic retention |
| [5:4] | MEM_x_RETSTATE | R/W | Memory bank retention state:<br>0x0 = Memory off<br>0x1 = Memory retention |
| [3:2] | MEM_x_ONSTATE | R/W | Memory bank on state:<br>0x3 = Memory on |
| [1:0] | POWERSTATE | R/W | Power state control:<br>0x0 = OFF<br>0x1 = RETENTION<br>0x2 = INACTIVE (not used)<br>0x3 = ON |

#### 8.12.2.2 PM_xxx_PWRSTST (Power State Status)

Reports current power state of power domain.

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [20] | LASTPOWERSTATEENTERED | R | Last low power state entered |
| [11:10] | LOGICSTATEST | R | Logic state status:<br>0x0 = Logic off<br>0x1 = Logic retention<br>0x2 = Reserved<br>0x3 = Logic on |
| [9:8] | MEM_STATEST_x | R | Memory bank state status:<br>0x0 = Memory off<br>0x1 = Memory retention<br>0x2 = Reserved<br>0x3 = Memory on |
| [5] | INTRANSITION | R | Power state transition status:<br>0x0 = No transition<br>0x1 = Transition ongoing |
| [1:0] | POWERSTATEST | R | Current power state:<br>0x0 = OFF<br>0x1 = RETENTION<br>0x2 = INACTIVE<br>0x3 = ON |

### 8.12.3 Reset Control Registers

#### PRM_RSTCTRL (Reset Control)

Global warm reset control.

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [1] | RST_GLOBAL_COLD_SW | R/W | Software cold reset |
| [0] | RST_GLOBAL_WARM_SW | R/W | Software warm reset |

#### PRM_RSTST (Reset Status)

Reports reset source.

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [9] | ICEPICK_RST | R/W1toClr | ICEPick reset |
| [6] | EXTERNAL_WARM_RST | R/W1toClr | External warm reset |
| [5] | WDT1_RST | R/W1toClr | Watchdog 1 reset |
| [4] | GLOBAL_COLD_RST | R/W1toClr | Global cold reset |
| [1] | GLOBAL_WARM_SW_RST | R/W1toClr | Global warm software reset |
| [0] | POWER_ON_RST | R/W1toClr | Power-on reset |

#### PRM_RSTTIME (Reset Timing)

Controls reset timing parameters.

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [14:10] | RSTTIME2 | R/W | Reset time 2 (in 32kHz clock cycles) |
| [9:0] | RSTTIME1 | R/W | Reset time 1 (in 32kHz clock cycles) |

### 8.12.4 Sleep/Wakeup Control Registers

#### PRM_SRAM_COUNT

Controls SRAM retention during sleep.

#### PRM_LDO_SRAM_xxx_CTRL

Controls SRAM LDO during retention.

### 8.12.5 Device Control Registers

Located in Control Module, but critical for power management.

#### DEEPSLEEP_CTRL (Control Module: 0x44E10470)

**Key Bit Fields:**

| Bits | Field | Type | Description |
|------|-------|------|-------------|
| [31:16] | Reserved | - | - |
| [15:3] | DSCOUNT | R/W | Deep sleep count (oscillator restart delay) |
| [0] | DSENABLE | R/W | Deep sleep enable (0=Disable, 1=Enable) |

### 8.12.6 IPC Registers

Inter-processor communication registers for Cortex-A8 and Cortex-M3 (Control Module: 0x44E10400+).

| Offset | Register Name | Purpose |
|--------|--------------|---------|
| 0x00 | IPC_MSG_REG0 | Command ID and Status |
| 0x04 | IPC_MSG_REG1 | Command Parameter 1 |
| 0x08 | IPC_MSG_REG2 | Command Parameter 2 |
| 0x0C | IPC_MSG_REG3 | Response/Status from CM3 |
| 0x10 | IPC_MSG_REG4 | Reserved |
| 0x14 | IPC_MSG_REG5 | Reserved |
| 0x18 | IPC_MSG_REG6 | Reserved |
| 0x1C | IPC_MSG_REG7 | Customer use |

**IPC_MSG_REG0 Format:**

| Bits | Field | Description |
|------|-------|-------------|
| [31:16] | CMD_ID | Command identifier |
| [15:0] | CMD_STAT | Command status |

---

## 8.13 Register Access Considerations

### 8.14.1 Synchronization Requirements

- **Clock Domain Transitions:** Software must ensure proper sequencing when changing CLKTRCTRL
- **DPLL Configuration:** Always put DPLL in bypass before changing M, N, or M2 values (except M2 on-the-fly changes)
- **Power Domain Transitions:** Check INTRANSITION bit before assuming power state change completed
- **Module Enable:** Always wait for IDLEST to show functional state before accessing module

### 8.14.2 Reset Sensitivity

Some register bits are marked as:
- **Cold reset only:** Preserved during warm reset
- **Warm reset sensitive:** Reset to default on warm reset
- **Power domain reset:** Reset when power domain is cycled

Check module documentation for specific reset behavior.

### 8.14.3 Access Restrictions

- **Read-only fields:** Writing has no effect
- **Write-to-clear bits:** Writing 1 clears the bit, writing 0 has no effect (e.g., reset status bits)
- **Reserved bits:** Should not be modified, read as 0

### 8.14.4 Common Pitfalls

1. **Forgetting to enable clock domain:** Module won't function if clock domain is in NO_SLEEP or SW_SLEEP
2. **Not waiting for lock:** Accessing DPLL outputs before lock completes causes undefined behavior
3. **Missing IDLEST check:** Accessing module registers before IDLEST shows functional can cause bus errors
4. **Power domain sequencing:** Incorrect power-up/power-down sequence can hang system
5. **Context loss:** Not saving context before entering low power modes with memory OFF
