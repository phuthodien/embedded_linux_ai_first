# **Chapter 8. The Inter-Integrated Circuit Protocol**

| In This Chapter                         |     |
|-----------------------------------------|-----|
| What's I2C/SMBus?                       | 234 |
| 2C Core<br>I                            | 235 |
| Bus Transactions                        | 237 |
| Device Example: EEPROM                  | 238 |
| Device Example: Real Time Clock         | 247 |
| I2C-dev                                 | 251 |
| Hardware Monitoring Using LM<br>Sensors | 251 |
| The Serial Peripheral Interface Bus     | 251 |
| The 1-Wire Bus                          | 254 |
| Debugging                               | 254 |
| Looking at the Sources                  | 255 |

The Inter-Integrated Circuit, or I2C (pronounced *I squared C*) bus and its subset, the *System Management Bus* (SMBus), are synchronous serial interfaces that are ubiquitous on desktops and embedded devices. Let's find out how the kernel supports I2C/SMBus host adapters and client devices by implementing example drivers to access an I2C EEPROM and an I2C RTC. And before wrapping up this chapter, let's also peek at two other serial interfaces supported by the kernel: the *Serial Peripheral Interface* or SPI (often pronounced *spy*) bus and the 1-wire bus.

All these serial interfaces (I2C, SMBus, SPI, and 1-wire) share two common characteristics:

- The amount of data exchanged is small.
- The required data transfer rate is low.

# **What's I2C/SMBus?**

I <sup>2</sup>C is a serial bus that is widely used in desktops and laptops to interface the processor with devices such as EEPROMs, audio codecs, and specialized chips that monitor parameters such as temperature and power-supply voltage. In addition, I2C is widely used in embedded devices to communicate with RTCs, smart battery circuits, multiplexers, port expanders, optical transceivers, and other similar devices. Because I2C is supported by a large number of microcontrollers, there are loads of cheap I2C devices available in the market today.

I <sup>2</sup>C and SMBus are master-slave protocols where communication takes place between a *host adapter* (or *host controller*) and *client devices* (or *slaves*). The host adapter is usually part of the South Bridge chipset on desktops and part of the microcontroller on embedded devices. Figure 8.1 shows an example I2C bus on PCcompatible hardware.

**Figure 8.1. I2C/SMBus on PC-compatible hardware.**

```
+-----------+
| Processor |
+-----------+
      ^
      |
      v
+-------------+
| North Bridge|
+-------------+
      ^    |---------->I²C/SMBus Host Adapter
      |    |                    +--------+ 
      v    |               |----| EEPROM |-----------------------------|
+-------------+  I2C       |    +--------+                             |
| South Bridge|--------------------------------------------------------|-------------  (bus)
+-------------+         |Control            |                          v
      |                 |                   |                   I²C/SMBus Client Devices
      |  AC'97          |                   |                      ^   ^
      |              +-------+      +----------------------+       |   |
      |--------------| CODEC |      | Temperature Sensor   |-------|   |
                     +-------+      |        Chip          |           |
                          |         +----------------------+           |
                          |____________________________________________|
```

I <sup>2</sup>C and its subset SMBus are 2-wire interfaces originally developed by Philips and Intel, respectively. The two wires are *clock* and bidirectional *data*, and the corresponding lines are called *Serial CLock* (SCL) and *Serial DAta* (SDA). Because the I2C bus needs only a pair of wires, it consumes less space on the circuit board. However, the supported bandwidths are also low. I2C allows up to 100Kbps in the standard mode and 400Kbps in a fast mode. (SMBus supports only up to 100Kbps, however.) The bus is thus suitable only for slow peripherals. Even though I2C supports bidirectional exchange, the communication is half duplex because there is only a single data wire.

I <sup>2</sup>C and SMBus devices own 7-bit addresses. The protocol also supports 10-bit addresses, but many devices respond only to 7-bit addressing, which yields a maximum of 127 devices on the bus. Due to the master-slave nature of the protocol, device addresses are also known as *slave addresses*.

#### **I <sup>2</sup>C Core**

The I2C core is a code base consisting of routines and data structures available to host adapter drivers and client drivers. Common code in the core makes the driver developer's job easier. The core also provides a level of indirection that renders client drivers independent of the host adapter, allowing them to work unchanged even if the client device is used on a board that has a different I2C host adapter. This philosophy of a core layer and its attendant benefits is also relevant for many other device driver classes in the kernel, such as PCMCIA, PCI, and USB.

In addition to the core, the kernel I2C infrastructure consists of the following:

- Device drivers for I2C host adapters. They fall in the realm of bus drivers and usually consist of an *adapter* driver and an *algorithm* driver. The former uses the latter to talk to the I2C bus.
- Device drivers for I2C client devices.
- i2c-dev, which allows the implementation of user mode I2C client drivers.

You are more likely to implement client drivers than adapter or algorithm drivers because there are a lot more I <sup>2</sup>C devices than there are I2C host adapters. So, we will confine ourselves to client drivers in this chapter.

Figure 8.2 illustrates the Linux I2C subsystem. It shows I2C kernel modules talking to a host adapter and a client device on an I2C bus.

**Figure 8.2. The Linux I2C subsystem.**

[View full size image]

```
                          User Space
           +--------------------+                             +--------------------------+
           |   User Application |                             | I²C User Mode            |
           +--------------------+                             |   Device Driver          |
                     |                                        +--------------------------+
                     |                                                   ^
---------------------|----------Kernel Space / User Space ---------------|----------------------------------------
                     v                                                   |
                 +-----------+                                           |
                 | /sys,/dev |                                           |
                 +-----------+                                           |
                     ^                                                   |
                     |                                                   |
                     v                                                   v
              +----------------+          +----------------+       +----------------+
              | I²C Client     |<---------|   i2c-core     |<----->|   i2c-dev      |
              |   Driver       |          +----------------+       +----------------+
              +----------------+                |
                      ^                         V
                      |                 +-------------------+
                      |                 | I²C Adapter/Algo  |
                      |                 |       Driver      |
                      |                 +-------------------+
                      |                             ^
----------------------|-----------------------------|------------- Hardware / Kernel Space ---------------------------
                      v                             v
             +-------------+     I²C Bus      +--------------------+
             | I²C Device  |==================| I²C Host Controller|
             +-------------+                  +--------------------+
```

Because SMBus is a subset of I2C, using only SMBus commands to talk to your device yields a driver that works with both SMBus and I2C adapters. Table 8.1 lists the SMBus-compatible data transfer routines provided by the I <sup>2</sup>C core.

**Table 8.1. SMBus-Compatible Data Access Functions Provided by the I2C Core**

| Function                    | Purpose                                                                                                                                |
|-----------------------------|----------------------------------------------------------------------------------------------------------------------------------------|
| i2c_smbus_read_byte()       | Reads a single byte from the device without<br>specifying a location offset. Uses the same offset<br>as the previously issued command. |
| i2c_smbus_write_byte()      | Sends a single byte to the device at the same<br>memory offset as the previously issued<br>command.                                    |
| i2c_smbus_write_quick()     | Sends a single bit to the device (in place of the<br>Rd/Wr bit shown in Listing 8.1).                                                  |
| i2c_smbus_read_byte_data()  | Reads a single byte from the device at a specified<br>offset.                                                                          |
| i2c_smbus_write_byte_data() | Sends a single byte to the device at a specified<br>offset.                                                                            |
| i2c_smbus_read_word_data()  | Reads 2 bytes from the specified offset.                                                                                               |
| i2c_smbus_write_word_data() | Sends 2 bytes to the specified offset.                                                                                                 |

| Function                     | Purpose                                                         |
|------------------------------|-----------------------------------------------------------------|
| i2c_smbus_read_block_data()  | Reads a block of data from the specified offset.                |
| i2c_smbus_write_block_data() | Sends a block of data (<= 32 bytes) to the<br>specified offset. |

# **Bus Transactions**

Before implementing an example driver, let's get a better understanding of the I2C protocol by peering at the wires through a magnifying glass. Listing 8.1 shows a code snippet that talks to an I2C EEPROM and the corresponding transactions that occur on the bus. The transactions were captured by connecting an I2C bus analyzer while running the code snippet. The code uses user mode I2C functions. (We talk more about user mode I2C programming in Chapter 19, "Drivers in User Space.")

**Listing 8.1. Transactions on the I2C Bus**

```
/* ... */
/*
 * Connect to the EEPROM. 0x50 is the device address.
 * smbus_fp is a file pointer into the SMBus device.
 */
ioctl(smbus_fp, 0x50, slave);
/* Write a byte (0xAB) at memory offset 0 on the EEPROM */
i2c_smbus_write_byte_data(smbus_fp, 0, 0xAB);
/*
 * This is the corresponding transaction observed
 * on the bus after the write:
 * S 0x50 Wr [A] 0 [A] 0xAB [A] P
 *
 * S is the start bit, 0x50 is the 7-bit slave address (0101000b),
 * Wr is the write command (0b), A is the Accept bit (or
 * acknowledgment) received by the host from the slave, 0 is the
 * address offset on the slave device where the byte is to be
 * written, 0xAB is the data to be written, and P is the stop bit.
 * The data enclosed within [] is sent from the slave to the
 * host, while the rest of the bits are sent by the host to the
 * slave.
 */
/* Read a byte from offset 0 on the EEPROM */
res = i2c_smbus_read_byte_data(smbus_fp, 0);
/*
 * This is the corresponding transaction observed
 * on the bus after the read:
 * S 0x50 Wr [A] 0 [A] S 0x50 Rd [A] [0xAB] NA P
 *
 * The explanation of the bits is the same as before, except that
 * Rd stands for the Read command (1b), 0xAB is the data received
 * from the slave, and NA is the Reverse Accept bit (or the
 * acknowledgment sent by the host to the slave).
 */
```

# **Device Example: EEPROM**

Our first example client device is an EEPROM sitting on an I2C bus, as shown in Figure 8.1. Almost all laptops and desktops have such an EEPROM for storing BIOS configuration information. The example EEPROM has two memory banks. The driver exports */dev* interfaces corresponding to each bank: */dev/eep/0* and */dev/eep/1.* Applications operate on these nodes to exchange data with the EEPROM.

Each I2C/SMBus client device is assigned a slave address that functions as the device identifier. The EEPROM in the example answers to two slave addresses, SLAVE\_ADDR1 and SLAVE\_ADDR2, one per bank.

The example driver uses I2C commands that are compatible with SMBus, so it works with both I2C and SMBus EEPROMs.

## **Initializing**

As is the case with all driver classes, I2C client drivers also own an init() entry point. Initialization entails allocating data structures, registering the driver with the I2C core, and hooking up with sysfs and the Linux device model. This is done in Listing 8.2.

#### **Listing 8.2. Initializing the EEPROM Driver**

```
/* Driver entry points */
static struct file_operations eep_fops = {
 .owner = THIS_MODULE,
 .llseek = eep_llseek,
 .read = eep_read,
 .ioctl = eep_ioctl,
 .open = eep_open,
 .release = eep_release,
 .write = eep_write,
};
static dev_t dev_number; /* Allotted Device Number */
static struct class *eep_class; /* Device class */
/* Per-device client data structure for each
 * memory bank supported by the driver
 */
struct eep_bank {
 struct i2c_client *client; /* I2C client for this bank */
 unsigned int addr; /* Slave address of this bank */
 unsigned short current_pointer; /* File pointer */
 int bank_number; /* Actual memory bank number */
 /* ... */ /* Spinlocks, data cache for
 slow devices,.. */
};
#define NUM_BANKS 2 /* Two supported banks */
#define BANK_SIZE 2048 /* Size of each bank */
struct ee_bank *ee_bank_list; /* List of private data
 structures, one per bank */
```

```
/*
 * Device Initialization
 */
int __init
eep_init(void)
{
 int err, i;
 /* Allocate the per-device data structure, ee_bank */
 ee_bank_list = kmalloc(sizeof(struct ee_bank)*NUM_BANKS,
 GFP_KERNEL);
 memset(ee_bank_list, 0, sizeof(struct ee_bank)*NUM_BANKS);
 /* Register and create the /dev interfaces to access the EEPROM
 banks. Refer back to Chapter 5, "Character Drivers" for
 more details */
 if (alloc_chrdev_region(&dev_number, 0,
 NUM_BANKS, "eep") < 0) {
 printk(KERN_DEBUG "Can't register device\n");
 return -1;
 }
 eep_class = class_create(THIS_MODULE, DEVICE_NAME);
 for (i=0; i < NUM_BANKS;i++) {
 /* Connect the file operations with cdev */
 cdev_init(&ee_bank[i].cdev, &ee_fops);
 /* Connect the major/minor number to the cdev */
 if (cdev_add(&ee_bank[i].cdev, (dev_number + i), 1)) {
 printk("Bad kmalloc\n");
 return 1;
 }
 class_device_create(eep_class, NULL, (dev_number + i),
 NULL, "eeprom%d", i);
 }
 /* Inform the I2C core about our existence. See the section
 "Probing the Device" for the definition of eep_driver */
 err = i2c_add_driver(&eep_driver);
 if (err) {
 printk("Registering I2C driver failed, errno is %d\n", err);
 return err;
 }
 printk("EEPROM Driver Initialized.\n");
 return 0;
}
```

Listing 8.2 initiates creation of the device nodes, but to complete their production, add the following to an appropriate rule file under */etc/udev/rules.d/*:

```
KERNEL:"eeprom[0-1]*", NAME="eep/%n"
```

This creates */dev/eep/0* and */dev/eep/1* in response to reception of the corresponding uevents from the kernel. A user mode program that needs to read from the nth memory bank can then operate on */dev/eep/n*.

Listing 8.3 implements the open() method for the EEPROM driver. The kernel calls eep\_open() when an application opens */dev/eep/X.* eep\_open() stores the per-device data structure in a private area so that it's directly accessible from the rest of the driver methods.

#### **Listing 8.3. Opening the EEPROM Driver**

```
 int
eep_open(struct inode *inode, struct file *file)
{
 /* The EEPROM bank to be opened */
 n = MINOR(file->f_dentry->d_inode->i_rdev);
 file->private_data = (struct ee_bank *)ee_bank_list[n];
 /* Initialize the fields in ee_bank_list[n] such as
 size, slave address, and the current file pointer */
 /* ... */
}
```

## **Probing the Device**

The I2C client driver, in partnership with the host controller driver and the I2C core, attaches itself to a slave device as follows:

During initialization, it registers a probe() method, which the I2C core invokes when an associated host controller is detected. In Listing 8.2, eep\_init() registered eep\_probe() by invoking i2c\_add\_driver(): **1.**

```
static struct i2c_driver eep_driver =
{
 .driver = {
 .name = "EEP", /* Name */
 },
 .id = I2C_DRIVERID_EEP, /* ID */
 .attach_adapter = eep_probe, /* Probe Method */
 .detach_client = eep_detach, /* Detach Method */
};
i2c_add_driver(&eep_driver); `
```

The driver identifier, I2C\_DRIVERID\_EEP, should be unique for the device and should be defined in *include/linux/i2c-id.h*.

When the core calls the driver's probe() method signifying the presence of a host adapter, it, in turn, invokes i2c\_probe() with arguments specifying the addresses of the slave devices that the driver is responsible for and an associated attach() routine. **2.**

Listing 8.4 implements eep\_probe(), the probe() method of the EEPROM driver. normal\_i2c specifies the EEPROM bank addresses and is populated as part of the i2c\_client\_address\_data structure. Additional fields in this structure can be used to request finer addressing control. You can ask the I2C core to ignore a range of addresses using the ignore field. Or you may use the probe field to specify (*adapter, slave address*) pairs if you want to bind a slave address to a particular host adapter. This will be useful, for example, if your processor supports two I2C host adapters, and you have an EEPROM on bus 1 and a temperature sensor on bus 2, both answering to the same slave address.

- The host controller walks the bus looking for the slave devices specified in Step 2. To do this, it generates a bus transaction such as S SLAVE\_ADDR Wr, where S is the start bit, SLAVE\_ADDR is the associated 7-bit slave address as specified in the device's datasheet, and Wr is the write command, as described in the section "Bus Transactions." If a working slave device exists on the bus, it'll respond by sending an acknowledgment bit ([A]). **3.**
- If the host adapter detects a slave in Step 3, the I2C core invokes the attach() routine supplied via the third argument to i2c\_probe() in Step 2. For the EEPROM driver, this routine is eep\_attach(), which registers a per-device client data structure, as shown in Listing 8.5. If your device expects an initial programming sequence (for example, registers on an I2C Digital Visual Interface transmitter chip have to be initialized before the chip can start functioning), perform those operations in this routine. **4.**

### **Listing 8.4. Probing the Presence of EEPROM Banks**

```
#include <linux/i2c.h>
/* The EEPROM has two memory banks having addresses SLAVE_ADDR1
 * and SLAVE_ADDR2, respectively
 */
static unsigned short normal_i2c[] = {
 SLAVE_ADDR1, SLAVE_ADDR2, I2C_CLIENT_END
};
static struct i2c_client_address_data addr_data = {
 .normal_i2c = normal_i2c,
 .probe = ignore,
 .ignore = ignore,
 .forces = ignore,
};
static int
eep_probe(struct i2c_adapter *adapter)
{
 /* The callback function eep_attach(), is shown
 * in Listing 8.5
 */
 return i2c_probe(adapter, &addr_data, eep_attach);
}
```

**Listing 8.5. Attaching a Client**

```
int
eep_attach(struct i2c_adapter *adapter, int address, int kind)
{
 static struct i2c_client *eep_client;
 eep_client = kmalloc(sizeof(*eep_client), GFP_KERNEL);
 eep_client->driver = &eep_driver; /* Registered in Listing 8.2 */
 eep_client->addr = address; /* Detected Address */
 eep_client->adapter = adapter; /* Host Adapter */
 eep_client->flags = 0;
 strlcpy(eep_client->name, "eep", I2C_NAME_SIZE);
 /* Populate fields in the associated per-device data structure */
 /* ... */
 /* Attach */
 i2c_attach_client(new_client);
}
```

## **Checking Adapter Capabilities**

Each host adapter might be limited by a set of constraints. An adapter might not support all the commands that Table 8.1 contains. For example, it might allow the SMBus read\_word command but not the read\_block command. A client driver has to check whether a command is supported by the adapter before using it.

The I2C core provides two functions to do this:

- **1.** i2c\_check\_functionality() checks whether a particular function is supported.
- **2.** i2c\_get\_functionality() returns a mask containing all supported functions.

See *include/linux/i2c.h* for the list of possible functionalities.

## **Accessing the Device**

To read data from the EEPROM, first glean information about its invocation thread from the private data field associated with the device node. Next, use SMBus-compatible data access routines provided by the I2C core (Table 8.1 shows the available functions) to read the data. Finally, send the data to user space and increment the internal file pointer so that the next read()/write() operation starts from where the last one ended. These steps are performed by Listing 8.6. The listing omits sanity and error checks for convenience.

#### **Listing 8.6. Reading from the EEPROM**

```
ssize_t
eep_read(struct file *file, char *buf,
 size_t count, loff_t *ppos)
{
 int i, transferred, ret, my_buf[BANK_SIZE];
 /* Get the private client data structure for this bank */
 struct ee_bank *my_bank =
 (struct ee_bank *)file->private_data;
 /* Check whether the smbus_read_word() functionality is
 supported */
 if (i2c_check_functionality(my_bank->client,
 I2C_FUNC_SMBUS_READ_WORD_DATA)) {
 /* Read the data */
 while (transferred < count) {
 ret = i2c_smbus_read_word_data(my_bank->client,
 my_bank->current_pointer+i);
 my_buf[i++] = (u8)(ret & 0xFF);
 my_buf[i++] = (u8)(ret >> 8);
 transferred += 2;
 }
 /* Copy data to user space and increment the internal
 file pointer. Sanity checks are omitted for simplicity */
 copy_to_user(buffer, (void *)my_buf, transferred);
 my_bank->current_pointer += transferred;
 }
 return transferred;
}
```

Writing to the device is done similarly, except that an i2c\_smbus\_write\_XXX() function is used instead.

Some EEPROM chips have a Radio Frequency Identification (RFID) transmitter to wirelessly transmit stored information. This is used to automate supply-chain processes such as inventory monitoring and asset tracking. Such EEPROMs usually implement safeguards via an access protection bank that controls access permissions to the data banks. In such cases, the driver has to wiggle corresponding bits in the access protection bank before it can operate on the data banks.

To access the EEPROM banks from user space, develop applications that operate on */dev/eep/n.* To dump the contents of the EEPROM banks, use od:

```
bash> od –a /dev/eep/0
0000000 S E R # dc4 ff soh R P nul nul nul nul nul nul nul
0000020 @ 1 3 R 1 1 5 3 Z J 1 V 1 L 4 6
0000040 5 1 0 H sp 1 S 2 8 8 8 7 J U 9 9
```

```
0000060 H 0 0 6 6 nul nul nul bs 3 8 L 5 0 0 3
0000100 Z J 1 N U B 4 6 8 6 V 7 nul nul nul nul
0000120 nul nul nul nul nul nul nul nul nul nul nul nul nul nul nul nul
*
0000400
```

As an exercise, take a stab at modifying the EEPROM driver to create */sys* interfaces to the EEPROM banks rather than the */dev* interfaces. You may reuse code from Listing 5.7, "Using Sysfs to Control the Parallel LED Board," in Chapter 5 to help you in this endeavor.

## **More Methods**

To obtain a fully functional driver, you need to add a few remaining entry points. These are hardly different from those of normal character drivers discussed in Chapter 5, so the code listings are not shown:

- To support the lseek() system call that assigns a new value to the internal file pointer, implement the llseek() driver method. The internal file pointer stores state information about EEPROM access.
- To verify data integrity, the EEPROM driver can support an ioctl() method to adjust and verify checksums of stored data.
- The poll() and fsync() methods are not relevant for the EEPROM.
- If you choose to compile the driver as a module, you have to supply an exit() method to unregister the device and clean up client-specific data structures. Unregistering the driver from the I2C core is a oneliner:

```
i2c_del_driver(&eep_driver);
```

# **Device Example: Real Time Clock**

Let's now take the example of an RTC chip connected to an embedded controller over the I2C bus. The connection diagram is shown in Figure 8.3.

```
+-------------------+         I²C Bus (SDA/SCL)          +---------+
| Embedded          | <================================> |   RTC   |
| Controller        |                                    +---------+
+-------------------+                                         |
                                                              | Battery Backup
                                                              |
                                                             ===
                                                             GND

                             [RTC also has a CLK pin output at the bottom labeled "CLK"]
```

**Figure 8.3. An I2C RTC on an embedded system.**

Assume that the I2C slave address of the RTC is 0x60 and that its register space is organized as shown in Table 8.2.

**Table 8.2. Register Layout on the I2C RTC**

| Register Name   | Description                | Offset |  |
|-----------------|----------------------------|--------|--|
| RTC_HOUR_REG    | Hour counter               | 0x0    |  |
| RTC_MINUTE_REG  | Minute counter             | 0x1    |  |
| RTC_SECOND_REG  | Second counter             | 0x2    |  |
| RTC_STATUS_REG  | Flags and interrupt status | 0x3    |  |
| RTC_CONTROL_REG | Enable/disable RTC         | 0x4    |  |

Let's base our driver for this chip on the EEPROM driver discussed previously. We will take the I2C client driver architecture, slave registration, and I2C core functions for granted and implement only the code that communicates with the RTC.

When the I2C core detects a device having the RTC's slave address (0x60) on the I2C bus, it invokes myrtc\_attach(). The invocation train is similar to that for eep\_attach() in Listing 8.5. Assume that you have to perform the following chip initializations in myrtc\_attach():

**1.** Clear the RTC status register (RTC\_STATUS\_REG).

Start the RTC (if it is not already running) by turning on appropriate bits in the RTC control register (RTC\_CONTROL\_REG). **2.**

To do this, let's build an i2c\_msg and generate I2C transactions on the bus using i2c\_transfer(). This transfer mechanism is exclusive to I2C and is not SMBus-compatible. To write to the two RTC registers referred to previously, you have to build two i2c\_msg messages. The first message sets the register offset. In our case, it's 3, the offset of RTC\_STATUS\_REG. The second message carries the desired number of bytes to the specified offset. In this context, it ferries 2 bytes, one each to RTC\_STATUS\_REG and RTC\_CONTROL\_REG.

```
```

```
#include <linux/i2c.h> /* For struct i2c_msg */
int
myrtc_attach(struct i2c_adapter *adapter, int addr, int kind)
{
 u8 buf[2];
 int offset = RTC_STATUS_REG; /* Status register lives here */
 struct i2c_msg rtc_msg[2];
 /* Write 1 byte of offset information to the RTC */
 rtc_msg[0].addr = addr; /* Slave address. In our case,
 this is 0x60 */
 rtc_msg[0].flags = I2C_M_WR; /* Write Command */
 rtc_msg[0].buf = &offset; /* Register offset for
 the next transaction */
 rtc_msg[0].len = 1; /* Offset is 1 byte long */
 /* Write 2 bytes of data (the contents of the status and
 control registers) at the offset programmed by the previous
 i2c_msg */
 rtc_msg[1].addr = addr; /* Slave address */
 rtc_msg[1].flags = I2C_M_WR; /* Write command */
 rtc_msg[1].buf = &buf[0]; /* Data to be written to control
 and status registers */
 rtc_msg[1].len = 2; /* Two register values */
 buf[0] = 0; /* Zero out the status register */
 buf[1] |= ENABLE_RTC; /* Turn on control register bits
 that start the RTC */
 /* Generate bus transactions corresponding to the two messages */
 i2c_transfer(adapter, rtc_msg, 2);
 /* ... */
 printk("My RTC Initialized\n");
}
```

Now that the RTC is initialized and ticking, you can glean the current time by reading the contents of RTC\_HOUR\_REG, RTC\_MINUTE\_REG, and RTC\_SECOND\_REG. This is done as follows:

```
#include <linux/rtc.h> /* For struct rtc_time */
int
```

```
myrtc_gettime(struct i2c_client *client, struct rtc_time *r_t)
{
 u8 buf[3]; /* Space to carry hour/minute/second */
 int offset = 0; /* Time-keeping registers start at offset 0 */
 struct i2c_msg rtc_msg[2];
 /* Write 1 byte of offset information to the RTC */
 rtc_msg[0].addr = addr; /* Slave address */
 rtc_msg[0].flags = 0; /* Write Command */
 rtc_msg[0].buf = &offset; /* Register offset for
 the next transaction */
 rtc_msg[0].len = 1; /* Offset is 1 byte long */
 /* Read current time by getting 3 bytes of data from offset 0
 (i.e., from RTC_HOUR_REG, RTC_MINUTE_REG, and RTC_SECOND_REG) */
 rtc_msg[1].addr = addr; /* Slave address */
 rtc_msg[1].flags = I2C_M_RD; /* Read command */
 rtc_msg[1].buf = &buf[0]; /* Data to be read from hour, minute
 and second registers */
 rtc_msg[1].len = 3; /* Three registers to read */
 /* Generate bus transactions corresponding to the above
 two messages */
 i2c_transfer(adapter, rtc_msg, 2);
 /* Read the time */
 r_t->tm_hour = BCD2BIN(buf[0]); /* Hour */
 r_t->tm_min = BCD2BIN(buf[1]); /* Minute */
 r_t->tm_sec = BCD2BIN(buf[2]); /* Second */
 return(0);
}
```

myrtc\_gettime() implements the bus-specific bottom layer of the RTC driver. The top layer of the RTC driver should conform to the kernel RTC API, as discussed in the section "RTC Subsystem" in Chapter 5. The advantage of this scheme is that applications can run unchanged irrespective of whether your RTC is internal to the South Bridge of a PC or externally connected to an embedded controller as in this example.

RTCs usually store time in *Binary Coded Decimal* (BCD), where each nibble represents a number between 0 and 9 (rather than between 0 and 15). The kernel provides a macro called BCD2BIN() to convert from BCD encoding to decimal and BIN2BCD() to perform the reverse operation. myrtc\_gettime() uses the former while reading time from RTC registers.

Look at *drivers/rtc/rtc-ds1307.c* for an example RTC driver that handles the -Dallas/Maxim DS13XX series of I2C RTC chips.

Being a 2-wire bus, the I2C bus does not have an interrupt request line that slave devices can assert, but some I <sup>2</sup>C host adapters have the capability to interrupt the processor to signal completion of data-transfer requests. This interrupt-driven operation is, however, transparent to I2C client drivers and is hidden inside the service routines offered by the I2C core. Assuming that the I2C host controller that is part of the embedded SoC in Figure 8.3 has the capability to interrupt the processor, the invocation of i2c\_transfer() in myrtc\_attach() might be doing the following under the hood:

Build a transaction corresponding to rtc\_msg[0] and write it to the bus using the services of the host

controller device driver.

- Wait until the host controller asserts a transmit complete interrupt signaling that rtc\_msg[0] is now on the wire.
- Inside the interrupt handler, look at the I2C host controller status register to see whether an acknowledgment has been received from the RTC slave.
- Return error if the host controller status and control registers indicate that all's not well.
- Repeat the same for rtc\_msg[1].

# **I2C-dev**

Sometimes, when you need to enable support for a large number of slow I2C devices, it makes sense to drive them wholly from user space. The I2C layer supports a driver called i2c-dev to achieve this. Fast forward to the section "User Mode I2C" in Chapter 19 for an example that implements a user mode I2C driver using i2c-dev.

# **Hardware Monitoring Using LM-Sensors**

The *LM-Sensors* project, hosted at www.lm-sensors.org, brings hardware-monitoring capabilities to Linux. Several computer systems use sensor chips to monitor parameters such as temperature, power supply voltage, and fan speed. Periodically tracking these parameters can be critical. A blown CPU fan can manifest in the form of strange and random software problems. Imagine the consequences if the system is a medical grade device!

LM-Sensors comes to the rescue with device drivers for many sensor chips, a utility called *sensors* to generate a health report, and a script called *sensors-detect* to scan your system and help you generate appropriate configuration files.

Most chips that offer hardware monitoring interface to the CPU via I2C/SMBus. Device drivers for such devices are normal I2C client drivers but reside in the *drivers/hwmon/* directory, rather than *drivers/i2c/chips/*. An example is National Semiconductor's LM87 chip, which can monitor multiple voltages, temperatures, and fans. Have a look at *drivers/hwmon/lm87.c* for its driver implementation. I2C driver IDs from 1000 to 1999 are reserved for sensor chips (look at *include/linux/i2c-id.h*).

Several sensor chips interface to the CPU via the ISA/LPC bus rather than I2C/SMBus. Others emit analog output that reaches the CPU through an *Analog-to-Digital Converter* (ADC). Drivers for such chips share the *drivers/hwmon/* directory with I2C sensor drivers. An example of a non-I2C sensor driver is *drivers/hwmon/hdaps.c*, the driver for the accelerometer present in several IBM/Lenovo laptops that we discussed in Chapter 7, "Input Drivers." Another example of a non-I2C sensor is the Winbond 83627HF Super I/O chip, which is driven by *drivers/hwmon/w83627hf.c*.

# **The Serial Peripheral Interface Bus**

The *Serial Peripheral Interface* (SPI) bus is a serial master-slave interface similar to I <sup>2</sup>C and comes built in on many microcontrollers. It uses four wires (compared to two on I <sup>2</sup>C): *Serial CLocK* (SCLK), *Chip Select* (CS), *Master Out Slave In* (MOSI), and *Master In Slave Out* (MISO). MOSI is used for shifting data into the slave device, and MISO is used for shifting data out of the slave device. Because the SPI bus has dedicated wires for transmitting and receiving data, it can operate in full-duplex mode, unlike the I2C bus. The typical speed of operation of SPI is in the low-megahertz range, unlike the mid-kilohertz range on I2C, so the former yields higher throughput.

SPI peripherals available in the market today include *Radio Frequency* (RF) chips, smart card interfaces, EEPROMs, RTCs, touch sensors, and ADCs.

The kernel provides a core API for exchanging messages over the SPI bus. A typical SPI client driver does the following:

Registers probe() and remove() methods with the SPI core. Optionally registers suspend() and resume() methods: **1.**

```
 #include <linux/spi/spi.h>
 static struct spi_driver myspi_driver = {
 .driver = {
 .name = "myspi",
 .bus = &spi_bus_type,
 .owner = THIS_MODULE,
 },
 .probe = myspidevice_probe,
 .remove = __devexit_p(myspidevice_remove),
 }
 spi_register_driver(&myspi_driver);
```

The SPI core creates an spi\_device structure corresponding to this device and passes this as an argument when it invokes the registered driver methods.

Exchanges messages with the SPI device using access functions such as spi\_sync() and spi\_async(). The former waits for the operation to complete, whereas the latter asynchronously triggers invocation of a registered callback routine when message transfer completes. These data access routines are invoked from suitable places such as the SPI interrupt handler, a sysfs method, or a timer handler. The following code snippet illustrates SPI message submission: **2.**

```
#include <linux/spi/spi.h>
struct spi_device *spi; /* Representation of a
 SPI device */
struct spi_transfer xfer; /* Contains transfer buffer
 details */
struct spi_message sm; /* Sequence of spi_transfer
 segments */
u8 *command_buffer; /* Data to be transferred */
```

```
int len; /* Length of data to be
 transferred */
spi_message_init(&sm); /* Initialize spi_message */
xfer.tx_buf = command_buffer; /* Device-specific data */
xfer.len = len; /* Data length */
spi_message_add_tail(&xfer, &sm); /* Add the message */
spi_sync(spi, &sm); /* Blocking transfer request */
```

For an example SPI device, consider the ADS7846 touch-screen controller that we briefly discussed in Chapter 7. This driver does the following:

- Registers probe(), remove(), suspend(), and resume() methods with the SPI core using spi\_register\_driver(). **1.**
- The probe() method registers the driver with the input subsystem using input\_register\_device() and requests an IRQ using request\_irq(). **2.**
- The driver gathers touch coordinates from its interrupt handler using spi\_async(). This function triggers invocation of a registered callback routine when the SPI message transfer completes. **3.**
- The callback function in turn, reports touch coordinates and clicks via the input event interface, */dev/input/eventX*, using input\_report\_abs() and input\_report\_key(), as discussed in Chapter 7. Applications such as X Windows and gpm seamlessly work with the event interface and respond to touch input. **4.**

A driver that wiggles I/O pins to get them to talk a protocol is called a *bit-banging* driver. For an example SPI bit-banging driver, look at *drivers/spi/spi\_butterfly.c*, which is a driver to talk to DataFlash chips that are present on *Butterfly* boards built by Atmel around their AVR processor family. For this, connect your host system's parallel port to the AVR Butterfly using a specially made dongle and use the *spi\_butterfly* driver do the bit banging. Look at *Documentation/spi/butterfly* for a detailed description of this driver.

Currently there is no support for user space SPI drivers à la i2c-dev. You only have the option of writing a kernel driver to talk to your SPI device.

In the embedded world, you may come across solutions where the processor uses a companion chip that integrates various functions. An example is the Freescale MC13783 Power Management and Audio Component (PMAC) used in tandem with the ARM9-based i.MX27 controller. The PMAC integrates an RTC, a battery charger, a touch-screen interface, an ADC module, and an audio codec. The processor and the PMAC communicate over SPI. The SPI bus does not contain an interrupt line, so the PMAC has the capability to externally interrupt the processor using a GPIO pin configured for this purpose.

# **The 1-Wire Bus**

The *1-wire protocol* developed by Dallas/Maxim uses a 1-wire (or *w1*) bus that carries both power and signal; the return ground path is provided using some other means. It provides a simple way to interface with slow devices by reducing space, cost, and complexity. An example device that works using this protocol is the *ibutton* (www.ibutton.com), which is used for sensing temperature, carrying data, or holding unique IDs.

Another w1 chip that interfaces through a single port pin of an embedded controller is the DS2433 4kb 1-wire EEPROM from Dallas/Maxim. The driver for this chip, *drivers/w1/slaves/w1\_ds2433.c*, exports access to the EEPROM via a sysfs node.

The main data structures associated with a w1 device driver are w1\_family and w1\_family\_ops, both defined in *w1\_family.h*.

# **Debugging**

To collect I2C-specific debugging messages, turn on a relevant combination of *I2C Core debugging messages*, *I2C Algorithm debugging messages*, *I2C Bus debugging messages*, and *I2C Chip debugging messages* under *Device Drivers I2C Support* in the kernel configuration menu. Similarly, for SPI debugging, turn on *Debug Support for SPI drivers* under *Device Drivers SPI Support*

To understand the flow of I2C packets on the bus, connect an I2C bus analyzer to your board as we did while running Listing 8.1. The lm-sensors package contains a tool called i2cdump that dumps register contents of devices on the I2C bus.

There is a mailing list dedicated to Linux I2C at <http://lists.lm-sensors.org/mailman/listinfo/i2c>.

# **Looking at the Sources**

In the 2.4 kernel source tree, a single directory (*drivers/i2c/*) contained all the I2C/SMBus-related sources. The I <sup>2</sup>C code in 2.6 kernels is organized hierarchically: The *drivers/i2c/busses/* directory contains adapter drivers, the *drivers/i2c/algos/* directory has algorithm drivers, and the *drivers/i2c/chips/* directory contains client drivers. You can find client drivers in other regions of the source tree, too. The *drivers/sound/* directory, for example, includes drivers for audio chipsets that use an I2C control interface. Take a look inside the *Documentation/i2c/* directory for tips and more examples.

Kernel SPI service functions live in *drivers/spi/spi.c.* The SPI driver for the ADS7846 touch controller is implemented in *drivers/input/touchscreen/ads7846.c.* The MTD subsystem discussed in Chapter 17, "Memory Technology Devices," implements drivers for SPI flash chips. An example is *drivers/mtd/devices/mtd\_dataflash.c*, the driver to access Atmel DataFlash SPI chips.

The *drivers/w1/* directory contains kernel support for the w1 protocol. Drivers for the host controller side of the w1 interface live in *drivers/w1/masters/*, and drivers for w1 slaves reside in *drivers/w1/slaves/*.

Table 8.3 summarizes the main data structures used in this chapter and their location in the kernel tree. Table 8.4 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 8.3. Summary of Data Structures**

| Data Structure          | Location                | Description                                                                            |
|-------------------------|-------------------------|----------------------------------------------------------------------------------------|
| i2c_driver              | include/linux/i2c.h     | Representation of an I2C driver                                                        |
| i2c_client_address_data | include/linux/i2c.h     | Slave addresses that an I2C<br>client driver is responsible for                        |
| i2c_client              | include/linux/i2c.h     | Identifies a chip connected to an<br>2C bus<br>I                                       |
| i2c_msg                 | include/linux/i2c.h     | Information pertaining to a<br>transaction that you want to<br>generate on the I2C bus |
| spi_driver              | include/linux/spi/spi.h | Representation of an SPI driver                                                        |
| spi_device              | include/linux/spi/spi.h | Representation of an SPI device                                                        |
| spi_transfer            | include/linux/spi/spi.h | Details of an SPI transfer buffer                                                      |
| spi_message             | include/linux/spi/spi.h | Sequence of spi_transfer<br>segments                                                   |
| w1_family               |                         | drivers/w1/w1_family.h Representation of a w1 slave<br>driver                          |
| w1_family_ops           |                         | drivers/w1/w1_family.h A w1 slave driver's entry points                                |

**Table 8.4. Summary of Kernel Programming Interfaces**

| Kernel Interface                                   | Location                                      | Description                                                                                                                                                                                              |
|----------------------------------------------------|-----------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| i2c_add_driver()                                   | include/linux/l2c.h<br>drivers/l2c/l2c-core.c | Registers driver entry points with the I <sup>2</sup> C core.                                                                                                                                            |
| 2c_del_driver()                                    | drivers/l2c/l2c-core.c                        | Removes a driver from the I <sup>2</sup> C core.                                                                                                                                                         |
| 2c_probe()                                         | drivers/i2c/i2c-core.c                        | Specifies the addresses of slave devices that the driver is responsible for and an associated attach() routine to be invoked if one of the specified addresses is detected by the I <sup>2</sup> C core. |
| c_attach_client()                                  | drivers/i2c/i2c-core.c                        | Adds a new client to the list of clients serviced by the associated I <sup>2</sup> C host adapter.                                                                                                       |
| c_detach_client()                                  | drivers/i2c/i2c-core.c                        | Detaches an active client. Usually done when the client driver or the associated host adapter unregisters.                                                                                               |
| c_check_functionality()                            | include/linux/i2c.h                           | Verifies whether a particular function is supported by the host adapter.                                                                                                                                 |
| c_get_functionality()                              | include/linux/i2c.h                           | Obtains a mask containing all functions supported by the host adapter.                                                                                                                                   |
| c_add_adapter()                                    | drivers/i2c/i2c-core.c                        | Registers a host adapter.                                                                                                                                                                                |
| _del_adapter()                                     | drivers/i2c/i2c-core.c                        | Unregisters a host adapter.                                                                                                                                                                              |
| us-compatible I <sup>2</sup> C data<br>ss routines | drivers/i2c/i2c-core.c                        | See Table 8.1.                                                                                                                                                                                           |
| _transfer()                                        | drivers/i2c/i2c-core.c                        | Sends an i2c_msg over the I <sup>2</sup> C bus. This function is not SMBuscompatible.                                                                                                                    |
| i_register_driver()                                | drivers/spi/spi.c                             | Registers driver entry points with the SPI core.                                                                                                                                                         |
| _unregister_driver()                               | include/linux/spi/spi.h                       | Unregisters an SPI driver.                                                                                                                                                                               |
| _message_init()                                    | include/linux/spi/spi.h                       | Initializes an SPI message.                                                                                                                                                                              |
| _message_add_tail()                                | include/linux/spi/spi.h                       | Adds an SPI message to a transfer list.                                                                                                                                                                  |
| i_sync()                                           | drivers/spi/spi.c                             | Synchronously transfers data over the SPI bus. This function blocks until completion.                                                                                                                    |
| pi_async()                                         | include/linux/spi/spi.h                       | Asynchronously transfers data over the SPI bus using a completion callback mechanism.                                                                                                                    |
|                                                    |                                               |                                                                                                                                                                                                          |
