# **Chapter 6. Serial Drivers**

```
In This Chapter

• Layered Architecture .......... 173
• UART Drivers .................. 176
• TTY Drivers ................... 192
• Line Disciplines  ............. 194
• Looking at the Sources ........ 205
```

The serial port is a basic communication channel used by a slew of technologies and applications. A chip known as the *Universal Asynchronous Receiver Transmitter* (UART) is commonly used to implement serial communication. On PC-compatible hardware, the UART is part of the Super I/O chipset, as shown in Figure 6.1.

**Figure 6.1. Connection diagram of the PC serial port.**

```
           +-----------+
           | Processor |
           +-----------+
                 ^
                 |
                 v
           +--------------+
           | North Bridge |
           +--------------+
                 ^
                 |
                 v
           +--------------+
           | South Bridge |
           +--------------+
                 |
               LPC Bus
                 |
        +----------------+
        |    Super I/O   |
        +----------------+
                 |
               UART
                 |
        +----------------------+
        | RS-232 Line Shifter |
        +----------------------+
                 |
           +-------------+
           | Serial Port |
           +-------------+
```

Though RS-232 communication channels are the common type of serial hardware, the kernel's serial subsystem is architected in a generic manner to serve diverse users. You will touch the serial subsystem if you

- Run a terminal session over an RS-232 serial link
- Connect to the Internet via a dialup, cellular, or software modem
- Interface with devices such as touch controllers, smart cards, Bluetooth chips, or Infrared dongles, which use a serial transport
- Emulate a serial port using a USB-to-serial converter
- Communicate over an RS-485 link, which is a multidrop variant of RS-232 that has larger range and better noise immunity

In this chapter, let's find out how the kernel structures the serial subsystem. We will use the example of a Linux cell phone to learn about low-level UART drivers and the example of a serial touch controller to discover the implementation details of higher-level line disciplines.

The UART usually found on PCs is National Semiconductor's 16550, or compatible chips from other vendors, so you will find references to "16550-type UART" in code and documentation. The 8250 chip is the predecessor of the 16550, so the Linux driver for PC UARTs is named *8250.c*.

# **Layered Architecture**

As you just saw, the users of the serial subsystem are many and different. This has motivated kernel developers to structure a layered serial architecture using the following building blocks:

- **1.** Low-level drivers that worry about the internals of the UART or other underlying serial hardware.
- A tty driver layer that interfaces with the low-level driver. A tty driver insulates higher layers from the intricacies of the hardware. **2.**
- Line disciplines that "cook" data exchanged with the tty driver. Line disciplines shape the behavior of the serial layer and help reuse lower layers to support different technologies. **3.**

To help custom driver implementations, the serial subsystem also provides core APIs that factor commonalities out of these layers.

Figure 6.2 shows the connection between the layers. N\_TTY, N\_IRDA, and N\_PPP are drop-in line disciplines that mold the serial subsystem to respectively support terminals, Infrared, and dialup networking. Figure 6.3 maps the serial subsystem to kernel source files.

**Figure 6.2. Connection between the layers in the serial subsystem.**

```
                       /dev/ttySX        /dev/ircommX          ppp0
                          ^                  ^                  ^
                          |                  |                  |
+-------------------+   +------+          +------+          +------+
|   TTY I/O Core    |---| N_TTY|----------|N_IRDA|----------|N_PPP |
+-------------------+   +------+          +------+          +------+
         ^                                               Line Disciplines
         |
         v
+-------------+
| TTY Driver  |
+-------------+
       ^
       |
       v
+----------------------+
| UART/Low-Level Driver|
+----------------------+
       ^
       |
       v
+----------------------------+
| Serial Port/Low-Level HW   |
+----------------------------+
```

**Figure 6.3. Serial layers mapped to kernel sources.**

[View full size image]

```
------------------ User Applications (System call interface) ------------------

                +-----------------+          +-----------------+     |    +-------------------+
                |     N_TTY       |          |     N_IRDA      |     |--->|     tty_io.c      |
                |    (n_tty.c)    |          |  (irtty_sir.c)  |     |    |    (Core Module)  |
                +-----------------+          +-----------------+     |    +-------------------+
                        ^
                        |
------------------------|-- Line Discipline Layer -----------------------------------
                        |
                +-----------------+                                         
                |  serial_core.c  |                                              +-----------------+  
                +-----------------+                                              |  usb-serial.c   | 
                        ^                       +--------------------------+     | (Core Module)   |
                        |                       |  USB-Serial Converter    |<--->+-----------------+
------------------------|------ TTY Layer ------|          Driver          |----------
                        |                       +--------------------------+
                +-----------------+                              ^
                |     8250.c      |                              |
                +-----------------+                              |
                        ^                                        |
------------------------|--- Low-Level driver -------------------|-------------
                        v                                        v
          16550-type UART           Infrared            USB-Serial Converter
                                                                 
                

------------------------------ Physical Layer --------------------------------
```

To illustrate the advantages of a layered serial architecture, let's use an example. Assume that you are using a USB-to-serial adapter to obtain serial capabilities on a laptop that does not have a serial port. One possible scenario is when you are debugging the kernel on a target embedded device from a host laptop using kgdb (kgdb is discussed in Chapter 21, "Debugging Device Drivers"), as shown in Figure 6.4.

**Figure 6.4. Using a USB-to-serial converter.**

```
[Host Laptop] --USB--> [USB–UART Adapter] --UART--> [Target Embedded Device]
```

As shown in Figure 6.3, you first need a suitable USB physical layer driver (the USB counterpart of the UART driver) on your host laptop. This is present in the kernel USB subsystem, *drivers/usb/.* Next, you need a tty driver to sit on top of the USB physical layer. The *usbserial* driver (*drivers/usb/serial/usb-serial.c*) is the core layer that implements a generic tty over USB-serial converters. This driver, in tandem with device-specific tty methods registered by the converter driver (*drivers/usb/serial/keyspan.c* if you are using a Keyspan adapter, for example), constitutes the tty layer. Last, but not the least, you need the N\_TTY line discipline for terminal I/O processing.

The tty driver insulates the line discipline and higher layers from the internals of USB. In fact, the line discipline still thinks it's running over a conventional UART. This is so because the tty driver pulls data from *USB Request Blocks* or URBs (discussed in Chapter 11, "Universal Serial Bus") and encapsulates it in the format expected by the N\_TTY line discipline. The layered architecture thus renders the implementation simpler—all blocks from the line discipline upward can be reused unchanged.

The preceding example uses a technology-specific tty driver and a generic line discipline. The reverse usage is also common. The Infrared stack, discussed in Chapter 16, "Linux Without Wires," uses a generic tty driver and a technology-specific line discipline called N\_IRDA.

As you might have noticed in Figure 6.2 and Figure 6.3, although UART drivers are char drivers, they do not directly expose interfaces to kernel system calls like regular char drivers that we saw in the preceding chapter. Rather, UART drivers (like keyboard drivers discussed in the next chapter) service another kernel layer, the tty layer. I/O system calls start their journey above top-level line disciplines and finally ripple down to UART drivers through the tty layer.

In the rest of this chapter, let's take a closer look at the different driver components of the serial layer. We start at the bottom of the serial stack with low-level UART drivers, move on to middle-level tty drivers, and then proceed to top-level line discipline drivers.

UART drivers revolve around three key data structures. All three are defined in *include/linux/serial\_core.h* :

The per-UART driver structure, struct uart\_driver : **1.**

```
struct uart_driver {
 struct module *owner; /* Module that owns this
 struct */
 const char *driver_name; /* Name */
 const char *dev_name; /* /dev node name
 such as ttyS */
 /* ... */
 int major; /* Major number */
 int minor; /* Minor number */
 /* ... */
 struct tty_driver *tty_driver; /* tty driver */
};
```

The comments against each field explain the associated semantics. The owner field allows the same benefits as that discussed in the previous chapter for the file\_operations structure.

struct uart\_port . One instance of this structure exists for each port owned by the UART driver: **2.**

```
struct uart_port {
 spinlock_t lock; /* port lock */
 unsigned int iobase; /* in/out[bwl]*/
 unsigned char __iomem *membase; /* read/write[bwl]*/
 unsigned int irq; /* irq number */
 unsigned int uartclk; /* base uart clock */
 unsigned char fifosize; /* tx fifo size */
 unsigned char x_char; /* xon/xoff flow
 control */
 /* ... */
};
```

struct uart\_ops . This is a superset of entry points that each UART driver has to support and describes the operations that can be done on physical hardware. The methods in this structure are invoked by the tty layer: **3.**

```
struct uart_ops {
 uint (*tx_empty)(struct uart_port *); /* Is TX FIFO empty? */
 void (*set_mctrl)(struct uart_port *,
 unsigned int mctrl); /* Set modem control params */
 uint (*get_mctrl)(struct uart_port *); /* Get modem control params */
 void (*stop_tx)(struct uart_port *); /* Stop xmission */
 void (*start_tx)(struct uart_port *); /* Start xmission */
 /* ... */
 void (*shutdown)(struct uart_port *); /* Disable the port */
```

```
 void (*set_termios)(struct uart_port *,
 struct termios *new,
 struct termios *old); /* Set terminal interface
 params */
 /* ... */
 void (*config_port)(struct uart_port *,
 int); /* Configure UART port */
 /* ... */
};
```

There are two important steps that a UART driver has to do to tie itself with the kernel: **1.**

Register with the serial core by calling

```
uart_register_driver(struct uart_driver *);
```

**2.** Invoke uart\_add\_one\_port(struct uart\_driver \* , struct uart\_port \*) to register each individual port that it supports. If your serial hardware is hotplugged, the ports are registered with the core from the entry point that probes the presence of the device. Look at the CardBus Modem driver in Listing 10.4 in Chapter 10 , "Peripheral Component Interconnect," for an example where the serial device is plugged hot. Note that some drivers use the wrapper registration function serial8250\_register\_port(struct uart\_port \*) , which internally invokes uart\_add\_one\_port() .

These data structures and registration functions constitute the least common denominator present in all UART drivers. Armed with these structures and routines, let's develop a sample UART driver.

## **Device Example: Cell Phone**

Consider a Linux cell phone built around an embedded *System-on-Chip* (SoC). The SoC has two built-in UARTs, but as shown in Figure 6.5 , both of them are used up, one for communicating with a cellular modem, and the other for interfacing with a Bluetooth chipset. Because there are no free UARTs for debug purposes, the phone uses two USB-to-serial converter chips, one to provide a debug terminal to a PC host, and the other to obtain a spare port. USB-to-serial converters, as you saw earlier in this chapter, let you connect serial devices to your PC via USB. We discuss more on USB-to-serial converters in Chapter 11 .

**Figure 6.5. USB\_UART ports on a Linux cell phone.**

[View full size image]

```
                            Linux Cell Phone
     --------------------------------------------------------------------------|
     |                                                                         |          
     |   +----------+       UART1                        /dev/ttyUU1           |
     |   |GSM/GPRS  |<------------------------------.        ^                 |
     |   +----------+                               |        |                 |
     |                                              |        |                 |
     |   +-------------+        UART3          +-----------------------+       |
     |   | Embedded SoC|<--------------------->|         CPLD          |       |
     |   +-------------+        UART4          +-----------------------+       |
     |                                              ^            ^             |
     |   +----------+       UART2                   | Serial     | Serial      |
     |   |Bluetooth |<------------------------------'            |             |
     |   +----------+                                            |             |
     |                                              +-----------------------+  |
     |                                              | USB‑to‑Serial chip     |  |
     |                              USB <---------->|                       |  |
     |                                              +-----------------------+  |
     |                                              +-----------------------+  |
     |                                              | USB‑to‑Serial chip     |  |
     |                              USB <---------->|                       |  |
     |                                              +-----------------------+  |
     |                                      /dev/ttyUU0                     |  |
     ---------------------------------------------------------------------------
                                          |
                                          |
                                     /dev/ttyUSB0
                                          |
                                   +---------------+
                                   | Host Laptop   |
                                   +---------------+
```

The serial sides of the two USB-to-serial converter chips are connected to the SoC via a *Complex Programmable Logic Device* or CPLD (see the section "CPLD/FPGA " in Chapter 18 , "Embedding Linux"). The CPLD creates two virtual UARTs (or USB\_UART s) by providing a three-register interface to access each USB-to-serial converter, as shown in Table 6.1 : a status register, a read-data register, and a write-data register. To write a character to a USB\_UART , loop on a bit in the status register that clears when there is space in the chip's internal transmit *first-in first-out* (FIFO) memory and then write the byte to the write-data register. To read a character, wait until a specified bit in the status register shows presence of data in the receive FIFO and then read from the read-data register.

#### UU\_STATUS\_REGISTER

Bits to check whether the transmit FIFO is full or whether the receive FIFO is empty 0x0

UU\_READ\_DATA\_REGISTER

Read a character from the USB\_UART

0x1

UU\_WRITE\_DATA\_REGISTER

Write a character to the USB\_UART 0x2

**Table 6.1. Register Layout of the USB\_UART**

| Register Name | Description | Offset from USB_UART |
|---------------|-------------|----------------------|
|               |             | Memory Base          |

At the PC end, use the appropriate Linux *usbserial* driver (for example, *drivers/usb/serial/ftdi\_sio.c* if you are using an FT232AM chip on the cell phone) to create and manage */dev/ttyUSBX* device nodes that correspond to the USB-serial ports. You may run terminal emulators such as *minicom* over one of these device nodes to obtain a console or debug terminal from the cell phone. At the cell phone end, we have to implement a UART driver for the USB\_UART s. This driver creates and manages */dev/ttyUUX* nodes that are responsible for communication at the device side of the link.

The cell phone shown in Figure 6.5 can act as an intelligent gateway for Bluetooth devices—to the GSM network and, hence, to the Internet. The phone can, for example, ferry data from your Bluetooth blood pressure monitor to your health-care provider's server on the Internet. Or it can alert a doctor if it detects a problem while communicating with your Bluetooth-enabled heart-rate monitor. The Bluetooth

MP3 player used in Chapter 13 , "Audio Drivers," and the Bluetooth pill dispenser used in Chapter 16 are also examples of devices that can use the Linux cell phone to get Internet-enabled.

Listing 6.1 implements the USB\_UART driver. It's implemented as a *platform driver.* A platform is a pseudo bus usually used to tie lightweight devices integrated into SoCs, with the Linux device model. A platform consists of

A platform device. The architecture-specific setup code adds the platform device using platform\_device\_register() or its simpler version, platform\_device\_register\_simple() . You may also register multiple platform devices at one shot using platform\_add\_devices() . The platform\_device structure defined in *include/linux/platform\_device.h* represents a platform device: **1.**

```
struct platform_device {
 const char *name; /* Device Name */
 u32 id; /* Use this field to register multiple
 instances of a platform device. In
 this example, the two USB_UARTs
 have different IDs. */
 struct device dev; /* Contains a release() method and
 platform data */
 /* ... */
};
```

A platform driver. The platform driver registers itself into the platform using platform\_driver\_register() . The platform\_driver structure, also defined in *include/linux/platform\_device.h* , represents a platform driver: **2.**

```
struct platform_driver {
 int (*probe)(struct platform_device *); /*Probe method*/
 int (*remove)(struct platform_device *);/*Remove method*/
 /* ... */
 /* The name field in the following structure should match
 the name field in the associated platform_device
 structure */
 struct device_driver driver;
};
```

See *Documentation/driver-model/platform.txt* for more on platform devices and drivers. For simplicity, our sample driver registers both the platform device and the platform driver.

During initialization, the USB\_UART driver first registers itself with the serial core using uart\_register\_driver() . When this is done, you will find a new line starting with usb\_uart in */proc/tty/drivers* . Next, the driver registers two platform devices (one per USB\_UART ) using platform\_device\_register\_simple() . As mentioned earlier, platform device registrations are usually done during boot-time board setup. Following this, the driver registers platform driver entry points (probe() , remove() , suspend() , and resume() ) using platform\_driver\_register() . The USB\_UART platform driver ties into both the above platform devices and has a matching name (usb\_uart ). After this step, you will see two new directories appearing in sysfs, each corresponding to a USB\_UART port: */sys/devices/platform/usb\_uart.0/* and */sys/devices/platform/usb\_uart.1/* .

Because the Linux device layer now detects a platform driver matching the name of the registered USB\_UART platform devices, it invokes the probe() entry point[1] (usb\_uart\_probe() ) belonging to the platform driver, once for each USB\_UART . The probe entry point adds the associated USB\_UART port using uart\_add\_one\_port() . This triggers invocation of the config\_port() entry point (part of the uart\_ops structure discussed earlier) that claims and maps the USB\_UART register space. If both USB\_UART ports are successfully added, the serial core emits the following kernel messages:

[1] Such platform devices usually cannot be hotplugged. This invocation semantics of the probe() method is different from what you will learn in later chapters for hotpluggable devices such as PCMCIA, PCI, and USB, but the similar structure of driver entry points helps the Linux device model to have a uniform and consistent view of all devices.

```
ttyUU0 at MMIO 0xe8000000 (irq = 3) is a USB_UART
ttyUU1 at MMIO 0xe9000000 (irq = 4) is a USB_UART
```

Claiming the IRQ, however, is deferred until an application opens the USB\_UART port. The IRQ is freed when the application closes the USB\_UART . Table 6.2 traces the driver's code path for claiming and freeing memory regions and IRQs.

```
Module Insert
usb_uart_init()
uart_register_driver()
usb_uart_probe()
uart_add_one_port()
usb_uart_config_port()
request_mem_region()
Module Unload
usb_uart_exit()
usb_unregister_driver()
usb_uart_remove()
uart_remove_one_port()
usb_uart_release_port()
release_mem_region()
Open /dev/ttyUUX
usb_uart_startup()
request_irq()
Close /dev/ttyUUX
usb_uart_shutdown()
free_irq()
Table 6.2.
Claiming
   and
 Freeing
 Memory
 and IRQ
Resources
```

In the transmit path, the driver collects egress data from the circular buffer associated with the UART port. Data is present in port->info->xmit.buf[port->info->xmit.tail] as is evident from the UART driver's start\_tx() entry point, usb\_uart\_start\_tx() .

In the receive path, the driver pushes data collected from the USB\_UART to the associated tty driver using tty\_insert\_flip\_char() and tty\_flip\_buffer\_push() . This is done in the receive interrupt handler, usb\_uart\_rxint() . Revisit this routine after reading the next section, "TTY Drivers ."

Listing 6.1 uses comments to explain the purpose of driver entry points and their operation. It leaves some of the entry points in the uart\_ops structure unimplemented to cut out extra detail.

#### **Listing 6.1. USB\_UART Driver for the Linux Cell Phone**

```
#include <linux/console.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <asm/irq.h>
#include <asm/io.h>
#define USB_UART_MAJOR 200 /* You've to get this assigned */
#define USB_UART_MINOR_START 70 /* Start minor numbering here */
#define USB_UART_PORTS 2 /* The phone has 2 USB_UARTs */
#define PORT_USB_UART 30 /* UART type. Add this to
 include/linux/serial_core.h */
/* Each USB_UART has a 3-byte register set consisting of
 UU_STATUS_REGISTER at offset 0, UU_READ_DATA_REGISTER at
 offset 1, and UU_WRITE_DATA_REGISTER at offset 2 as shown
 in Table 6.1 */
#define USB_UART1_BASE 0xe8000000 /* Memory base for USB_UART1 */
#define USB_UART2_BASE 0xe9000000 /* Memory base for USB_UART2 */
#define USB_UART_REGISTER_SPACE 0x3
/* Semantics of bits in the status register */
#define USB_UART_TX_FULL 0x20 /* TX FIFO is full */
#define USB_UART_RX_EMPTY 0x10 /* TX FIFO is empty */
#define USB_UART_STATUS 0x0F /* Parity/frame/overruns? */
#define USB_UART1_IRQ 3 /* USB_UART1 IRQ */
#define USB_UART2_IRQ 4 /* USB_UART2 IRQ */
#define USB_UART_FIFO_SIZE 32 /* FIFO size */
#define USB_UART_CLK_FREQ 16000000
static struct uart_port usb_uart_port[]; /* Defined later on */
/* Write a character to the USB_UART port */
static void
usb_uart_putc(struct uart_port *port, unsigned char c)
{
 /* Wait until there is space in the TX FIFO of the USB_UART.
 Sense this by looking at the USB_UART_TX_FULL bit in the
 status register */
 while (__raw_readb(port->membase) & USB_UART_TX_FULL);
 /* Write the character to the data port*/
 __raw_writeb(c, (port->membase+1));
}
/* Read a character from the USB_UART */
static unsigned char
usb_uart_getc(struct uart_port *port)
{
 /* Wait until data is available in the RX_FIFO */
 while (__raw_readb(port->membase) & USB_UART_RX_EMPTY);
 /* Obtain the data */
```

```
 return(__raw_readb(port->membase+2));
}
/* Obtain USB_UART status */
static unsigned char
usb_uart_status(struct uart_port *port)
{
 return(__raw_readb(port->membase) & USB_UART_STATUS);
}
/*
 * Claim the memory region attached to USB_UART port. Called
 * when the driver adds a USB_UART port via uart_add_one_port().
 */
static int
usb_uart_request_port(struct uart_port *port)
{
 if (!request_mem_region(port->mapbase, USB_UART_REGISTER_SPACE,
 "usb_uart")) {
 return -EBUSY;
 }
 return 0;
}
/* Release the memory region attached to a USB_UART port.
 * Called when the driver removes a USB_UART port via
 * uart_remove_one_port().
 */
static void
usb_uart_release_port(struct uart_port *port)
{
 release_mem_region(port->mapbase, USB_UART_REGISTER_SPACE);
}
/*
 * Configure USB_UART. Called when the driver adds a USB_UART port.
 */
static void
usb_uart_config_port(struct uart_port *port, int flags)
{
 if (flags & UART_CONFIG_TYPE && usb_uart_request_port(port) == 0)
 {
 port->type = PORT_USB_UART;
 }
}
/* Receive interrupt handler */
static irqreturn_t
usb_uart_rxint(int irq, void *dev_id)
{
 struct uart_port *port = (struct uart_port *) dev_id;
 struct tty_struct *tty = port->info->tty;
 unsigned int status, data;
 /* ... */
 do {
 /* ... */
 /* Read data */
 data = usb_uart_getc(port);
```

```
 /* Normal, overrun, parity, frame error? */
 status = usb_uart_status(port);
 /* Dispatch to the tty layer */
 tty_insert_flip_char(tty, data, status);
 /* ... */
 } while (more_chars_to_be_read()); /* More chars */
 /* ... */
 tty_flip_buffer_push(tty);
 return IRQ_HANDLED;
}
/* Called when an application opens a USB_UART */
static int
usb_uart_startup(struct uart_port *port)
{
 int retval = 0;
 /* ... */
 /* Request IRQ */
 if ((retval = request_irq(port->irq, usb_uart_rxint, 0,
 "usb_uart", (void *)port))) {
 return retval;
 }
 /* ... */
 return retval;
}
/* Called when an application closes a USB_UART */
static void
usb_uart_shutdown(struct uart_port *port)
{
 /* ... */
 /* Free IRQ */
 free_irq(port->irq, port);
 /* Disable interrupts by writing to appropriate
 registers */
 /* ... */
}
/* Set UART type to USB_UART */
static const char *
usb_uart_type(struct uart_port *port)
{
 return port->type == PORT_USB_UART ? "USB_UART" : NULL;
}
/* Start transmitting bytes */
static void
usb_uart_start_tx(struct uart_port *port)
{
 while (1) {
 /* Get the data from the UART circular buffer and
 write it to the USB_UART's WRITE_DATA register */
 usb_uart_putc(port,
 port->info->xmit.buf[port->info->xmit.tail]);
 /* Adjust the tail of the UART buffer */
 port->info->xmit.tail = (port->info->xmit.tail + 1) &
 (UART_XMIT_SIZE - 1);
 /* Statistics */
```

```
 port->icount.tx++;
 /* Finish if no more data available in the UART buffer */
 if (uart_circ_empty(&port->info->xmit)) break;
 }
 /* ... */
}
/* The UART operations structure */
static struct uart_ops usb_uart_ops = {
 .start_tx = usb_uart_start_tx, /* Start transmitting */
 .startup = usb_uart_startup, /* App opens USB_UART */
 .shutdown = usb_uart_shutdown, /* App closes USB_UART */
 .type = usb_uart_type, /* Set UART type */
 .config_port = usb_uart_config_port, /* Configure when driver
 adds a USB_UART port */
 .request_port = usb_uart_request_port,/* Claim resources
 associated with a
 USB_UART port */
 .release_port = usb_uart_release_port,/* Release resources
 associated with a
 USB_UART port */
#if 0 /* Left unimplemented for the USB_UART */
 .tx_empty = usb_uart_tx_empty, /* Transmitter busy? */
 .set_mctrl = usb_uart_set_mctrl, /* Set modem control */
 .get_mctrl = usb_uart_get_mctrl, /* Get modem control
 .stop_tx = usb_uart_stop_tx, /* Stop transmission */
 .stop_rx = usb_uart_stop_rx, /* Stop reception */
 .enable_ms = usb_uart_enable_ms, /* Enable modem status
 signals */
 .set_termios = usb_uart_set_termios, /* Set termios */
#endif
};
static struct uart_driver usb_uart_reg = {
 .owner = THIS_MODULE, /* Owner */
 .driver_name = "usb_uart", /* Driver name */
 .dev_name = "ttyUU", /* Node name */
 .major = USB_UART_MAJOR, /* Major number */
 .minor = USB_UART_MINOR_START, /* Minor number start */
 .nr = USB_UART_PORTS, /* Number of UART ports */
 .cons = &usb_uart_console, /* Pointer to the console
 structure. Discussed in Chapter
 12, "Video Drivers" */
};
/* Called when the platform driver is unregistered */
static int
usb_uart_remove(struct platform_device *dev)
{
 platform_set_drvdata(dev, NULL);
 /* Remove the USB_UART port from the serial core */
 uart_remove_one_port(&usb_uart_reg, &usb_uart_port[dev->id]);
 return 0;
}
/* Suspend power management event */
static int
usb_uart_suspend(struct platform_device *dev, pm_message_t state)
```

```
{
 uart_suspend_port(&usb_uart_reg, &usb_uart_port[dev->id]);
 return 0;
}
/* Resume after a previous suspend */
static int
usb_uart_resume(struct platform_device *dev)
{
 uart_resume_port(&usb_uart_reg, &usb_uart_port[dev->id]);
 return 0;
}
/* Parameters of each supported USB_UART port */
static struct uart_port usb_uart_port[] = {
 {
 .mapbase = (unsigned int) USB_UART1_BASE,
 .iotype = UPIO_MEM, /* Memory mapped */
 .irq = USB_UART1_IRQ, /* IRQ */
 .uartclk = USB_UART_CLK_FREQ, /* Clock HZ */
 .fifosize = USB_UART_FIFO_SIZE, /* Size of the FIFO */
 .ops = &usb_uart_ops, /* UART operations */
 .flags = UPF_BOOT_AUTOCONF, /* UART port flag */
 .line = 0, /* UART port number */
 },
 {
 .mapbase = (unsigned int)USB_UART2_BASE,
 .iotype = UPIO_MEM, /* Memory mapped */
 .irq = USB_UART2_IRQ, /* IRQ */
 .uartclk = USB_UART_CLK_FREQ, /* CLock HZ */
 .fifosize = USB_UART_FIFO_SIZE, /* Size of the FIFO */
 .ops = &usb_uart_ops, /* UART operations */
 .flags = UPF_BOOT_AUTOCONF, /* UART port flag */
 .line = 1, /* UART port number */
 }
};
/* Platform driver probe */
static int __init
usb_uart_probe(struct platform_device *dev)
{
 /* ... */
 /* Add a USB_UART port. This function also registers this device
 with the tty layer and triggers invocation of the config_port()
 entry point */
 uart_add_one_port(&usb_uart_reg, &usb_uart_port[dev->id]);
 platform_set_drvdata(dev, &usb_uart_port[dev->id]);
 return 0;
}
struct platform_device *usb_uart_plat_device1; /* Platform device
 for USB_UART 1 */
struct platform_device *usb_uart_plat_device2; /* Platform device
 for USB_UART 2 */
static struct platform_driver usb_uart_driver = {
 .probe = usb_uart_probe, /* Probe method */
 .remove = __exit_p(usb_uart_remove), /* Detach method */
```

```
 .suspend = usb_uart_suspend, /* Power suspend */
 .resume = usb_uart_resume, /* Resume after a suspend */
 .driver = {
 .name = "usb_uart", /* Driver name */
 },
};
/* Driver Initialization */
static int __init
usb_uart_init(void)
{
 int retval;
 /* Register the USB_UART driver with the serial core */
 if ((retval = uart_register_driver(&usb_uart_reg))) {
 return retval;
 }
 /* Register platform device for USB_UART 1. Usually called
 during architecture-specific setup */
 usb_uart_plat_device1 =
 platform_device_register_simple("usb_uart", 0, NULL, 0);
 if (IS_ERR(usb_uart_plat_device1)) {
 uart_unregister_driver(&usb_uart_reg);
 return PTR_ERR(usb_uart_plat_device1);
 }
 /* Register platform device for USB_UART 2. Usually called
 during architecture-specific setup */
 usb_uart_plat_device2 =
 platform_device_register_simple("usb_uart", 1, NULL, 0);
 if (IS_ERR(usb_uart_plat_device2)) {
 uart_unregister_driver(&usb_uart_reg);
 platform_device_unregister(usb_uart_plat_device1);
 return PTR_ERR(usb_uart_plat_device2);
 }
 /* Announce a matching driver for the platform
 devices registered above */
 if ((retval = platform_driver_register(&usb_uart_driver))) {
 uart_unregister_driver(&usb_uart_reg);
 platform_device_unregister(usb_uart_plat_device1);
 platform_device_unregister(usb_uart_plat_device2);
 }
 return 0;
}
/* Driver Exit */
static void __exit
usb_uart_exit(void)
{
 /* The order of unregistration is important. Unregistering the
 UART driver before the platform driver will crash the system */
 /* Unregister the platform driver */
 platform_driver_unregister(&usb_uart_driver);
 /* Unregister the platform devices */
 platform_device_unregister(usb_uart_plat_device1);
```

```
 platform_device_unregister(usb_uart_plat_device2);
 /* Unregister the USB_UART driver */
 uart_unregister_driver(&usb_uart_reg);
}
module_init(usb_uart_init);
module_exit(usb_uart_exit);
```

# **RS-485**

RS-485 is not a standard PC interface like RS-232, but in the embedded space, you may come across computers that use RS-485 connections to reliably communicate with control systems. RS-485 uses differential signals that let it exchange data over distances of up to a few thousand feet, unlike RS-232 that has a range of only a few dozen feet. On the processor side, the RS-485 interface is a UART operating in half-duplex mode. So, before sending data from the transmit FIFO to the wire, the UART device driver needs to additionally enable the RS-485 transmitter and disable the receiver, possibly by wiggling associated GPIO pins. To obtain data from the wire to the receive FIFO, the UART driver has to perform the reverse operation.

You have to enable/disable the RS-485 transmitter/receiver at the right places in the serial layer. If you disable the transmitter too soon, it might not get sufficient time to drain the last bytes from the transmit FIFO, and this can result in data truncation. If you disable the transmitter too late, on the other hand, you prevent data reception for that much time, which might lead to receive data loss.

RS-485 supports multidrop, so the higher-layer protocol must implement a suitable addressing mechanism if you have multiple devices connected to the bus. RS-485 does not support hardware flow control lines using *Request To Send* (RTS) and *Clear To Send* (CTS).

# **TTY Drivers**

Let's now take a look at the structures and registration functions that lie at the heart of tty drivers. Three structures are important for their operation:

struct tty\_struct defined in *include/linux/tty.h.* This structure contains all state information associated with an open tty. It's an enormous structure, but here are some important fields: **1.**

```
 struct tty_struct {
 int magic; /* Magic marker */
 struct tty_driver *driver; /* Pointer to the tty
 driver */
 struct tty_ldisc ldisc; /* Attached Line
 discipline */
 /* ... */
 struct tty_flip_buffer flip; /* Flip Buffer. See
 below. */
 /* ... */
 wait_queue_head_t write_wait; /* See the section
 "Line Disciplines" */
 wait_queue_head_t read_wait; /* See the section
 "Line Disciplines" */
 /* ... */
 };
```

struct tty\_flip\_buffer or the *flip buffer* embedded inside tty\_struct. This is the centerpiece of the data collection and processing mechanism: **2.**

```
 struct tty_flip_buffer {
 /* ... */
 struct semaphore pty_sem; /* Serialize */
 char *char_buf_ptr; /* Pointer to the flip
 buffer */
 /* ... */
 unsigned char char_buf[2*TTY_FLIPBUF_SIZE]; /* The flip
 buffer */
 /* ... */
 };
```

The low-level serial driver uses one half of the flip buffer for gathering data, while the line discipline uses the other half for processing the data. The buffer pointers used by the serial driver and the line discipline are then flipped, and the process continues. Have a look at the function flush\_to\_ldisc() in *drivers/char/tty\_io.c* to see the flip in action.

In recent kernels, the tty\_flip\_buffer structure has been somewhat redesigned. It's now made up of a buffer header (tty\_bufhead) and a buffer list (tty\_buffer):

```
struct tty_bufhead {
 /* ... */
 struct semaphore pty_sem; /* Serialize */
```

```
 struct tty_buffer *head, tail, free; /* See below */
 /* ... */
};
struct tty_buffer {
 struct tty_buffer *next;
 char *char_buf_ptr; /* Pointer to the flip buffer */
 /* ... */
 unsigned long data[0]; /* The flip buffer, memory for
 which is dynamically
 allocated */
};
```

struct tty\_driver defined in *include/linux/tty\_driver.h.* This specifies the programming interface between tty drivers and higher layers: **3.**

```
struct tty_driver {
 int magic; /* Magic number */
 /* ... */
 int major; /* Major number */
 int minor_start; /* Start of minor number */
 /* ... */
 /* Interface routines between a tty driver and higher
 layers */
 int (*open)(struct tty_struct *tty, struct file *filp);
 void (*close)(struct tty_struct *tty, struct file *filp);
 int (*write)(struct tty_struct *tty,
 const unsigned char *buf, int count);
 void (*put_char)(struct tty_struct *tty,
 unsigned char ch);
 /* ... */
};
```

Like a UART driver, a tty driver needs to perform two steps to register itself with the kernel:

- **1.** Call tty\_register\_driver(struct tty\_driver \*tty\_d) to register itself with the tty core.
- **2.** Call

```
tty_register_device(struct tty_driver *tty_d,
 unsigned device_index,
 struct device *device)
```

to register each individual tty that it supports.

We won't develop a sample tty driver, but here are some common ones used on Linux:

- Serial emulation over Bluetooth, discussed in Chapter 16, is implemented in the form of a tty driver. This driver (*drivers/net/bluetooth/rfcomm/tty.c*) calls tty\_register\_driver() during initialization and tty\_register\_device() while handling each incoming Bluetooth connection.
- To work with a system console on a Linux desktop, you need the services of *virtual terminals* (VTs) if you

are in text mode or *pseudo terminals* (PTYs) if you are in graphics mode. VTs and PTYs are implemented as tty drivers and live in *drivers/char/vt.c* and *drivers/char/pty.c*, respectively.

- The tty driver used over conventional UARTs resides in *drivers/serial/serial\_core.c*.
- The USB-serial tty driver is in *drivers/usb/serial/usb-serial.c*.

# **Line Disciplines**

Line disciplines provide an elegant mechanism that lets you use the same serial driver to run different technologies. The low-level physical driver and the tty driver handle the transfer of data to and from the hardware, while line disciplines are responsible for processing the data and transferring it between kernel space and user space.

The serial subsystem supports 17 standard line disciplines. The default line discipline that gets attached when you open a serial port is N\_TTY, which implements terminal I/O processing. N\_TTY is responsible for "cooking" characters received from the keyboard. Depending on user request, it maps the control character to newline, converts lowercase to uppercase, expands tabs, and echoes characters to the associated VT. N\_TTY also supports a *raw* mode used by editors, which leaves all the preceding processing to user applications. See Figure 7.3 in the next chapter, "Input Drivers," to learn how the keyboard subsystem is connected to N\_TTY. The example tty drivers listed at the end of the previous section, "TTY Drivers," use N\_TTY by default.

Line disciplines also implement network interfaces over serial transport protocols. For example, line disciplines that are part of the *Point-to-Point Protocol* (N\_PPP) and the *Serial Line Internet Protocol* (N\_SLIP) subsystems, frame packets, allocate and populate associated networking data structures, and pass the data on to the corresponding network protocol stack. Other line disciplines handle *Infrared Data* (N\_IRDA) and the *Bluetooth Host Control Interface* (N\_HCI).

### **Device Example: Touch Controller**

Let's take a look at the internals of a line discipline by implementing a simple line discipline for a serial touchscreen controller. Figure 6.6 shows how the touch controller is connected on an embedded laptop derivative. The *Finite State Machine* (FSM) of the touch controller is a candidate for being implemented as a line discipline because it can leverage the facilities and interfaces offered by the serial layer.

**Figure 6.6. Connection diagram of a touch controller on a PC-derivative.**

#### **Open and Close**

To create a line discipline, define a struct tty\_ldisc and register a prescribed set of entry points with the kernel. Listing 6.2 contains a code snippet that performs both these operations for the example touch controller.

#### **Listing 6.2. Line Discipline Operations**

```
struct tty_ldisc n_touch_ldisc = {
 TTY_LDISC_MAGIC, /* Magic */
 "n_tch", /* Name of the line discipline */
 N_TCH, /* Line discipline ID number */
 n_touch_open, /* Open the line discipline */
 n_touch_close, /* Close the line discipline */
 n_touch_flush_buffer, /* Flush the line discipline's read
 buffer */
 n_touch_chars_in_buffer, /* Get the number of processed characters in
 the line discipline's read buffer */
 n_touch_read, /* Called when data is requested
 from user space */
 n_touch_write, /* Write method */
 n_touch_ioctl, /* I/O Control commands */
 NULL, /* We don't have a set_termios
 routine */
 n_touch_poll, /* Poll */
 n_touch_receive_buf, /* Called by the low-level driver
 to pass data to user space*/
 n_touch_receive_room, /* Returns the room left in the line
 discipline's read buffer */
 n_touch_write_wakeup /* Called when the low-level device
 driver is ready to transmit more
 data */
};
/* ... */
if ((err = tty_register_ldisc(N_TCH, &n_touch_ldisc))) {
 return err;
}
```

In Listing 6.2, n\_tch is the name of the line discipline, and N\_TCH is the line discipline identifier number. You have to specify the value of N\_TCH in *include/linux/tty.h*, the header file that contains all line discipline definitions. Line disciplines active on your system can be found in */proc/tty/ldiscs*.

Line disciplines gather data from their half of the tty flip buffer, process it, and copy the resulting data to a local *read* buffer. For N\_TCH, n\_touch\_receive\_room() returns the memory left in the read buffer, while n\_touch\_chars\_in\_buffer() returns the number of processed characters in the read buffer that are ready to be delivered to user space. n\_touch\_write() and n\_touch\_write\_wakeup() do nothing because N\_TCH is a read-only device. n\_touch\_open() takes care of allocating memory for the main line discipline data structures, as shown in Listing 6.3.

#### **Listing 6.3. Opening the Line Discipline**

```
/* Private structure used to implement the Finite State Machine
(FSM) for the touch controller. The controller and the processor
communicate using a specific protocol that the FSM implements */
struct n_touch {
 int current_state; /* Finite State Machine */
 spinlock_t touch_lock; /* Spinlock */
 struct tty_struct *tty; /* Associated tty */
 /* Statistics and other housekeeping */
 /* ... */
} *n_tch;
/* Device open() */
static int
n_touch_open(struct tty_struct *tty)
{
 /* Allocate memory for n_tch */
 if (!(n_tch = kmalloc(sizeof(struct n_touch), GFP_KERNEL))) {
 return -ENOMEM;
 }
 memset(n_tch, 0, sizeof(struct n_touch));
 tty->disc_data = n_tch; /* Other entry points now
 have direct access to n_tch */
 /* Allocate the line discipline's local read buffer
 used for copying data out of the tty flip buffer */
 tty->read_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
 if (!tty->read_buf) return -ENOMEM;
 /* Clear the read buffer */
 memset(tty->read_buf, 0, BUFFER_SIZE);
 /* Initialize lock */
 spin_lock_init(&ntch->touch_lock);
 /* Initialize other necessary tty fields.
 See drivers/char/n_tty.c for an example */
 /* ... */
 return 0;
}
```

You might want to set N\_TCH as the default line discipline (rather than N\_TTY) when-ever the serial port connected to the touch controller is opened. See the section "Changing Line Disciplines" to see how to change line disciplines from user space.

#### **Read Path**

For interrupt-driven devices, the read data path usually consists of two threads working in tandem:

**1.** A top thread originating from the user process requesting the read

**2.** A bottom thread springing from the interrupt service routine that collects data from the device

Figure 6.7 shows these threads associated with the read data flow. The interrupt handler queues the receive\_buf() method (n\_touch\_receive\_buf() in our example) as a task. You can override this behavior by setting the tty->low\_latency flag.

```
                             read()

        Terminal I/O                           Network I/O
          (TTY, TCH)                 (PPP, SLIP, Bluetooth, IrDA)
               \                                  /
                \                                /
                 v                              v
              tty_read()              Networking Protocols
                 ^                             ^
                 |                             |
      ------------ User Space / Kernel Space boundary ------------

                         Line Discipline Layer
      -----------------------------------------------------------------
      { Local Read Buffer (tty->read_buf) }          { sk_buff }

            ldisc.read()  --read_wait_queue-->  ldisc.receive_buf()
                    |                             ^         |
                    | Unthrottle                  |         |Throttle
                    v                             |         v 
             ldisc.chars_in_buf()                 |     ldisc.receive_room()
                                                  |
                                                  |
                                  ----------- queue_task -------------
                                                   ^
                                                   |
                                                   |
                                        tty_flip_buffer_push()      { Flip Buffer (tty->flip.char_buf) }
                                                   ^
                                                   |
                                                Serial ISR
```

**Figure 6.7. Line discipline read path.**

The touch controller and the processor communicate using a specific protocol described in the controller's data sheet. The driver implements this communication protocol using an FSM as indicated earlier. Listing 6.4 encodes this FSM as part of the receive\_buf() entry point, n\_touch\_receive\_buf().

**Listing 6.4. The n\_touch\_receive\_buf() Method**

```
static void
n_touch_receive_buf(struct tty_struct *tty,
 const unsigned char *cp, char *fp, int count)
{
 /* Work on the data in the line discipline's half of
 the flip buffer pointed to by cp */
 /* ... */
 /* Implement the Finite State Machine to interpret commands/data
 arriving from the touch controller and put the processed data
 into the local read buffer */
....................................................................................
 /* Datasheet-dependent Code Region */
 switch (tty->disc_data->current_state) {
 case RESET:
 /* Issue a reset command to the controller */
 tty->driver->write(tty, 0, mode_stream_command,
 sizeof(mode_stream_command));
 tty->disc_data->current_state = STREAM_DATA;
 /* ... */
 break;
 case STREAM_DATA:
 /* ... */
 break;
 case PARSING:
 /* ... */
 tty->disc_data->current_state = PARSED;
 break;
 case PARSED:
 /* ... */
 }
....................................................................................
 if (tty->disc_data->current_state == PARSED) {
 /* If you have a parsed packet, copy the collected coordinate
 and direction information into the local read buffer */
 spin_lock_irqsave(&tty->disc_data->touch_lock, flags);
 for (i=0; i < PACKET_SIZE; i++) {
 tty->disc_data->read_buf[tty->disc_data->read_head] =
 tty->disc_data->current_pkt[i];
 tty->disc_data->read_head =
 (tty->disc_data->read_head + 1) & (BUFFER_SIZE - 1);
 tty->disc_data->read_cnt++;
 }
 spin_lock_irqrestore(&tty->disc_data->touch_lock, flags);
 /* ... */ /* See Listing 6.5 */
 }
}
```

n\_touch\_receive\_buf() processes the data arriving from the serial driver. It exchanges a series of commands and responses with the touch controller and puts the received coordinate and direction (press/release)

information into the line discipline's read buffer. Accesses to the read buffer have to be serialized using a spinlock because it's used by both ldisc.receive\_buf() and ldisc.read() threads shown in Figure 6.7 (n\_touch\_receive\_buf() and n\_touch\_read(), respectively, in our example). As you can see in Listing 6.4, n\_touch\_receive\_buf() dispatches commands to the touch controller by directly calling the write() entry point of the serial driver.

n\_touch\_receive\_buf() needs to do a couple more things:

- The top read() thread in Figure 6.7 puts the calling process to sleep if no data is available. So, n\_touch\_receive\_buf() has to wake up that thread and let it read the data that was just processed. **1.**
- If the line discipline is running out of read buffer space, n\_touch\_receive\_buf() has to request the serial driver to throttle data arriving from the device. ldisc.read() is responsible for requesting the corresponding unthrottling when it ferries the data to user space and frees memory in the read buffer. The serial driver uses software or hardware flow control mechanisms to achieve the throttling and unthrottling. **2.**

Listing 6.5 performs these two operations.

#### **Listing 6.5. Waking Up the Read Thread and Throttling the Serial Driver**

```
/* n_touch_receive_buf() continued.. */
/* Wake up any threads waiting for data */
if (waitqueue_active(&tty->read_wait) &&
 (tty->read_cnt >= tty->minimum_to_wake))
 wake_up_interruptible(&tty->read_wait);
}
/* If we are running out of buffer space, request the
 serial driver to throttle incoming data */
if (n_touch_receive_room(tty) < TOUCH_THROTTLE_THRESHOLD) {
 tty->driver.throttle(tty);
}
/* ... */
```

A wait queue (tty->read\_wait) is used to synchronize between the ldisc.read() and ldisc.receive\_buf() threads. ldisc.read() adds the calling process to the wait queue when it does not find data to read, while ldisc.receive\_buf() wakes the ldisc.read() thread when there is data available to be read. So, n\_touch\_read() does the following:

- If there is no data to be read yet, put the calling process to sleep on the read\_wait queue. The process gets woken by n\_touch\_receive\_buf() when data arrives.
- If data is available, collect it from the local read buffer (tty->read\_buf[tty->read\_tail]) and dispatch it to user space.
- If the serial driver has been throttled and if enough space is available in the read buffer after this read, ask the serial driver to unthrottle.

Networking line disciplines usually allocate an sk\_buff (the basic Linux networking data structure discussed in Chapter 15, "Network Interface Cards") and use this as the read buffer. They don't have a read() method, because the corresponding receive\_buf() copies received data into the allocated sk\_buff and directly passes it to the associated protocol stack.

#### **Write Path**

A line discipline's write() entry point performs any post processing that is required before passing the data down to the low-level driver.

If the underlying driver is not able to accept all the data that the line discipline offers, the line discipline puts the requesting thread to sleep. The driver's interrupt handler wakes the line discipline when it is ready to receive more data. To do this, the driver calls the write\_wakeup() method registered by the line discipline. The associated synchronization is done using a wait queue (tty->write\_wait), and the operation is similar to that of the read\_wait queue described in the previous section.

Many networking line disciplines have no write() methods. The protocol implementation directly transmits the frames down to the serial device driver. However, these line disciplines usually still have a write\_wakeup() entry point to respond to the serial driver's request for more transmit data.

N\_TCH does not need a write() method either, because the touch controller is a read-only device. As you saw in Listing 6.4, routines in the receive path directly talk to the low-level UART driver when they need to send command frames to the controller.

#### **I/O Control**

A user program can send commands to a device via ioctl() calls, as discussed in Chapter 5, "Character Drivers." When an application opens a serial device, it can usually issue three classes of ioctls to it:

- Commands supported by the serial device driver, such as TIOCMSET that sets modem information
- Commands supported by the tty driver, such as TIOCSETD that changes the attached line discipline
- Commands supported by the attached line discipline, such as a command to reset the touch controller in the case of N\_TCH

The ioctl() implementation for N\_TCH is largely standard. Supported commands depend on the protocol described in the touch controller's data sheet.

#### **More Operations**

Another line discipline operation is flush\_buffer(), which is used to flush any data pending in the read buffer. flush\_buffer() is also called when a line discipline is closed. It wakes up any read threads that are waiting for more data as follows:

```
if (tty->link->packet){
 wake_up_interruptible(&tty->disc_data->read_wait);
}
```

Yet another entry point (not supported by N\_TCH) is set\_termios(). The N\_TTY line discipline supports the

set\_termios() interface, which is used to set options specific to line discipline data processing. For example, you may use set\_termios() to put the line discipline in raw mode or "cooked" mode. Some options specific to the touch controller (such as changing the baud rate, parity, and number of stop bits) are implemented by the set\_termios() method of the underlying device driver.

The remaining entry points such as poll() are fairly standard, and you can return to Chapter 5 in case you need assistance.

You may compile your line discipline as part of the kernel or dynamically load it as a module. If you choose to compile it as a module, you must, of course, also provide functions to be called during module initialization and exit. The former is usually the same as the init() method. The latter needs to clean up private data structures and unregister the line discipline. Unregistering the discipline is a one-liner:

```
tty_unregister_ldisc(N_TCH);
```

An easier way to drive a serial touch controller is by leveraging the services offered by the kernel's *input* subsystem and the built-in *serport* line discipline. We look at that technique in the next chapter.

#### **Changing Line Disciplines**

N\_TCH gets bound to the low-level serial driver when a user space program opens the serial port connected to the touch controller. But sometimes, a user-space application might want to attach a different line discipline to the device. For instance, you might want to write a program that dumps raw data received from the touch controller without processing it. Listing 6.6 opens the touch controller and changes the line discipline to N\_TTY to dump the data that is coming in.

#### **Listing 6.6. Changing a Line Discipline from User Space**

```
fd = open("/dev/ttySX", O_RDONLY | O_NOCTTY);
/* At this point, N_TCH is attached to /dev/ttySX, the serial port used
 by the touch controller. Switch to N_TTY */
ldisc = N_TTY;
ioctl(fd, TIOCSETD, &ldisc);
/* Set termios to raw mode and dump the data coming in */
/* ... */
```

The TIOCSETD ioctl() closes the current line discipline and opens the newly requested line discipline.

# **Looking at the Sources**

The serial core resides in *drivers/serial/*, but tty implementations and low-level drivers are scattered across the source tree. The driver files referred to in Figure 6.3, for example, live in four different directories: *drivers/serial/, drivers/char/, drivers/usb/serial/*, and *drivers/net/irda/.* The *drivers/serial/* directory, which now also contains UART drivers, didn't exist in the 2.4 kernel; UART-specific code used to be dispersed between *drivers/char/* and *arch/your-arch/* directories. The present code partitioning is more logical because UART drivers are not the only folks that access the serial layer—devices such as USB-to-serial converters and IrDA dongles also need to talk to the serial core.

Look at *drivers/serial/imx.c* for a real-world, low-level UART driver. It handles UARTs that are part of Freescale's i.MX series of embedded controllers.

For a list of line disciplines supported on Linux, see *include/linux/tty.h.* To get a feel of networking line disciplines, look at the corresponding source files for PPP (*drivers/net/ppp\_async.c*), Bluetooth (*drivers/bluetooth/hci\_ldisc.c*), Infrared (*drivers/net/irda/irtty-sir.c*), and SLIP (*drivers/net/slip.c*).

Table 6.3 contains a summary of the main data structures used in this chapter and the location of their definitions in the source tree. Table 6.4 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 6.3. Summary of Data Structures**

| Data Structure             | Location                        | Description                                                                 |
|----------------------------|---------------------------------|-----------------------------------------------------------------------------|
| uart_driver                | include/linux/serial_core.h     | Representation of a low-level<br>UART driver.                               |
| uart_port                  | include/linux/serial_core.h     | Representation of a UART port.                                              |
| uart_ops                   | include/linux/serial_core.h     | Entry points supported by UART<br>drivers.                                  |
| platform_device            | include/linux/platform_device.h | Representation of a platform<br>device.                                     |
| platform_driver            | include/linux/platform_device.h | Representation of a platform<br>driver.                                     |
| tty_struct                 | include/linux/tty.h             | State information about a tty.                                              |
| tty_bufhead,<br>tty_buffer | include/linux/tty.h             | These two structures implement<br>the flip buffer associated with a<br>tty. |
| tty_driver                 | include/linux/tty_driver.h      | Programming interface between<br>tty drivers and higher layers.             |
| tty_ldisc                  | include/linux/tty_ldisc.h       | Entry points supported by a line<br>discipline.                             |

**Table 6.4. Summary of Kernel Programming Interfaces**

| Kernel Interface                                                                          | Location                      | Description                                                            |
|-------------------------------------------------------------------------------------------|-------------------------------|------------------------------------------------------------------------|
| uart_register_driver()                                                                    | drivers/serial/sderial_core.c | Registers a UART driver with the<br>serial core                        |
| uart_add_one_port()                                                                       | drivers/serial/sderial_core.c | Registers a UART port supported<br>by the UART driver                  |
| uart_unregister_driver()                                                                  | drivers/serial/sderial_core.c | Removes a UART driver from the<br>serial core                          |
| platform_device register()<br>platform_device_register_simple()<br>platform_add_devices() | drivers/base/platform.c       | Registers a platform device                                            |
| platform_device_unregister()                                                              | drivers/base/platform.c       | Unregisters a platform device                                          |
| platform_driver_register()/<br>platform_driver_unregister()                               | drivers/base/platform.c       | Registers/unregisters a platform<br>driver                             |
| tty_insert_flip_char()                                                                    | include/linux/tty_flip.h      | Adds a character to the tty flip<br>buffer                             |
| tty_flip_buffer_push()                                                                    | drivers/char/tty_io.c         | Queues a request to push the<br>flip buffer to the line discipline     |
| tty_register_driver()                                                                     | drivers/char/tty_io.c         | Registers a tty driver with the<br>serial core                         |
| tty_unregister_driver()                                                                   | drivers/char/tty_io.c         | Removes a tty driver from the<br>serial core                           |
| tty_register_ldisc()                                                                      | drivers/char/tty_io.c         | Creates a line discipline by<br>registering prescribed entry<br>points |
| tty_unregister_ldisc()                                                                    | drivers/char/tty_io.c         | Removes a line discipline from<br>the serial core                      |

Some serial data transfer scenarios are complex. You might need to mix and match different serial layer blocks, as you saw in Figure 6.3. Some situations may necessitate a data path passing through multiple line disciplines. For example, setting up a dialup connection over Bluetooth involves the movement of data through the HCI line discipline as well as the PPP line discipline. If you can, establish such a connection and step through the code flow using a kernel debugger.

Look at the implementation of mousedev (*drivers/input/mousedev.c*) for a complete example.
