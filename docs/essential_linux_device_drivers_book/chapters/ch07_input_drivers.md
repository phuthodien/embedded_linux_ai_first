# **Chapter 7. Input Drivers**

# **In This Chapter**

Input Event Drivers

Input Device Drivers

Debugging

Looking at the Sources

The kernel's *input* subsystem was created to unify scattered drivers that handle diverse classes of data-input devices such as keyboards, mice, trackballs, joysticks, roller wheels, touch screens, accelerometers, and tablets. The input subsystem brings the following advantages to the table:

- Uniform handling of functionally similar input devices even when they are physically different. For example, all mice, such as PS/2, USB or Bluetooth, are treated alike.
- An easy *event* interface for dispatching input reports to user applications. Your driver does not have to create and manage */dev* nodes and related access methods. Instead, it can simply invoke input APIs to send mouse movements, key presses, or touch events upstream to user land. Applications such as X Windows work seamlessly over the event interfaces exported by the input subsystem.
- Extraction of common portions out of input drivers and a resulting abstraction that simplifies the drivers and introduces consistency. For example, the input subsystem offers a collection of low-level drivers called *serio* that provides access to input hardware such as serial ports and keyboard controllers.

Figure 7.1 illustrates the operation of the input subsystem. The subsystem contains two classes of drivers that work in tandem: *event* drivers and *device* drivers. Event drivers are responsible for interfacing with applications, whereas device drivers are responsible for low-level communication with input devices. The mouse event generator, *mousedev*, is an example of the former, and the PS/2 mouse driver is an example of the latter. Both event drivers and device drivers can avail the services of an efficient, bug-free, reusable core, which lies at the heart of the input subsystem.

```
                          Figure 7.1 – The input subsystem

        +-----------+   +-----+   +-----+   +-------------------+
        | X Windows |   | Qt  |   | gpm |   | Custom Application|
        +-----------+   +-----+   +-----+   +-------------------+
                ^         ^         ^                 ^
                |         |         |                 |
                +---------+---------+-----------------+
                              |
                   +------------------------------------+
                   |  /dev/input/mice                   |
                   |  /dev/input/eventX                 |
                   |  /dev/input/js                     |
                   +------------------------------------+           
                              ^                     ^               
                              |                     |               
------------------- User Space / Kernel Space ------|------------------
                                                    |               
                       +-------------+              |        +---------+         
                       |  Input Core |--------|     |        | Console |         
                       +-------------+        |     |        +---------+         
                             |                |     |              ^
                             |                |     |              |
                             |                |     |  +--------------------------+                 
                             |                |     |  |      Virtual Terminal    |           
                             |                |     |  |      (keyboard input)    |                
                             |                |     |  +--------------------------+                
                             |                |     |              ^
                             v                V     |              |
                  +-------------------+   +-----------------------------+
                  | Input Device      |   | Input Event Drivers         |
                  | Drivers           |   | (mousedev, evdev,           |
                  +-------------------+   |  joydev, keyboard)          |
                       ^                  +-----------------------------+
                       |
                       |          
   ---------+-------------+----------------------------+-------------+----------------------+----------+----
            |             |                            |             |                      |          |                  
      +-------+        +--------+                +-----------+   +-----+                +------+   +-----+
      | Serio |        | USB Hid|                | Bluetooth |   | SPI |                |ISA/LPC|  |GPIO |
      +-------+        +--------+                |  Hidp     |   +-----+                +------+   +-----+
                                                 +-----------+
         ^                 ^                           ^             ^                      ^          ^
---------|-----------------|---------------------------|-------------|----------------------|----------|--------- Kernel Space / Hardware boundary
                      
 +----------------+   +----------------+   +----------------+   +------------+   +---------------+   +------------------+
 | PS/2 Keyboard, |   | USB Mouse and  |   | Bluetooth Mouse|   | SPI Touch  |   | Accelerometer |   | Roller Wheeler   |
 | PS/2 Mouse,    |   | Keyboard       |   | and Keyboard   |   | Controller |   +---------------+   +------------------+
 | Serial Touch   |   +----------------+   +----------------+   +------------+
 | Controller     |
 +----------------+      
```

Because event drivers are standardized and available for all input classes, you are more likely to implement a device driver than an event driver. Your device driver can use a suitable existing event driver via the input core to interface with user applications. Note that this chapter uses the term *device driver* to refer to an input device driver as opposed to an input event driver.

# **Input Event Drivers**

The event interfaces exported by the input subsystem have evolved into a standard that many graphical windowing systems understand. Event drivers offer a hardware-independent abstraction to talk to input devices, just as the frame buffer interface (discussed in Chapter 12, "Video Drivers") presents a generic mechanism to communicate with display devices. Event drivers, in tandem with frame buffer drivers, insulate *graphical user interfaces* (GUIs) from the vagaries of the underlying hardware.

## **The Evdev Interface**

*Evdev* is a generic input event driver. Each event packet produced by evdev has the following format, defined in *include/linux/input.h*:

```
struct input_event {
 struct timeval time; /* Timestamp */
 __u16 type; /* Event Type */
 __u16 code; /* Event Code */
 __s32 value; /* Event Value */
};
```

To learn how to use evdev, let's implement an input device driver for a virtual mouse.

#### **Device Example: Virtual Mouse**

This is how our virtual mouse works: An application (*coord.c*) emulates mouse movements and dispatches coordinate information to the virtual mouse driver (*vms.c*) via a sysfs node, */sys/devices/platform/vms/coordinates.* The virtual mouse driver (*vms* driver for short) channels these movements upstream via evdev. Figure 7.2 shows the details.

**Figure 7.2. An input driver for a virtual mouse.**

[View full size image]

```
                         Simulated Mouse (User Space)
                         ----------------------------

                         +---------------------------+
                         | Coordinate Generator      |
                         |        (coord.c)          |
                         +-------------+-------------+
                                       |
                                       v
                         /sys/.../vms/coordinates
                                       |
=========================== User Space / Kernel Space ===========================

                                       v
                         +---------------------------+
                         |   Virtual Mouse Driver    |
                         |          (vms.c)          |
                         +-------------+-------------+
                                       |
                                       |
                                       |    Input Core
                                       |    /     \
                                       v   v       v
                                      Virtual    evdev
                                      input dev     |
                                                    v
                                           /dev/input/eventX
                                                    |
                                                    v
                                                   gpm
                                                    |
                                                    v
                                                 Console
```

*General-purpose mouse* (*gpm*) is a server that lets you use a mouse in text mode without assistance from an X server. Gpm understands evdev messages, so the vms driver can directly communicate with it. After you have everything in place, you can see the cursor dancing over your screen to the tune of the virtual mouse movements streamed by *coord.c*.

Listing 7.1 contains *coord.c*, which continuously generates random X and Y coordinates. Mice, unlike joysticks or touch screens, produce relative coordinates, so that is what *coord.c* does. The vms driver is shown in Listing 7.2.

**Listing 7.1. Application to Simulate Mouse Movements (coord.c)**

```
#include <fcntl.h>
int
main(int argc, char *argv[])
{
 int sim_fd;
 int x, y;
 char buffer[10];
 /* Open the sysfs coordinate node */
 sim_fd = open("/sys/devices/platform/vms/coordinates", O_RDWR);
 if (sim_fd < 0) {
 perror("Couldn't open vms coordinate file\n");
 exit(-1);
 }
 while (1) {
 /* Generate random relative coordinates */
 x = random()%20;
 y = random()%20;
 if (x%2) x = -x; if (y%2) y = -y;
 /* Convey simulated coordinates to the virtual mouse driver */
 sprintf(buffer, "%d %d %d", x, y, 0);
 write(sim_fd, buffer, strlen(buffer));
 fsync(sim_fd);
 sleep(1);
 }
 close(sim_fd);
}
```

#### **Listing 7.2. Input Driver for the Virtual Mouse (vms.c)**

```
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
struct input_dev *vms_input_dev; /* Representation of an input device */
static struct platform_device *vms_dev; /* Device structure */
 /* Sysfs method to input simulated
 coordinates to the virtual
 mouse driver */
static ssize_t
write_vms(struct device *dev,
 struct device_attribute *attr,
 const char *buffer, size_t count)
{
 int x,y;
 sscanf(buffer, "%d%d", &x, &y);
```

```
 /* Report relative coordinates via the
 event interface */
 input_report_rel(vms_input_dev, REL_X, x);
 input_report_rel(vms_input_dev, REL_Y, y);
 input_sync(vms_input_dev);
 return count;
}
/* Attach the sysfs write method */
DEVICE_ATTR(coordinates, 0644, NULL, write_vms);
/* Attribute Descriptor */
static struct attribute *vms_attrs[] = {
 &dev_attr_coordinates.attr,
 NULL
};
/* Attribute group */
static struct attribute_group vms_attr_group = {
 .attrs = vms_attrs,
};
/* Driver Initialization */
int __init
vms_init(void)
{
 /* Register a platform device */
 vms_dev = platform_device_register_simple("vms", -1, NULL, 0);
 if (IS_ERR(vms_dev)) {
 PTR_ERR(vms_dev);
 printk("vms_init: error\n");
 }
 /* Create a sysfs node to read simulated coordinates */
 sysfs_create_group(&vms_dev->dev.kobj, &vms_attr_group);
 /* Allocate an input device data structure */
 vms_input_dev = input_allocate_device();
 if (!vms_input_dev) {
 printk("Bad input_alloc_device()\n");
 }
 /* Announce that the virtual mouse will generate
 relative coordinates */
 set_bit(EV_REL, vms_input_dev->evbit);
 set_bit(REL_X, vms_input_dev->relbit);
 set_bit(REL_Y, vms_input_dev->relbit);
 /* Register with the input subsystem */
 input_register_device(vms_input_dev);
 printk("Virtual Mouse Driver Initialized.\n");
 return 0;
}
/* Driver Exit */
void
```

```
vms_cleanup(void)
{
 /* Unregister from the input subsystem */
 input_unregister_device(vms_input_dev);
 /* Cleanup sysfs node */
 sysfs_remove_group(&vms_dev->dev.kobj, &vms_attr_group);
 /* Unregister driver */
 platform_device_unregister(vms_dev);
 return;
}
module_init(vms_init);
module_exit(vms_cleanup);
```

Let's take a closer look at Listing 7.2. During initialization, the vms driver registers itself as an input device driver. For this, it first allocates an input\_dev structure using the core API, input\_allocate\_device():

```
vms_input_dev = input_allocate_device();
```

It then announces that the virtual mouse generates relative events:

```
set_bit(EV_REL, vms_input_dev->evbit); /* Event Type is EV_REL */
```

Next, it declares the event codes that the virtual mouse produces:

```
set_bit(REL_X, vms_input_dev->relbit); /* Relative 'X' movement */
set_bit(REL_Y, vms_input_dev->relbit); /* Relative 'Y' movement */
```

If your virtual mouse is also capable of generating button clicks, you need to add this to vms\_init():

```
set_bit(EV_KEY, vms_input_dev->evbit); /* Event Type is EV_KEY */
set_bit(BTN_0, vms_input_dev->keybit); /* Event Code is BTN_0 */
```

Finally, the registration:

```
input_register_device(vms_input_dev);
```

write\_vms() is the sysfs store() method that attaches to */sys/devices/platform/vms/coordinates.* When *coord.c* writes an X/Y pair to this file, write\_vms() does the following:

```
input_report_rel(vms_input_dev, REL_X, x);
input_report_rel(vms_input_dev, REL_Y, y);
input_sync(vms_input_dev);
```

The first statement generates a REL\_X event or a relative device movement in the X direction. The second produces a REL\_Y event or a relative movement in the Y direction. input\_sync() indicates that this event is complete, so the input subsystem collects these two events into a single evdev packet and sends it out of the door through */dev/input/eventX*, where *X* is the interface number assigned to the vms driver. An application reading this file will receive event packets in the input\_event format described earlier. To request gpm to attach to this event interface and accordingly chase the cursor around your screen, do this:

#### **bash> gpm -m /dev/input/eventX -t evdev**

The ADS7846 touch controller driver and the accelerometer driver, discussed respectively under the sections "Touch Controllers" and "Accelerometers" later, are also evdev users.

#### **More Event Interfaces**

The vms driver utilizes the generic evdev event interface, but input devices such as keyboards, mice, and touch controllers have custom event drivers. We will look at them when we discuss the corresponding device drivers.

To write your own event driver and export it to user space via */dev/input/mydev*, you have to populate a structure called input\_handler and register it with the input core as follows:

```
static struct input_handler my_event_handler = {
 .event = mydev_event, /* Handle event reports sent by
 input device drivers that use
 this event driver's services */
 .fops = &mydev_fops, /* Methods to manage
 /dev/input/mydev */
 .minor = MYDEV_MINOR_BASE, /* Minor number of
                          /dev/input/mydev */
 .name = "mydev", /* Event driver name */
 .id_table = mydev_ids, /* This event driver can handle
 requests from these IDs */
 .connect = mydev_connect, /* Invoked if there is an
 ID match */
 .disconnect = mydev_disconnect, /* Called when the driver unregisters
 */
};
/* Driver Initialization */
static int __init
mydev_init(void)
{
 /* ... */
 input_register_handler(&my_event_handler);
 /* ... */
 return 0;
}
```

Look at the implementation of mousedev (*drivers/input/mousedev.c*) for a complete example.

# **Input Device Drivers**

Let's turn our attention to drivers for common input devices such as keyboards, mice, and touch screens. But first, let's take a quick look at an off-the-shelf hardware access facility available to input drivers.

## **Serio**

The *serio* layer offers library routines to access legacy input hardware such as i8042-compatible keyboard controllers and the serial port. PS/2 keyboards and mice interface with the former, whereas serial touch controllers connect to the latter. To communicate with hardware serviced by serio, for example, to send a command to a PS/2 mouse, register prescribed callback routines with serio using serio\_register\_driver().

To add a new driver as part of serio, register open()/close()/start()/stop()/write() entry points using serio\_register\_port (). Look at *drivers/input/serio/serport.c* for an example.

As you can see in Figure 7.1, serio is only one route to access low-level hardware. Several input device drivers instead rely on low-level support from bus layers such as USB or SPI.

### **Keyboards**

Keyboards come in different flavors—legacy PS/2, USB, Bluetooth, Infrared, and so on. Each type has a specific input device driver, but all use the same keyboard event driver, thus ensuring a consistent interface to their users. The keyboard event driver, however, has a distinguishing feature compared to other event drivers: It passes data to another kernel subsystem (the tty layer), rather than to user space via */dev* nodes.

#### **PC Keyboards**

The PC keyboard (also called PS/2 keyboard or AT keyboard) interfaces with the processor via an i8042 compatible keyboard controller. Desktops usually have a dedicated keyboard controller, but on laptops, keyboard interfacing is one of the responsibilities of a general-purpose embedded controller (see the section "Embedded Controllers" in Chapter 20, "More Devices and Drivers"). When you press a key on a PC keyboard, this is the road it takes:

- The keyboard controller (or the embedded controller) scans and decodes the keyboard matrix and takes care of nuances such as key debouncing. **1.**
- The keyboard device driver, with the help of serio, reads raw *scancodes* from the keyboard controller for each key press and release. The difference between a press and a release is in the most significant bit, which is set for the latter. A push on the "a" key, for example, yields a pair of scancodes, 0x1e and 0x9e. Special keys are escaped using 0xE0, so a jab on the right-arrow key produces the sequence, (0xE0 0x4D 0xE0 0xCD). You may use the *showkey* utility to observe scancodes emanating from the controller (the symbol attaches explanations): **2.**

```
bash> showkey -s
kb mode was UNICODE
[ if you are trying this under X, it might not work since
 the X server is also reading /dev/console ]
 press any key (program terminates 10s after last
 keypress)...
```

```
 ...
 0x1e 0x9e A push of the "a" key
```

The keyboard device driver converts received scancodes to *keycodes*, based on the input mode. To see the keycode corresponding to the "a" key: **3.**

```
bash> showkey
...
keycode 30 press A push of the "a" key
keycode 30 release Release of the "a" key
```

To report the keycode upstream, the driver generates an input event, which passes control to the keyboard event driver.

The keyboard event driver undertakes keycode translation depending on the loaded key map. (See man pages of *loadkeys* and the map files present in */lib/kbd/keymaps.*) It checks whether the translated keycode is tied to actions such as switching the virtual console or rebooting the system. To glow the CAPSLOCK and NUMLOCK LEDs instead of rebooting the system in response to a Ctrl+Alt+Del push, add the following to the Ctrl+Alt+Del handler of the keyboard event driver, *drivers/char/keyboard.c*: **4.**

```
static void fn_boot_it(struct vc_data *vc,
 struct pt_regs *regs)
{
+ set_vc_kbd_led(kbd, VC_CAPSLOCK);
+ set_vc_kbd_led(kbd, VC_NUMLOCK);
- ctrl_alt_del();
}
```

For regular keys, the translated keycode is sent to the associated virtual terminal and the N\_TTY line discipline. (We discussed virtual terminals and line disciplines in Chapter 6, "Serial Drivers.") This is done as follows by *drivers/char/keyboard.c*: **5.**

```
/* Add the keycode to flip buffer */
tty_insert_flip_char(tty, keycode, 0);
/* Schedule */
con_schedule_flip(tty);
```

The N\_TTY line discipline processes the input thus received from the keyboard, echoes it to the virtual console, and lets user-space applications read characters from the */dev/ttyX* node connected to the virtual terminal.

Figure 7.3 shows the data flow from the time you push a key on your keyboard until the time it's echoed on your virtual console. The left half of the figure is hardware-specific, and the right half is generic. As per the design goal of the input subsystem, the underlying hardware interface is transparent to the keyboard event driver and the tty layer. The input core and the clearly defined event interfaces thus insulate input users from the intricacies of the hardware.

**Figure 7.3. Data flow from a PS/2-compatible keyboard.**

[View full size image]

```
                     Hardware-Specific Half                          Generic Half

   ------------------------------                          ----------------------------
   | Input Hardware              |                         | Input User               |
   |                             |                         |                          |
   |  +----------------------+   |                         |   +------------------+   |
   |  | Keyboard Controller  |   |                         |   |   N_TTY          |   |
   |  | (Desktops)           |   |                         |   | Line Discipline  |   |
   |  | Embedded Controller  |   |                         |   +------------------+   |
   |  | (Laptops)            |   |                         |            ^             |
   |  +----------------------+   |                         |            |             |
   ------------------------------                          |   +------------------+   |
                 |                                         |   | Virtual Terminal |   |
                 |                                         |   +------------------+   |
              (keyboard)                                   |            ^             |
                 v                                         |            |             |
          ---------------------                            | +----------------------+ |
          |       Serio        |                           | | Key Handlers for     | |
          ---------------------                            | | Sysrq, Reboot,       | |
                 |                                         | | Scroll, Meta,        | |
                 |                                         | | Console switch,      | |
                 v                                         | | Caps...              | |
          ---------------------                            | +----------------------+ |
          |       ISR          |                           |            ^             |
          ---------------------                            |            |             |
                 |                                         | +--------------------+   |
                 v                                         | | Keyboard           |   |
          ---------------------                            | | Translation Tables |   |
          |     Scancode       |                           | +--------------------+   |
          ---------------------                            |            ^             |
                 |                                         |            |             |
                 v                                         |   +------------------+   |
          ---------------------                            |   |   Event Handler  |   |
          |     Keycode        |                           |   +------------------+   |
          ---------------------                            |            ^             |
                                                           |            |             |
                                                           | Input Event Driver       |
   ------------------------------                          | (drivers/char/           |
   | Input Device Driver        |                          |   keyboard.c)            |
   | (drivers/input/keyboard/   |                          ----------------------------
   |          atkbd.c)          |
   ------------------------------

                             \                             /
                              \                           /
                                   +----------------+
                                   |   Input Core   |
                                   +----------------+
```

**USB and Bluetooth Keyboards**

The USB specifications related to *human interface devices* (HID) stipulate the protocol that USB keyboards, mice, keypads, and other input peripherals use for communication. On Linux, this is implemented via the *usbhid* USB client driver, which is responsible for the USB HID class (0x03). Usbhid registers itself as an input device driver. It conforms to the input API and reports input events appropriate to the connected HID.

To understand the code path for a USB keyboard, revert to Figure 7.3 and modify the hardware-specific left half. Replace the keyboard controller in the Input Hardware box with a USB controller, serio with the USB core layer, and the Input Device Driver box with the usbhid driver.

For a Bluetooth keyboard, replace the keyboard controller in Figure 7.3 with a Bluetooth chipset, serio with the Bluetooth core layer, and the Input Device Driver box with the Bluetooth hidp driver.

USB and Bluetooth are discussed in detail in Chapter 11, "Universal Serial Bus," and Chapter 16, "Linux Without Wires," respectively.

## **Mice**

Mice, like keyboards, come with different capabilities and have different interfacing options. Let's look at the common ones.

#### **PS/2 Mice**

Mice generate relative movements in the X and Y axes. They also possess one or more buttons. Some have scroll wheels, too. The input device driver for PS/2-compatible legacy mice relies on the serio layer to talk to the underlying controller. The input event driver for mice, called *mousedev*, reports mouse events to user applications via */dev/input/mice*.

#### **Device Example: Roller Mouse**

To get a feel of a real-world mouse device driver, let's convert the roller wheel discussed in Chapter 4, "Laying the Groundwork," into a variation of the generic PS/2 mouse. The "roller mouse" generates one-dimensional movement in the Y-axis. Clockwise and anticlockwise turns of the wheel produce positive and negative relative Y coordinates respectively (like the scroll wheel in mice), while pressing the roller wheel results in a left button mouse event. The roller mouse is thus ideal for navigating menus in devices such as smart phones, handhelds, and music players.

The roller mouse device driver implemented in Listing 7.3 works with windowing systems such as X Windows. Look at roller\_mouse\_init() to see how the driver declares its mouse-like capabilities. Unlike the roller wheel driver in Listing 4.1 of Chapter 4, the roller mouse driver needs no read() or poll() methods because events are reported using input APIs. The roller interrupt handler roller\_isr() also changes accordingly. Gone are the housekeepings done in the interrupt handler using a wait queue, a spinlock, and the store\_movement() routine to support read() and poll().

In Listing 7.3, the leading **+** and - denote the differences from the roller wheel driver implemented in Listing 4.1 of Chapter 4.

#### **Listing 7.3. The Roller Mouse Driver**

```
+ #include <linux/input.h>
+ #include <linux/interrupt.h>
+ /* Device structure */
+ struct {
+ /* ... */
+ struct input_dev dev;
+ } roller_mouse;
+ static int __init
+ roller_mouse_init(void)
+ {
+ /* Allocate input device structure */
+ roller_mouse->dev = input_allocate_device();
+
```

```
+ /* Can generate a click and a relative movement */
+ roller_mouse->dev->evbit[0] = BIT(EV_KEY) | BIT(EV_REL);
+ /* Can move only in the Y-axis */
+ roller_mouse->dev->relbit[0] = BIT(REL_Y);
+
+ /* My click should be construed as the left button
+ press of a mouse */
+ roller_mouse->dev->keybit[LONG(BTN_MOUSE)] = BIT(BTN_LEFT);
+ roller_mouse->dev->name = "roll";
+
+ /* For entries in /sys/class/input/inputX/id/ */
+ roller_mouse->dev->id.bustype = ROLLER_BUS;
+ roller_mouse->dev->id.vendor = ROLLER_VENDOR;
+ roller_mouse->dev->id.product = ROLLER_PROD;
+ roller_mouse->dev->id.version = ROLLER_VER;
+ /* Register with the input subsystem */
+ input_register_device(roller_mouse->dev);
+}
/* Global variables */
- spinlock_t roller_lock = SPIN_LOCK_UNLOCKED;
- static DECLARE_WAIT_QUEUE_HEAD(roller_poll);
/* The Roller Interrupt Handler */
static irqreturn_t
roller_interrupt(int irq, void *dev_id)
{
 int i, PA_t, PA_delta_t, movement = 0;
 /* Get the waveforms from bits 0, 1 and 2
 of Port D as shown in Figure 7.1 */
 PA_t = PORTD & 0x07;
 /* Wait until the state of the pins change.
 (Add some timeout to the loop) */
 for (i=0; (PA_t==PA_delta_t); i++){
 PA_delta_t = PORTD & 0x07;
 }
 movement = determine_movement(PA_t, PA_delta_t);
- spin_lock(&roller_lock);
-
- /* Store the wheel movement in a buffer for
- later access by the read()/poll() entry points */
- store_movements(movement);
-
- spin_unlock(&roller_lock);
-
- /* Wake up the poll entry point that might have
- gone to sleep, waiting for a wheel movement */
- wake_up_interruptible(&roller_poll);
-
+ if (movement == CLOCKWISE) {
+ input_report_rel(roller_mouse->dev, REL_Y, 1);
+ } else if (movement == ANTICLOCKWISE) {
```

```
+ input_report_rel(roller_mouse->dev, REL_Y, -1);
+ } else if (movement == KEYPRESSED) {
+ input_report_key(roller_mouse->dev, BTN_LEFT, 1);
+ }
+ input_sync(roller_mouse->dev);
 return IRQ_HANDLED;
}
```

#### **Trackpoints**

A *trackpoint* is a pointing device that comes integrated with the PS/2-type keyboard on several laptops. This device includes a joystick located among the keys and mouse buttons positioned under the spacebar. A trackpoint essentially functions as a mouse, so you can operate it using the PS/2 mouse driver.

Unlike a regular mouse, a trackpoint offers more movement control. You can command the trackpoint controller to change properties such as sensitivity and inertia. The kernel has a special driver, *drivers/input/mouse/trackpoint.c*, to create and manage associated sysfs nodes. For the full set of track point configuration options, look under */sys/devices/platform/i8042/serioX/serioY/*.

#### **Touchpads**

A *touchpad* is a mouse-like pointing device commonly found on laptops. Unlike conventional mice, a touchpad does not have moving parts. It can generate mouse-compatible relative coordinates but is usually used by operating systems in a more powerful mode that produces absolute coordinates. The communication protocol used in absolute mode is similar to the PS/2 mouse protocol, but not compatible with it.

The basic PS/2 mouse driver is capable of supporting devices that conform to different variations of the bare PS/2 mouse protocol. You may add support for a new mouse protocol to the base driver by supplying a protocol driver via the psmouse structure. If your laptop uses the Synaptics touchpad in absolute mode, for example, the base PS/2 mouse driver uses the services of a Synaptics protocol driver to interpret the streaming data. For an end-to-end understanding of how the Synaptics protocol works in tandem with the base PS/2 driver, look at the following four code regions collected in Listing 7.4:

- The PS/2 mouse driver, *drivers/input/mouse/psmouse-base.c*, instantiates a psmouse\_protocol structure with information regarding supported mouse protocols (including the Synaptics touchpad protocol).
- The psmouse structure, defined in *drivers/input/mouse/psmouse.h*, ties various PS/2 protocols together.
- synaptics\_init() populates the psmouse structure with the address of associated protocol functions.
- The protocol handler function synaptics\_process\_byte(), populated in synaptics\_init(), gets called from interrupt context when serio senses mouse movement. If you unfold synaptics\_process\_byte(), you will see touchpad movements being reported to user applications via mousedev.

#### **Listing 7.4. PS/2 Mouse Protocol Driver for the Synaptics Touchpad**

```
drivers/input/mouse/psmouse-base.c:
/* List of supported PS/2 mouse protocols */
static struct psmouse_protocol psmouse_protocols[] = {
 {
 .type = PSMOUSE_PS2, /* The bare PS/2 handler */
 .name = "PS/2",
 .alias = "bare",
 .maxproto = 1,
 .detect = ps2bare_detect,
 },
 /* ... */
 {
 .type = PSMOUSE_SYNAPTICS, /* Synaptics TouchPad Protocol */
 .name = "SynPS/2",
 .alias = "synaptics",
 .detect = synaptics_detect, /* Is the protocol detected? */
 .init = synaptics_init, /* Initialize Protocol Handler */
 },
 /* ... */
}
drivers/input/mouse/psmouse.h:
/* The structure that ties various mouse protocols together */
struct psmouse {
 struct input_dev *dev; /* The input device */
 /* ... */
 /* Protocol Methods */
 psmouse_ret_t (*protocol_handler)
 (struct psmouse *psmouse, struct pt_regs *regs);
 void (*set_rate)(struct psmouse *psmouse, unsigned int rate);
 void (*set_resolution)
 (struct psmouse *psmouse, unsigned int resolution);
 int (*reconnect)(struct psmouse *psmouse);
 void (*disconnect)(struct psmouse *psmouse);
 /* ... */
};
drivers/input/mouse/synaptics.c:
/* init() method of the Synaptics protocol */
int synaptics_init(struct psmouse *psmouse)
{
 struct synaptics_data *priv;
 psmouse->private = priv = kmalloc(sizeof(struct synaptics_data),
 GFP_KERNEL);
 /* ... */
 /* This is called in interrupt context when mouse
 movement is sensed */
 psmouse->protocol_handler = synaptics_process_byte;
 /* More protocol methods */
 psmouse->set_rate = synaptics_set_rate;
 psmouse->disconnect = synaptics_disconnect;
 psmouse->reconnect = synaptics_reconnect;
 /* ... */
```

```
}
drivers/input/mouse/synaptics.c:
/* If you unfold synaptics_process_byte() and look at
 synaptics_process_packet(), you can see the input
 events being reported to user applications via mousedev */
static void synaptics_process_packet(struct psmouse *psmouse)
{
 /* ... */
 if (hw.z > 0) {
 /* Absolute X Coordinate */
 input_report_abs(dev, ABS_X, hw.x);
 /* Absolute Y Coordinate */
 input_report_abs(dev, ABS_Y,
 YMAX_NOMINAL + YMIN_NOMINAL - hw.y);
 }
 /* Absolute Z Coordinate */
 input_report_abs(dev, ABS_PRESSURE, hw.z);
 /* ... */
 /* Left TouchPad button */
 input_report_key(dev, BTN_LEFT, hw.left);
 /* Right TouchPad button */
 input_report_key(dev, BTN_RIGHT, hw.right);
 /* ... */
}
```

#### **USB and Bluetooth Mice**

USB mice are handled by the same input driver (*usbhid*) that drives USB keyboards. Similarly, the *hidp* driver that implements support for Bluetooth keyboards also takes care of Bluetooth mice.

As you would expect, USB and Bluetooth mice drivers channel device data through mousedev.

## **Touch Controllers**

In Chapter 6, we implemented a device driver for a serial touch controller in the form of a line discipline called N\_TCH. The input subsystem offers a better and easier way to implement that driver. Refashion the finite state machine in N\_TCH as an input device driver with the following changes:

- Serio offers a line discipline called *serport* for accessing devices connected to the serial port. Use serport's services to talk to the touch controller. **1.**
- Instead of passing coordinate information to the tty layer, generate input reports via evdev as you did in Listing 7.2 for the virtual mouse. **2.**

With this, the touch screen is accessible to user space via */dev/input/eventX.* The actual driver implementation is left as an exercise.

An example of a touch controller that does not interface via the serial port is the Analog Devices ADS7846 chip, which communicates over a *Serial Peripheral Interface* (SPI). The driver for this device uses the services of the SPI core rather than serio. The section "The Serial Peripheral Interface Bus" in Chapter 8, "The Inter-Integrated Circuit Protocol," discusses SPI. Like most touch drivers, the ADS7846 driver uses the evdev interface to dispatch touch information to user applications.

Some touch controllers interface over USB. An example is the 3M USB touch controller, driven by *drivers/input/touchscreen/usbtouchscreen.c*.

Many PDAs have four-wire resistive touch panels superimposed on their LCDs. The X and Y plates of the panel (two wires for either axes) connect to an analog-to-digital converter (ADC), which provides a digital readout of the analog voltage difference arising out of touching the screen. An input driver collects the coordinates from the ADC and dispatches it to user space.

Different instances of the same touch panel may produce slightly different coordinate ranges (maximum values in the X and Y directions) due to the nuances of manufacturing processes. To insulate applications from this variation, touch screens are *calibrated* prior to use. Calibration is usually initiated by the GUI by displaying cross-marks at screen boundaries and other vantage points, and requesting the user to touch those points. The generated coordinates are programmed back into the touch controller using appropriate commands if it supports self-calibration, or used to scale the coordinate stream in software otherwise.

The input subsystem also contains an event driver called *tsdev* that generates coordinate information according to the Compaq touch-screen protocol. If your system reports touch events via *tsdev*, applications that understand this protocol can elicit touch input from */dev/input/tsX.* This driver is, however, scheduled for removal from the mainline kernel in favor of the user space *tslib* library. *Documentation/feature-removalschedule.txt* lists features that are going away from the kernel source tree.

## **Accelerometers**

An accelerometer measures acceleration. Several IBM/Lenovo laptops have an accelerometer that detects sudden movement. The generated information is used to protect the hard disk from damage using a mechanism called *Hard Drive Active Protection System* (HDAPS), analogous to the way a car airbag shields a passenger from injury. The HDAPS driver is implemented as a platform driver that registers with the input subsystem. It uses evdev to stream the X and Y components of the detected acceleration. Applications can read acceleration events via */dev/input/eventX* to detect conditions, such as shock and vibe, and perform a defensive action, such as parking the hard drive's head. The following command spews output if you move the laptop (assume that event3 is assigned to HDAPS):

**bash> od –x /dev/input/event3** 0000000 a94d 4599 1f19 0007 0003 0000 ffed ffff ...

The accelerometer also provides information such as temperature, keyboard activity, and mouse activity, all of which can be gleaned via files in */sys/devices/platform/hdaps/.* Because of this, the HDAPS driver is part of the hardware monitoring (*hwmon*) subsystem in the kernel sources. We talk about hardware monitoring in the section "Hardware Monitoring with LM-Sensors" in the next chapter.

### **Output Events**

Some input device drivers also handle output events. For example, the keyboard driver can glow the CAPSLOCK

LED, and the PC speaker driver can sound a beep. Let's zoom in on the latter. During initialization, the speaker driver declares its output capability by setting appropriate *evbits* and registering a callback routine to handle the output event:

```
drivers/input/misc/pcspkr.c:
static int __devinit pcspkr_probe(struct platform_device *dev)
{
 /* ... */
 /* Capability Bits */
 pcspkr_dev->evbit[0] = BIT(EV_SND);
 pcspkr_dev->sndbit[0] = BIT(SND_BELL) | BIT(SND_TONE);
 /* The Callback routine */
 pcspkr_dev->event = pcspkr_event;
 err = input_register_device(pcspkr_dev);
 /* ... */
}
/* The callback routine */
static int pcspkr_event(struct input_dev *dev, unsigned int type,
 unsigned int code, int value)
{
 /* ... */
 /* I/O programming to sound a beep */
 outb_p(inb_p(0x61) | 3, 0x61);
 /* set command for counter 2, 2 byte write */
 outb_p(0xB6, 0x43);
 /* select desired HZ */
 outb_p(count & 0xff, 0x42);
 outb((count >> 8) & 0xff, 0x42);
 /* ... */
}
```

To sound the beeper, the keyboard event driver generates a sound event (EV\_SND) as follows:

```
input_event(handle->dev, EV_SND, /* Type */
 SND_TONE, /* Code */
 hz /* Value */);
```

This triggers execution of the callback routine, pcspkr\_event(), and you hear the beep.

# **Debugging**

You can use the *evbug* module as a debugging aid if you're developing an input driver. It dumps the (*type*, *code*, *value*) tuple (see struct input\_event defined previously) corresponding to events generated by the input subsystem. Figure 7.4 contains data captured by evbug while operating some input devices:

**Figure 7.4. Evbug output.**

```
/* Touchpad Movement */
evbug.c Event. Dev: isa0060/serio1/input0: Type: 3, Code: 28, Value: 0
evbug.c Event. Dev: isa0060/serio1/input0: Type: 1, Code: 325, Value: 0
evbug.c Event. Dev: isa0060/serio1/input0: Type: 0, Code: 0, Value: 0
/* Trackpoint Movement */
evbug.c Event. Dev: synaptics-pt/serio0/input0: Type: 2, Code: 0, Value: -1
evbug.c Event. Dev: synaptics-pt/serio0/input0: Type: 2, Code: 1, Value: -2
evbug.c Event. Dev: synaptics-pt/serio0/input0: Type: 0, Code: 0, Value: 0
/* USB Mouse Movement */
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 2, Code: 1, Value: -1
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 0, Code: 0, Value: 0
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 2, Code: 0, Value: 1
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 0, Code: 0, Value: 0
/* PS/2 Keyboard keypress 'a' */
evbug.c Event. Dev: isa0060/serio0/input0: Type: 4, Code: 4, Value: 30
evbug.c Event. Dev: isa0060/serio0/input0: Type: 1, Code: 30, Value: 0
evbug.c Event. Dev: isa0060/serio0/input0: Type: 0, Code: 0, Value: 0
/* USB keyboard keypress 'a' */
evbug.c Event. Dev: usb-0000:00:1d.1-1/input0: Type: 1, Code: 30, Value: 1
evbug.c Event. Dev: usb-0000:00:1d.1-1/input0: Type: 0, Code: 0, Value: 0
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 1, Code: 30, Value: 0
evbug.c Event. Dev: usb-0000:00:1d.1-2/input0: Type: 0, Code: 0, Value: 0
```

To make sense of the dump in Figure 7.4, remember that touchpads generate absolute coordinates (EV\_ABS) or event type 0x03, trackpoints produce relative coordinates (EV\_REL) or event type 0x02, and keyboards emit key events (EV\_KEY) or event type 0x01. Event type 0x0 corresponds to an invocation of input\_sync(), which does the following:

```
input_event(dev, EV_SYN, SYN_REPORT, 0);
```

This translates to a (*type, code, value*) tuple of (0x0, 0x0, 0x0) and completes each input event.

# **Looking at the Sources**

Most input event drivers are present in the *drivers/input/* directory. The keyboard event driver, however, lives in *drivers/char/keyboard.c*, because it's connected to virtual terminals and not to device nodes under */dev/input/*.

You can find input device drivers in several places. Drivers for legacy keyboards, mice, and joysticks, reside in separate subdirectories under *drivers/input/.* Bluetooth input drivers live in *net/bluetooth/hidp/.* You can also find input drivers in regions such as *drivers/hwmon/* and *drivers/media/video/.* Event types, codes, and values, are defined in *include/linux/input.h*.

The serio subsystem stays in *drivers/input/serio/.* Sources for the serport line discipline is in *drivers/input/serio/serport.c. Documentation/input/* contains more details on different input interfaces.

Table 7.1 summarizes the main data structures used in this chapter and their location inside the source tree. Table 7.2 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

**Table 7.1. Summary of Data Structures**

| Data Structure | Location                                            | Description                                                  |
|----------------|-----------------------------------------------------|--------------------------------------------------------------|
| input_event    | include/linux/input.h                               | Each event packet produced by<br>evdev has this format.      |
| input_dev      | include/linux/input.h                               | Representation of an input<br>device.                        |
| input_handler  | include/linux/serial_core.h                         | Contains the entry points<br>supported by an event driver.   |
|                | psmouse_protocol drivers/input/mouse/psmouse-base.c | Information about a supported<br>PS/2 mouse protocol driver. |
| psmouse        | drivers/input/mouse/psmouse.h                       | Methods supported by a PS/2<br>mouse driver.                 |

**Table 7.2. Summary of Kernel Programming Interfaces**

| Kernel Interface          | Location              | Description                                                   |
|---------------------------|-----------------------|---------------------------------------------------------------|
| input_register_device()   | drivers/input/input.c | Registers a device with the input<br>core                     |
| input_unregister_device() | drivers/input/input.c | Removes a device from the input<br>core                       |
| input_report_rel()        | include/linux/input.h | Generates a relative movement<br>in a specified direction     |
| input_report_abs()        | include/linux/input.h | Generates an absolute<br>movement in a specified<br>direction |
| input_report_key()        | include/linux/input.h | Generates a key or a button<br>press                          |

| Kernel Interface                                          | Location                 | Description                                                                                                                                                 |
|-----------------------------------------------------------|--------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| input_sync()                                              | include/linux/input.h    | Indicates that the input<br>subsystem can collect previously<br>generated events into an evdev<br>packet and send it to user space<br>via /dev/input/inputX |
| input_register_handler()                                  | drivers/input/input.c    | Registers a custom event driver                                                                                                                             |
| sysfs_create_group()                                      | fs/sysfs/group.c         | Creates a sysfs node group with<br>specified attributes                                                                                                     |
| sysfs_remove_group()                                      | fs/sysfs/group.c         | Removes a sysfs group created<br>using sysfs_create_group()                                                                                                 |
| tty_insert_flip_char()                                    | include/linux/tty_flip.h | Sends a character to the line<br>discipline layer                                                                                                           |
| platform_device_register_simple() drivers/base/platform.c |                          | Creates a simple platform device                                                                                                                            |
| platform_device_unregister()                              | drivers/base/platform.c  | Unregisters a platform device                                                                                                                               |
