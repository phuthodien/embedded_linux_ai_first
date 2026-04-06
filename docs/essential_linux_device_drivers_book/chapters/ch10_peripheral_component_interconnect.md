# **Chapter 10. Peripheral Component Interconnect**

| In This Chapter                        |     |
|----------------------------------------|-----|
| The PCI Family                         | 278 |
| Addressing and Identification          | 281 |
| Accessing PCI Regions                  | 285 |
| Direct Memory Access                   | 288 |
| Device Example: Ethernet-Modem<br>Card | 292 |
| Debugging                              | 308 |
| Looking at the Sources                 | 308 |

*Peripheral Component Interconnect* (PCI) is an omnipresent I/O backbone. Whether you are backing up data on a storage server, capturing video from your desktop, or surfing the web from your laptop, PCI might be serving you in some avatar or the other. PCI, and form factors adapted or derived from PCI such as Mini PCI, CardBus, PCI Extended, PCI Express, PCI Express Mini Card, and Express Card have become de facto peripheral connection technologies on today's computers.

# **The PCI Family**

PCI is a high-speed bus used for communication between the CPU and I/O devices. The PCI specification enables transfer of 32 bits of data in parallel at 33MHz or 66MHz, yielding a peak throughput of 266MBps.

CardBus is a derivative of PCI and has the form factor of a PC Card. CardBus cards are also 32-bits wide and run at 33MHz. Even though CardBus and PCMCIA cards use the same 68-pin connectors, CardBus devices support 32 data lines compared to 16 for PCMCIA by multiplexing address and data lines as done in the PCI bus.

Mini PCI, also a 33MHz 32-bit bus, is another adaptation of PCI found in small-footprint computers such as laptops. A PCI card can talk via a Mini PCI slot using a compatible connector.

An extension to PCI called PCI Extended (or PCI-X) expands the bus width to 64 bits, frequency to 133MHz, and the throughput to about 1GBps. PCI-X 2.0 is the current version of the standard.

PCI Express (PCIe or PCI-E) is the present generation of the PCI family. Unlike the parallel PCI bus, PCIe uses a serial protocol to transfer data. PCIe supports a maximum of 32 serial *links.* Each PCIe link (in the commonly used version 1.1 of the specification) yields a throughput of 250MBps in each transfer direction, thus producing a maximum PCIe data rate of 8GBps in each direction. PCIe 2.0 is the current version of the standard and supports higher data rates.

Serial communication is faster and cheaper than parallel data transfer due to the absence of factors such as signal interference, so the industry trend is to move from parallel buses to serial technologies. PCIe and its adaptations aim to replace PCI and its derivatives, and this shift is also part of the methodology change from parallel to serial communication. Several I/O interfaces discussed in this book, such as RS-232, USB, FireWire, SATA, Ethernet, Fibre Channel, and InfiniBand, are serial communication architectures.

The CardBus equivalent in the PCIe family is the Express Card. Express Cards directly connect to the system bus via a PCIe link or USB 2.0 (discussed in the next chapter), and circumvent middlemen such as CardBus controllers. Mini PCI's cousin in the PCIe family is PCI Express Mini Card.

Recent laptops support Express Card slots instead of (or in addition to) CardBus, and PCI Express Mini Card slots in place of Mini PCI. The former two have smaller footprints and higher speeds compared to the latter two.

Table 10.1 summarizes the important relatives of PCI. From the kernel's perspective, all these technologies are compatible with one another. A kernel PCI driver will work with all related technologies mentioned previously; so even though we base example code in this chapter on a CardBus card, the concepts apply to other PCI derivatives, too.

**Table 10.1. PCI's Siblings, Children, and Cousins**

| Bus Name              | Characteristics                                                                                                           | Form Factor                                                                                                               |
|-----------------------|---------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------|
| PCI                   | 32-bit bus at 33MHz or 66MHz;<br>yields up to 266MBps.                                                                    | Internal slot in desktops and<br>servers.                                                                                 |
| Mini PCI              | 32-bit bus at 33MHz.                                                                                                      | Internal slot in laptops.                                                                                                 |
| CardBus               | 32-bit bus at 33MHz.                                                                                                      | External PC card slot in laptops.<br>Compatible with PCI.                                                                 |
| PCI Extended (PCI-X)  | 64-bit bus at 133 MHz, yielding<br>up to 1GBps.                                                                           | Internal slot in desktops and<br>servers. Wider than PCI, but a<br>PCI card can be plugged into a<br>PCI-X slot.          |
| PCI Express (PCIe)    | 250MBps per PCIe link in each<br>transfer direction, yielding a<br>maximum throughput of 8GBps<br>in each direction.      | Replaces the internal PCI slot in<br>newer systems. PCIe is a serial<br>protocol unlike native PCI, which<br>is parallel. |
| PCI Express Mini Card | 250MBps in each direction if the<br>interface is based on a PCIe link;<br>60MBps if the interface is based<br>on USB 2.0. | Replaces Mini PCI as the internal<br>slot in newer laptops. Smaller<br>form factor than Mini PCI.                         |

| Bus Name     | Characteristics                                                                                                           | Form Factor                                                                                                             |
|--------------|---------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------|
| Express Card | 250MBps in each direction if the<br>interface is based on a PCIe link;<br>60MBps if the interface is based<br>on USB 2.0. | Thin external slot in newer<br>laptops that replaces CardBus.<br>Interfaces with the system bus<br>via PCIe or USB 2.0. |

Solutions based on the PCI family are available for a vast spectrum of hardware domains:

- Networking technologies such as Gigabit Ethernet, WiFi, ATM, Token Ring, and ISDN.
- Host adapters for storage technologies, such as SCSI.
- Host controllers for I/O buses such as USB, FireWire, IDE, I2C, and PCMCIA. On PC-compatible systems, these host controllers function as bridges between the PCI controller on the South Bridge and the bus technology they source. Verify this by running *lspci* (discussed later).
- Graphics, video streaming, and data capture.
- Serial port and parallel port cards.
- Sound cards.
- Devices such as Watchdogs, EDAC-capable memory controllers, and game ports.

For the driver developer, the PCI family offers an attractive advantage: a system of automatic device configuration. Unlike drivers for the older ISA generation, PCI drivers need not implement complex probing logic. During boot, the BIOS-type boot firmware (or the kernel itself if so configured) walks the PCI bus and assigns resources such as interrupt levels and I/O base addresses. The device driver gleans this assignment by peeking at a memory region called the PCI configuration space. PCI devices possess 256 bytes of configuration memory. The top 64 bytes of the configuration space is standardized and holds registers that contain details such as the status, interrupt line, and I/O base addresses. PCIe and PCI-X 2.0 offer an extended configuration space of 4KB. We will learn how to operate on the PCI configuration space later.

Figure 10.1 shows PCI in a PC-compatible system. Components integrated into the South Bridge such as controller silicon for USB, IDE, I2C, LPC, and Ethernet reside off the PCI bus. Some of these controllers contain an internal PCI-to-PCI bridge to source a dedicated PCI bus for the respective I/O technology. The South Bridge additionally contains an external PCI bus to connect I/O peripherals such as CardBus controllers and WiFi chipsets. Figure 10.1 also shows PCI address tuples corresponding to each connected subsystem. This will get clearer when we learn about PCI addressing next.

**Figure 10.1. PCI inside a PC South Bridge.**

```
=========================== CPU & CHIPSET ============================

           +-----------+
           | Processor |
           +-----------+
                 |
                 v
           +-------------+
           | North Bridge|
           +-------------+
                 |
                 v
           +----------------------------------+
           |           South Bridge           |
           |----------------------------------|
           | Hub Interface        0:1e.0      |
           |                                  |
           | IDE                 0:1f.0       |
           | I2C                 0:1f.1       |
           | LPC                 0:1f.2       |
           | Firewire            0:1f.3       |
           |                                  |
           | USB                 0:1d.X       |
           | Ethernet            1:0.0        |
           | External PCI IF     0:1e.0       |
           | AC'97 Audio         0:1f.1       |
           +----------------------------------+

=========================== PCI BUS ============================

                    (External PCI Interface)
                               |
                               v
        --------------------------------------------------
        |                     |                        |
        v                     v                        v

   +----------------+   +------------------+    +-------------------+
   | WiFi Chipset   |   | CardBus Ctrl     |    | SCSI Host Adapter|
   |    2:X.Y       |   |    2:0.0         |    |                   |
   +----------------+   +------------------+    +-------------------+
                                |
                                v
                          +------------------------+
                          | Ethernet-Modem Card    |
                          | 3:0.0  &  3:0.1        |
                          +------------------------+
```

# **Addressing and Identification**

PCI devices are addressed using *bus*, *device*, and *function* numbers, and they are identified via *vendorIDs*, *deviceIDs*, and *class codes*. Let's learn these concepts with the help of the *lspci* utility that is part of the *PCI Utilities* package downloadable from [http://mj.ucw.cz/pciutils.shtml.](http://mj.ucw.cz/pciutils.shtml)

Assume that you're using a Xircom Ethernet-Modem multifunction CardBus card on a Pentium-class laptop served by a Texas Instruments PCI4510 CardBus controller, as shown in Figure 10.1. Run lspci:

### **bash>lspci**

```
00:00.0 Host bridge: Intel Corporation 82852/82855 GM/GME/PM/GMV Processor to I/O
Controller (rev 02)
...
02:00.0 CardBus bridge: Texas Instruments PCI4510 PC card Cardbus Controller (rev 03)
...
03:00.0 Ethernet controller: Xircom Cardbus Ethernet 10/100 (rev 03)
03:00.1 Serial controller: Xircom Cardbus Ethernet + 56k Modem (rev 03)
```

Consider the tuple (XX:YY.Z) at the beginning of each entry in the preceding output. XX stands for the PCI bus number. A PCI domain can host up to 256 buses. In the laptop used previously, the CardBus bridge is connected to PCI bus 2. This bridge sources another PCI bus numbered 3 that hosts the Xircom card.

YY is the PCI device number. Each bus can connect to a maximum of 32 PCI devices. Each device can, in turn, implement up to eight functions represented by Z. The Xircom card can simultaneously perform two functions. Thus, 03:00.0 addresses the Ethernet function of the card, while 03:00.1 corresponds to its modem communication function. Issue lspci –t to elicit a tree-like layout of the PCI buses and devices on your system:

```
bash> lspci –t
```

```
-[0000:00]-+-00.0
 +-00.1
 +-00.3
 +-02.0
 +-02.1
 +-1d.0
 +-1d.1
 +-1d.2
 +-1d.7
 +-1e.0-[0000:02-05]--+-[0000:03]-+-00.0
 | | \-00.1
 | \-[0000:02]-+-00.0
 | +-00.1
 | +-01.0
 | \-02.0
 +-1f.0
```

As you can see from the preceding output (and in Figure 10.1), to walk the PCI bus and reach the Xircom modem (03:00.01) or Ethernet controller (03:00.0), you have to start from your PCI domain (labeled 0000 in the preceding output), traverse a PCI-to-PCI bridge (00:1e.0), and then cross a PCI-to-CardBus host controller (02:0.0). The sysfs representation of the PCI subsystem mirrors this layout:

```
bash> ls /sys/devices/pci0000:00/0000:00:1e.0/0000:02:00.0/0000:03:00.0/
...
net:eth2 Ethernet
...
bash> ls /sys/devices/pci0000:00/0000:00:1e.0/0000:02:00.0/0000:03:00.1/
...
tty:ttyS1 Modem
...
```

As you saw earlier, PCI devices possess a 256-byte memory region that holds configuration registers. This space is the key to identify the make and capabilities of PCI cards. Let's take a peek inside the configuration spaces of the CardBus controller and the Xircom dual-function card previously used. The Xircom card has two configuration spaces, one per supported function:

## Code View:

```
bash> lspci –x
00:00.0 Host bridge: Intel Corporation 82852/82855 GM/GME/PM/GMV Processor to I/O
Controller (rev 02)
00: 86 80 80 35 06 01 90 20 02 00 00 06 00 00 80 00
10: 08 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
20: 00 00 00 00 00 00 00 00 00 00 00 00 14 10 5c 05
30: 00 00 00 00 40 00 00 00 00 00 00 00 00 00 00 00
...
02:00.0 CardBus bridge: Texas Instruments PCI4510 PC card Cardbus Controller (rev 03)
00: 4c 10 44 ac 07 00 10 02 03 00 07 06 20 a8 82 00
10: 00 00 00 b0 a0 00 00 22 02 03 04 b0 00 00 00 f0
20: 00 f0 ff f1 00 00 00 d2 00 f0 ff d3 00 30 00 00
30: fc 30 00 00 00 34 00 00 fc 34 00 00 0b 01 00 05
...
03:00.0 Ethernet controller: Xircom Cardbus Ethernet 10/100 (rev 03)
00: 5d 11 03 00 07 00 10 02 03 00 00 02 00 40 80 00
10: 01 30 00 00 00 00 00 d2 00 08 00 d2 00 00 00 00
20: 00 00 00 00 00 00 00 00 07 01 00 00 5d 11 81 11
30: 00 00 00 00 dc 00 00 00 00 00 00 00 0b 01 14 28
03:00.1 Serial controller: Xircom Cardbus Ethernet + 56k Modem (rev 03)
00: 5d 11 03 01 03 00 10 02 03 02 00 07 00 00 80 00
10: 81 30 00 00 00 10 00 d2 00 18 00 d2 00 00 00 00
20: 00 00 00 00 00 00 00 00 07 02 00 00 5d 11 81 11
30: 00 00 00 00 dc 00 00 00 00 00 00 00 0b 01 00 00
```

PCI registers are little-endian, so factor that while interpreting the preceding output. You may also dump PCI configuration regions via sysfs. So, to look at the configuration space of the Ethernet function of the Xircom card, do this:

```
bash> od -x /sys/devices/pci0000:00/0000:00:1e.0/0000:02:00.0/0000:03:00.1/config
0000000 115d 0003 0007 0210 0003 0200 4000 0080
0000020 3001 0000 0000 d200 0800 d200 0000 0000
0000040 0000 0000 0000 0000 0107 0000 115d 1181
```

...

Table 10.2 explains some of the values shown in the preceding dump. The first two bytes contain the vendor ID, which identifies the company that manufactured the card. PCI vendor IDs are maintained and assigned globally. (Point your browser to www.pcidatabase.com for a database.) As you can decipher from the preceding output, Intel, Texas Instruments, and Xircom (now acquired by Intel) own vendor IDs of 0x8086, 0x104C, and 0x115D, respectively. The next two bytes are specific to the functionality of the card and constitute its device ID. From the preceding output, the Ethernet functionality of the Xircom card owns a device ID of 0x0003, while the modem answers to a device ID of 0x0103. PCI cards additionally possess subvendor and subdevice IDs (see words at offsets 44 and 46 in the preceding dump) to further pinpoint their identity.

**Table 10.2. PCI Configuration Space Semantics**

| Configuration Space Offset | Semantics                                  | Values from the Dump Output<br>for the Xircom Card |
|----------------------------|--------------------------------------------|----------------------------------------------------|
| 0                          | Vendor ID                                  | 0x115D                                             |
| 2                          | Device ID                                  | 0x0003                                             |
| 10                         | Class code                                 | 0x0200                                             |
| 16 to 39                   | Base address register 0<br>(BAR 0) to BAR5 | 0x30010000                                         |
| 44                         | Subvendor ID                               | 0x115D                                             |
| 46                         | Subdevice ID                               | 0x1181                                             |

Ten bytes into the configuration space lies the code that describes the class of the device. PCI bridges have a class code starting with 0x06, network devices possess a class code beginning with 0x02, and communication devices own a class code commencing with 0x07. Thus, in the preceding example, the CardBus bridge, the Ethernet card, and the serial modem own class codes of 0x0607, 0x0200, and 0x0700, respectively. You can find class code definitions in *include/linux/pci\_ids.h*.

PCI drivers register the vendor IDs, device IDs, and class codes that they support with the PCI subsystem. Using this database, the PCI subsystem binds an inserted card to the appropriate device driver after gleaning its identity from its configuration space. We will see how this is done when we implement an example driver later.

# **Accessing PCI Regions**

PCI devices contain three addressable regions: configuration space, I/O ports, and device memory. Let's learn how to access these memory regions from a device driver.

## **Configuration Space**

The kernel offers a set of six functions that your driver can use to operate on PCI configuration space:

```
pci_read_config_[byte|word|dword](struct pci_dev *pdev,
 int offset, int *value);
and
pci_write_config_[byte|word|dword](struct pci_dev *pdev,
 int offset, int value);
```

In the argument list, struct pci\_dev is the PCI device structure, and offset is the byte position in the configuration space that you want to access. For read functions, value is a pointer to a supplied data buffer, and for write routines, it contains the data to be written.

Let's consider some examples:

To decipher the IRQ number assigned to a card function, use the following:

```
unsigned char irq;
pci_read_config_byte(pdev, PCI_INTERRUPT_LINE, &irq);
```

As per the PCI specification, offset 60 inside the PCI configuration space holds the IRQ number assigned to the card. All configuration register offsets are expressively defined in *include/linux/pci\_regs.h*, so use PCI\_INTERRUPT\_LINE rather than 60 to specify this offset. Similarly, to read the PCI status register (two bytes at offset six in the configuration space), do this:

```
unsigned short status;
pci_read_config_word(pdev, PCI_STATUS, &status);
```

Only the first 64 bytes of the configuration space are standardized. The device manufacturer defines desired semantics to the rest. The Xircom card used earlier, assigns four bytes at offset 64 for power management purposes. To disable power management, the Xircom CardBus driver, *drivers/net/tulip/xircom\_cb.c*, does this:

```
#define PCI_POWERMGMT 0x40
pci_write_config_dword(pdev, PCI_POWERMGMT, 0x0000);
```

## **I/O and Memory**

PCI cards have up to six I/O or memory regions. I/O regions contain registers, and memory regions hold data. Video cards, for example, have I/O spaces that accommodate control registers and memory regions that map to frame buffers. Not all cards have addressable memory regions, however. The semantics of I/O and memory spaces are hardware-dependent and can be obtained from the device data sheet.

Like for configuration memory, the kernel offers a set of helpers to operate on I/O and memory regions of PCI devices:

```
```

```
unsigned long pci_resource_[start|len|end|flags] (struct pci_dev *pdev, int bar);
```

To operate on an I/O region such as the device control registers of a PCI video card, the driver needs to do the following:

**1.** Get the I/O base address from the appropriate base address register (bar) in the configuration space:

```
unsigned long io_base = pci_resource_start(pdev, bar);
```

This assumes that the device control registers for this card are mapped to the memory region associated with bar, whose value can range from 0 through 5, as shown in Table 10.2.

**2.** Mark this region as being spoken for, using the kernel's request\_region() regulatory mechanism discussed in Chapter 5, "Character Drivers":

```
request_region(io_base, length, "my_driver");
```

Here, length is the size of the control register space and my\_driver identifies the region's owner. Look for the entry containing my\_driver in */proc/ioports* to spot this memory region.

You may instead use the wrapper function pci\_request\_region(), defined in *drivers/pci/pci.c*.

**3.** Add the register's offset obtained from the data-sheet, to the base address gleaned in Step 1. Operate on this address using the inb() and outb() family of functions discussed in Chapter 5:

```
/* Read */
register_data = inl(io_base + REGISTER_OFFSET);
/* Use */
/* ... */
/* Write */
outl(register_data, iobase + REGISTER_OFFSET);
```

To operate on a memory region such as the frame buffer on the above PCI video card, follow these steps:

**1.** Get the base address, length, and flags associated with the memory region:

```
unsigned long mmio_base = pci_resource_start(pdev, bar);
unsigned long mmio_length = pci_resource_length(pdev, bar);
unsigned long mmio_flags = pci_resource_flags(pdev, bar);
```

This assumes that this memory is mapped to the base address register, bar.

**2.** Mark ownership of this region using the kernel's request\_mem\_region() regulatory mechanism:

```
request_mem_region(mmio_base, mmio_length, "my_driver");
```

You may instead use the wrapper function pci\_request\_region(), mentioned previously.

**3.** Obtain CPU access to the device memory obtained in Step 1. Certain memory regions, such as the ones that hold registers, need to guard against side effects, so they are marked as not being prefetchable (or cacheable) by the CPU. Other regions, such as the one used in this example, can be cached. Depending on the access flag, use the appropriate function to obtain kernel virtual addresses corresponding to the mapped region:

```
void __iomem *buffer;
if (flags & IORESOURCE_CACHEABLE) {
 buffer = ioremap(mmio_base, mmio_length);
} else {
 buffer = ioremap_nocache(mmio_base, mmio_length);
}
```

To be safe, and to avoid performing the preceding checks, use the services of pci\_iomap() defined in *lib/iomap.c* instead:

```
buffer = pci_iomap(pdev, bar, mmio_length);
```

# **Direct Memory Access**

*Direct Memory Access* (DMA) is the capability to transfer data from a peripheral to main memory without the CPU's intervention. DMA boosts the performance of peripherals manyfold, because it doesn't burn CPU cycles to move data. PCI networking cards and IDE disk drives are common examples of peripherals relying on DMA for data transfer.

DMA is initiated by a DMA master. The PC motherboard has a DMA controller on the South Bridge that can master the I/O bus and initiate DMA to or from a peripheral. This is usually the case for legacy ISA cards. However, buses such as PCI can master the bus and initiate DMA transfers. CardBus cards are similar to PCI and also support DMA mastering. PCMCIA devices, on the other hand, do not support DMA mastering, but the PCMCIA controller, which is usually wired to a PCI bus, might have DMA mastering capabilities.

The issue of *cache coherency* is synonymous with DMA. For optimum performance, processors cache recently accessed bytes, so data passing between the CPU and main memory streams through the processor cache. During DMA, however, data travels directly between the DMA controller and main memory and, hence, bypasses the processor cache. This evasion has the potential to introduce inconsistencies because the processor might work on stale data living in its cache. Some architectures automatically synchronize the cache with main memory using a technique called *bus snooping.* Many others rely on software to achieve coherency, however. We will learn how to perform coherent DMA operations after introducing a few more topics.

DMA can occur *synchronously* or *asynchronously.* An example of the former is DMA from a system frame buffer to an LCD controller. A user application writes pixel data to a DMA-mapped frame buffer via */dev/fbX*, while the LCD controller uses DMA to collect this data synchronously at timed intervals. We discuss more about this in Chapter 12, "Video Drivers." An example of asynchronous DMA is the transmit and receive of data frames between the CPU and a network card discussed in Chapter 15, "Network Interface Cards."

System memory regions that are the source or destination of DMA transfers are called DMA buffers. If a bus interface has addressing limitations, that'll affect the memory range that can hold DMA buffers. So, DMA buffers suitable for a 24-bit bus such as ISA can live only in the bottom 16MB of system memory called ZONE\_DMA (see the section "Allocating Memory" in Chapter 2, "A Peek Inside the Kernel"). PCI buses are 32-bits wide by default, so you won't usually face such limitations on 32-bit platforms. To inform the kernel about any special needs of DMA-able buffers, use the following:

```
dma_set_mask(struct device *dev, u64 mask);
```

If this function returns success, you may DMA to any address that is mask bits in length. For example, the e1000 PCI-X Gigabit Ethernet driver (*drivers/net/e1000/e1000\_main.c*) does the following:

```
if (!(err = pci_set_dma_mask(pdev, DMA_64BIT_MASK))) {
 /* System supports 64-bit DMA */
 pci_using_dac = 1;
} else {
 /* See if 32-bit DMA is supported */
 if ((err = pci_set_dma_mask(pdev, DMA_32BIT_MASK))) {
 /* No, let's abort */
 E1000_ERR("No usable DMA configuration, aborting\n");
 return err;
 }
 /* 32-bit DMA */
 pci_using_dac = 0;
}
```

I/O devices view DMA buffers through the lens of the bus controller and any intervening *I/O memory management unit* (IOMMU). Because of this, I/O devices work with *bus* addresses, rather than physical or kernel virtual addresses. So, when you inform a PCI card about the location of a DMA buffer, you have to let it know the buffer's bus address. DMA service routines map the kernel virtual address of DMA buffers to bus addresses so that both the device and the CPU can access the buffers. Bus addresses are of type dma\_addr\_t, defined in *include/asm-your-arch/types.h*.

There are a couple more concepts worth knowing about DMA. One is the idea of *bounce buffers.* Bounce buffers reside in DMA-able regions and are used as temporary memory when DMA is requested to/from non-DMA-able memory regions. An example is DMA to an address higher than 4GB from a 32-bit PCI peripheral when there is no intervening IOMMU. Data is first transferred to a bounce buffer and then copied to the final destination. The second concept is a flavor of DMA called *scatter-gather.* When data to be DMA'ed is spread over discontinuous regions, scatter*-*gather capability enables the hardware to gather contents of the scattered buffers at one go. The reverse occurs when data is DMA'ed from the card to buffers scattered in memory. Scatter-gather capability boosts performance by eliminating the need to service multiple DMA requests.

The kernel features a healthy API that masks many of the internal details of configuring DMA. This API gets simpler if you are writing a driver for a PCI card that supports bus mastering. (Most PCI cards do.) PCI DMA routines are essentially wrappers around the generic DMA service routines and are defined in *include/asmgeneric/pci-dma-compat.h.* In this chapter, we use only the PCI DMA API.

The kernel provides two classes of DMA service routines to PCI drivers:

*Consistent* (or coherent) DMA access methods. These routines guarantee data coherency in the face of DMA activity. If both the PCI device and the CPU are likely to frequently operate on a DMA buffer, consistency is crucial, so use the consistent API. The trade-off is a degree of performance penalty. To obtain a consistent DMA buffer, call this service routine: **1.**

```
void *pci_alloc_consistent(struct pci_dev *pdev,
 size_t size,
 dma_addr_t *dma_handle);
```

This function allocates a DMA buffer, generates its bus address, and returns the associated kernel virtual address. The first two arguments respectively hold the PCI device structure (which is discussed later) and the size of the requested DMA buffer. The third argument, dma\_handle, is a pointer to the bus address that the function call generates. The following snippet allocates and frees a consistent DMA buffer:

```
/* Allocate */
void *vaddr = pci_alloc_consistent(pdev, size,
 &dma_handle);
/* Use */
/* ... */
/* Free */
pci_free_consistent(pdev, size, vaddr, dma_handle);
```

*Streaming* DMA access methods. These routines do not guarantee consistency and are faster as a result. They are useful when there is not much need for shared access between the CPU and the I/O device. When a streamed buffer has been mapped for device access, the driver has to explicitly unmap (or sync) it before the CPU can reliably operate on it. There are two families of streaming access routines: pci\_[map|unmap|dma\_sync]\_single() and pci\_[map|unmap|dma\_sync]\_sg(). **2.**

The first function family maps, unmaps, and synchronizes a single preallocated DMA buffer. pci\_map\_single() is prototyped as follows:

```
dma_addr_t pci_map_single(struct pci_dev *pdev, void *ptr,
```

```
 size_t size, int direction);
```

The first three arguments respectively hold the PCI device structure, the kernel virtual address of a preallocated DMA buffer, and the size of the supplied buffer. The fourth argument, direction, can be one of the following: PCI\_DMA\_BIDIRECTION, PCI\_DMA\_TODEVICE, PCI\_DMA\_FROMDEVICE, or PCI\_DMA\_NONE. The names are self-explanatory, but the first option is expensive, and the last is for debugging. We discuss streamed DMA mapping further when we develop an example driver later.

The second function family maps, unmaps, and synchronizes a scatter-gather list of DMA buffers. pci\_map\_sg() is prototyped as follows:

```
int pci_map_sg(struct pci_dev *pdev,
 struct scatterlist *sgl,
 int num_entries, int direction);
```

The scattered list is specified using the second argument, struct scatterlist, defined in *include/asmyour-arch/scatterlist.h.* num\_entries is the number of entries in the scatterlist. The first and last arguments are the same as that described for pci\_map\_single(). The function returns the number of mapped entries:

```
num_mapped = pci_map_sg(pdev, sgl, num_entries,
 PCI_DMA_TODEVICE);
for (i=0; i<num_mapped; i++) {
 /* sg_dma_address(&sgl[i]) returns the bus address
 of this entry */
 /* sg_dma_len(&sgl[i]) returns the length of this region
 */
}
```

Let's summarize the characteristics of coherent and streaming DMA to help you decide their suitability for your usage scenario:

- Coherent mappings are simple to code but expensive to use. Streaming mappings have the reverse characteristic.
- Coherent mappings are preferred when both the CPU and the I/O device need to frequently manipulate the DMA buffer. This is usually the case for synchronous DMA. An example is the frame buffer driver mentioned previously, where each DMA operates on the same buffer. Because the CPU and the video controller are constantly accessing the frame buffer, it makes sense to use coherent mappings in this situation.
- Use streaming mappings when the I/O device owns the buffer for long durations. Streamed DMA is common for asynchronous operation when each DMA operates on a different buffer. An example is a network driver, where the buffers that carry transmit packets are mapped and unmapped on-the-fly.
- DMA descriptors are good candidates for coherent mapping. DMA descriptors contain metadata about DMA buffers such as their address and length and are frequently accessed by both the CPU and the device. Mapping descriptors on-the-fly is expensive because that entails frequent unmappings and remappings (or sync operations).

# **Device Example: Ethernet-Modem Card**

Armed with the knowledge acquired so far, let's venture to write a skeletal device driver for a fictitious Ethernet-Modem dual-function CardBus card and see how it can be used for networking on a LAN and for establishing a dialup connection to an Internet service provider. You will essentially need one device driver per supported function. Assuming you already have a serial driver (we learned to write serial drivers in Chapter 6, "Serial Drivers") and an Ethernet driver (we will learn to implement network drivers in Chapter 15) that support the chipsets used on the card, let's tinker with those drivers and get them to work with the CardBus interface. The example here is generic but is loosely based on the kernel driver for the Xircom card that we used previously. The Ethernet and modem portions of the Xircom driver live separately in *drivers/net/tulip/xircom\_cb.c* and *drivers/serial/8250\_pci.c*, respectively.

## **Initializing and Probing**

PCI drivers use an array of pci\_device\_id structures defined in *include/linux/mod\_devicetable.h* to describe the identity of the cards they support:

```
struct pci_device_id {
 __u32 vendor, device; /* Vendor and Device IDs */
 __u32 subvendor, subdevice; /* Subvendor and Subdevice IDs */
 __u32 class, classmask; /* Class and class mask */
 kernel_ulong_t driver_data; /* Private data */
};
```

The semantics of the first six fields in pci\_device\_id conform to the PCI parlance discussed previously. The last field driver\_data is private to the driver and is commonly used to co-relate configuration information if the driver supports multiple cards.

The Ethernet-Modem card has a device ID and a configuration space corresponding to each of its two functions. Because the two card functions are unconnected, you need separate PCI drivers to handle them. The *drivers/net/* directory is a good place to hold the Ethernet driver, and *drivers/serial/* is the right location to place its serial counterpart. The Ethernet driver in Listing 10.1 supports the network function and announces a set of associated IDs in its pci\_device\_id table. The serial driver in Listing 10.2 is similar, except that it's responsible for the modem function. The associated class codes and class masks are left unstated by both drivers because the vendor ID/device ID combination itself uniquely identifies their functionality.

The PCI subsystem provides macros such as PCI\_DEVICE() and PCI\_DEVICE\_CLASS() to ease the creation of the pci\_device\_id table. PCI\_DEVICE(), for example, creates a pci\_device\_id element from the specified vendor ID and device ID. So you may rewrite network\_device\_pci\_table in Listing 10.1 as follows:

```
struct pci_device_id network_driver_pci_table[] __devinitdata = {
 {PCI_DEVICE(MY_VENDOR_ID, MY_DEVICE_ID_NET)
 .driver_data = (unsigned long)network_driver_private_data},
 {0},
};
```

The MODULE\_DEVICE\_TABLE() macro in Listing 10.1 and Listing 10.2 marks the pci\_device\_id table in the module image. This information loads the module on demand when the CardBus card is inserted. We explored this mechanism in the section "Module Autoload" in Chapter 4, "Laying the Groundwork," and used it in the context of pcmcia\_device\_id in Chapter 9, "PCMCIA and Compact Flash."

When the PCI hotplug layer senses the presence of a card with properties matching the ones announced by the

pci\_device\_id table of a driver, it invokes the probe() method belonging to that driver. This gives an opportunity to the driver to claim the card. Obviously, PCI drivers have to associate their pci\_device\_id table with their probe() method. This tie up is achieved by the pci\_driver structure that drivers register with the PCI subsystem during initialization. To perform this registration, drivers call pci\_register\_driver().

**Listing 10.1. Registering the Network Function**

```
#include <linux/pci.h>
#define MY_VENDOR_ID 0xABCD
#define MY_DEVICE_ID_NET 0xEF01
/* The set of PCI cards that this driver supports. Only a single
 entry in our case. Look at include/linux/mod_devicetable.h for
 the definition of pci_device_id */
struct pci_device_id network_driver_pci_table[] __devinitdata = {
{
 { MY_VENDOR_ID, /* Interface chip manufacturer ID */
 MY_DEVICE_ID_NET, /* Device ID for the network
 function */
 PCI_ANY_ID, /* Subvendor ID wild card */
 PCI_ANY_ID, /* Subdevice ID wild card */
 0, 0, /* class and classmask are
 unspecified */
 network_driver_private_data /* Use this to co-relate
 configuration information if the
 driver supports multiple
 cards. Can be an enumerated type. */
 }, {0},
};
/* struct pci_driver is defined in include/linux/pci.h */
struct pci_driver network_pci_driver = {
 .name = "ntwrk", /* Unique name */
 .probe = net_driver_probe, /* See Listing 10.3 */
 .remove = __devexit_p(net_driver_remove),/* See Listing 10.3 */
 .id_table = network_driver_pci_table, /* See above */
 /* suspend() and resume() methods that implement power
 management are not used by this driver */
};
/* Ethernet driver initialization */
static int __init
network_driver_init(void)
{
 pci_register_driver(&network_pci_driver);
 return 0;
}
/* Ethernet driver exit */
static void __exit
network_driver_exit(void)
{
 pci_unregister_driver(&network_pci_driver);
}
```

```
module_init(network_driver_init);
module_exit(network_driver_exit);
MODULE_DEVICE_TABLE(pci, network_driver_pci_table);
```

### **Listing 10.2. Registering the Modem Function**

```
#include <linux/pci.h>
#define MY_VENDOR_ID 0xABCD
#define MY_DEVICE_ID_MDM 0xEF02
/* The set of PCI cards that this driver supports */
struct pci_device_id modem_driver_pci_table[] __devinitdata = {
{
 { MY_VENDOR_ID, /* Interface chip manufacturer ID */
 MY_DEVICE_ID_MDM, /* Device ID for the modem
 function */
 PCI_ANY_ID, /* Subvendor ID wild card */
 PCI_ANY_ID, /* Subdevice ID wild card */
 0, 0, /* class and classmask are
 unspecified */
 modem_driver_private_data /* Use this to co-relate
 configuration information if the driver
 supports multiple cards. Can be an
 enumerated type. */
 }, {0},
};
struct pci_driver modem_pci_driver = {
 .name = "mdm", /* Unique name */
 .probe = modem_driver_probe, /* See Listing 10.4 */
 .remove = __devexit_p(modem_driver_remove),/* See Listing 10.4 */
 .id_table = modem_driver_pci_table, /* See above */
 /* suspend() and resume() methods that implement power
 management are not used by this driver */
};
/* Modem driver initialization */
static int __init
modem_driver_init(void)
{
 pci_register_driver(&modem_pci_driver);
 return 0;
}
/* Modem driver exit */
static void __exit
modem_driver_exit(void)
{
 pci_unregister_driver(&modem_pci_driver);
}
module_init(modem_driver_init);
module_exit(modem_driver_exit);
```

```
MODULE_DEVICE_TABLE(pci, modem_driver_pci_table);
```

Listing 10.3 implements the probe() method for the network function. This

- Enables the PCI device
- Discovers resource information such as I/O base addresses and IRQ
- Allocates and populates a networking data structure associated with this device
- Registers itself with the kernel networking layer

Listing 10.4 performs similar work for the modem function. In this case, the driver registers with the kernel serial layer instead of the networking layer.

Listings 10.3 and 10.4 also implement remove() methods, which are invoked when the CardBus card is ejected or when the driver module is unloaded. remove() is the reverse of probe(); it frees resources and unregisters the driver from relevant subsystems. The \_\_devexit\_p() macro that Listing 10.1 uses to declare the remove() method discards the supplied function at compile time if you haven't enabled hotplug support and if the driver is not a dynamically loadable module.

The PCI subsystem calls probe() with two arguments:

- A pointer to pci\_dev, the data structure that describes this PCI device. This structure, defined in *include/linux/pci.h*, is maintained by the PCI subsystem for each PCI device on your system. **1.**
- A pointer to pci\_device\_id, the entry in the driver's pci\_device\_id table that matches the information found in the configuration space of the inserted card. **2.**

### **Listing 10.3. Probing the Network Function**

```
#include <linux/pci.h>
unsigned long ioaddr;
/* Probe method */
static int __devinit
net_driver_probe(struct pci_dev *pdev,
 const struct pci_device_id *id)
{
 /* The net_device structure is defined in include/linux/netdevice.h.
 See Chapter 15, "Network Interface Cards", for the description */
```

```
 struct net_device *net_dev;
 /* Ask low-level PCI code to enable I/O and memory regions for
 this device. Look up the IRQ for the device that the PCI
 subsystem allotted when it walked the bus */
 pci_enable_device(pdev);
 /* Use this device in bus mastering mode, since the network
 function of this card is capable of DMA */
 pci_set_master(pdev);
 /* Allocate an Ethernet interface and fill in generic values in
 the net_dev structure. prv_data is the private driver data
 structure that contains buffers, locks, and so on. This is
 left undefined. Wait until Chapter 15 for more on
 alloc_etherdev() */
 net_dev = alloc_etherdev(sizeof(struct prv_data));
 /* Populate net_dev with your network device driver methods */
 net_dev->hard_start_xmit = &mydevice_xmit; /* See Listing 10.6 */
 /* More net_dev initializations */
 /* ... */
 /* Get the I/O address for this PCI region. All card registers
 specified in Table 10.3 are assumed to be in bar 0 */
 ioaddr = pci_resource_start(pdev, 0);
 /* Claim a 128-byte I/O region */
 request_region(ioaddr, 128, "ntwrk");
 /* Fill in resource information obtained from the PCI layer */
 net_dev->base_addr = ioaddr;
 net_dev->irq = pdev->irq;
 /* ... */
 /* Setup DMA. Defined in Listing 10.5 */
 dma_descriptor_setup(pdev);
 /* Register the driver with the network layer. This will allot
 an unused ethX interface */
 register_netdev(net_dev);
 /* ... */
}
/* Remove method */
static void __devexit
net_driver_remove(struct pci_dev *pdev)
{
 /* Free transmit and receive DMA buffers.
 Defined in Listing 10.5 */
 dma_descriptor_release(pdev);
 /* Release memory regions */
 /* ... */
 /* Unregister from the networking layer */
 unregister_netdev(dev);
 free_netdev(dev);
```

```
 /* ... */
}
```

### **Listing 10.4. Probing the Modem Function**

```
/* Probe method */
static int __devinit
modem_driver_probe(struct pci_dev *pdev,
 const struct pci_device_id *id)
{
 struct uart_port port; /* See Chapter 6, "Serial Drivers" */
 /* Ask low-level PCI code to enable I/O and memory regions
 for this PCI device */
 pci_enable_device(pdev);
 /* Get the PCI IRQ and I/O address to be used and populate the
 uart_port structure (see Chapter 6) with these resources. Look at
 include/linux/pci.h for helper functions */
 /* ... */
 /* Register this information with the serial layer and get an
 unused ttySX port allotted to the card. Look at Chapter 6 for
 more on serial drivers */
 serial8250_register_port(&port);
 /* ... */
}
/* Remove method */
static void __devexit
modem_driver_remove(struct pci_dev *dev)
{
 /* Unregister the port from the serial layer */
 serial8250_unregister_port(&port);
 /* Disable device */
 pci_disable_device(dev);
}
```

To recap, let's trace the code path from the time you insert the Ethernet-Modem CardBus card until you are allotted a network interface (ethX) and a serial port (*/dev/ttySX*):

For each supported CardBus function, the corresponding driver initialization routine registers a pci\_device\_id table of supported cards and a probe() routine. This is shown in Listing 10.1 and Listing 10.2. **1.**

- The PCI hotplug layer detects card insertion and gleans the vendor ID and device ID of each device function from the card's PCI configuration space. **2.**
- Because the IDs match with those registered by the card's Ethernet and serial drivers, the corresponding probe() methods are invoked. This results in the invocation of net\_driver\_probe() and modem\_driver\_probe() described respectively in Listing 10.3 and Listing 10.4. **3.**
- The probe() methods configure the Ethernet and modem portions of the PCI driver with resource information. This leads to the allocation of a networking interface (ethX) and a serial port (ttySX) to the card. From this point on, application data flows over these interfaces. **4.**

## **Data Transfer**

The network function belonging to the sample CardBus device uses the following strategy for data transfer: The card expects the device driver to supply it with an array of two receive DMA descriptors and an array of two transmit DMA descriptors. Each DMA descriptor contains the address of an associated data buffer, its length, and a control word. You can use the control word to tell the device whether the descriptor contains valid data. For a transmit descriptor, you may also program it to request an interrupt after data transmission. The card looks for a valid descriptor and DMA's data to/from the associated data buffer. To suit this elementary scheme, the example driver uses only the coherent DMA interface. The driver coherently allocates a large buffer that holds the descriptors and their associated data buffers. The receive and transmit buffers are 1536 bytes long to match the *maximum transmission unit* (MTU) of Ethernet frames. The descriptors and buffers are pictorially shown in Figure 10.2. The top 24 bytes of each array in the figure hold two 12-byte DMA descriptors, and the rest of the memory is occupied by two 1536-byte DMA buffers. The 12-byte descriptor layout shown in the figure is assumed to match the format specified in the card's data sheet.

**Figure 10.2. DMA descriptors and buffers for the CardBus device.**

```
Receive DMA Descriptors and Buffers                                             Transmit DMA Descriptors and Buffers
(dma_buffer_rx / dma_bus_rx)                                                        (dma_buffer_tx / dma_bus_tx)

     |--------------+----------------------------------+         |--------------+----------------------------------+
     |              | Bus Addr of DMA Buffer 1         |  0      |              | Bus Addr of DMA Buffer 1         |  0
     |              +----------------------------------+         |              +----------------------------------+
     |              | Length of DMA Buffer 1           |  4      |              | Length of DMA Buffer 1           |  4
     |              +----------------------------------+         |              +----------------------------------+
     |              | Control Word 1                   |  8      |              | Control Word 1                   |  8
     |              +----------------------------------+         |              +----------------------------------+
     |      |-------| Bus Addr of DMA Buffer 2         | 12      |      |-------| Bus Addr of DMA Buffer 2         | 12
     |      |       +----------------------------------+         |      |       +----------------------------------+
     |      |       | Length of DMA Buffer 2           | 16      |      |       | Length of DMA Buffer 2           | 16
     |------|------>+----------------------------------+         |------|------>+----------------------------------+
            |       | Control Word 2                   | 20             |       | Control Word 2                   | 20
            |       +----------------------------------+                |       +----------------------------------+
            |       | 1536-byte RX DMA Buffer 1        | 24             |       | 1536-byte TX DMA Buffer 1        | 24
            |       +----------------------------------+                |       +----------------------------------+
            |       | 1536-byte RX DMA Buffer 2        |1560            |       | 1536-byte TX DMA Buffer 2        |1560
            |------>+----------------------------------+                |------>+----------------------------------+
                                                        3096                                                        3096
```

Table 10.3 shows the register layout of the card's network function.

**Table 10.3. Register Layout of the Card's Network Function**

| Register Name   | Description                                                                                  | Offset into I/O Space |
|-----------------|----------------------------------------------------------------------------------------------|-----------------------|
| DMA_RX_REGISTER | Holds the bus address of the receive DMA<br>descriptor array (dma_bus_rx)                    | 0x0                   |
| DMA_TX_REGISTER | Holds the bus address of the transmit DMA<br>descriptor array (dma_bus_tx)                   | 0x4                   |
|                 | CONTROL_REGISTER Control word that commands the card to initiate<br>DMA, stop DMA, and so on | 0x8                   |

**Listing 10.5. Setting Up DMA Descriptors and Buffers**

```
/* Device-specific data structure for the Ethernet Function */
struct device_data {
 struct pci_dev *pdev; /* The PCI Device structure */
 struct net_device *ndev; /* The Net Device structure */
 void *dma_buffer_rx; /* Kernel virtual address of the
 receive descriptor */
```

```
 dma_addr_t dma_bus_rx; /* Bus address of the receive
 descriptor */
 void *dma_buffer_tx; /* Kernel virtual address of the
 transmit descriptor */
 dma_addr_t dma_bus_tx; /* Bus address of the transmit
 descriptor */
 /* ... */
 spin_lock_t device_lock; /* Serialize */
} *mydev_data;
/* On-card registers related to DMA */
#define DMA_RX_REGISTER_OFFSET 0x0 /* Offset of the register
 holding the bus address
 of the RX descriptor */
#define DMA_TX_REGISTER_OFFSET 0x4 /* Offset of the register
 holding the bus address
 of the TX descriptor */
#define CONTROL_REGISTER 0x8 /* Offset of the control
 register */
/* Control Register Defines */
#define INITIATE_XMIT 0x1
/* Descriptor control word definitions */
#define FREE_FLAG 0x1 /* Free Descriptor */
#define INTERRUPT_FLAG 0x2 /* Assert interrupt after DMA */
/* Invoked from Listing 10.3 */
static void
dma_descriptor_setup(struct pci_dev *pdev)
{
 /* Allocate receive DMA descriptors and buffers */
 mydev_data->dma_buffer_rx =
 pci_alloc_consistent(pdev, 3096, &mydev_data->dma_bus_rx);
 /* Fill the two receive descriptors as shown in Figure 10.2 */
 /* RX descriptor 1 */
 mydev_data->dma_buffer_rx[0] = cpu_to_le32((unsigned long)
 (mydev_data->dma_bus_rx + 24)); /* Buffer address */
 mydev_data->dma_buffer_rx[1] = 1536; /* Buffer length */
 mydev_data->dma_buffer_rx[2] = FREE_FLAG; /* Descriptor is free */
 /* RX descriptor 2 */
 mydev_data->dma_buffer_rx[3] = cpu_to_le32((unsigned long)
 (mydev_data->dma_bus_rx + 1560)); /* Buffer address */
 mydev_data->dma_buffer_rx[4] = 1536; /* Buffer length */
 mydev_data->dma_buffer_rx[5] = FREE_FLAG; /* Descriptor is free */
 wmb(); /* Write Memory Barrier */
 /* Write the address of the receive descriptor to the appropriate
 register in the card. The I/O base address, ioaddr, was populated
 in Listing 10.3 */
 outl(cpu_to_le32((unsigned long)mydev_data->dma_bus_rx),
 ioaddr + DMA_RX_REGISTER_OFFSET);
 /* Allocate transmit DMA descriptors and buffers */
 mydev_data->dma_buffer_tx =
 pci_alloc_consistent(pdev, 3096, &mydev_data->dma_bus_tx);
 /* Fill the two transmit descriptors as shown in Figure 10.2 */
 /* TX descriptor 1 */
```

```
 mydev_data->dma_buffer_tx[0] = cpu_to_le32((unsigned long)
 (mydev_data->dma_bus_tx + 24)); /* Buffer address */
 mydev_data->dma_buffer_tx[1] = 1536; /* Buffer length */
 /* Valid descriptor. Generate an interrupt
 after completing the DMA */
 mydev_data->dma_buffer_tx[2] = (FREE_FLAG | INTERRUPT_FLAG);
 /* TX descriptor 2 */
 mydev_data->dma_buffer_tx[3] = cpu_to_le32((unsigned long)
 (mydev_data->dma_bus_tx + 1560)); /* Buffer address */
 mydev_data->dma_buffer_tx[4] = 1536; /* Buffer length */
 mydev_data->dma_buffer_tx[5] = (FREE_FLAG | INTERRUPT_FLAG);
 wmb(); /* Write Memory Barrier */
 /* Write the address of the transmit descriptor to the appropriate
 register in the card. The I/O base, ioaddr, was populated in
 Listing 10.3 */
 outl(cpu_to_le32((unsigned long)mydev_data->dma_bus_tx),
 ioaddr + DMA_TX_REGISTER_OFFSET);
}
/* Invoked from Listing 10.3 */
static void
dma_descriptor_release(struct pci_dev *pdev)
{
 pci_free_consistent(pdev, 3096, mydev_data->dma_bus_tx);
 pci_free_consistent(pdev, 3096, mydev_data->dma_bus_rx);
}
```

Listing 10.5 enforces a write barrier by calling wmb() to prevent the CPU from reordering the outl() before populating the DMA descriptor. On an x86 processor, wmb() reduces to a NOP because Intel CPUs enforce writes in program order. When writing the DMA descriptor address to the card and when populating the buffer's bus address inside the DMA descriptor, the driver converts the native byte order to PCI little-endian format using cpu\_to\_le32(). On Intel CPUs, this again has no effect because both PCI and Intel processors communicate in little-endian. On several other architectures, for example, an ARM9 CPU running in the big-endian mode, both wmb() and cpu\_to\_le32() assume significance.

Now that you have the descriptors and buffers mapped and ready to go, it's time to look at how data is exchanged between the system and the CardBus device, as shown in Listing 10.6. We won't dwell on the network interfaces and networking data structures because Chapter 15 is devoted to doing that.

**Listing 10.6. Receiving and Transmitting Data**

```
/* The interrupt handler */
static irqreturn_t
mydevice_interrupt(int irq, void *devid)
{
 struct sk_buff *skb;
 /* ... */
 /* If this is a receive interrupt, collect the packet and pass it
 on to higher layers. Look at the control word in each RX DMA
 descriptor to figure out whether it contains data. Assume for
 convenience that the first RX descriptor was used by the card
```

```
 to DMA this received packet */
 packet_size = mydev_data->dma_buffer_rx[1];
 /* In real world drivers, the sk_buff is pre-allocated, stream-
 mapped, and supplied to the card after setting the FREE_FLAG
 during device open(). The received data is directly
 DMA'ed to this sk_buff instead of the memcpy() performed here,
 as you will learn in Chapter 15. The card clears the FREE_FLAG
 before the DMA */
 skb = dev_alloc_skb(packet_size); /* See Figure 15.2 of Chapter 15 */
 skb->dev = mydev_data->ndev; /* Owner network device */
 memcpy(skb, mydev_data->dma_buffer_rx[24], packet_size);
 /* Pass the received data to higher-layer protocols */
 skb_put(skb, packet_size);
 netif_rx(skb);
 /* ... */
 /* Make the descriptor available to the card again */
 mydev_data->dma_buffer_rx[2] |= FREE_FLAG;
}
/* This function is registered in Listing 10.3 and is called from
 the networking layer. More on network device interfaces in
 Chapter 15 */
static int
mydevice_xmit(struct sk_buff *skb, struct net_device *dev)
{
 /* Use a valid TX descriptor. Look at Figure 10.2 */
 /* Locking has been omitted for simplicity */
 if (mydev_data->dma_buffer_tx[2] & FREE_FLAG) {
 /* Use first TX descriptor */
 /* In the real world, DMA occurs directly from the sk_buff as
 you will learn later on! */
 memcpy(mydev_data->dma_buffer_tx[24], skb->data, skb->len);
 mydev_data->dma_buffer_tx[1] = skb->len;
 mydev_data->dma_buffer_tx[2] &= ~FREE_FLAG;
 mydev_data->dma_buffer_tx[2] |= INTERRUPT_FLAG;
 } else if (mydev_data->dma_buffer[5] & FREE_FLAG) {
 /* Use second TX descriptor */
 memcpy(mydev_data->dma_buffer_tx[1560], skb->data, skb->len);
 mydev_data->dma_buffer_tx[4] = skb->len;
 mydev_data->dma_buffer_tx[5] &= ~FREE_FLAG;
 mydev_data->dma_buffer_tx[5] |= INTERRUPT_FLAG;
 } else {
 return –EIO; /* Signal error to the caller */
 }
 wmb(); /* Don't reorder writes across this barrier */
 /* Ask the card to initiate DMA. ioaddr is defined
 in Listing 10.3 */
 outl(INITIATE_XMIT, ioaddr + CONTROL_REGISTER);
}
```

When the CardBus device receives an Ethernet packet, it DMAs it to a free RX descriptor and interrupts the CPU. The interrupt handler mydevice\_interrupt() collects the packet from the receive DMA buffer, copies it to a networking data structure (sk\_buff), and passes it on to higher protocol layers.

The transmit routine my\_device\_xmit() is responsible for initiating DMA in the reverse direction. It DMAs transmit packets to card memory. For this, my\_device\_xmit() chooses a TX DMA descriptor that is unused by the card (or whose FREE\_FLAG is set) and uses the associated transmit buffer for data transfer. FREE\_FLAG is cleared soon after, signaling that the descriptor now belongs to the card. The descriptor is released in the interrupt handler (FREE\_FLAG is set again) when the card asserts an interrupt after completing the transmit (not shown in Listing 10.6).

This example driver uses a simplified buffer management scheme that is not performance-sensitive. High-speed network drivers implement a more elaborate plan that employs a combination of coherent and streaming DMA mappings. They maintain linked lists of transmit and receive descriptors and implement free and in-use pools for buffer management. Their receive and transmit data structures look like this:

```
/* Ring of receive buffers */
struct rx_list {
 void *dma_buffer_rx; /* Kernel virtual address of the
 transmit descriptor */
 dma_addr_t dma_bus_rx; /* Bus address of the transmit
 descriptor */
 unsigned int size; /* Buffer size */
 struct list_head next_desc; /* Pointer to the next element */
 struct sk_buff *skb; /* Network Packet */
 dma_addr_t sk_bus; /* Bus address of network packet */
} *rxlist;
/* Ring of transmit buffers */
struct tx_list {
 void *dma_buffer_tx; /* Kernel virtual address of the
 receive descriptor */
 dma_addr_t dma_bus_tx; /* Bus address of the transmit
 descriptor */
 unsigned int size; /* Buffer size */
 struct list_head next_desc; /* Pointer to the next element */
 struct sk_buff *skb; /* Network Packet */
 dma_addr_t sk_bus; /* Bus address of network packet */
} *txlist;
```

The receive and transmit DMA descriptors (rxlist->dma\_buffer\_rx and txlist->dma\_buffer\_tx) are mapped coherently as done in Listing 10.5. The payload buffers (rxlist->skb->data and txlist->skb->data) are, however, mapped using streaming DMA. The receive buffers are preallocated and stream mapped into a free pool during device open, while the transmit buffers are mapped on-the-fly. This avoids the extra data copy performed by mydevice\_interrupt() from the coherently mapped receive DMA buffer to the network buffer (and the extra copy done by mydevice\_xmit() in the reverse direction).

```
/* Preallocating/replenishing receive buffers. Also see the section, "Buffer
 Management and Concurrency Control" in Chapter 15 */
/* ... */
struct sk_buff *skb = dev_alloc_skb();
skb_reserve(skb, NET_IP_ALIGN);
/* Map using streaming DMA */
rxlist->sk_bus = pci_map_single(pdev, rxlist->skb->data,
 rxlist->skb->len, PCI_DMA_TODEVICE);
```

```
/* Allocate a DMA descriptor and populate it with the address mapped above. Add the descriptor to the receive descriptor ring */ ^{\prime } ... */
```

 $\langle \cdot \rangle$ 

# **Debugging**

Enable *Bus Options PCI Support PCI Debugging* in the kernel configuration menu to ask the PCI core to emit debug messages. Explore */proc/bus/pci/devices* and */sys/devices/pciX:Y/* for information about PCI devices on your system such as the CardBus Ethernet-Modem card discussed in this chapter. */proc/interrupts* lists IRQs active on your system, including those used by the PCI layer.

As you saw, lspci gleans information about all PCI buses and devices on your system. You may also use it to dump the configuration space of PCI cards.

A PCI bus analyzer can help debug low-level problems and tune performance.

# **Looking at the Sources**

PCI core and bus access routines live in *drivers/pci/.* To obtain a list of helper routines offered by the PCI subsystem, search for EXPORT\_SYMBOL inside this directory. For definitions and prototypes related to the PCI layer, look at *include/linux/pci\*.h*.

You can spot several PCI device drivers in subdirectories under *drivers/net/*, *drivers/scsi/*, and *drivers/video/.* To locate all PCI drivers, recursively grep the *drivers/* tree for pci\_register\_driver().

If you do not find a good starting point to develop a custom PCI network driver, begin with the skeletal PCI network driver *drivers/net/pci-skeleton.c.* For a brief tutorial on PCI programming, look at *Documentation/pci.txt.* For a description of the PCI DMA API, read *Documentation/DMA-mapping.txt*.

Table 10.4 summarizes the main data structures used in this chapter. Table 10.5 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 10.4. Summary of Data Structures**

| Data Structure | Location                            | Description                                                                 |
|----------------|-------------------------------------|-----------------------------------------------------------------------------|
| pci_dev        | include/linux/pci.h                 | Representation of a PCI device                                              |
| pci_driver     | include/linux/pci.h                 | Representation of a PCI driver                                              |
| pci_device_id  | include/linux/mod_devicetable.h     | Identity of a PCI card                                                      |
| dma_addr_t     | include/asm-your-arch/types.h       | Bus address of a DMA buffer                                                 |
| scatterlist    | include/asm-your-arch/scatterlist.h | Scatter-gather list of DMA<br>buffers                                       |
| sk_buff        | include/linux/skbuff.h              | Main networking data structure<br>(see Chapter 15 for more<br>explanations) |

**Table 10.5. Summary of Kernel Programming Interfaces**

| Kernel Interface                                                                                                                                              | Location                                    | Description                                                                                                                       |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| pci_read_config_byte()<br>pci_read_config_word()<br>pci_read_config_dword()<br>pci_write_config_byte()<br>pci_write_config_word()<br>pci_write_config_dword() | include/linux/pci.h<br>drivers/pci/access.c | Routines to operate on the PCI<br>configuration space.                                                                            |
| pci_resource_start()<br>pci_resource_len()<br>pci_resource_end()<br>pci_resource_flags()                                                                      | include/linux/pci.h                         | These routines operate on PCI<br>I/O and memory regions to<br>obtain the base address, length,<br>end address, and control flags. |
| pci_request_region()                                                                                                                                          | drivers/pci/pci.c                           | Reserves PCI I/O or memory<br>regions.                                                                                            |

| Kernel Interface                                    | Location                                                                             | Description                                                                                                  |
|-----------------------------------------------------|--------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| ioremap()<br>ioremap_nocache()<br>pci_iomap()       | include/asm-your<br>arch/io.h                                                        | Obtains CPU access to device<br>memory.                                                                      |
|                                                     | arch/your<br>arch/mm/ioremap.c                                                       |                                                                                                              |
|                                                     | lib/iomap.c                                                                          |                                                                                                              |
| pci_set_dma_mask()                                  | drivers/pci/pci.c                                                                    | If this function returns success,<br>you may DMA to any address<br>within the mask specified as<br>argument. |
| pci_alloc_consistent()                              | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Obtains a cache-coherent DMA<br>buffermapping.                                                               |
| pci_free_consistent()                               | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Unmaps a cache-coherent DMA<br>buffer.                                                                       |
| pci_map_single()                                    | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Obtains a streaming DMA buffer<br>mapping.                                                                   |
| pci_unmap_single()                                  | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Unmaps a streaming DMA buffer.                                                                               |
| pci_dma_sync_single()                               | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Synchronizes a streaming DMA<br>buffer so that the CPU can<br>reliably operate on it.                        |
| pci_map_sg()<br>pci_unmap_sg()<br>pci_dma_sync_sg() | include/asm<br>generic/pci-dma<br>compat.h include/asm<br>your-arch/dma<br>mapping.h | Maps/unmaps/synchronizes a<br>scatter-gather list of streaming<br>DMA buffers.                               |
| pci_register_driver()                               | include/linux/pci.h<br>drivers/pci/pci-driver.c                                      | Registers a driver with the PCI<br>core.                                                                     |
| pci_unregister_driver()                             | drivers/pci/pci-driver.c                                                             | Unregisters a driver from the PCI<br>core.                                                                   |
| pci_enable_device()                                 | drivers/pci/pci.c                                                                    | Asks low-level PCI code to<br>enable I/O and memory regions<br>for this device.                              |

| Kernel Interface     | Location          | Description                                   |
|----------------------|-------------------|-----------------------------------------------|
| pci_disable_device() | drivers/pci/pci.c | Reverse of<br>pci_enable_device().            |
| pci_set_master()     | drivers/pci/pci.c | Sets the device in DMA bus<br>mastering mode. |
