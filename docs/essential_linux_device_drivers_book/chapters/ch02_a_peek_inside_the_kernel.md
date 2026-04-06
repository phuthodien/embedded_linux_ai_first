# **Chapter 2. A Peek Inside the Kernel**

| In This Chapter                          |    |
|------------------------------------------|----|
| Booting Up                               | 18 |
| Kernel Mode and User Mode                | 30 |
| Process Context and Interrupt<br>Context | 30 |
| Kernel Timers                            | 31 |
| Concurrency in the Kernel                | 39 |
| Process Filesystem                       | 49 |
| Allocating Memory                        | 49 |
| Looking at the Sources                   | 52 |

Before we start our journey into the mystical world of Linux device drivers, let's familiarize ourselves with some basic kernel concepts by looking at several kernel regions through the lens of a driver developer. We learn about kernel timers, synchronization mechanisms, and memory allocation. But let's start our expedition by getting a view from the top; let's skim through boot messages emitted by the kernel and hit the breaks whenever something looks interesting.

# **Booting Up**

Figure 2.1 shows the Linux boot sequence on an x86-based computer. Linux boot on x86-based hardware is set into motion when the BIOS loads the *Master Boot Record* (MBR) from the boot device. Code resident in the MBR looks at the partition table and reads a Linux bootloader such as GRUB, LILO, or SYSLINUX from the active

partition. The final stage of the bootloader loads the compressed kernel image and passes control to it. The kernel uncompresses itself and turns on the ignition.

```
+------------------+
|     Power On     |
+------------------+
          |
          v
+------------------+
|       BIOS       |
+------------------+
          |
          v
+---------------------------+
| Bootloader (GRUB/LILO/..)|
+---------------------------+
          |
          v
+---------------------------+
|    Real Mode Kernel       |   <- x86 Real Mode
+---------------------------+
          |
          |  switch to Protected Mode
          |  (arch/x86/boot/pm.c)
          v
+---------------------------+
| Protected Mode Kernel     |   <- x86 Protected Mode
+---------------------------+
          |
          v
+---------------------------+
|      The init Process     |
+---------------------------+
          |
          v
+---------------------------+
| User Processes & Daemons  |
+---------------------------+
```

**Figure 2.1. Linux boot sequence on x86-based hardware.**

x86-based processors have two modes of operation, *real* mode and *protected* mode. In real mode, you can access only the first 1MB of memory, that too without any protection. Protected mode is sophisticated and lets you tap into many advanced features of the processor such as paging. The CPU has to pass through real mode en route to protected mode. This road is a one-way street, however. You can't switch back to real mode from protected mode.

The first-level kernel initializations are done in real mode assembly. Subsequent startup is performed in protected mode by the function start\_kernel() defined in *init/main.c*, the source file you modified in the previous chapter. start\_kernel() begins by initializing the CPU subsystem. Memory and process management are put in place soon after. Peripheral buses and I/O devices are started next. As the last step in the boot sequence, the *init* program, the parent of all Linux processes, is invoked. Init executes user-space scripts that start necessary kernel services. It finally spawns terminals on consoles and displays the login prompt.

Each following section header is a message from Figure 2.2 generated during boot progression on an x86-based

laptop. The semantics and the messages may change if you are booting the kernel on other architectures. If some explanations in this section sound rather cryptic, don't worry; the intent here is only to give you a picture from 100 feet above and to let you savor a first taste of the kernel's flavor. Many concepts mentioned here in passing are covered in depth later on.

**Figure 2.2. Kernel boot messages.**

```
Linux version 2.6.23.1y (root@localhost.localdomain) (gcc version 4.1.1 20061011 (Red
Hat 4.1.1-30)) #7 SMP PREEMPT Thu Nov 1 11:39:30 IST 2007
BIOS-provided physical RAM map:
 BIOS-e820: 0000000000000000 - 000000000009f000 (usable)
 BIOS-e820: 000000000009f000 - 00000000000a0000 (reserved)
 ...
758MB LOWMEM available.
...
Kernel command line: ro root=/dev/hda1
...
Console: colour VGA+ 80x25
...
Calibrating delay using timer specific routine.. 1197.46 BogoMIPS (lpj=2394935)
...
CPU: L1 I cache: 32K, L1 D cache: 32K
CPU: L2 cache: 1024K
...
Checking 'hlt' instruction... OK.
...
Setting up standard PCI resources
...
NET: Registered protocol family 2
IP route cache hash table entries: 32768 (order: 5, 131072 bytes)
TCP established hash table entries: 131072 (order: 9, 2097152 bytes)
...
checking if image is initramfs... it is
Freeing initrd memory: 387k freed
...
io scheduler noop registered
io scheduler anticipatory registered (default)
...
00:0a: ttyS0 at I/O 0x3f8 (irq = 4) is a NS16550A
...
Uniform Multi-Platform E-IDE driver Revision: 7.00alpha2
ide: Assuming 33MHz system bus speed for PIO modes; override with idebus=xx
ICH4: IDE controller at PCI slot 0000:00:1f.1
Probing IDE interface ide0...
hda: HTS541010G9AT00, ATA DISK drive
hdc: HL-DT-STCD-RW/DVD DRIVE GCC-4241N, ATAPI CD/DVD-ROM drive
...
serio: i8042 KBD port at 0x60,0x64 irq 1
mice: PS/2 mouse device common for all mice
...
Synaptics Touchpad, model: 1, fw: 5.9, id: 0x2c6ab1, caps: 0x884793/0x0
...
agpgart: Detected an Intel 855GM Chipset.
...
Intel(R) PRO/1000 Network Driver - version 7.3.20-k2
...
ehci_hcd 0000:00:1d.7: EHCI Host Controller
...
```

```
Yenta: CardBus bridge found at 0000:02:00.0 [1014:0560]
...
Non-volatile memory driver v1.2
...
kjournald starting. Commit interval 5 seconds
EXT3 FS on hda2, internal journal
EXT3-fs: mounted filesystem with ordered data mode.
...
INIT: version 2.85 booting
...
```

### **BIOS-Provided Physical RAM Map**

The kernel assembles the system memory map from the BIOS, and this is one of the first boot messages you will see:

```
BIOS-provided physical RAM map:
BIOS-e820: 0000000000000000 - 000000000009f000 (usable)
...
BIOS-e820: 00000000ff800000 - 0000000100000000 (reserved)
```

Real mode initialization code uses the BIOS int 0x15 service with function number 0xe820(hence the string BIOS-e820 in the preceding message) to obtain the system memory map. The memory map indicates reserved and usable memory ranges, which is subsequently used by the kernel to create its free memory pool. We discuss more on the BIOS-supplied memory map in the section "Real Mode Calls" in Appendix B, "Linux and the BIOS."

### **758MB LOWMEM Available**

The normally addressable kernel memory region (< 896MB) is called *low memory.* The kernel memory allocator, kmalloc(), returns memory from this region. Memory beyond 896MB (called *high memory*) can be accessed only using special mappings.

During boot, the kernel calculates and displays the total pages present in these memory zones. We take a deeper look at memory zones later in this chapter.

### **Kernel Command Line: ro root=/dev/hda1**

Linux bootloaders usually pass a command line to the kernel. Arguments in the command line are similar to the argv[] list passed to the main() function in C programs, except that they are passed to the kernel instead. You may add command-line arguments to the bootloader configuration file or supply them from the bootloader prompt at runtime.[1] If you are using the GRUB bootloader, the configuration file is either */boot/grub/grub.conf* or */boot/grub/menu.lst* depending on your distribution. If you are a LILO user, the configuration file is */etc/lilo.conf.* An example *grub.conf* file (with comments added) is listed here. You can figure out the genesis of the preceding boot message if you look at the line following title kernel 2.6.23:

<sup>[1]</sup> Bootloaders on embedded devices are usually "slim" and do not support configuration files or equivalent mechanisms. Because of this, many non-x86 architectures support a kernel configuration option called CONFIG\_CMDLINE that you can use to supply the kernel command line at build time.

```
timeout 5 #5 second to alter boot order or parameters
title kernel 2.6.23 #Boot Option 1
 #The boot image resides in the first partition of the first disk
 #under the /boot/ directory and is named vmlinuz-2.6.23. 'ro'
 #indicates that the root partition should be mounted read-only.
 kernel (hd0,0)/boot/vmlinuz-2.6.23 ro root=/dev/hda1
 #Look under section "Freeing initrd memory:387k freed"
 initrd (hd0,0)/boot/initrd
#...
```

Command-line arguments affect the code path traversed during boot. As a simple example, assume that the command-line argument of interest is called bootmode. If this parameter is set to 1, you would like to print some debug messages during boot and switch to a runlevel of 3 at the end of the boot. (Wait until the boot messages are printed out by the init process to learn the semantics of runlevels.) If bootmode is instead set to 0, you would prefer the boot to be relatively laconic, and the runlevel set to 2. Because you are already familiar with *init/main.c*, let's add the following modification to it:

```
static unsigned int bootmode = 1;
static int __init
is_bootmode_setup(char *str)
{
 get_option(&str, &bootmode);
 return 1;
}
/* Handle parameter "bootmode=" */
__setup("bootmode=", is_bootmode_setup);
if (bootmode) {
 /* Print verbose output */
 /* ... */
}
/* ... */
/* If bootmode is 1, choose an init runlevel of 3, else
 switch to a run level of 2 */
if (bootmode) {
 argv_init[++args] = "3";
} else {
 argv_init[++args] = "2";
}
/* ... */
```

Rebuild the kernel as you did earlier and try out the change. We discuss more about kernel command-line arguments in the section "Memory Layout" in Chapter 18, "Embedding Linux."

## **Calibrating Delay...1197.46 BogoMIPS (lpj=2394935)**

During boot, the kernel calculates the number of times the processor can execute an internal delay loop in one *jiffy*, which is the time interval between two consecutive ticks of the system timer. As you would expect, the calculation has to be calibrated to the processing speed of your CPU. The result of this calibration is stored in a kernel variable called loops\_per\_jiffy. One place where the kernel makes use of loops\_per\_jiffy is when a device driver desires to delay execution for small durations in the order of microseconds.

To understand the delay-loop calibration code, let's take a peek inside calibrate\_delay(), defined in *init/calibrate.c.* This function cleverly derives floating-point precision using the integer kernel. The following snippet (with some comments added) shows the initial portion of the function that carves out a coarse value for loops\_per\_jiffy:

```
loops_per_jiffy = (1 << 12); /* Initial approximation = 4096 */
printk(KERN_DEBUG "Calibrating delay loop... ");
while ((loops_per_jiffy <<= 1) != 0) {
ticks = jiffies; /* As you will find out in the section, "Kernel
 Timers," the jiffies variable contains the
 number of timer ticks since the kernel
 started, and is incremented in the timer
 interrupt handler */
 while (ticks == jiffies); /* Wait until the start
 of the next jiffy */
 ticks = jiffies;
 /* Delay */
 __delay(loops_per_jiffy);
 /* Did the wait outlast the current jiffy? Continue if
 it didn't */
 ticks = jiffies - ticks;
 if (ticks) break;
}
loops_per_jiffy >>= 1; /* This fixes the most significant bit and is
 the lower-bound of loops_per_jiffy */
```

The preceding code begins by assuming that loops\_per\_jiffy is greater than 4096, which translates to a processor speed of roughly one *million instructions per second* (MIPS). It then waits for a fresh jiffy to start and executes the delay loop, \_\_delay(loops\_per\_jiffy). If the delay loop outlasts the jiffy, the previous value of loops\_per\_jiffy (obtained by bitwise right-shifting it by one) fixes its *most significant bit* (MSB). Otherwise, the function continues by checking whether it will obtain the MSB by bitwise left-shifting loops\_per\_jiffy. When the kernel thus figures out the MSB of loops\_per\_jiffy, it works on the lower-order bits and fine-tunes its precision as follows:

```
loopbit = loops_per_jiffy;
/* Gradually work on the lower-order bits */
while (lps_precision-- && (loopbit >>= 1)) {
 loops_per_jiffy |= loopbit;
 ticks = jiffies;
 while (ticks == jiffies); /* Wait until the start
 of the next jiffy */
ticks = jiffies;
```

```
 /* Delay */
 __delay(loops_per_jiffy);
 if (jiffies != ticks) /* longer than 1 tick */
 loops_per_jiffy &= ~loopbit;
}
```

The preceding snippet figures out the exact combination of the lower bits of loops\_per\_jiffy when the delay loop crosses a jiffy boundary. This calibrated value is used to derive an unscientific measure of the processor speed, known as *BogoMIPS.* You can use the BogoMIPS rating as a relative measurement of how fast a CPU can run. On a 1.6GHz Pentium M-based laptop, the delay-loop calibration yielded a value of 2394935 for loops\_per\_jiffy as announced by the preceding boot message. The BogoMIPS value is obtained as follows:

BogoMIPS = loops\_per\_jiffy \* Number of jiffies in 1 second \* Number of instructions consumed by the internal delay loop in units of 1 million

```
= (2394935 * HZ * 2) / (1 million)
= (2394935 * 250 * 2) / (1000000)
= 1197.46 (as displayed in the preceding boot message)
```

We further discuss jiffies, HZ, and loops\_per\_jiffy in the section "Kernel Timers" later in this chapter.

## **Checking HLT Instruction**

Because the Linux kernel is supported on a variety of hardware platforms, the boot code checks for architecture-dependent bugs. Verifying the sanity of the *halt* (HLT) instruction is one such check.

The HLT instruction supported by x86 processors puts the CPU into a low-power sleep mode that continues until the next hardware interrupt occurs. The kernel uses the HLT instruction when it wants to put the CPU in the idle state (see function cpu\_idle() defined in *arch/x86/kernel/process\_32.c*).

For problematic CPUs, the no-hlt kernel command-line argument can be used to disable the HLT instruction. If no-hlt is turned on, the kernel busy-waits while it's idle, rather than keep the CPU cool by putting it in the HLT state.

The preceding boot message is generated when the startup code in *init/main.c* calls check\_bugs() defined in *include/asm-your-arch/bugs.h*.

#### **NET: Registered Protocol Family 2**

The Linux socket layer is a uniform interface through which user applications access various networking protocols. Each protocol registers itself with the socket layer using a unique family number (defined in *include/linux/socket.h*) assigned to it. Family 2 in the preceding message stands for AF\_INET (Internet Protocol).

Another registered protocol family often found in boot messages is AF\_NETLINK (Family 16). Netlink sockets offer a method to communicate between user processes and the kernel. Functionalities accomplished via netlink sockets include accessing the routing table and the *Address Resolution Protocol* (ARP) table (see *include/linux/netlink.h* for the full usage list). Netlink sockets are more suitable than system calls to accomplish such tasks because they are asynchronous, simpler to implement, and dynamically linkable.

Another protocol family commonly enabled in the kernel is AF\_UNIX or UNIX-domain sockets. Programs such as X Windows use them for interprocess communication on the same system.

### **Freeing Initrd Memory: 387k Freed**

*Initrd* is a memory-resident virtual disk image loaded by the bootloader. It's mounted as the initial root filesystem after the kernel boots, to hold additional dynamically loadable modules required to mount the disk partition that holds the actual root filesystem. Because the kernel runs on different hardware platforms that use diverse storage controllers, it's not feasible for distributions to enable device drivers for all possible disk drives in the base kernel image. Drivers specific to your system's storage device are packed inside initrd and loaded after the kernel boots, but before the root filesystem is mounted. To create an initrd image, use the mkinitrd command.

The 2.6 kernel includes a feature called *initramfs* that bring several benefits over initrd. Whereas the latter emulates a disk (hence called *initramdisk* or *initrd*) and suffers the overheads associated with the Linux block I/O subsystem such as caching, the former essentially gets the cache itself mounted like a filesystem (hence called *initramfs*).

Initramfs, like the page cache over which it's built, grows and shrinks dynamically unlike initrd and, hence, reduces memory wastage. Also, unlike initrd, which requires you to include the associated filesystem driver (e.g., EXT2 drivers if you have an EXT2 filesystem on your initrd), initramfs needs no filesystem support. The initramfs code is tiny because it's just a small layer on top of the page cache.

You can pack your initial root filesystem into a compressed *cpio* archive[2] and pass it to the kernel command line using the initrd= argument or build it as part of the kernel image using the INITRAMFS\_SOURCE menu option during kernel configuration. With the latter, you may either provide the filename of a cpio archive or the path name to a directory tree containing your initramfs layout. During boot, the kernel extracts the files into an initramfs root filesystem (also called *rootfs*) and executes a top-level */init* program if it finds one. This method of obtaining an initial rootfs is especially useful for embedded platforms, where all system resources are at a premium. To create an initramfs image, use mkinitramfs. Look at *Documentation/filesystems/ramfs-rootfsinitramfs.txt* for more documentation.

[2] cpio is a UNIX file archival format. You can download it from www.gnu.org/software/cpio.

In this case, we are using initramfs by supplying a compressed cpio archive of the initial root filesystem to the kernel using the initrd= command-line argument. After unpacking the contents of the archive into rootfs, the kernel frees the memory where the archive resides (387K in this case) and announces the above boot message. The freed pages are then doled out to other parts of the kernel that request memory.

As discussed in Chapter 18, initrd and initramfs are sometimes used to hold the actual root filesystem on embedded devices during development.

### **IO Scheduler Anticipatory Registered (Default)**

The main goal of an I/O scheduler is to increase system throughput by minimizing disk seek times, which is the latency to move the disk head from its existing position to the disk sector of interest. The 2.6 kernel provides four different I/O schedulers: *Deadline, Anticipatory, Complete Fair Queuing*, and *Noop.* As the preceding kernel message indicates, the kernel sets *Anticipatory* as the default I/O scheduler. We look at I/O scheduling in Chapter 14, "Block Drivers."

#### **Setting Up Standard PCI Resources**

The next phase of the boot process probes and initializes I/O buses and peripheral controllers. The kernel probes PCI hardware by walking the PCI bus, and then initializes other I/O subsystems. Take a look at the boot messages in Figure 2.3 to see announcements regarding the initialization of the SCSI subsystem, the USB controller, the video chip (part of the 855 North Bridge chipset in the messages below), the serial port (8250 UART in this case), PS/2 keyboard and mouse, floppy drives, ramdisk, the loopback device, the IDE controller

(part of the ICH4 South Bridge chipset in this example), the touchpad, the Ethernet controller (e1000 in this case), and the PCMCIA controller. The identity of the corresponding I/O device is labeled against .

**Figure 2.3. Initializing buses and peripheral controllers during boot.**

```

SCSI subsystem initialized                        -> SCSI
usbcore: registered new driver hub                -> USB
agpgart: Detected an Intel 855 Chipset.           -> Video
[drm] Initialized drm 1.0.0 20040925
PS/2 Controller [PNP0303:KBD, PNP0f13:MOU]
  at 0x60,0x64 irq 1,12 serio: i8042 KBD port     -> Keyboard
serial8250: ttyS0 at I/O 0x3f8 (irq = 4)
  is a NS16550A                                   -> Serial Port
Floppy drive(s): fd0 is 1.44M                     -> Floppy
RAMDISK driver initialized: 16 RAM disks
  of 4096K size 1024 blocksize                    -> Ramdisk
loop: loaded (max 8 devices)                      -> Loop back
ICH4: IDE controller at PCI slot 0000:00:1f.1     -> Hard Disk
...

input: SynPS/2 Synaptics TouchPad as
  /class/input/input1                             -> Touchpad
e1000: eth0: e1000_probe: Intel PRO/1000
  Network Connection                              -> Ethernet
Yenta: CardBus bridge found at
  0000:02:00.0 [1014:0560]                        -> PCMCIA/CardBus
...
```

This book discusses many of these driver subsystems in separate chapters. Note that some of these messages might manifest only later on in the boot process if the drivers are dynamically linked to the kernel as loadable modules.

#### **EXT3-fs: Mounted Filesystem**

The EXT3 filesystem has become the de facto filesystem on Linux. It adds a journaling layer on top of the veteran EXT2 filesystem to facilitate quick recovery after a crash. The aim is to regain a consistent filesystem state without having to go through a time-consuming filesystem check (fsck) operation. EXT2 remains the work engine, while the EXT3 layer additionally logs file transactions on a memory area called *journal* before committing the actual changes to disk. EXT3 is backward-compatible with EXT2, so you can add an EXT3 coating to your existing EXT2 filesystem or peel off the EXT3 to get back your original EXT2 filesystem.

### EXT4

The latest version in the EXT filesystem series is EXT4, which has been included in the mainline kernel starting with the 2.6.19 release, with a tag of "experimental" and a name of ext4dev. EXT4 is largely backward-compatible with EXT3. The home page of the EXT4 project is at www.bullopensource.org/ext4.

EXT3 starts a kernel helper thread (we will have an in-depth discussion on kernel threads in the next chapter)

called *kjournald* to assist in journaling. When EXT3 is operational, the kernel mounts the root filesystem and gets ready for business:

```
EXT3-fs: mounted filesystem with ordered data mode
kjournald starting. Commit interval 5 seconds
VFS: Mounted root (ext3 filesystem).
INIT: Version 2.85 Booting
```

*Init*, the parent of all Linux processes, is the first program to run after the kernel finishes its boot sequence. In the last few lines of *init/main.c*, the kernel searches different locations in its attempt to locate init:

```
if (ramdisk_execute_command) { /* Look for /init in initramfs */
 run_init_process(ramdisk_execute_command);
}
if (execute_command) { /* You may override init and ask the kernel
 to execute a custom program using the
 "init=" kernel command-line argument. If
 you do that, execute_command points to the
 specified program */
 run_init_process(execute_command);
}
/* Else search for init or sh in the usual places .. */
run_init_process("/sbin/init");
run_init_process("/etc/init");
run_init_process("/bin/init");
run_init_process("/bin/sh");
panic("No init found. Try passing init= option to kernel.");
```

Init receives directions from */etc/inittab*. It first executes system initialization scripts present in */etc/rc.sysinit.* One of the important responsibilities of this script is to activate the swap partition, which triggers a boot message such as this:

```
Adding 1552384k swap on /dev/hda6
```

Let's take a closer look at what this means. Linux user processes own a virtual address space of 3GB (see the section "Allocating Memory"). Out of this, the pages constituting the "working set" are kept in RAM. However, when there are too many programs demanding memory resources, the kernel frees up some used RAM pages by storing them in a disk partition called *swap space.* According to a rule of thumb, the size of the swap partition should be twice the amount of RAM. In this case, the swap space lives in the disk partition */dev/hda6* and has a size of 1552384K bytes.

Init then goes on to run scripts present in the */etc/rc.d/rcX.d/* directory, where *X* is the runlevel specified in inittab. A runlevel is an execution state corresponding to the desired boot mode. For example, multiuser text mode corresponds to a runlevel of 3, while X Windows associates with a runlevel of 5. So, if you see the message, INIT: Entering runlevel 3, init has started executing scripts in the */etc/rc.d/rc3.d/* directory. These scripts start the dynamic device-naming subsystem udev (which we discuss in Chapter 4, "Laying the Groundwork") and load kernel modules responsible for driving networking, audio, storage, and so on:

```
Starting udev: [ OK ]
Initializing hardware... network audio storage [Done]
...
```

Init finally spawns terminals on virtual consoles. You can now log in.

# **Kernel Mode and User Mode**

Some operating systems, such as MS-DOS, always execute in a single CPU mode, but UNIX-like operating systems use dual modes to effectively implement time-sharing. On a Linux machine, the CPU is either in a trusted *kernel mode* or in a restricted *user mode.* All user processes execute in user mode, whereas the kernel itself executes in kernel mode.

Kernel mode code has unrestricted access to the entire processor instruction set and to the full memory and I/O space. If a user mode process needs these privileges, it has to channel requests through device drivers or other kernel mode code via system calls. User mode code is allowed to page fault, however, whereas kernel mode code isn't.

In 2.4 and earlier kernels, only user mode processes could be context switched out and replaced by other processes. Kernel mode code could monopolize the processor until either

It voluntarily relinquished the CPU.

or

An interrupt or an exception occurred.

With the introduction of kernel preemption in the 2.6 release, most kernel mode code can also be context switched.

# **Process Context and Interrupt Context**

The kernel accomplishes useful work using a combination of process contexts and interrupt contexts. Kernel code that services system calls issued by user applications runs on behalf of the corresponding application processes and is said to execute in process context. Interrupt handlers, on the other hand, run asynchronously in interrupt context. Processes contexts are not tied to any interrupt context and vice versa.

Kernel code running in process context is preemptible. An interrupt context, however, always runs to completion and is not preemptible. Because of this, there are restrictions on what can be done from interrupt context. Code executing from interrupt context cannot do the following:

- Go to sleep or relinquish the processor
- Acquire a mutex
- Perform time-consuming tasks
- Access user space virtual memory

Look at section "Interrupt Handing" in Chapter 4 for a full discussion of the interrupt context.

# **Kernel Timers**

The working of many parts of the kernel is critically dependent on the passage of time. The Linux kernel makes use of different timers provided by the hardware to provide time-dependent services such as busy-waiting and sleep-waiting. The processor wastes cycles while it busy-waits but relinquishes the CPU when it sleep-waits. Naturally, the former is done only when the latter is not feasible. The kernel also facilitates scheduling of functions that desire to run after a specified time duration has elapsed.

Let's first discuss the semantics of some important kernel timer variables such as jiffies, HZ, and xtime. Next, let's measure execution times on a Pentium-based system using the Pentium *Time Stamp Counter* (TSC). Let's also see how Linux uses the *Real Time Clock* (RTC).

### **HZ and Jiffies**

System timers interrupt the processor (or "pop") at programmable frequencies. This frequency, or the number of timer ticks per second, is contained in the kernel variable HZ. Choosing a value for HZ is a trade-off. A large HZ results in finer timer granularity, and hence better scheduling resolution. However, bigger values of HZ also result in larger overhead and higher power consumption, because more cycles are burnt in the timer interrupt context.

The value of HZ is architecture-dependent. On x86 systems, HZ used to be set to 100 in 2.4 kernels by default. With 2.6, this value changed to 1000, but with 2.6.13, it was lowered to 250. On ARM-based platforms, 2.6 kernels set HZ to 100. With current kernels, you can choose a value for HZ at build time through the configuration menu. The default setting for this option depends on your distribution.

The 2.6.21 kernel introduced support for a tickless kernel (CONFIG\_NO\_HZ), which dynamically triggers timer interrupts depending on system load. The tickless system implementation is outside the scope of this chapter.

jiffies holds the number of times the system timer has popped since the system booted. The kernel increments the jiffies variable, HZ times every second. Thus, on a kernel with a HZ value of 100, a jiffy is a 10-millisecond duration, whereas on a kernel with HZ set to 1000, a jiffy is only 1-millisecond long.

To better understand HZ and jiffies, consider the following code snippet from the IDE driver (*drivers/ide/ide.c*) that polls disk drives for busy status:

```
unsigned long timeout = jiffies + (3*HZ);
while (hwgroup->busy) {
 /* ... */
 if (time_after(jiffies, timeout)) {
 return -EBUSY;
 }
 /* ... */
}
return SUCCESS;
```

The preceding code returns SUCCESS if the busy condition gets cleared in less than 3 seconds, and -EBUSY otherwise. 3\*HZ is the number of jiffies present in 3 seconds. The calculated timeout, (jiffies + 3\*HZ), is thus the new value of jiffies after the 3-second timeout elapses. The time\_after() macro compares the current value of jiffies with the requested timeout, taking care to account for wraparound due to overflows. Related functions available for doing similar comparisons are time\_before(), time\_before\_eq(), and time\_after\_eq().

jiffies is defined as *volatile*, which asks the compiler not to optimize access to the variable. This ensures that jiffies, which is updated by the timer interrupt handler during each tick, is reread during each pass through the loop.

For the reverse conversion from jiffies to seconds, take a look at this snippet from the USB host controller driver, *drivers/usb/host/ehci-sched.c*:

```
if (stream->rescheduled) {
 ehci_info(ehci, "ep%ds-iso rescheduled " "%lu times in %lu
 seconds\n", stream->bEndpointAddress, is_in? "in":
 "out", stream->rescheduled,
 ((jiffies – stream->start)/HZ));
}
```

The preceding debug statement calculates the amount of time in seconds within which this USB endpoint stream (we discuss USB in Chapter 11, "Universal Serial Bus") was rescheduled stream->rescheduled times. (jiffies-stream->start) is the number of jiffies that elapsed since the rescheduling started. The division by HZ converts that value into seconds.

The 32-bit jiffies variable overflows in approximately 50 days, assuming a HZ value of 1000. Because system uptimes can be many times that duration, the kernel provides a variable called jiffies\_64 to hold 64-bit (u64) jiffies. The linker positions jiffies\_64 such that its bottom 32 bits collocate with jiffies. On 32-bit machines, the compiler needs two instructions to assign one u64 variable to another, so reading jiffies\_64 is not atomic. To get around this problem, the kernel provides a function, get\_jiffies\_64(). Look at cpufreq\_stats\_update() defined in *drivers/cpufreq/cpufreq\_stats.c* for a usage example.

#### **Long Delays**

In kernel terms, delays in the order of jiffies are considered long durations. A possible, but nonoptimal, way to accomplish long delays is by busy-looping. A function that busy-waits has a dog-in-the-manger attitude. It neither uses the processor for doing useful work nor lets others use it. The following code hogs the processor for 1 second:

```
unsigned long timeout = jiffies + HZ;
while (time_before(jiffies, timeout)) continue;
```

A better approach is to sleep-wait, instead of busy-wait. Your code yields the processor to others, while waiting for the time delay to elapse. This is done using schedule\_timeout():

```
unsigned long timeout = jiffies + HZ;
schedule_timeout(timeout); /* Allow other parts of the
 kernel to run */
```

The delay guarantee is only on the lower bound of the timeout. Whether from kernel space or from user space, it's difficult to get more precise control over timeouts than the granularity of HZ because process time slices are updated by the kernel scheduler only during timer ticks. Also, even if your process is scheduled to run after the

specified timeout, the scheduler can decide to pick another process from the run queue based on priorities.[3]

[3] These scheduler properties have changed with the advent of the CFS scheduler in the 2.6.23 kernel. Linux process schedulers are discussed in Chapter 19, "Drivers in User Space."

Two other functions that facilitate sleep-waiting are wait\_event\_timeout() and msleep(). Both of them are implemented with the help of schedule\_timeout(). wait\_event\_timeout() is used when your code desires to resume execution if a specified condition becomes true or if a timeout occurs. msleep() sleeps for the specified number of milliseconds.

Such long-delay techniques are suitable for use only from process context. Sleep-waiting cannot be done from interrupt context because interrupt handlers are not allowed to schedule() or sleep. (See "Interrupt Handling" in Chapter 4 for a list of do's and don'ts for code executing in interrupt context.) Busy-waiting for a short duration is possible from interrupt context, but long busy-waiting in that context is considered a mortal sin. Equally taboo is long busy-waiting with interrupts disabled.

The kernel also provides timer APIs to execute a function at a point of time in the future. You can dynamically define a timer using init\_timer() or statically create one with DEFINE\_TIMER(). After this is done, populate a timer\_list with the address and parameters of your handler function, and register it using add\_timer():

```
#include <linux/timer.h>
struct timer_list my_timer;
init_timer(&my_timer); /* Also see setup_timer() */
my_timer.expire = jiffies + n*HZ; /* n is the timeout in number
 of seconds */
my_timer.function = timer_func; /* Function to execute
 after n seconds */
my_timer.data = func_parameter; /* Parameter to be passed
 to timer_func */
add_timer(&my_timer); /* Start the timer */
```

Note that this is a one-shot timer. If you want to run timer\_func() periodically, you also need to add the preceding code inside timer\_func() to schedule itself after the next timeout:

```
static void timer_func(unsigned long func_parameter)
{
 /* Do work to be done periodically */
 /* ... */
 init_timer(&my_timer);
 my_timer.expire = jiffies + n*HZ;
 my_timer.data = func_parameter;
 my_timer.function = timer_func;
 add_timer(&my_timer);
}
```

You may use mod\_timer() to change the expiration of my\_timer, del\_timer() to cancel my\_timer, and timer\_pending() to see whether my\_timer is pending at the moment. If you look at *kernel/timer.c*, you will find that schedule\_timeout() internally uses these same APIs.

User-space functions such as clock\_settime() and clock\_gettime() are used to access kernel timer services from user space. A user application may use setitimer() and getitimer() to control the delivery of an alarm signal when a specified timeout expires.

### **Short Delays**

In kernel terms, sub-jiffy delays qualify as short durations. Such delays are commonly requested from both process and interrupt contexts. Because it is not possible to use jiffy-based methods to implement sub-jiffy delays, the methods discussed in the previous section to sleep-wait cannot be used for small timeouts. The only solution is to busy-wait.

Kernel APIs that implement short delays are mdelay(), udelay(), and ndelay(), which support millisecond, microsecond, and nanosecond delays, respectively. The actual implementations of these functions are architecture-specific and may not be available on all platforms.

Busy-waiting for short durations is accomplished by measuring the time the processor takes to execute an instruction and looping for the necessary number of iterations. As discussed earlier in this chapter, the kernel performs this measurement during boot and stores the value in a variable called loops\_per\_jiffy. The shortdelay APIs use loops\_per\_jiffy to decide the number of times they need to busy-loop. To achieve a 1 microsecond delay during a handshake process, the USB host controller driver, *drivers/usb/host/ehci-hcd.c*, calls udelay(), which internally uses loops\_per\_jiffy:

```
do {
 result = ehci_readl(ehci, ptr);
 /* ... */
 if (result == done) return 0;
 udelay(1); /* Internally uses loops_per_jiffy */
 usec--;
} while (usec > 0);
```

### **Pentium Time Stamp Counter**

The *Time Stamp Counter* (TSC) is a 64-bit register present in Pentium-compatible processors that counts the number of clock cycles consumed by the processor since startup. Because the TSC gets incremented at the rate of the processor cycle speed, it provides a high-resolution timer. The TSC is commonly used for profiling and instrumenting code. It is accessed using the rdtsc instruction to measure execution time of intervening code with microsecond precision. TSC ticks can be converted to seconds by dividing by the CPU clock speed, which can be read from the kernel variable, cpu\_khz.

In the following snippet, low\_tsc\_ticks contains the lower 32 bits of the TSC, while high\_tsc\_ticks contains the higher 32 bits. The lower 32 bits overflow in a few seconds depending on your processor speed but is sufficient for many code instrumentation purposes as shown here:

```
unsigned long low_tsc_ticks0, high_tsc_ticks0;
unsigned long low_tsc_ticks1, high_tsc_ticks1;
unsigned long exec_time;
rdtsc(low_tsc_ticks0, high_tsc_ticks0); /* Timestamp
 before */
printk("Hello World\n"); /* Code to be
 profiled */
rdtsc(low_tsc_ticks1, high_tsc_ticks1); /* Timestamp after */
exec_time = low_tsc_ticks1 - low_tsc_ticks0;
```

exec\_time measured 871 (or half a microsecond) on a 1.8GHz Pentium box.

Support for high-resolution timers (CONFIG\_HIGH\_RES\_TIMERS) has been merged with the 2.6.21 kernel. It makes use of hardware-specific high-speed timers to provide high-precision capabilities to APIs such as nanosleep(). On Pentium-class machines, the kernel leverages the TSC to offer this capability.

## **Real Time Clock**

The RTC tracks absolute time in nonvolatile memory. On x86 PCs, RTC registers constitute the top few locations of a small chunk of battery-powered[4] *complementary metal oxide semiconductor* (CMOS) memory. Look at Figure 5.1 in Chapter 5, "Character Drivers," for the location of the CMOS in the legacy PC architecture. On embedded systems, the RTC might be internal to the processor, or externally connected via the I2C or SPI buses discussed in Chapter 8, "The Inter-Integrated Circuit Protocol."

[4] RTC batteries last for many years and usually outlive the life span of computers, so you should never have to replace them.

You may use the RTC to do the following:

- Read and set the absolute clock, and generate interrupts during clock updates.
- Generate periodic interrupts with frequencies ranging from 2HZ to 8192HZ.
- Set alarms

Many applications need the concept of absolute time or *wall time.* Because jiffies is relative to the time when the system booted, it does not contain wall time. The kernel maintains wall time in a variable called xtime. During boot, xtime is initialized to the current wall time by reading the RTC. When the system halts, the wall time is written back to the RTC. You can use do\_gettimeofday() to read wall time with the highest resolution supported by the hardware:

```
#include <linux/time.h>
static struct timeval curr_time;
do_gettimeofday(&curr_time);
my_timestamp = cpu_to_le32(curr_time.tv_sec); /* Record timestamp */
```

There are also a bunch of functions available to user-space code to access wall time. They include the following:

- time(), which returns the calendar time, or the number of seconds since Epoch (00:00:00 on January 1, 1970)
- localtime(), which returns the calendar time in broken-down format
- mktime(), which does the reverse of localtime()

gettimeofday(), which returns the calendar time with microsecond precision if your platform supports it

Another way to use the RTC from user space is via the character device, */dev/rtc.* Only one process is allowed to access this device at a time.

We discuss more about RTC drivers in Chapter 5 and Chapter 8. In Chapter 19, we develop an example user application that uses */dev/rtc* to perform periodic work with microsecond precision.

# **Concurrency in the Kernel**

With the arrival of multicore laptops, *Symmetric Multi Processing* (SMP) is no longer confined to the realm of hitech users. SMP and kernel preemption are scenarios that generate multiple threads of execution. These threads can simultaneously operate on shared kernel data structures. Because of this, accesses to such data structures have to be serialized.

Let's discuss the basics of protecting shared kernel resources from concurrent access. We start with a simple example and gradually introduce complexities such as interrupts, kernel preemption, and SMP.

#### **Spinlocks and Mutexes**

A code area that accesses shared resources is called a *critical* section. Spinlocks and mutexes (short for *mutual exclusion*) are the two basic mechanisms used to protect critical sections in the kernel. Let's look at each in turn.

A spinlock ensures that only a single thread enters a critical section at a time. Any other thread that desires to enter the critical section has to remain spinning at the door until the first thread exits. Note that we use the term *thread* to refer to a thread of execution, rather than a kernel thread.

The basic usage of spinlocks is as follows:

```
#include <linux/spinlock.h>
spinlock_t mylock = SPIN_LOCK_UNLOCKED; /* Initialize */
/* Acquire the spinlock. This is inexpensive if there
 * is no one inside the critical section. In the face of
 * contention, spinlock() has to busy-wait.
 */
spin_lock(&mylock);
/* ... Critical Section code ... */
spin_unlock(&mylock); /* Release the lock */
```

In contrast to spinlocks that put threads into a spin if they attempt to enter a busy critical section, mutexes put contending threads to sleep until it's their turn to occupy the critical section. Because it's a bad thing to consume processor cycles to spin, mutexes are more suitable than spinlocks to protect critical sections when the estimated wait time is long. In mutex terms, anything more than two context switches is considered long, because a mutex has to switch out the contending thread to sleep, and switch it back in when it's time to wake it up.

In many cases, therefore, it's easy to decide whether to use a spinlock or a mutex:

- If the critical section needs to sleep, you have no choice but to use a mutex. It's illegal to schedule, preempt, or sleep on a wait queue after acquiring a spinlock.
- Because mutexes put the calling thread to sleep in the face of contention, you have no choice but to use spinlocks inside interrupt handlers. (You will learn more about the constraints of the interrupt context in Chapter 4.)

Basic mutex usage is as follows:

```
#include <linux/mutex.h>
/* Statically declare a mutex. To dynamically
 create a mutex, use mutex_init() */
static DEFINE_MUTEX(mymutex);
/* Acquire the mutex. This is inexpensive if there
 * is no one inside the critical section. In the face of
 * contention, mutex_lock() puts the calling thread to sleep.
 */
mutex_lock(&mymutex);
/* ... Critical Section code ... */
mutex_unlock(&mymutex); /* Release the mutex */
```

To illustrate the use of concurrency protection, let's start with a critical section that is present only in process context and gradually introduce complexities in the following order:

- Critical section present only in process context on a Uniprocessor (UP) box running a nonpreemptible kernel. **1.**
- Critical section present in process and interrupt contexts on a UP machine running a nonpreemptible kernel. **2.**
- **3.** Critical section present in process and interrupt contexts on a UP machine running a preemptible kernel.
- Critical section present in process and interrupt contexts on an SMP machine running a preemptible kernel. **4.**

### The Old Semaphore Interface

The mutex interface, which replaces the older semaphore interface, originated in the –rt tree and was merged into the mainline with the 2.6.16 kernel release. The semaphore interface is still around, however. Basic usage of the semaphore interface is as follows:

```
#include <asm/semaphore.h> /* Architecture dependent
 header */
/* Statically declare a semaphore. To dynamically
 create a semaphore, use init_MUTEX() */
static DECLARE_MUTEX(mysem);
down(&mysem); /* Acquire the semaphore */
/* ... Critical Section code ... */
up(&mysem); /* Release the semaphore */
```

Semaphores can be configured to allow a predetermined number of threads into the critical section simultaneously. However, semaphores that permit more than a single holder at a time are rarely used.

#### **Case 1: Process Context, UP Machine, No Preemption**

This is the simplest case and needs no locking, so we won't discuss this further.

#### **Case 2: Process and Interrupt Contexts, UP Machine, No Preemption**

In this case, you need to disable only interrupts to protect the critical region. To see why, assume that A and B are process context threads, and C is an interrupt context thread, all vying to enter the same critical section, as shown in Figure 2.4.

**Figure 2.4. Process and interrupt context threads inside a critical section.**

```
                 Process Context                  Process Context                Interrupt Context
                    Thread A                         Thread B                        Thread C
                     (A)                             (B)                             (C)
                      |                               |                               |
                      |                               |                               |
                      v                               v                               v
                    +---+                           +---+                            +---+
                    | A |                           | B |                            | C |
                    +---+                           +---+                            +---+
                      |                               |                               |  ^
                      |                               |                               |  |
                      |-------------------------------.-------------------------------|  |
                                      \               |              /                   |
                                       \  ____________|             /                    |
                           _____________v v                         v                    |
                          |        +-----|---------------------------------+             |
                          |        |     |       CRITICAL SECTION          |             |
                          |        +-----|---------------------------------+             |
                          |             / \                                              |
                          |     Thread A   Thread B                                      |
    Disable Interrupts <--|                                                              |     
                                                                                         |
              Restore Interrupt State <--------------------------------------------------|
```

Because Thread C is executing in interrupt context and always runs to completion before yielding to Thread A or Thread B, it need not worry about protection. Thread A, for its part, need not be concerned about Thread B (and vice versa) because the kernel is not preemptible. Thus, Thread A and Thread B need to guard against only the possibility of Thread C stomping through the critical section while they are inside the same section. They achieve this by disabling interrupts prior to entering the critical section:

```
Point A:
 local_irq_disable(); /* Disable Interrupts in local CPU */
 /* ... Critical Section ... */
 local_irq_enable(); /* Enable Interrupts in local CPU */
```

However, if interrupts were already disabled when execution reached Point A, local\_irq\_enable() creates the unpleasant side effect of reenabling interrupts, rather than restoring interrupt state. This can be fixed as follows:

```
unsigned long flags;
Point A:
 local_irq_save(flags); /* Disable Interrupts */
 /* ... Critical Section ... */
 local_irq_restore(flags); /* Restore state to what
 it was at Point A */
```

This works correctly irrespective of the interrupt state at Point A.

**Case 3: Process and Interrupt Contexts, UP Machine, Preemption**

If preemption is enabled, mere disabling of interrupts won't protect your critical region from being trampled over. There is the possibility of multiple threads simultaneously entering the critical section in process context. Referring back to Figure 2.4 in this scenario, Thread A and Thread B now need to protect themselves against each other in addition to guarding against Thread C. The solution apparently, is to disable kernel preemption before the start of the critical section and reenable it at the end, in addition to disabling/reenabling interrupts.

For this, Thread A and Thread B use the *irq* variant of spinlocks:

```
unsigned long flags;
Point A:
 /* Save interrupt state.
 * Disable interrupts - this implicitly disables preemption */
 spin_lock_irqsave(&mylock, flags);
 /* ... Critical Section ... */
 /* Restore interrupt state to what it was at Point A */
 spin_unlock_irqrestore(&mylock, flags);
```

Preemption state need not be explicitly restored to what it was at Point A because the kernel internally does that for you via a variable called the *preemption counter.* The counter gets incremented whenever preemption is disabled (using preempt\_disable()) and gets decremented whenever preemption is enabled (using preempt\_enable()). Preemption kicks in only if the counter value is zero.

#### **Case 4: Process and Interrupt Contexts, SMP Machine, Preemption**

Let's now assume that the critical section executes on an SMP machine. Your kernel has been configured with CONFIG\_SMP and CONFIG\_PREEMPT turned on.

In the scenarios discussed this far, spinlock primitives have done little more than enable/disable preemption and interrupts. The actual locking functionality has been compiled away. In the presence of SMP, the locking logic gets compiled in, and the spinlock primitives are rendered SMP-safe. The SMP-enabled semantics is as follows:

```
unsigned long flags;
Point A:
 /*
 - Save interrupt state on the local CPU
 - Disable interrupts on the local CPU. This implicitly disables
 preemption.
 - Lock the section to regulate access by other CPUs
 */
 spin_lock_irqsave(&mylock, flags);
 /* ... Critical Section ... */
 /*
 - Restore interrupt state and preemption to what it
 was at Point A for the local CPU
 - Release the lock
 */
 spin_unlock_irqrestore(&mylock, flags);
```

On SMP systems, only interrupts on the local CPU are disabled when a spinlock is acquired. So, a process context thread (say Thread A in Figure 2.4) might be running on one CPU, while an interrupt handler (say Thread C in Figure 2.4) is executing on another CPU. An interrupt handler on a nonlocal processor thus needs to spin-wait until the process context code on the local processor exits the critical section. The interrupt context code calls spin\_lock()/spin\_unlock() to do this:

```
spin_lock(&mylock);
/* ... Critical Section ... */
spin_unlock(&mylock);
```

Similar to the irq variants, spinlocks also have *bottom half* (BH) flavors. spin\_lock\_bh() disables bottom halves when the lock is acquired, whereas spin\_unlock\_bh() reenables bottom halves when the lock is released. We discuss bottom halves in Chapter 4.

```
The –rt tree
```

The real time (-rt) tree, also called the CONFIG\_PREEMPT\_RT patch-set, implements low-latency modifications to the kernel. The patch-set, downloadable from www.kernel.org/pub/linux/kernel/projects/rt, allows most of the kernel to be preempted, partly by replacing many spinlocks with mutexes. It also incorporates high-resolution timers. Several -rt features have been integrated into the mainline kernel. You will find detailed documentation at the project's wiki hosted at [http://rt.wiki.kernel.org/.](http://rt.wiki.kernel.org/)

The kernel has specialized locking primitives in its repertoire that help improve performance under specific conditions. Using a mutual-exclusion scheme tailored to your needs makes your code more powerful. Let's take a look at some of the specialized exclusion mechanisms.

## **Atomic Operators**

*Atomic operators* are used to perform lightweight one-shot operations such as bumping counters, conditional increments, and setting bit positions. Atomic operations are guaranteed to be serialized and do not need locks for protection against concurrent access. The implementation of atomic operators is architecture-dependent.

To check whether there are any remaining data references before freeing a kernel network buffer (called an skbuff), the skb\_release\_data() routine defined in *net/core/skbuff.c* does the following:

```
if (!skb->cloned ||
 /* Atomically decrement and check if the returned value is zero */
 !atomic_sub_return(skb->nohdr ? (1 << SKB_DATAREF_SHIFT) + 1 :
 1,&skb_shinfo(skb)->dataref)) {
 /* ... */
 kfree(skb->head);
}
```

While skb\_release\_data() is thus executing, another thread using skbuff\_clone() (defined in the same file) might be simultaneously incrementing the data reference counter:

```
/* ... */
/* Atomically bump up the data reference count */
atomic_inc(&(skb_shinfo(skb)->dataref));
/* ... */
```

The use of atomic operators protects the data reference counter from being trampled by these two threads. It

also eliminates the hassle of using locks to protect a single integer variable from concurrent access.

The kernel also supports operators such as set\_bit(), clear\_bit(), and test\_and\_set\_bit() to atomically engage in bit manipulations. Look at *include/asm-your-arch/atomic.h* for the atomic operators supported on your architecture.

### **Reader-Writer Locks**

Another specialized concurrency regulation mechanism is a reader-writer variant of spinlocks. If the usage of a critical section is such that separate threads either read from or write to a shared data structure, but don't do both, these locks are a natural fit. Multiple reader threads are allowed inside a critical region simultaneously. Reader spinlocks are defined as follows:

```
rwlock_t myrwlock = RW_LOCK_UNLOCKED;
read_lock(&myrwlock); /* Acquire reader lock */
/* ... Critical Region ... */
read_unlock(&myrwlock); /* Release lock */
```

However, if a writer thread enters a critical section, other reader or writer threads are not allowed inside. To use writer spinlocks, you would write this:

```
rwlock_t myrwlock = RW_LOCK_UNLOCKED;
write_lock(&myrwlock); /* Acquire writer lock */
/* ... Critical Region ... */
write_unlock(&myrwlock); /* Release lock */
```

Look at the IPX routing code present in *net/ipx/ipx\_route.c* for a real-life example of a reader-writer spinlock. A reader-writer lock called ipx\_routes\_lock protects the IPX routing table from simultaneous access. Threads that need to look up the routing table to forward packets request reader locks. Threads that need to add or delete entries from the routing table acquire writer locks. This improves performance because there are usually far more instances of routing table lookups than routing table updates.

Like regular spinlocks, reader-writer locks also have corresponding irq variants—namely, read\_lock\_irqsave(), read\_lock\_irqrestore(), write\_lock\_irqsave(), and write\_lock\_irqrestore(). The semantics of these functions are similar to those of regular spinlocks.

Sequence locks or *seqlocks*, introduced in the 2.6 kernel, are reader-writer locks where writers are favored over readers. This is useful if write operations on a variable far outnumber read accesses. An example is the jiffies\_64 variable discussed earlier in this chapter. Writer threads do not wait for readers who may be inside a critical section. Because of this, reader threads may discover that their entry inside a critical section has failed and may need to retry:

```
u64 get_jiffies_64(void) /* Defined in kernel/time.c */
{
 unsigned long seq;
 u64 ret;
 do {
 seq = read_seqbegin(&xtime_lock);
 ret = jiffies_64;
 } while (read_seqretry(&xtime_lock, seq));
 return ret;
}
```

Writers protect critical regions using write\_seqlock() and write\_sequnlock().

The 2.6 kernel introduced another mechanism called *Read-Copy Update* (RCU), which yields improved performance when readers far outnumber writers. The basic idea is that reader threads can execute without locking. Writer threads are more complex. They perform update operations on a copy of the data structure and replace the pointer that readers see. The original copy is maintained until the next context switch on all CPUs to ensure completion of all ongoing read operations. Be aware that using RCU is more involved than using the primitives discussed thus far and should be used only if you are sure that it's the right tool for the job. RCU data structures and interface functions are defined in *include/linux/rcupdate.h.* There is ample documentation in *Documentation/RCU/\**.

For an RCU usage example, look at *fs/dcache.c.* On Linux, each file is associated with directory entry information (stored in a structure called *dentry*), metadata information (stored in an inode), and actual data (stored in data blocks). Each time you operate on a file, the components in the file path are parsed, and the corresponding dentries are obtained. The dentries are kept cached in a data structure called the *dcache*, to speed up future operations. At any time, the number of dcache lookups is much more than dcache updates, so references to the dcache are protected using RCU primitives.

### **Debugging**

Concurrency-related problems are generally hard to debug because they are usually difficult to reproduce. It's a good idea to enable SMP (CONFIG\_SMP) and preemption (CONFIG\_PREEMPT) while compiling and testing your code, even if your production kernel is going to run on a UP machine with preemption disabled. There is a kernel configuration option under *Kernel hacking* called *Spinlock and rw-lock debugging* (CONFIG\_DEBUG\_SPINLOCK) that can help you catch some common spinlock errors. Also available are tools such as lockmeter [\(http://oss.sgi.com/projects/lockmeter/](http://oss.sgi.com/projects/lockmeter/)) that collect lock-related statistics.

A common concurrency problem occurs when you forget to lock an access to a shared resource. This results in different threads "racing" through that access in an unregulated manner. The problem, called a *race condition*, might manifest in the form of occasional strange code behavior.

Another potential problem arises when you miss releasing held locks in certain code paths, resulting in deadlocks. To understand this, consider the following example:

```
spin_lock(&mylock); /* Acquire lock */
/* ... Critical Section ... */
if (error) { /* This error condition occurs rarely */
 return -EIO; /* Forgot to release the lock! */
}
spin_unlock(&mylock); /* Release lock */
```

After the occurrence of the error condition, any thread trying to acquire mylock gets deadlocked, and the kernel might freeze.

If the problem first manifests months or years after you write the code, it'll be all the more tough to go back and debug it. (There is a related debugging example in the section "Kdump" in Chapter 21, "Debugging Device Drivers.") To avoid such unpleasant encounters, concurrency logic should be designed when you architect your software.

# **Process Filesystem**

The process filesystem (*procfs*) is a virtual filesystem that creates windows into the innards of the kernel. The data you see when you browse procfs is generated by the kernel on-the-fly. Files in procfs are used to configure kernel parameters, look at kernel structures, glean statistics from device drivers, and get general system information.

Procfs is a pseudo filesystem. This means that files resident in procfs are not associated with physical storage devices such as hard disks. Instead, data in those files is dynamically created on demand by the corresponding entry points in the kernel. Because of this, file sizes in procfs get shown as zero. Procfs is usually mounted under the */proc* directory during kernel boot; you can see this by invoking the mount command.

To get a first feel of the capabilities of procfs, examine the contents of */proc/cpuinfo*, */proc/meminfo*, */proc/interrupts*, */proc/tty/driver/serial*, */proc/bus/usb/devices*, and */proc/stat.* Certain kernel parameters can be changed at runtime by writing to files under */proc/sys/*. For example, you can change kernel printk log levels by echoing a new set of values to */proc/sys/kernel/printk*. Many utilities (such as ps) and system performance monitoring tools (such as sysstat) internally derive information from files residing under */proc*.

*Seq files*, introduced in the 2.6 kernel, simplify large procfs operations. They are described in Appendix C, "Seq Files."

# **Allocating Memory**

Some device drivers have to be aware of the existence of *memory zones.* In addition, many drivers need the services of memory-allocation functions. In this section, let's briefly discuss both.

The kernel organizes physical memory into *pages.* The page size depends on the architecture. On x86-based machines, it's 4096 bytes. Each page in physical memory has a struct page (defined in *include/linux/mm\_types.h*) associated with it:

```
struct page {
 unsigned long flags; /* Page status */
 atomic_t _count; /* Reference count */
 /* ... */
 void * virtual; /* Explained later on */
};
```

On 32-bit x86 systems, the default kernel configuration splits the available 4GB address space into a 3GB virtual memory space for user processes and a 1GB space for the kernel, as shown in Figure 2.5. This imposes a 1GB limit on the amount of physical memory that the kernel can handle. In reality, the limit is 896MB because 128MB of the address space is occupied by kernel data structures. You may increase this limit by changing the 3GB/1GB split during kernel configuration, but you will incur the displeasure of memory-intensive applications if you reduce the virtual address space of user processes.

```
        4GB                                        4GB
+----------------------+                +----------------------+
|      ZONE_HIGH       |                |    KERNEL SPACE      |
|   (896MB -> 4GB)     |                |   (3GB  ->  4GB)     |
+----------------------+ 896MB          +----------------------+
|     ZONE_NORMAL      |                |                      |
|   (16MB -> 896MB)    |                |                      |
+----------------------+ 16MB           |                      |
|      ZONE_DMA        |                |     USER SPACE       |
|    (0  -> 16MB)      |                |   (0    ->  3GB)     |
+----------------------+ 0              +----------------------+
  Physical Address Space                 Virtual Address Space
```

**Figure 2.5. Default address space split on a 32-bit PC system.**

Kernel addresses that map the low 896MB differ from physical addresses by a constant offset and are called *logical addresses.* With "high memory" support, the kernel can access memory beyond 896MB by generating *virtual addresses* for those regions using special mappings. All logical addresses are kernel virtual addresses, but not vice versa.

This leads us to the following kernel memory zones:

- ZONE\_DMA (<16MB), the zone used for *Direct Memory Access* (DMA). Because legacy ISA devices have 24 address lines and can access only the first 16MB, the kernel tries to dedicate this area for such devices. **1.**
- ZONE\_NORMAL (16MB to 896MB), the normally addressable region, also called low memory. The "virtual" field in struct page for low memory pages contains the corresponding logical addresses. **2.**
- ZONE\_HIGH (>896MB), the space that the kernel can access only after mapping resident pages to regions in ZONE\_NORMAL (using kmap() and kunmap()). The corresponding kernel addresses are virtual and not logical. The "virtual" field in struct page for high memory pages points to NULL if the page is not kmapped. **3.**

kmalloc() is a memory-allocation function that returns contiguous memory from ZONE\_NORMAL. The prototype is as follows:

```
void *kmalloc(int count, int flags);
```

Where count is the number of bytes to allocate, and flags is a mode specifier. All supported flags are listed in *include/linux./gfp.h* (gfp stands for *get free pages*), but these are the commonly used ones:

- GFP\_KERNEL Used by process context code to allocate memory. If this flag is specified, kmalloc() is allowed to go to sleep and wait for pages to get freed up. **1.**
- GFP\_ATOMIC Used by interrupt context code to get hold of memory. In this mode, kmalloc() is not allowed to sleep-wait for free pages, so the probability of successful allocation with GFP\_ATOMIC is lower than with GFP\_KERNEL. **2.**

Because memory returned by kmalloc() retains the contents from its previous incarnation, there could be a security risk if it's exposed to user space. To get zeroed kmalloced memory, use kzalloc().

If you need to allocate large memory buffers, and you don't require the memory to be physically contiguous, use vmalloc() rather than kmalloc():

```
void *vmalloc(unsigned long count);
```

Here count is the requested allocation size. The function returns kernel virtual addresses.

vmalloc() enjoys bigger allocation size limits than kmalloc() but is slower and can't be called from interrupt context. Moreover, you cannot use the physically discontiguous memory returned by vmalloc() to perform *Direct Memory Access* (DMA). High-performance network drivers commonly use vmalloc() to allocate large descriptor rings when the device is opened.

The kernel offers more sophisticated memory allocation techniques. These include *look aside buffers*, *slabs*, and *mempools*, which are beyond the scope of this chapter.

# **Looking at the Sources**

Kernel boot starts with the execution of real mode assembly code living in the *arch/x86/boot/* directory. Look at *arch/x86/kernel/setup\_32.c* to see how the protected mode kernel obtains information gleaned by the real mode kernel.

The first boot message is printed by code residing in *init/main.c.* Dig inside *init/calibrate.c* to learn more about BogoMIPS calibration and *include/asm-your-arch/bugs.h* for an insight into architecture-dependent checks.

Timer services in the kernel consist of architecture-dependent portions that live in *arch/your-arch/kernel/* and generic portions implemented in *kernel/timer.c.* For related definitions, look at the header files, *include/linux/time\*.h*.

jiffies is defined in *linux/jiffies.h.* The value for HZ is processor-dependent and can be found in *include/asmyour-arch/param.h*.

Memory management sources reside in the top-level *mm/* directory.

Table 2.1 contains a summary of the main data structures used in this chapter and the location of their definitions in the source tree. Table 2.2 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 2.1. Summary of Data Structures**

| Data Structure              | Location                             | Description                                                                                             |
|-----------------------------|--------------------------------------|---------------------------------------------------------------------------------------------------------|
| HZ                          | include/asm-your-arch/param.h        | Number of times the system timer ticks<br>in 1 second                                                   |
| loops_per_jiffy init/main.c |                                      | Number of times the processor executes<br>an internal delay-loop in 1 jiffy                             |
| timer_list                  | include/linux/timer.h                | Used to hold the address of a routine<br>that you want to execute at some point<br>in the future        |
| timeval                     | include/linux/time.h                 | Timestamp                                                                                               |
| spinlock_t                  | include/linux/spinlock_types.h       | A busy-locking mechanism to ensure that<br>only a single thread enters a critical<br>section            |
| semaphore                   | include/asm-your<br>arch/semaphore.h | A sleep-locking mechanism that allows a<br>predetermined number of users to enter<br>a critical section |
| mutex                       | include/linux/mutex.h                | The new interface that replaces<br>semaphore                                                            |
| rwlock_t                    | include/linux/spinlock_types.h       | Reader-writer spinlock                                                                                  |
| page                        | include/linux/mm_types.h             | Kernel's representation of a physical<br>memory page                                                    |

**Table 2.2. Summary of Kernel Programming Interfaces**

| Kernel Interface                                                    | Location                                                       | Description                                                                                                                                     |
|---------------------------------------------------------------------|----------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|
| time_after()<br>time_after_eq()<br>time_before()<br>ime_before_eq() | include/linux/jiffies.h                                        | Compares the current value of jiffies with a<br>specified future value                                                                          |
| schedule_timeout()                                                  | kernel/timer.c                                                 | Schedules a process to run after a specified<br>timeout has elapsed                                                                             |
| wait_event_timeout()                                                | include/linux/wait.h                                           | Resumes execution if a specified condition<br>becomes true or if a timeout occurs                                                               |
| DEFINE_TIMER()                                                      | include/linux/timer.h                                          | Statically defines a timer                                                                                                                      |
| init_timer()                                                        | kernel/timer.c                                                 | Dynamically defines a timer                                                                                                                     |
| add_timer()                                                         | include/linux/timer.h                                          | Schedules the timer for execution after the<br>timeout has elapsed                                                                              |
| mod_timer()                                                         | kernel/timer.c                                                 | Changes timer expiration                                                                                                                        |
| timer_pending()                                                     | include/linux/timer.h                                          | Checks if a timer is pending at the moment                                                                                                      |
| udelay()                                                            | include/asm-your<br>arch/delay.h arch/your<br>arch/lib/delay.c | Busy-waits for the specified number of<br>microseconds                                                                                          |
| rdtsc()                                                             | include/asm-x86/msr.h                                          | Gets the value of the TSC on Pentium-compatible<br>processors                                                                                   |
| do_gettimeofday()                                                   | kernel/time.c                                                  | Obtains wall time                                                                                                                               |
| local_irq_disable()                                                 | include/asm-your<br>arch/system.h                              | Disables interrupts on the local CPU                                                                                                            |
| local_irq_enable()                                                  | include/asm-your<br>arch/system.h                              | Enables interrupts on the local CPU                                                                                                             |
| local_irq_save()                                                    | include/asm-your<br>arch/system.h                              | Saves interrupt state and disables interrupts                                                                                                   |
| local_irq_restore()                                                 | include/asm-your<br>arch/system.h                              | Restores interrupt state to what it was when the<br>matching local_irq_save() was called                                                        |
| spin_lock()                                                         | include/linux/spinlock.h<br>kernel/spinlock.c                  | Acquires a spinlock.                                                                                                                            |
| spin_unlock()                                                       | include/linux/spinlock.h                                       | Releases a spinlock                                                                                                                             |
| spin_lock_irqsave()                                                 | include/linux/spinlock.h<br>kernel/spinlock.c                  | Saves interrupt state, disables interrupts and<br>preemption on local CPU, and locks their critical<br>section to regulate access by other CPUs |
| spin_unlock_irqrestore() include/linux/spinlock.h                   | kernel/spinlock.c                                              | Restores interrupt state and preemption and<br>releases the lock                                                                                |
| DEFINE_MUTEX()                                                      | include/linux/mutex.h                                          | Statically declares a mutex                                                                                                                     |
| mutex_init()                                                        | include/linux/mutex.h                                          | Dynamically declares a mutex                                                                                                                    |
| mutex_lock()                                                        | kernel/mutex.c                                                 | Acquires a mutex                                                                                                                                |

| Kernel Interface                                                                                                                                                   | Location                                      | Description                                                                                  |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------|----------------------------------------------------------------------------------------------|
| mutex_unlock()                                                                                                                                                     | kernel/mutex.c                                | Releases a mutex                                                                             |
| DECLARE_MUTEX()                                                                                                                                                    | include/asm-your<br>arch/semaphore.h          | Statically declares a semaphore                                                              |
| init_MUTEX()                                                                                                                                                       | include/asm-your<br>arch/semaphore.h          | Dynamically declares a semaphore                                                             |
| up()                                                                                                                                                               | arch/your<br>arch/kernel/semaphore.c          | Acquires a semaphore                                                                         |
| down()                                                                                                                                                             | arch/your<br>arch/kernel/semaphore.c          | Releases a semaphore                                                                         |
| atomic_inc()<br>atomic_inc_and_test()<br>atomic_dec()<br>atomic_dec_and_test()<br>clear_bit()<br>set_bit()<br>test_bit()<br>test_and_set_bit()                     | include/asm-your<br>arch/atomic.h             | Atomic operators to perform lightweight<br>operations                                        |
| read_lock()<br>read_unlock()<br>read_lock_irqsave()<br>read_lock_irqrestore()<br>write_lock()<br>write_unlock()<br>write_lock_irqsave()<br>write_lock_irqrestore() | include/linux/spinlock.h<br>kernel/spinlock.c | Reader-writer variant of spinlocks                                                           |
| down_read()<br>up_read()<br>down_write()<br>up_write()                                                                                                             | kernel/rwsem.c                                | Reader-writer variant of semaphores                                                          |
| read_seqbegin()<br>read_seqretry()<br>write_seqlock()<br>write_sequnlock()                                                                                         | include/linux/seqlock.h                       | Seqlock operations                                                                           |
| kmalloc()                                                                                                                                                          | include/linux/slab.h<br>mm/slab.c             | Allocates physically contiguous memory from<br>ZONE_NORMAL                                   |
| kzalloc()                                                                                                                                                          | include/linux/slab.h<br>mm/util.c             | Obtains zeroed kmalloced memory                                                              |
| kfree()                                                                                                                                                            | mm/slab.c                                     | Releases kmalloced memory                                                                    |
| vmalloc()                                                                                                                                                          | mm/vmalloc.c                                  | Allocates virtually contiguous memory that is not<br>guaranteed to be physically contiguous. |
