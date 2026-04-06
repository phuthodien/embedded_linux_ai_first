# **Chapter 4. Laying the Groundwork**

| In This Chapter                 |     |
|---------------------------------|-----|
| Introducing Devices and Drivers | 90  |
| Interrupt Handling              | 92  |
| The Linux Device Model          | 103 |
| Memory Barriers                 | 114 |
| Power Management                | 114 |
| Looking at the Sources          | 115 |
|                                 |     |

We are now within whispering distance of writing a device driver. Before doing that, however, let's equip ourselves with some driver concepts. We start the chapter by getting an idea of the book's problem statement; we will look at the typical devices and I/O interfaces present on PC-compatible systems and embedded computers. Interrupt handling is an integral part of most drivers, so we next cover the art of writing interrupt handlers. We then turn our attention to the new device model introduced in the 2.6 kernel. The new model is built around abstractions such as *sysfs*, *kobjects*, *device classes*, and *udev*, which distill commonalities from device drivers. The new device model also weeds policies out of kernel space and pushes them to user space, resulting in a total revamp of features such as */dev* node management, hotplug, coldplug, module autoload, and firmware download.

# **Introducing Devices and Drivers**

User applications cannot directly communicate with hardware because that entails possessing privileges such as executing special instructions and handling interrupts. Device drivers assume the burden of interacting with hardware and export interfaces that applications and the rest of the kernel can use to access devices. Applications operate on devices via nodes in the */dev* directory and glean device information using nodes in the */sys* directory.[1]

[1] As you'll learn later, networking applications route their requests to the underlying driver using a different mechanism.

Figure 4.1 shows the hardware block diagram of a typical PC-compatible system. As you can see, the system supports diverse devices and interface technologies such as memory, video, audio, USB, PCI, WiFi, PCMCIA, I2C, IDE, Ethernet, serial port, keyboard, mouse, floppy drive, parallel port, and Infrared. The memory controller and the graphics controller are part of a *North Bridge* chipset in the PC architecture, whereas peripheral buses are sourced out of a *South Bridge*.

**Figure 4.1. Hardware block diagram of a PC-compatible system.**

```
                              +-----------+
                              | Processor |
                              +-----------+
                                     |
                                     v
                         +------------------------+
                         |      North Bridge      |
                         | +--------------------+ |
 DRAM -------------------| | Memory Controller  | |
                         | +--------------------+ |
                         | +--------------------+ |
 Display Unit ---------- | | Graphics Control.  | |
                         | +--------------------+ |
                         +------------------------+
                                     |
                                     v
                         +------------------------+
                         |      South Bridge      |
                         +------------------------+
                                     |
                                     v
                           +------------------+
                           |  Hub Interface   |
                           +------------------+
          _____________________|_______________________________________________________________________________________________________________________________________________________
         /          |           |          |       |      \                                                     |                                                                    |
        v           v           v          v       v       v                                                    V                                                                    v

+-----------+  +---------+  +------+   +-----------+   +-----------+                                        +----------+      +----------------------+      +------------+         +-----------------+      +------------------+
| RTC/CMOS |   |  PCI    |  | IDE  |   |   I²C     |   |   LPC     |-----------------|                      |  USB     |----> |  External PCI Intf. |---->  | Ethernet   |         | AC'97 Controller|----> |  Audio CODEC     |----> Speaker / MIC
+-----------+  +---------+  +------+   +-----------+   +-----------+                 |                      +----------+      +----------------------+      +------------+         +-----------------+      +------------------+
                                |            |              |                        |                        |   |                     |
                                v            v              v                        v                        |   |                     v
                           +----------+  +--------+   +------------------+    +------------------------+      |   +--> WiFi Chipset     +--> CardBus Controller --> CardBus Card
                           | Hard Disk|  | EEPROM |   |  Super I/O       |    | Firmware Hub (BIOS)    |      |                         |
                           +----------+  +--------+   +------------------+    +------------------------+      +--> SCSI Host Adapter    +---------------------------> PCMCIA Card
                                                         | 
                                                         v
          -------------------------------------------------------------------------------------
          |    Serial Port | PS/2 Keyboard | PS/2 Mouse | Parallel | Floppy Drive | Infrared  |
          |                                |            |  Port    |              |           |
          |                                |            |          |              |           |
          |                                |            |          |              |           |
          |                                |            |          |              |           |
          -------------------------------------------------------------------------------------

```

Figure 4.2 illustrates a similar block diagram for a hypothetical embedded device. This diagram contains several interfaces not typical in the PC world such as flash memory, LCD, touch screen, and cellular modem.

**Figure 4.2. Hardware block diagram of an embedded system.**

```
                   +--------------------------------------------------+
                   | Domain-specific electronics /                    |
                   | Bluetooth / Infrared / GSM / GPRS / GPS /        |
                   | 3G / Biometrics / SmartCard / Encryption         |
                   +--------------------------------------------------+
                                 ^
                                 |      
                                 |
                    UART / USB / I2C / SPI / CAN                     Embedded SoC 
                         +-----------------------------------------------------------------------+
                         |                                                                       |
           SDRAM ------> |                  ADDR/Data/Chipselect     NAND UART                   | <----- Flash Memory
     Connectivity ---->  |  CPU Core                                        Controller           | <----- Debug Serial Port
                         |      |                     Internal Local Bus       |                 |
                   |-----|     --------------|--------------------------------------  GPIO Ports |
                   |     |  LCD Controller   |         |           |        |     |              |
                   |     |                   |         |           |        |     |              |    +-------------------+
                   |     |                   |         |           |        |     |              |----|Keys, Buttons, LEDS|
                   |     |                                                                       |    +-------------------+
                   |     |     I2C          I2S       SPI       USB OTG     SD   PCMCIA          |
                   |     +-----------------------------------------------------------------------+
                   |            |            |         |          |       |   |    |         +---------+
           +----------------+   |            |         |          |       |   |    |---------|CF Slot  |
           | LCD Panel      |   |        +---------+   |          |       |   |              +---------+
           +----------------+   |--------| CODEC   |   |     +---------+  |  +---------+
           | Touch Panel    |   |        +---------+   |     |USB Host |  |  |SD Slot  |
           +----------------+   |        +---------+   |     |  Port   |  |  +---------+
                  |   |         |        |   RTC   |---|     +---------+  |
                  |   |         |        +---------+   |  +---------+   +-----------+
                  |   |         |        +---------+   |--|  EEPROM |   | USB Device|
                  |   |         |        |         |   |  |         |   | Port      |        
           +-----------------+  |--------| Smart   |   |  +---------+   +-----------+
           |                 |  |        | Battery |
           | Touch Controller|  |        +---------+ 
           +-----------------+   
              |
              V
     SPI/USB/Serial/ADC
```

Naturally, the capability to access peripheral devices is a crucial part of a system's functioning. Device drivers provide the engine to achieve this. The rest of the chapters in this book will zoom in on a device interface and teach you how to implement the corresponding device driver.

# **Interrupt Handling**

Because of the indeterminate nature of I/O, and speed mismatches between I/O devices and the processor, devices request the processor's attention by asserting certain hardware signals asynchronously. These hardware signals are called *interrupts.* Each interrupting device is assigned an associated identifier called an *interrupt request* (IRQ) number. When the processor detects that an interrupt has been generated on an IRQ, it abruptly stops what it's doing and invokes an *interrupt service routine* (ISR) registered for the corresponding IRQ. *Interrupt handlers* (ISRs) execute in interrupt context.

## **Interrupt Context**

ISRs are critical pieces of code that directly converse with the hardware. They are given the privilege of instant execution in the larger interest of system performance. However, if ISRs are not quick and lightweight, they contradict their own philosophy. VIPs are given preferential treatment, but it's incumbent on them to minimize the resulting inconvenience to the public. To compensate for rudely interrupting the current thread of execution, ISRs have to politely execute in a restricted environment called interrupt context (or *atomic context*).

Here is a list of do's and don'ts for code executing in interrupt context:

- It's a jailable offense if your interrupt context code goes to sleep. Interrupt handlers cannot relinquish the processor by calling sleepy functions such as schedule\_timeout(). Before invoking a kernel API from your interrupt handler, penetrate the nested invocation train and ensure that it does not internally trigger a blocking wait. For example, input\_register\_device() looks harmless from the surface, but tosses a call to kmalloc() under the hood specifying GFP\_KERNEL as an argument. As you saw in Chapter 2, "A Peek Inside the Kernel," if your system's free memory dips below a watermark, kmalloc() sleep-waits for memory to get freed up by the swapper, if you invoke it in this manner. **1.**
- For protecting critical sections inside interrupt handlers, you can't use mutexes because they may go to sleep. Use spinlocks instead, and use them only if you must. **2.**
- Interrupt handlers cannot directly exchange data with user space because they are not connected to user land via process contexts. This brings us to another reason why interrupt handlers cannot sleep: The scheduler works at the granularity of processes, so if interrupt handlers sleep and are scheduled out, how can they be put back into the run queue? **3.**
- Interrupt handlers are supposed to get out of the way quickly but are expected to get the job done. To circumvent this Catch-22, interrupt handlers usually split their work into two. The slim *top half* of the handler flags an acknowledgment claiming that it has serviced the interrupt but, in reality, offloads all the hard work to a fat *bottom half.* Execution of the bottom half is deferred to a later point in time when all interrupts are enabled. You will learn to develop bottom halves while discussing *softirqs* and *tasklets* later. **4.**
- You need not design interrupt handlers to be reentrant. When an interrupt handler is running, the corresponding IRQ is disabled until the handler returns. So, unlike process context code, different instances of the same handler will not run simultaneously on multiple processors. **5.**
- Interrupt handlers can be interrupted by handlers associated with IRQs that have higher priority. You can prevent this nested interruption by specifically requesting the kernel to treat your interrupt handler as a **6.**

*fast* handler. Fast handlers run with all interrupts disabled on the local processor. Before disabling interrupts or labeling your interrupt handler as fast, be aware that interrupt-off times are bad for system performance. More the interrupt-off times, more is the interrupt latency, or the delay before a generated interrupt is serviced. Interrupt latency is inversely proportional to the real time responsiveness of the system.

A function can check the value returned by in\_interrupt() to find out whether it's executing in interrupt context.

Unlike asynchronous interrupts generated by external hardware, there are classes of interrupts that arrive synchronously. Synchronous interrupts are so called because they don't occur unexpectedly—the processor itself generates them by executing an instruction. Both external and synchronous interrupts are handled by the kernel using identical mechanisms.

Examples of synchronous interrupts include the following:

- Exceptions, which are used to report grave runtime errors
- Software interrupts such as the int 0x80 instruction used to implement system calls on the x86 architecture

### **Assigning IRQs**

Device drivers have to connect their IRQ number to an interrupt handler. For this, they need to know the IRQ assigned to the device they're driving. IRQ assignments can be straightforward or may require complex probing. In the PC architecture, for example, timer interrupts are assigned IRQ 0, and RTC interrupts answer to IRQ 8. Modern bus technologies such as PCI are sophisticated enough to respond to queries regarding their IRQs (assigned by the BIOS when it walks the bus during boot). PCI drivers can poke into earmarked regions in the device's configuration space and figure out the IRQ. For older devices such as *Industries Standard Architecture* (ISA)-based cards, the driver might have to leverage hardware-specific knowledge to probe and decipher the IRQ.

Take a look at */proc/interrupts* for a list of active IRQs on your system.

#### **Device Example: Roller Wheel**

Now that you have learned the basics of interrupt handling, let's implement an interrupt handler for an example roller wheel device. Roller wheels can be found on some phones and PDAs for easy menu navigation and are capable of three movements: clockwise rotation, anticlockwise rotation, and key-press. Our imaginary roller wheel is wired such that any of these movements interrupt the processor on IRQ 7. Three low order bits of *General Purpose I/O* (GPIO) Port D of the processor are connected to the roller device. The waveforms generated on these pins corresponding to different wheel movements are shown in Figure 4.3. The job of the interrupt handler is to decipher the wheel movements by looking at the Port D GPIO data register.

**Figure 4.3. Sample wave forms generated by the roller wheel.**

```
Anticlockwise Rotation
----------------------

PORT D0:  ___■■■___■■■___■■■___
PORT D1: ______■■■___■■■___■■■_
PORT D2: _______________________   

Clockwise Rotation
------------------

PORT D0: ______■■■___■■■___■■■_
PORT D1:  ___■■■___■■■___■■■___
PORT D2: _______________________   (luôn mức thấp)

Key Press
---------

PORT D0: _________________________
PORT D1: _________________________
PORT D2: ______■■■■■■■■■■■■______   (xung mức cao dài khi nhấn phím)
```

The driver has to first request the IRQ and associate an interrupt handler with it:

```
#define ROLLER_IRQ 7
static irqreturn_t roller_interrupt(int irq, void *dev_id);
if (request_irq(ROLLER_IRQ, roller_interrupt, IRQF_DISABLED |
 IRQF_TRIGGER_RISING, "roll", NULL)) {
 printk(KERN_ERR "Roll: Can't register IRQ %d\n", ROLLER_IRQ);
 return -EIO;
}
```

Let's look at the arguments passed to request\_irq(). The IRQ number is not queried or probed but hard-coded to ROLLER\_IRQ in this simple case as per the hardware connection. The second argument, roller\_interrupt(), is the interrupt handler routine. Its prototype specifies a return type of irqreturn\_t, which can be IRQ\_HANDLED if the interrupt is handled successfully or IRQ\_NONE if it isn't. The return value assumes more significance for I/O technologies such as PCI, where multiple devices can share the same IRQ.

The IRQF\_DISABLED flag specifies that this interrupt handler has to be treated as a fast handler, so the kernel

has to disable interrupts while invoking the handler. IRQF\_TRIGGER\_RISING announces that the roller wheel generates a rising edge on the interrupt line when it wants to signal an interrupt. In other words, the roller wheel is an *edge-sensitive* device. Some devices are instead *level-sensitive* and keep the interrupt line asserted until the CPU services it. To flag an interrupt as level-sensitive, use the IRQF\_TRIGGER\_HIGH flag. Other possible values for this argument include IRQF\_SAMPLE\_RANDOM (used in the section, "Pseudo Char Drivers" in Chapter 5, "Character Drivers") and IRQF\_SHARED (used to specify that this IRQ is shared among multiple devices).

The next argument, "roll", is used to identify this device in data generated by files such as */proc/interrupts.* The final parameter, set to NULL in this case, is relevant only for shared interrupt handlers and is used to identify each device sharing the IRQ line.

Starting with the 2.6.19 kernel, there have been some changes to the interrupt handler interface. Interrupt handlers used to take a third argument (struct pt\_regs \*) that contained a pointer to CPU registers, but this has been removed starting with the 2.6.19 kernel. Also, the IRQF\_xxx family of interrupt flags replaced the SA\_xxx family. For example, with earlier kernels, you had to use SA\_INTERRUPT rather than IRQF\_DISABLED to mark an interrupt handler as fast.

Driver initialization is not a good place for requesting an IRQ because that can hog that valuable resource even when the device is not in use. So, device drivers usually request the IRQ when the device is opened by an application. Similarly, the IRQ is freed when the application closes the device and not while exiting the driver module. Freeing an IRQ is done as follows:

```
free_irq(int irq, void *dev_id);
```

Listing 4.1 shows the implementation of the roller interrupt handler. roller\_interrupt() takes two arguments: the IRQ and the device identifier passed as the final argument to the associated request\_irq(). Look at Figure 4.3 side by side with this listing.

#### **Listing 4.1. The Roller Interrupt Handler**

```
spinlock_t roller_lock = SPIN_LOCK_UNLOCKED;
static DECLARE_WAIT_QUEUE_HEAD(roller_poll);
static irqreturn_t
roller_interrupt(int irq, void *dev_id)
{
 int i, PA_t, PA_delta_t, movement = 0;
 /* Get the waveforms from bits 0, 1 and 2
 of Port D as shown in Figure 4.3 */
 PA_t = PORTD & 0x07;
 /* Wait until the state of the pins change.
 (Add some timeout to the loop) */
 for (i=0; (PA_t==PA_delta_t); i++){
 PA_delta_t = PORTD & 0x07;
 }
 movement = determine_movement(PA_t, PA_delta_t); /* See below */
```

```
 spin_lock(&roller_lock);
 /* Store the wheel movement in a buffer for
 later access by the read()/poll() entry points */
 store_movements(movement);
 spin_unlock(&roller_lock);
 /* Wake up the poll entry point that might have
 gone to sleep, waiting for a wheel movement */
 wake_up_interruptible(&roller_poll);
 return IRQ_HANDLED;
}
int
determine_movement(int PA_t, int PA_delta_t)
{
 switch (PA_t){
 case 0:
 switch (PA_delta_t){
 case 1:
 movement = ANTICLOCKWISE;
 break;
 case 2:
 movement = CLOCKWISE;
 break;
 case 4:
 movement = KEYPRESSED;
 break;
 }
 break;
 case 1:
 switch (PA_delta_t){
 case 3:
 movement = ANTICLOCKWISE;
 break;
 case 0:
 movement = CLOCKWISE;
 break;
 }
 break;
 case 2:
 switch (PA_delta_t){
 case 0:
 movement = ANTICLOCKWISE;
 break;
 case 3:
 movement = CLOCKWISE;
 break;
 }
 break;
 case 3:
 switch (PA_delta_t){
 case 2:
 movement = ANTICLOCKWISE;
 break;
 case 1:
 movement = CLOCKWISE;
 break;
```

```
 }
 case 4:
 movement = KEYPRESSED;
 break;
 }
}
```

Driver entry points such as read() and poll() operate in tandem with roller\_interrupt(). For example, when the handler deciphers wheel movement, it wakes up any waiting poll() threads that may have gone to sleep in response to a select() system call issued by an application such as X Windows. Revisit Listing 4.1 and implement the complete roller driver after learning the internals of character drivers in Chapter 5.

Listing 7.3 in Chapter 7, "Input Drivers," takes advantage of the kernel's *input* interface to convert this roller wheel into a roller mouse.

Let's end this section by introducing some functions that enable and disable interrupts on a particular IRQ. enable\_irq(ROLLER\_IRQ) enables interrupt generation when the roller wheel moves, while disable\_irq(ROLLER\_IRQ) does the reverse. disable\_irq\_nosync(ROLLER\_IRQ) disables roller interrupts but does not wait for any currently executing instance of roller\_interrupt() to return. This nosync flavor of disable\_irq() is faster but can potentially cause race conditions. Use this only when you know that there can be no races. An example user of disable\_irq\_nosync() is *drivers/ide/ide-io.c*, which blocks interrupts during initialization, because some systems have trouble with that.

### **Softirqs and Tasklets**

As discussed previously, interrupt handlers have two conflicting requirements: They are responsible for the bulk of device data processing, but they have to exit as fast as possible. To bail out of this situation, interrupt handlers are designed in two parts: a hurried and harried top half that interacts with the hardware, and a relaxed bottom half that does most of the processing with all interrupts enabled. Unlike interrupts, bottom halves are synchronous because the kernel decides when to execute them. The following mechanisms are available in the kernel to defer work to a bottom half: softirqs, tasklets, and work queues.

Softirqs are the basic bottom half mechanism and have strong locking requirements. They are used only by a few performance-sensitive subsystems such as the networking layer, SCSI layer, and kernel timers. Tasklets are built on top of softirqs and are easier to use. It's recommended to use tasklets unless you have crucial scalability or speed requirements. A primary difference between a softirq and a tasklet is that the former is reentrant whereas the latter isn't. Different instances of a softirq can run simultaneously on different processors, but that is not the case with tasklets.

To illustrate the usage of softirqs and tasklets, assume that the roller wheel in the previous example has inherent hardware problems due to the presence of moving parts (say, the wheel gets stuck occasionally) resulting in the generation of out-of-spec waveforms. A stuck wheel can continuously generate spurious interrupts and potentially freeze the system. To get around this problem, capture the wave stream, run some analysis on it, and dynamically switch from interrupt mode to a polled mode if the wheel looks stuck, and vice versa if it's unstuck. Capture the wave stream from the interrupt handler and perform the analysis from a bottom half. Listing 4.2 implements this using softirqs, and Listing 4.3 uses tasklets. Both are simplified variants of Listing 4.1. This reduces the handler to two functions: roller\_capture() that obtains a wave snippet from GPIO Port D, and roller\_analyze() that runs an algorithmic analysis on the wave and switches to polled mode if required.

**Listing 4.2. Using Softirqs to Offload Work from Interrupt Handlers**

```
void __init
roller_init()
{
 /* ... */
 /* Open the softirq. Add an entry for ROLLER_SOFT_IRQ in
 the enum list in include/linux/interrupt.h */
 open_softirq(ROLLER_SOFT_IRQ, roller_analyze, NULL);
}
/* The bottom half */
void
roller_analyze()
{
 /* Analyze the waveforms and switch to polled mode if required */
}
/* The interrupt handler */
static irqreturn_t
roller_interrupt(int irq, void *dev_id)
{
 /* Capture the wave stream */
 roller_capture();
 /* Mark softirq as pending */
 raise_softirq(ROLLER_SOFT_IRQ);
 return IRQ_HANDLED;
}
```

To define a softirq, you have to statically add an entry to *include/linux/interrupt.h.* You can't define one dynamically. raise\_softirq() announces that the corresponding softirq is pending execution. The kernel will execute it at the next available opportunity. This can be during exit from an interrupt handler or via the *ksoftirqd* kernel thread.

**Listing 4.3. Using Tasklets to Offload Work from Interrupt Handlers**

```
struct roller_device_struct { /* Device-specific structure */
 /* ... */
 struct tasklet_struct tsklt;
 /* ... */
}
void __init roller_init()
{
 struct roller_device_struct *dev_struct;
 /* ... */
 /* Initialize tasklet */
 tasklet_init(&dev_struct->tsklt, roller_analyze, dev);
}
/* The bottom half */
void
roller_analyze()
{
/* Analyze the waveforms and switch to
 polled mode if required */
}
/* The interrupt handler */
static irqreturn_t
roller_interrupt(int irq, void *dev_id)
{
 struct roller_device_struct *dev_struct;
 /* Capture the wave stream */
 roller_capture();
 /* Mark tasklet as pending */
 tasklet_schedule(&dev_struct->tsklt);
 return IRQ_HANDLED;
}
```

tasklet\_init() dynamically initializes a tasklet. The function does not allocate memory for a tasklet\_struct, rather you have to pass the address of an allocated one. tasklet\_schedule() announces that the corresponding tasklet is pending execution. Like for interrupts, the kernel offers a bunch of functions to control the execution state of tasklets on systems having multiple processors:

- tasklet\_enable() enables tasklets.
- tasklet\_disable() disables tasklets and waits until any currently executing tasklet instance has exited.
- tasklet\_disable\_nosync() has semantics similar to disable\_irq\_nosync(). The function does not wait for active instances of the tasklet to finish execution.

You have seen the differences between interrupt handlers and bottom halves, but there are a few similarities, too. Interrupt handlers and tasklets are both not reentrant. And neither of them can go to sleep. Also, interrupt handlers, tasklets, and softirqs cannot be preempted.

Work queues are a third way to defer work from interrupt handlers. They execute in process context and are allowed to sleep, so they can use drowsy functions such as mutexes. We discussed work queues in the preceding chapter when we looked at various kernel helper facilities. Table 4.1 compares softirqs, tasklets, and work queues.

**Table 4.1. Comparing Softirqs, Tasklets, and Work Queues**

|                      | Softirqs                                                                                                  | Tasklets                                                                                                     | Work Queues                                  |
|----------------------|-----------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|----------------------------------------------|
| Execution<br>context | Deferred work runs in<br>interrupt context.                                                               | Deferred work runs in<br>interrupt context.                                                                  | Deferred work runs in<br>process context.    |
| Reentrancy           | Can run simultaneously<br>on different CPUs.                                                              | Cannot run<br>simultaneously on<br>different CPUs. Different<br>CPUs can run different<br>tasklets, however. | Can run simultaneously<br>on different CPUs. |
| Sleep<br>semantics   | Cannot go to sleep.                                                                                       | Cannot go to sleep.                                                                                          | May go to sleep.                             |
| Preemption           | Cannot be<br>preempted/scheduled.                                                                         | Cannot be<br>preempted/scheduled.                                                                            | May be<br>preempted/scheduled.               |
| Ease of use          | Not easy to use.                                                                                          | Easy to use.                                                                                                 | Easy to use.                                 |
| When to use          | If deferred work will not<br>go to sleep and if you<br>have crucial scalability<br>or speed requirements. | If deferred work will not<br>go to sleep.                                                                    | If deferred work may go<br>to sleep.         |

There is an ongoing debate in LKML on the feasibility of getting rid of the tasklet interface. Tasklets enjoy more priority than process context code, so they present latency problems. Moreover, as you learned, they are constrained not to sleep and to execute on the same CPU. It's being suggested that all existing tasklets be converted to softirqs or work queues on a case-by-case basis.

The –rt patch-set alluded to in Chapter 2 moves interrupt handling to kernel threads to achieve wider preemption coverage.

# **The Linux Device Model**

The new Linux device model introduces C++-like abstractions that factor out commonalities from device drivers into bus and core layers. Let's look at the different components constituting the device model such as *udev*, *sysfs*, *kobjects*, and *device classes* and their effects on key kernel subsystems such as */dev* node management, hotplug, firmware download, and module autoload. *Udev* is the best vantage point to view the benefits of the device model, so let's start from there.

### **Udev**

Years ago when Linux was young, it was not fun to administer device nodes. All the needed nodes (which could run into thousands) had to be statically created under the */dev* directory. This problem, in fact, dated all the way back to original UNIX systems. With the advent of the 2.4 kernels came *devfs*, which introduced dynamic device node creation. Devfs provided services to generate device nodes in an in-memory filesystem, but the onus of naming the nodes still rested with device drivers. Device naming policy is administrative and does not mix well with the kernel, however. The place for policy is in header files, kernel module parameters, or user space. Udev arrived on the scene to push device management to user space.

Udev depends on the following to do its work:

- Kernel *sysfs* support, which is an important part of the Linux device model. Sysfs is an in-memory filesystem mounted under */sys* at boot time (look at */etc/fstab* for the specifier). We will look at sysfs in the next section, but for now, take the corresponding sysfs file accesses for granted. **1.**
- **2.** A set of user-space daemons and utilities such as *udevd* and *udevinfo*.
- User-specified rules located in the */etc/udev/rules.d/* directory. You may frame rules to get a consistent view of your devices. **3.**

To understand how to use udev, let's look at an example. Assume that you have a USB DVD drive and a USB CD-RW drive. Depending on the order in which you hotplug these devices, one of them is assigned the name */dev/sr0*, and the other gets the name */dev/sr1.* During pre-udev days, you had to figure out the associated names before you could use the devices. But with udev, you can consistently view the DVD (as say, */dev/usbdvd)* and the CD-RW (as say, */dev/usbcdrw*) irrespective of the order in which they are plugged in or out.

First, pull product attributes from corresponding files in sysfs. Assume that the (Targus) DVD drive has been assigned the device node */dev/sr0* and that the (Addonics) CD-RW drive has been given the name */dev/sr1*. Use udevinfo to collect device information:

```
bash> udevinfo -a -p /sys/block/sr0
...
looking at the device chain at
'/sys/devices/pci0000:00/0000:00:1d.7/usb1/1-4':
 BUS=»usb»
 ID=»1-4»
 SYSFS{bConfigurationValue}=»1»
 ...
```

```
 SYSFS{idProduct}=»0701»
 SYSFS{idVendor}=»05e3»
 SYSFS{manufacturer}=»Genesyslogic»
 SYSFS{maxchild}=»0»
 SYSFS{product}=»USB Mass Storage Device»
 ...
bash> udevinfo -a -p /sys/block/sr1
 ...
 looking at the device chain at
 '/sys/devices/pci0000:00/0000:00:1d.7/usb1/1-3':
 BUS=»usb»
 ID=»1-3»
 SYSFS{bConfigurationValue}=»2»
 ...
 SYSFS{idProduct}=»0302»
 SYSFS{idVendor}=»0dbf»
 SYSFS{manufacturer}=»Addonics»
 SYSFS{maxchild}=»0»
 SYSFS{product}=»USB to IDE Cable»
 ...
```

Next, let's use the product information gleaned to identify the devices and add udev naming rules. Create a file called */etc/udev/rules.d/40-cdvd.rules* and add the following rules to it:

```
BUS="usb", SYSFS{idProduct}="0701", SYSFS{idVendor}="05e3",
KERNEL="sr[0-9]*", NAME="%k", SYMLINK="usbdvd"
BUS="usb", SYSFS{idProduct}="0302", SYSFS{idVendor}="0dbf",
KERNEL="sr[0-9]*", NAME="%k", SYMLINK="usbcdrw"
```

The first rule tells udev that whenever it finds a USB device with a product ID of 0x0701, vendor ID of 0x05e3, and a name starting with *sr*, it should create a node of the same name under */dev* and produce a symbolic link named *usbdvd* to the created node. Similarly, the second rule orders creation of a symbolic link named *usbcdrw* for the CD-RW drive.

To test for syntax errors in your rules, run udevtest on */sys/block/sr\**. To turn on verbose messages in */var/log/messages*, set udev\_log to "yes" in */etc/udev/udev.conf*. To repopulate the */dev* directory with newly added rules on-the-fly, restart udev using udevstart. When this is done, your DVD drive consistently appears to the system as */dev/usbdvd*, and your CD-RW drive always appears as */dev/usbcdrw.* You can deterministically mount them from shell scripts using commands such as these:

```
mount /dev/usbdvd /mnt/dvd
```

Consistent naming of device nodes (and network interfaces) is not the sole capability of udev. It has metamorphed into the Linux hotplug manager, too. Udev is also in charge of automatically loading modules on demand and downloading microcode onto devices that need them. But before digging into those capabilities, let's obtain a basic understanding of the innards of the device model.

**Sysfs, Kobjects, and Device Classes**

Sysfs, kobjects, and device classes are the building blocks of the device model but are publicity shy and prefer to remain behind the scenes. They are mostly in the usage domain of bus and core implementations, and hide inside APIs that provide services to device drivers.

Sysfs is the user-space manifestation of the kernel's structured device model. It's similar to procfs in that both are in-memory filesystems containing information about kernel data structures. Whereas procfs is a generic window into kernel internals, sysfs is specific to the device model. Sysfs is, hence, not a replacement for procfs. Information such as process descriptors and sysctl parameters belong to procfs and not sysfs. As will be apparent soon, udev depends on sysfs for most of its extended functions.

Kobjects introduce an encapsulation of common object properties such as usage reference counts. They are usually embedded within larger structures. The following are the main fields of a kobject, which is defined in *include/linux/kobject.h*:

A *kref* object that performs reference count management. The kref\_init() interface initializes a kref, kref\_get() increments the reference count associated with the kref, and kref\_put() decrements the reference count and frees the object if there are no remaining references. The URB structure (explained in Chapter 11, "Universal Serial Bus"), for example, contains a kref to track the number of references to it.[2] **1.**

```
[2] The usb_alloc_urb() interface calls kref_init(), usb_submit_urb() invokes kref_get(), and usb_free_urb() calls
kref_put().
```

- **2.** A pointer to a *kset*, which is an object set to which the kobject belongs.
- **3.** A *kobj\_type*, which is an object type that describes the kobject.

Kobjects are intertwined with sysfs. Every kobject instantiated within the kernel has a sysfs representation.

The concept of device classes is another feature of the device model and is an interface you're more likely to use in a driver. The class interface abstracts the idea that each device falls under a broader class (or category) of devices. A USB mouse, a PS/2 keyboard, and a joystick all fall under the *input* class and own entries under */sys/class/input/*.

Figure 4.4 shows the sysfs hierarchy on a laptop that has an external USB mouse connected to it. The top-level *bus*, *class*, and *device* directories are expanded to show that sysfs provides a view of the USB mouse based on its device type as well as its physical connection. The mouse is an input class device but is physically a USB device answering to two endpoint addresses, a control endpoint *ep00*, and an interrupt endpoint, *ep81.* The USB port in question belongs to the USB host controller on bus 2, and the USB host controller itself is bridged to the CPU via the PCI bus. If these details are not making much sense at this point, don't worry; rewind to this section after reading the chapters that teach input drivers (Chapter 7), PCI drivers (Chapter 10, "Peripheral Component Interconnect"), and USB drivers (Chapter 11).

**Figure 4.4. Sysfs hierarchy of a USB mouse.**

```
[/sys]
 +[block]
 -[bus]—[usb]—[devices]—[usb2]—[2-2]—[2-2:1.0]-[usbendpoint:usbdev2.2-ep81]
 -[class]-[input]—[mouse2]—[device]—[bus]—[usbendpoint:usbdev2.2-ep81]
 -[usb_device]—[usbdev2.2]—[device]—[bus]
 -[usb_endpoint]—[usbdev2.2-ep00]—[device]
 —[usbdev2.2-ep81]—[device]
 -[devices]—[pci0000:00]—[0000:00:1d:1]—[usb2]—[2-2]—[2-2:1.0]
 +[firmware]
 +[fs]
 +[kernel]
 +[module]
 +[power]
```

Browse through */sys* looking for entries that associate with another device (for example, your network card) to get a better feel of its hierarchical organization. The section "Addressing and Identification" in Chapter 10 illustrates how sysfs mirrors the physical connection of a CardBus Ethernet-Modem card on a laptop.

The class programming interface is built on top of kobjects and sysfs, so it's a good place to start digging to understand the end-to-end interactions between the components of the device model. Let's turn to the RTC driver for an example. The RTC driver (*drivers/char/rtc.c*) is a miscellaneous (or "misc") driver. We discuss misc drivers in detail when we look at character device drivers in Chapter 5.

Insert the RTC driver module and look at the nodes created under */sys* and */dev:*

```
bash> modprobe rtc
bash> ls -lR /sys/class/misc
drwr-xr-x 2 root root 0 Jan 15 01:23 rtc
/sys/class/misc/rtc:
total 0
-r--r--r-- 1 root root 4096 Jan 15 01:23 dev
--w------- 1 root root 4096 Jan 15 01:23 uevent
bash> ls -l /dev/rtc
crw-r--r-- 1 root root 10, 135 Jan 15 01:23 /dev/rtc
```

*/sys/class/misc/rtc/dev* contains the major and minor numbers (discussed in the next chapter) assigned to this device, */sys/class/misc/rtc/uevent* is used for coldplugging (discussed in the next section), and */dev/rtc* is used by applications to access the RTC driver.

Let's understand the code flow through the device model. Misc drivers utilize the services of misc\_register() during initialization, which looks like this if you peel off some code:

```
/* ... */
dev = MKDEV(MISC_MAJOR, misc->minor);
misc->class = class_device_create(misc_class, NULL, dev,
 misc->dev,
 "%s", misc->name);
if (IS_ERR(misc->class)) {
 err = PTR_ERR(misc->class);
 goto out;
```

```
}
/* ... */
```

Figure 4.5 continues to peel off more layers to get to the bottom of the device modeling. It illustrates the transitions that ripple through classes, kobjects, sysfs, and udev, which result in the generation of the */sys* and */dev* files listed previously.

```
User Space                     Kernel Space
-----------                    -------------------------------

modprobe rtc  ---------------->  rtc_init()
                                   |
                                   v
                           misc_register(&rtc_dev)
                                   |
                                   v
                           class_device_register()
                                   |
        -----------------------------------------------------------------------------
        |                         |                  |                              |
        v                         |                  v                              v
   kobject_add()                  |      class_device_add_attrs()        kobject_uevent(KOBJ_ADD)
        |                         |                  |                             |
        v                         v                  |                             v
sysfs_create_dir()   class_device_create_file()      |                     kobject_uevent_env()
        |                         |                  v                             |
        v                         v        class_device_create_file()              v
/sys/class/misc/rtc/      sysfs_create_file()        |                     udevd receives the uevent via 
                                |                    |                     netlink sockets and creates /dev
                                v                    v                     nodes after consulting its rules
                   /sys/class/misc/rtc/uevent    sysfs_create_file()       database
                                                         |                         |
                                                         v                         v
                                             /sys/class/misc/rtc/dev             /dev/rtc
```

**Figure 4.5. Tying the pieces of the device model.**

Look at the parallel port LED driver (Listing 5.6 in the section "Talking to the Parallel Port" in Chapter 5) and the virtual mouse input driver (Listing 7.2 in the section "Device Example: Virtual Mouse" in Chapter 7) for examples on creating device control files inside sysfs.

Another abstraction that is part of the device model is the *bus-device-driver* programming interface. Kernel device support is cleanly structured into buses, devices, and drivers. This renders the individual driver implementations simpler and more general. *Bus* implementations can, for example, search for drivers that can handle a particular device.

Consider the kernel's I2C subsystem (explored in Chapter 8, "The Inter-Integrated Circuit Protocol"). The I2C layer consists of a core infrastructure, device drivers for bus adapters, and drivers for client devices. The I2C

core layer registers each detected I2C bus adapter using bus\_register(). When an I2C client device (say, an *Electrically Erasable Programmable Read-Only Memory* [EEPROM] chip) is probed and detected, its existence is recorded via device\_register(). Finally, the I2C EEPROM client driver registers itself using driver\_register(). These registrations are performed indirectly using service functions offered by the I2C core.

bus\_register() adds a corresponding entry to */sys/bus/*, while device\_register() adds entries under */sys/devices/.* struct bus\_type, struct device, and struct device\_driver are the main data structures used respectively by buses, devices, and drivers. Take a peek inside *include/linux/device.h* for their definitions.

## **Hotplug and Coldplug**

Devices connected to a running system on-the-fly are said to be *hotplugged*, whereas those connected prior to system boot are considered to be *coldplugged.* Earlier, the kernel used to notify user space about hotplug events by invoking a helper program registered via the */proc* filesystem. But when current kernels detect hotplug, they dispatch uevents to user space via netlink sockets. Netlink sockets are an efficient mechanism to communicate between kernel space and user space using socket APIs. At the user-space end, udevd, the daemon that manages device node creation and removal, receives the uevents and manages hotplug.

To see how hotplug handling has evolved recently, let's consider progressive levels of udev running different versions of the 2.6 kernel:

With a *udev-039* package and a 2.6.9 kernel, when the kernel detects a hotplug event, it invokes the user space helper registered with */proc/sys/kernel/hotplug.* This defaults to */sbin/hotplug*, which receives attributes of the hotplugged device in its environment. */sbin/hotplug* looks inside the hotplug configuration directory (usually */etc/hotplug.d/default/*) and runs, for example, */etc/hotplug.d/default/10-udev.hotplug*, after executing other scripts under */etc/hotplug/*. **1.**

```
bash> ls -l /etc/hotplug.d/default/
...
lrwcrwxrwx 1 root root 14 May 11 2005 10-udev.hotplug -> /sbin/udevsend
...
```

When */sbin/udevsend* thus gets executed, it passes the hotplugged device information to udevd.

With udev-058 and a 2.6.11 kernel, the story changes somewhat. The udevsend utility replaces /*sbin/hotplug*: **2.**

```
bash> cat /proc/sys/kernel/hotplug
/sbin/udevsend
```

With the latest levels of udev and the kernel, udevd assumes full responsibility of managing hotplug without depending on udevsend. It now pulls hotplug events directly from the kernel via netlink sockets (see Figure 4.4). */proc/sys/kernel/hotplug* contains nothing: **3.**

```
bash> cat /proc/sys/kernel/hotplug
bash>
```

Udev also handles coldplug. Because udev is part of user space and is started only after the kernel boots, a special mechanism is needed to emulate hotplug events over coldplugged devices. At boot time, the kernel creates a file named *uevent* under sysfs for all devices and emits coldplug events to those files. When udev starts, it reads all the *uevent* files from */sys* and generates hotplug uevents for each coldplugged device.

#### **Microcode Download**

You have to feed microcode to some devices before they can get ready for action. The microcode gets executed by an on-card microcontroller. Device drivers used to store microcode inside static arrays in header files. But this has become untenable because microcode is usually distributed as proprietary binary images by device vendors, and that doesn't mix homogeneously with the GPL-ed kernel. Another reason against mixing firmware with kernel sources is that they run on different release time lines. The solution apparently is to separately maintain microcode in user space and pass it down to the kernel when required. Sysfs and udev provide an infrastructure to achieve this.

Let's take the example of the Intel PRO/Wireless 2100 WiFi mini PCI card found on several laptops. The card is built around a microcontroller that needs to execute externally supplied microcode for normal operation. Let's walk through the steps that the Linux driver follows to download microcode to the card. Assume that you have obtained the required microcode image (*ipw2100-1.3.fw*) from <http://ipw2100.sourceforge.net/firmware.php> and saved it under */lib/firmware/* on your system and that you have inserted the driver module *ipw2100.ko*:

**1.** During initialization, the driver invokes the following:

```
request_firmware(..,"ipw2100-1.3.fw",..);
```

- **2.** This dispatches a hotplug uevent to user space, along with the identity of the requested microcode image.
- **3.** Udevd receives the uevent and responds by invoking */sbin/firmware\_helper.* For this, it uses a rule similar to the following from a file under /*etc/udev/rules.d/*:

```
ACTION=="add", SUBSYSTEM=="firmware", RUN="/sbin/firmware_helper"
```

- **4.** */sbin/firmware\_helper* looks inside /*lib/firmware/* and locates the requested microcode image *ipw2100- 1.3.fw.* It dumps the image to */sys/class/0000:02:02.0/data*. (0000:02:02 is the PCI *bus:device:function* identifier of the WiFi card in this case.)
- **5.** The driver receives the microcode and downloads it onto the device. When done, it calls release\_firmware() to free the corresponding data structures.
- **6.** The driver goes through the rest of the initializations and the WiFi adapter beacons.

## **Module Autoload**

Automatically loading kernel modules on demand is a convenient feature that Linux supports. To understand how the kernel emits a "module fault" and how udev handles it, let's insert a Xircom CardBus Ethernet adapter into a laptop's PC Card slot:

**1.** During compile time, the identity of supported devices is generated as part of the driver module object. Take a peek at the driver that supports the Xircom CardBus Ethernet combo card (*drivers/net/tulip/xircom\_cb.c*) and find this snippet:

```
static struct pci_device_id xircom_pci_table[] = {
 {0x115D, 0x0003, PCI_ANY_ID, PCI_ANY_ID,},
 {0,},
};
/* Mark the device table */
MODULE_DEVICE_TABLE(pci, xircom_pci_table);
```

This declares that the driver can support any card having a PCI vendor ID of 0x115D and a PCI device ID of 0x0003 (more on this in Chapter 10). When you install the driver module, the depmod utility looks inside the module image and deciphers the IDs present in the device table. It then adds the following entry to */lib/modules/kernel-version/modules.alias*:

```
alias pci:v0000115Dd00000003sv*sd*bc*sc*i* xircom_cb
```

where v stands for VendorID, d for DeviceID, sv for subvendorID, and \* for wildcard match.

**2.** When you hotplug the Xircom card into a CardBus slot, the kernel generates a uevent that announces the identity of the newly inserted device. You may look at the generated uevent using udevmonitor:

```
bash> udevmonitor --env
 ...
 MODALIAS=pci:v0000115Dd00000003sv0000115Dsd00001181bc02sc00i00
 ...
```

**3.** Udevd receives the uevent via a netlink socket and invokes modprobe with the above MODALIAS that the kernel passed up to it:

```
modprobe pci:v0000115Dd00000003sv0000115Dsd00001181bc02sc00i00
```

**4.** Modprobe finds the matching entry in */lib/modules/kernel-version/modules.alias* created during Step 1, and proceeds to insert *xircom\_cb:*

### **bash> lsmod** Module Size Used by xircom\_cb 10433 0 ...

The card is now ready to surf.

You may want to revisit this section after reading Chapter 10.

### Udev on Embedded Devices

One school of thought deprecates the use of udev in favor of statically created device nodes on embedded devices for the following reasons:

- Udev creates */dev* nodes during each reboot, compared to static nodes that are created only once during software install. If your embedded device uses flash storage, flash pages that hold */dev* nodes suffer an erase-write cycle on each boot in the case of the former, and this reduces flash life span. (Flash memory is discussed in detail in Chapter 17, "Memory Technology Devices.") You do have the option of mounting */dev* over a RAM-based filesystem, however.
- Udev contributes to increased boot time.
- Udev features such as dynamic creation of */dev* nodes and autoloading of modules create a degree of indeterminism that some solution designers prefer to avoid on special-purpose embedded devices, especially ones that do not interact with the outside world via hotpluggable buses. According to this point of view, static node creation and boot-time insertion of any modules provide more control over the system and make it easier to test.

# **Memory Barriers**

Many processors and compilers reorder instructions to achieve optimal execution speeds. The reordering is done such that the new instruction stream is semantically equivalent to the original one. However, if you are, for example, writing to memory mapped registers on an I/O device, instruction reordering can generate unexpected side effects. To prevent the processor from reordering instructions, you can insert a barrier in your code. The wmb() function inserts a road block that prevents writes from moving through it, rmb() provides a read barricade that disallows reads from crossing it, and mb() results in a read-write barrier.

In addition to the CPU-to-hardware interactions referred to previously, memory barriers are also relevant for CPU-to-CPU interactions on SMP systems. If your CPU's data cache is operating in write-back mode (in which data is not copied from cache to memory until it's absolutely necessary), you might want to stall the instruction stream until the cache-to-memory queue is drained. This is relevant, for example, when you encounter instructions that acquire or release locks. Barriers are used in this scenario to obtain a consistent perception across CPUs.

We revisit memory barriers when we discuss PCI drivers in Chapter 10 and flash map drivers in Chapter 17. In the meanwhile, stop by *Documentation/memory-barriers.txt* for an explanation of different kinds of memory barriers.

# **Power Management**

Power management is critical on devices running on battery, such as laptops and handhelds. Linux drivers need to be aware of power states and have to transition across states in response to events such as *standby, sleep*, and *low battery.* Drivers utilize power-saving features supported by the underlying hardware when they switch to modes that consume less power. For example, the storage driver spins down the disk, whereas the video driver blanks the display.

Power-aware code in device drivers is only one piece of the overall power management framework. Power management also features participation from user space daemons, utilities, configuration files, and boot firmware. Two popular power management mechanisms are APM (discussed in the section, "Protected Mode Calls" in Appendix B, "Linux and the BIOS") and *Advanced Configuration and Power Interface* (ACPI). APM is getting obsolete, and ACPI has emerged as the de facto power management strategy on Linux systems. ACPI is further discussed in Chapter 20, "More Devices and Drivers."

# **Looking at the Sources**

The core interrupt handling code is generic and is in the *kernel/irq/* directory. The architecture-specific portions can be found in *arch/your-arch/kernel/irq.c.* The function do\_IRQ() defined in this file is a good place to start your journey into the kernel interrupt handling mechanism.

The kernel softirq and tasklet implementations live in *kernel/softirq.c.* This file also contains additional functions that offer more fine-grained control over softirqs and tasklets. Look at *include/linux/interrupt.h* for softirq vector enumerations and prototypes required to implement your interrupt handler. For a real-life example of writing interrupt handlers and bottom halves, start from the handler that is part of *drivers/net/lib8390.c* and follow the trail into the networking stack.

The kobject implementation and related programming interfaces live in *lib/kobject.c* and *include/linux/kobject.h.* Look at *drivers/base/sys.c* for the sysfs implementation. You will find device class APIs in *drivers/base/class.c.* Dispatching hotplug uevents via netlink sockets is done by *lib/kobject\_uevent.c.* You may download udev sources and documentation from www.kernel.org/pub/linux/utils/kernel/hotplug/udev.html.

For a fuller understanding of how APM is implemented on x86 Linux, look at *arch/x86/kernel/apm\_32.c, include/linux/apm\_bios.h*, and *include/asm-x86/mach-default/apm.h* in the kernel tree. If you are curious to know how APM is implemented on BIOS-less architectures such as ARM, look at *include/linux/apm-emulation.h* and its users. The kernel's ACPI implementation lives in *drivers/acpi/*.

Table 4.2 contains a summary of the main data structures used in this chapter and the location of their definitions in the source tree. Table 4.3 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 4.2. Summary of Data Structures**

| Data Structure Location     |                                          | Description                                                        |
|-----------------------------|------------------------------------------|--------------------------------------------------------------------|
|                             | tasklet_struct include/linux/interrupt.h | Manages a tasklet, which is a method to<br>implement bottom halves |
| kobject                     | include/linux/kobject.h                  | Encapsulates common properties of a<br>kernel object               |
| kset                        | include/linux/kobject.h                  | An object set to which a kobject belongs                           |
| kobj_type                   | include/linux/kobject.h                  | An object type that describes a kobject                            |
| class                       | include/linux/device.h                   | Abstracts the idea that a driver falls<br>under a broader category |
| bus device<br>device_driver | include/linux/device.h                   | Structures that form the pillars under the<br>Linux device model   |

**Table 4.3. Summary of Kernel Programming Interfaces**

| Kernel Interface | Location            | Description                                                    |
|------------------|---------------------|----------------------------------------------------------------|
| request_irq()    | kernel/irq/manage.c | Requests an IRQ and associates an<br>interrupt handler with it |
| free_irq()       | kernel/irq/manage.c | Frees an IRQ                                                   |

| Kernel Interface                                                                                                                                                 | Location                                                                       | Description                                                                                                                                             |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------|
| disable_irq()                                                                                                                                                    | kernel/irq/manage.c                                                            | Disables the interrupt associated with<br>a supplied IRQ                                                                                                |
| disable_irq_nosync()                                                                                                                                             | kernel/irq/manage.c                                                            | Disables the interrupt associated with<br>a supplied IRQ without waiting for<br>any currently executing instances of<br>the interrupt handler to return |
| enable_irq()                                                                                                                                                     | kernel/irq/manage.c                                                            | Re-enables the interrupt that has<br>been disabled using disable_irq()<br>or disable_irq_nosync()                                                       |
| open_softirq()                                                                                                                                                   | kernel/softirq.c                                                               | Opens a softirq                                                                                                                                         |
| raise_softirq()                                                                                                                                                  | kernel/softirq.c                                                               | Marks the softirq as pending<br>execution                                                                                                               |
| tasklet_init()                                                                                                                                                   | kernel/softirq.c                                                               | Dynamically initializes a tasklet                                                                                                                       |
| tasklet_schedule()                                                                                                                                               | include/linux/interrupt.hkernel/softirq.c Marks a tasklet as pending execution |                                                                                                                                                         |
| tasklet_enable()                                                                                                                                                 | include/linux/interrupt.h                                                      | Enables a tasklet                                                                                                                                       |
| tasklet_disable()                                                                                                                                                | include/linux/interrupt.h                                                      | Disables a tasklet                                                                                                                                      |
| tasklet_disable_nosync()                                                                                                                                         | include/linux/interrupt.h                                                      | Disables a tasklet without waiting for<br>active instances to finish execution                                                                          |
| class_device_register()                                                                                                                                          | drivers/base/class.c                                                           | Family of functions in the Linux                                                                                                                        |
| kobject_add()                                                                                                                                                    | lib/kobject.c                                                                  | device model that create/destroy a<br>class, device class, and associated                                                                               |
| sysfs_create_dir()                                                                                                                                               | lib/kobject_uevent.c                                                           | kobjects and sysfs files                                                                                                                                |
| class_device_create()                                                                                                                                            | fs/sysfs/dir.c                                                                 |                                                                                                                                                         |
| class_device_destroy()<br>class_create()<br>class_destroy()<br>class_device_create_file()<br>sysfs_create_file()<br>class_device_add_attrs()<br>kobject_uevent() | fs/sysfs/file.c                                                                |                                                                                                                                                         |

This finishes our exploration of device driver concepts. You might want to dip back into this chapter while developing your driver.
