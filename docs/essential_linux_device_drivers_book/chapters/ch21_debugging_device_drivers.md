# **Chapter 21. Debugging Device Drivers**

| In This Chapter               |     |  |  |
|-------------------------------|-----|--|--|
| Kernel Debuggers              | 596 |  |  |
| Kernel Probes                 | 609 |  |  |
| Kexec and Kdump               | 620 |  |  |
| Profiling                     | 629 |  |  |
| Tracing                       | 634 |  |  |
| Linux Test Project            | 638 |  |  |
| User Mode Linux               | 638 |  |  |
| Diagnostic Tools              | 638 |  |  |
| Kernel Hacking Config Options | 639 |  |  |
| Test Equipment                | 640 |  |  |
|                               |     |  |  |

Now that we have learned how to implement diverse classes of device drivers, let's take a step back and explore some debugging techniques. Investing time in logic design and software engineering before code development and staring hard at the code after development can minimize or even eliminate bugs. But because that is easier said than done, and because we are all humans, developers need debugging tools. In this chapter, let's look at a variety of methods to debug kernel code.

## Reliability, Availability, Serviceability

Many systems, especially mission critical ones, have a need for reliability, availability, and serviceability (RAS). The Linux RAS effort has resulted in the development of several powerful tools. Exercisers such as the Linux Test Project (LTP) measure the reliability and robustness of your kernel port. CPU hotplugging and the Linux High Availability (HA) project can be seen in the context of availability. Kernel debuggers, Kprobes, Kdump, EDAC, and the Linux Trace Toolkit (LTT) come under the ambit of serviceability. The line dividing these classifications is sometimes thin; you can use any or a combination of these methods to suit your debugging needs. For example, output from a kernel profiler such as *OProfile* can be used either to search for potential code bottlenecks (reliability) or to debug a field problem (serviceability).

# **Kernel Debuggers**

The Linux kernel has no built-in debugger support. Whether to include a debugger as part of the stock kernel is an oft-debated point in kernel mailing lists. The instruction level *Kernel Debugger* (kdb) and the source-level *Kernel GNU Debugger* (kgdb) are the two main Linux kernel debuggers. As of today, whether you use kdb or kgdb, you need to download relevant patches and apply them to your kernel sources. Even if you want to stay away from the hassle of patching your kernel sources with debugger support, you can glean information about kernel panics and peek at kernel variables via the plain *GNU Debugger* (gdb). JTAG debuggers use hardwareassisted debugging and are powerful, but expensive.

Kernel debuggers make kernel internals more transparent. You can single-step through instructions, disassemble instructions, display and modify kernel variables, and look at stack traces. In this chapter, let's learn the basics of kernel debuggers with the help of some examples.

## **Entering a Debugger**

You can enter a kernel debugger in multiple ways. One way is to pass command-line arguments that ask the kernel to enter the debugger during boot. Another way is via software or hardware *breakpoint*s. A breakpoint is an address where you want execution stopped and control transferred to the debugger. A software breakpoint replaces the instruction at that address with something else that causes an exception. You may set software breakpoints either using debugger commands or by inserting them into your code. For x86-based systems, you can set a software breakpoint in your kernel source code as follows:

```
asm(" int $3");
```

Alternatively, you can invoke the BREAKPOINT macro, which translates to the appropriate architecture-dependent instruction.

You may use hardware breakpoints in place of software breakpoints if the instruction where you need to stop is in flash memory, where it cannot be replaced by the debugger. A hardware breakpoint needs processor support. The corresponding address has to be added to a debug register. You can only have as many hardware

breakpoints as the number of debug registers supported by the processor.

You may also ask a debugger to set a *watchpoint* on a variable. The debugger stops execution whenever an instruction modifies data at the watchpoint address.

Yet another common method to enter a debugger is by hitting an attention key, but there are instances when this won't work. If your code is sitting in a tight loop after disabling interrupts, the kernel will not get a chance to process the attention key and enter the debugger. For example, you can't enter the debugger via an attention key if your code does something like this:

```
unsigned long flags;
local_irq_save(flags);
while (1) continue;
local_irq_restore(flags);
```

When control is transferred to the debugger, you can start your analysis using various debugger commands.

## **Kernel Debugger (kdb)**

Kdb is an instruction-level debugger used for debugging kernel code and device drivers. Before you can use it, you need to patch your kernel sources with kdb support and recompile the kernel. (Refer to the section "Downloads" for information on downloading kdb patches.) The main advantage of kdb is that it's easy to set up, because you don't need an additional machine to do the debugging (unlike kgdb). The main disadvantage is that you need to correlate your sources with disassembled code (again, unlike kgdb).

Let's wet our toes in kdb with the help of an example. Here's the crime scene: You have modified a kernel serial driver to work with your x86-based hardware. But the driver isn't working, and you would like kdb to help nab the culprit.

Let's start our search for fingerprints by setting a breakpoint at the serial driver open() entry point. Remember, because kdb is not a source-level debugger, you have to open your sources and try to match the instructions with your C code. Let's list the source snippet in question:

### *drivers/serial/myserial.c***:**

```
static int rs_open(struct tty_struct *tty, struct file *filp)
{
 struct async_struct *info;
 /* ... */
 retval = get_async_struct(line, &info);
 if (retval) return(retval);
 tty->driver_data = info;
 /* Point A */
 /* ... */
}
```

Press the Pause key and enter kdb. Let's find out how the disassembled rs\_open() looks. As usual, all debug sessions shown in this chapter attach explanations using the symbol.

```
Entering kdb (current=0xc03f6000, pid 0) on processor 0 due to
Keyboard Entry
```

```
kdb> id rs_open Disassemble rs_open
0xc01cce00 rs_open: sub $0x1c, %esp
0xc01cce03 rs_open+0x03: mov $ffffffed, %ecx
...
0xc01cce4b rs_open+0x4b: call 0xc01ccca0, get_async_struct
...
0xc01cce56 rs_open+0x56: mov 0xc(%esp,1), %eax
0xc01cce5a rs_open+0x5a: mov %eax, 0x9a4(%ebx)
...
```

Point A in the source code is a good place to attach a breakpoint because you can peek at both the tty structure and the info structure to see what's going on.

Looking side by side at the source and the disassembly, rs\_open+0x5a corresponds to Point A. Note that correlation is easier if the kernel is compiled without optimization flags.

Set a breakpoint at rs\_open+0x5a (which is address 0xc01cce5a) and continue execution by exiting the debugger:

```
kbd> bp rs_open+0x5a Set breakpoint
kbd> go Continue execution
```

Now you need to get the kernel to call rs\_open() to hit the breakpoint. To trigger this, execute an appropriate user-space program. In this case, echo some characters to the corresponding serial port (*/dev/ttySX*):

**bash> echo "kerala monsoons" > /dev/ttySX**

This results in the invocation of rs\_open(). The breakpoint gets hit, and kdb assumes control:

Entering kdb on processor 0 due to Breakpoint @ 0xc01cce5a kdb>

Let's now find out the contents of the info structure. If you look at the disassembly, one instruction before the breakpoint (rs\_open+0x56), you will see that the EAX register contains the address of the info structure. Let's look at the register contents:

```
kbd> r Dump register contents
eax = 0xcf1ae680 ebx = 0xce03b000 ecx = 0x00000000
...
```

So, 0xcf1ae680 is the address of the info structure. Dump its contents using the md command:

```
kbd> md 0xcf1ae680 Memory dump
0xcf1ae680 00005301 0000ABC 00000000 10000400
...
```

To make sense of this dump, let's look at the corresponding structure definition. info is defined as struct async\_struct in *include/linux/serialP.h* as follows:

```
struct async_struct {
 int magic; /* Magic Number */
 unsigned long port; /* I/O Port */
 int hub6;
 /* ... */
};
```

If you match the dump with the definition, 0x5301 is the magic number and 0xABC is the I/O port. Well, isn't this interesting! 0xABC doesn't look like a valid port. If you have done enough serial port debugging, you will know that the I/O port base addresses and IRQs are configured in *include/asm-x86/serial.h* for x86-based hardware. Change the port definition to the correct value, recompile the kernel, and continue your testing!

## **Kernel GNU Debugger (kgdb)**

Kgdb is a source-level debugger. It is easier to use than kdb because you don't have to spend time correlating assembly code with your sources. However it's more difficult to set up because an additional machine is needed to front-end the debugging.

You have to use gdb in tandem with kgdb to step through kernel code. gdb runs on the host machine, while the kgdb-patched kernel (refer to the "Downloads" section for information on downloading kgdb patches) runs on the target hardware. The host and the target are connected via a serial null-modem cable, as shown in Figure 21.1. [1]

[1] You can also launch kgdb debug sessions over Ethernet.

```text
KGDB Remote Debugging Setup
============================

  +---------------------------+                    +-------------------+
  |       Target Machine      |                    |   Host Machine    |
  |  (kernel patched with     |~~~~ Serial Cable ~~|  (running gdb)    |
  |        kgdb)              |                    |                   |
  +---------------------------+                    +-------------------+

Connection:
-----------
- Target machine running a kernel patched with kgdb
- Host running gdb
- Connected via Serial Cable
```

**Figure 21.1. Kgdb setup.**

You have to inform the kernel about the identity and baud rate of the serial port via command-line arguments. Depending on the bootloader used, add the following kernel arguments to either *syslinux.cfg*, *lilo.conf*, or *grub.conf*:

kgdbwait kgdb8250=X,115200

kgdbwait asks the kernel to wait until a connection is established with the host-side gdb, X is the serial port connected to the host, and 115200 is the baud rate used for communication.

Now configure the same baud rate on the host side:

If your host computer is a laptop that does not have a serial port, you may use a USB-to-serial converter for the debug session. In that case, instead of */dev/ttySX*, use the */dev/ttyUSBX* node created by the usbserial driver. Figure 6.4 of Chapter 6, "Serial Drivers," illustrates this scenario.

Let's learn kgdb basics using the example of a buggy kernel module. Modules are easier to debug because the entire kernel need not be recompiled after making code changes, but remember to compile your module with the -g option to generate symbolic information. Because modules are dynamically loaded, the debugger needs to be informed about the symbolic information that the module contains. Listing 21.1 contains a buggy trojan\_function(). Assume that it's defined in *drivers/char/my\_module.c*.

### **Listing 21.1. Buggy Function**

```
char buffer;
int
trojan_function()
{
 int *my_variable = 0xAB, i;
 /* ... */
 Point A:
 i = *my_variable; /* Kernel Panic: my_variable points
 to bad memory */
 return(i);
}
```

Insert *my\_module.ko* on the target and look inside */sys/module/my\_module/sections/* to decipher ELF (*Executable and Linking Format*) section addresses.[2] The .text section in ELF files contains code, .data contains initialized variables, .rodata contains initialized read-only variables such as strings, and .bss contains variables that are not initialized during startup. The addresses of these sections are available in the form of the files *.text*, *.data*, *.rodata*, and *.bss* in */sys/module/my\_module/sections/* if you enable CONFIG\_KALLSYMS during kernel configuration. To obtain the code section address, for instance, do this:

[2] If you are still using a 2.4 kernel, get the section addresses using the –m option to insmod instead:

#### **bash> insmod my\_module.o –m**

```
Using /lib/modules/2.x.y/kernel/drivers/char/my_module.o
Sections: Size Address Align
.this 00000060 e091a000 2**2
.text 00001ec0 e091a060 2**4
...
.rodata 0000004c e091d1fc 2**2
.data 00000048 e091d260 2**5
.bss 000000e4 e091d2c0 2**5
...
```

```
bash> cat /sys/module/my_module/sections/.text
0xe091a060
```

More module load information is available from */proc/modules* and */proc/kallsyms*.

After you have the section addresses, invoke gdb on the host-side machine:

```
bash> gdb vmlinux vmlinux is the uncompressed kernel
(gdb) target remote /dev/ttySX Connect to the target
```

Because you passed kgdbwait as a kernel command-line argument, gdb gets control when the kernel boots on the target. Now inform gdb about the preceding section addresses using the add-symbol-file command:

```
(gdb) add-symbol-file drivers/char/mymodule.ko 0xe091a060
 -s .rodata 0xe091d1fc -s .data 0xe091d260 -s .bss 0xe091d2c0
add symbol table from file "drivers/char/my_module.ko" at
 .text_addr = 0xe091a060
 .rodata_addr = 0xe091d1fc
 .data_addr = 0xe091d260
 .bss_addr = 0xe091d2c0
(y or n) y
Reading symbols from drivers/char/mymodule.ko ...done.
```

To debug the kernel panic, let's set a breakpoint at trojan\_function():

```
(gdb) b trojan_function Set breakpoint
(gdb) c Continue execution
```

When kgdb hits the breakpoint, let's look at the stack trace, single-step until Point A, and display the value of my\_variable:

```
(gdb) bt Back (stack) trace
#0 trojan_function () at my_module.c :124
#1 0xe091a108 in my_parent_function (my_var1=438, my_var2=0xe091d288)
..
(gdb) step
(gdb) step Continue to single-step up to
 Point A
(gdb) p my_variable
$0 = 0
```

There is an obvious bug here. my\_variable points to NULL because trojan\_function() forgot to allocate memory for it. Let's just allocate the memory using kgdb, circumvent the kernel crash, and continue testing:

```
(gdb) p &buffer Print address of buffer
$1 = 0xe091a100 ""
(gdb) set my_variable=0xe091a100 my_variable = &buffer
```

Kgdb ports are available for several architectures such as x86, ARM, and PowerPC. When you use kgdb to debug a target embedded device (instead of the PC shown in Figure 21.1), the gdb front-end that you run on your host system needs to be compiled to work with your target platform. For example, to debug a device driver developed for an ARM-based embedded device from your x86-based host development system, you have to use the appropriately generated gdb, often named arm-linux-gdb. The exact name depends on the distribution you use.

## **GNU Debugger (gdb)**

As mentioned earlier, you can use plain gdb to gather some kernel debug information. However, you can't step through kernel code, set breakpoints, or modify kernel variables. Let's use gdb to debug the kernel panic caused by the buggy function in Listing 21.1, but assume this time that trojan\_function() is compiled as part of the kernel and not as a module, because you can't easily peek inside modules using gdb.

This is part of the "oops" message generated when trojan\_function() is executed:

```
Unable to handle kernel NULL pointer dereference at
virtual address 000000ab
 ...
 eax: f7571de0 ebx: ffffe000 ecx: f6c78000 edx: f98df870
 ...
 Stack: c019d731 00000000
 ...
 bffffbe8 c0108fab
Call Trace: [<c019d731>] [<c013b8ac>] [<c0108fab>]
...
```

Copy this cryptic "oops" message to *oops.txt* and use the *ksymoops* utility to obtain more verbose output. You might need to hand-copy the message if the system is hung:

### **bash> ksymoops oops.txt**

```
Code; c019d710 <trojan_function+0/10>
00000000 <_EIP>:
Code; c019d710 <trojan_function+0/10> <=====
 0: a1 ab 00 00 00 mov 0xab,%eax <=====
Code; c019d715 <trojan_function+5/10>
 5: c3 ret
```

2.6 kernels emit "oops" output that can be used as is without the need of decoding using *ksymoops* if you enable CONFIG\_KALLSYMS during kernel configuration.

Looking at the preceding dump, the "oops" has occurred inside trojan\_function(). Let's use gdb to obtain more information. In the following invocation, *vmlinux* is the uncompressed kernel image, and */proc/kcore* is the kernel address space:

**bash> gdb vmlinux /proc/kcore**

```
(gdb) p xtime Test the waters by printing a kernel variable
$0 = 1113173755
```

Repeated access to the same variable will not yield refreshed values due to gdb's cached access. You can force a fresh access by rereading the core file using gdb's core-file command. Let's now look at the disassembly of trojan\_function():

```
(gdb) x/2i trojan_function Disassemble trojan_function
0xc019d710 <trojan_function>: mov 0xab, %eax
0xc019d715 <trojan_function+5>: ret
```

trojan\_function() looks laconic when seen in assembly due to compiler optimizations. It's effectively copying the contents of address 0xab to the EAX register, which holds the return value from functions on x86-based systems. But 0xab does not look like a valid kernel address! Fix the bug by allocating valid memory space to my\_variable, recompile, and continue your testing.

## **JTAG Debuggers**

JTAG debuggers use hardware-assist to debug code. You need specialized monitor hardware[3] and a front-end user interface (some JTAG debuggers use gdb as the front-end) to step through code. JTAG can also be used for purposes other than debugging, such as burning code onto on-board flash memory, as discussed in Chapter 18, "Embedding Linux." JTAG connectors are common on development boards but are usually not part of production units.

[3] Some JTAG debuggers work with several processor architectures if you suitably replace the probe that connects the debugger to the target board.

JTAG debuggers usually connect to target hardware via serial port, USB, or Ethernet. With Ethernet, you can remotely access the JTAG debugger, and hence the target board, even if the board itself does not possess a network interface.

Figure 21.2 shows a JTAG-based remote debugging session in action. The JTAG debugger used in this scenario supports a gdb front end. The development host and the JTAG hardware are connected to an Ethernet LAN. The debug serial port on the target hardware is connected to the serial port on the JTAG box. Figure 21.2 achieves remote debugging on the Linux development host using five terminal sessions. Terminal 1 runs gdb, which connects to the JTAG box over the network using telnet:

```
(gdb) target remote 10.1.1.2:1001 10.1.1.2 is the IP address of
 the JTAG hardware. 1001 is the
 JTAG TCP port that listens to
 incoming gdb connections
```

**Figure 21.2. An example JTAG-based remote debug setup.**

[View full size image]

```text
Remote Linux Development Host - Debug Setup
============================================

Network Layout:
---------------
  [Remote Power Switch]  10.1.1.4
         |
  [Target Hardware]      10.1.1.3
         |  (Serial Port)
  [JTAG Debugger]        10.1.1.2
         |
         +----------- Ethernet ----------- [Remote Linux Development Host]  10.1.1.1

VNC Session on Remote Linux Development Host:
----------------------------------------------

+-------------------------------+  +-------------------------------+  +-------------------------------+
| Terminal 1 (GDB Session)      |  | Terminal 2 (Target Console)   |  | Terminal 3 (JTAG Control)     |
|-------------------------------|  |-------------------------------|  |-------------------------------|
| bash> gdb vmlinux             |  | bash> telnet 10.1.1.2 50      |  | bash> telnet 10.1.1.2         |
| (gdb) target remote           |  |                               |  | jtag>                         |
|       10.1.1.2:1001           |  | target:bash>                  |  |                               |
| (gdb)                         |  |                               |  |                               |
+-------------------------------+  +-------------------------------+  +-------------------------------+

+-------------------------------+  +------------------------------------------------------+
| Terminal 4 (Hard Reset)       |  | Terminal 5 (Exported Root Filesystem)                |
|-------------------------------|  |------------------------------------------------------|
| bash> elinks 10.1.1.4         |  | bash> cat /etc/exports                               |
|                               |  | /path/to/exported/root                               |
| [Target PowerON]              |  | 10.1.1.3(rw,sync,no_root_squash,no_all_squash)       |
| [Target PowerOFF]             |  | bash> cp testcode /path/to/exported/root             |
+-------------------------------+  +------------------------------------------------------+

IP Address Summary:
-------------------
  10.1.1.1  Remote Linux Development Host
  10.1.1.2  JTAG Debugger
  10.1.1.3  Target Hardware
  10.1.1.4  Remote Power Switch
```

To debug boot portions of the kernel, for example, set a gdb breakpoint at start\_kernel(). (You can find its address from *System.map*, which is generated in the root of your source tree when you build the kernel.)

Terminal 2 attaches a serial console to the target. A telnet client running on Terminal 2 connects to a prespecified TCP port on the JTAG box, which is configured (using Terminal 3) to tunnel data arriving via its serial port:

```
bash> telnet 10.1.1.2 50 10.1.1.2 is the IP address of
 the JTAG hardware. 50 is the
 JTAG TCP port that tunnels data
 arriving via its serial port
```

This is equivalent to running an emulator such as *minicom* after directly connecting the target's debug serial port to the host (instead of to the JTAG box, as shown in Figure 21.2), but that'll constrain the host to be physically adjacent to the target.

Terminal 3 telnets to the JTAG box and offers debugger-specific semantics. You can use it for example, to do the following:

Pull a JTAG definition script over TFTP from the host and execute it during JTAG boot. A JTAG definition

script usually initializes the processor, clock registers, chip select registers, and memory banks. After this is done, the JTAG hardware is ready to download code on to the target and execute it. The JTAG manufacturer usually provides definition files for all supported platforms, so you are likely to have a close starting point for your board.

- Download your bootloader, kernel, or stand-alone code from the host over TFTP, to flash memory or RAM on the target. File formats such as ELF and binary are usually supported by JTAG debuggers.
- Single-step code, set breakpoints, examine registers, and dump memory regions.
- Reset the target.

JTAG debugging can be flaky at times, so if you are debugging remotely, it might be a good idea to power the target via a remote power control switch, as shown in Figure 21.2. That way, you can hard-reset the target from the host using a web browser, as shown in Terminal 4. You may also choose to power the JTAG hardware via a remote power switch. That will let you test run a bootloader directly from flash without the intervention of JTAG and its definition files.

If the target board possesses a network interface, it can mount its root filesystem over NFS from the development host. (See the section "NFS-Mounted Root" in Chapter 18 for details on doing this.) Terminal 5 on the host operates locally on the exported root filesystem.[4]

[4] You may have more such terminals depending on your debug scenario. If you are using an oscilloscope that has remote display capabilities, for example, you may operate it via a web browser on another terminal.

If your team is scattered geographically, run Terminals 1 through 5 within an environment such as *Virtual Network Computing* (VNC). If VNC is not already part of your distribution, download it from www.realvnc.com. With such a setup, you can debug the electrons on your remote board from the comfort of your home! Some JTAG vendors provide a sophisticated integrated development environment[5] that encompasses all the functionalities previously detailed, so you don't need to manage VNC terminal sessions if you're using one of those.

[5] While JTAG hardware is independent of the target operating system, the front-end interface is likely to have OS dependencies.

During hardware bring up, when you are porting your bootloader or other stand-alone code to the target, it's a good idea to first generate an ELF image and debug it from RAM before running it from flash. Remember, however, to eliminate bootloader initializations that duplicate the ones performed by the JTAG definition script.

A key advantage of JTAG debuggers is that you can use a single tool to debug the different pieces that constitute your firmware solution. So, you can use the same debugger to debug the BIOS, bootloader, base kernel, device driver modules, as well as user-space applications, at source level.

## **Downloads**

You may download kdb patches for the x86 and IA64 architectures from [http://oss.sgi.com/projects/kdb.](http://oss.sgi.com/projects/kdb) Each supported kernel version needs two patches: a common one and an architecture-dependent one.

The home page for the kgdb project is [http://kgdb.sourceforge.net.](http://kgdb.sourceforge.net) The website also has documentation on configuring and using kgdb.

If your Linux distribution does not already contain gdb, you can obtain it from www.gnu.org/software/gdb/gdb.html.
