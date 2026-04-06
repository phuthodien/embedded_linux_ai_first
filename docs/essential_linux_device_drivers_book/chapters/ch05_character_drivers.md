# **Chapter 5. Character Drivers**

| In This Chapter              |     |
|------------------------------|-----|
| Char Driver Basics           | 120 |
| Device Example: System CMOS  | 121 |
| Sensing Data Availability    | 139 |
| Talking to the Parallel Port | 145 |
| RTC Subsystem                | 156 |
| Pseudo Char Drivers          | 157 |
| Misc Drivers                 | 160 |
| Character Caveats            | 166 |
| Looking at the Sources       | 167 |
|                              |     |

You are now all set to make a foray into writing simple, albeit real-world, device drivers. In this chapter, let's look at the internals of a character (or char) device driver, which is kernel code that sequentially accesses data from a device. Char drivers can capture raw data from several types of devices: printers, mice, watchdogs, tapes, memory, RTCs, and so on. They are however, not suitable for managing data residing on block devices capable of random access such as hard disks, floppies, or compact discs.

# **Char Driver Basics**

Let's start with a top-down view. To access a char device, a system user invokes a suitable application program. The application is responsible for talking to the device, but to do that, it needs to elicit the identity of a suitable driver. The contact details of the driver are exported to user space via the */dev* directory:

#### **bash> ls -l /dev**

```
total 0
crw------- 1 root root 5, 1 Jul 16 10:02 console
...
lrwxrwxrwx 1 root root 3 Oct 6 10:02 cdrom -> hdc
...
brw-rw---- 1 root disk 3, 0 Oct 6 2007 hda
brw-rw---- 1 root disk 3, 1 Oct 6 2007 hda1
...
crw------- 1 root tty 4, 1 Oct 6 10:20 tty1
crw------- 1 root tty 4, 2 Oct 6 10:02 tty2
```

The first character in each line of the ls output denotes the driver type: c signifies a char driver, b stands for a block driver, and l denotes a symbolic link. The numbers in the fifth column are called *major* numbers, and those in the sixth column are *minor* numbers. A major number broadly identifies the driver, whereas a minor number pinpoints the exact device serviced by the driver. For example, the IDE block storage driver */dev/hda* owns a major number of 3 and is in charge of handling the hard disk on your system, but when you further specify a minor number of 1 (*/dev/hda1*), that narrows it down to the first disk partition. Char and block drivers occupy different spaces, so you can have same major number assigned to a char as well as a block driver.

Let's take a step further and peek inside a char driver. From a code-flow perspective, char drivers have the following:

- An initialization (or init()) routine that is responsible for initializing the device and seamlessly tying the driver to the rest of the kernel via registration functions.
- A set of entry points (or methods) such as open(), read(), ioctl(), llseek(), and write(), which directly correspond to I/O system calls invoked by user applications over the associated */dev* node.
- Interrupt routines, bottom halves, timer handlers, helper kernel threads, and other support infrastructure. These are largely transparent to user applications.

From a data-flow perspective, char drivers own the following key data structures:

- **1.** A per-device structure. This is the information repository around which the driver revolves.
- struct cdev, a kernel abstraction for character drivers. This structure is usually embedded inside the perdevice structure referred previously. **2.**
- **3.** struct file\_operations, which contains the addresses of all driver entry points.
- **4.** struct file, which contains information about the associated */dev* node.
