/*
 * echo_misc.c - Misc character driver to control LED on GPIO0_31
 *
 * Write "1"/"on" to /dev/echo_misc to turn LED on, "0"/"off" to turn off.
 * Read from /dev/echo_misc to get current state.
 * GPIO control is done by direct register access (no gpiolib).
 *
 * Hardware: BeagleBone Black, AM335x SoC
 * Pin: GPIO0_31 = P9_13 (gpmc_wpn, mux mode 7)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define DEVICE_NAME	"echo_misc"

/* AM335x GPIO0 registers (TRM SPRUH73Q, ch25) */
#define GPIO0_BASE		0x44E07000
#define GPIO0_SIZE		0x1000

#define GPIO_OE			0x134	/* Output Enable: 0=output, 1=input */
#define GPIO_DATAOUT		0x13C
#define GPIO_SETDATAOUT		0x194
#define GPIO_CLEARDATAOUT	0x190

#define GPIO0_31_BIT		BIT(31)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("Misc char driver: LED control on GPIO0_31 via direct register access");

static void __iomem *gpio0_base;
static int led_state;

static void led_set(int on)
{
	if (on)
		writel(GPIO0_31_BIT, gpio0_base + GPIO_SETDATAOUT);
	else
		writel(GPIO0_31_BIT, gpio0_base + GPIO_CLEARDATAOUT);
	led_state = on;
}

static int echo_open(struct inode *inode, struct file *filep)
{
	pr_info(DEVICE_NAME ": opened by %s (pid %d)\n",
		current->comm, current->pid);
	return 0;
}

static int echo_release(struct inode *inode, struct file *filep)
{
	pr_info(DEVICE_NAME ": closed by %s (pid %d)\n",
		current->comm, current->pid);
	return 0;
}

static ssize_t echo_read(struct file *filep, char __user *buf,
			  size_t count, loff_t *offset)
{
	char state_buf[4];
	int len;

	if (*offset > 0)
		return 0;

	len = snprintf(state_buf, sizeof(state_buf), "%d\n", led_state);

	if (count < len)
		return -EINVAL;

	if (copy_to_user(buf, state_buf, len))
		return -EFAULT;

	*offset += len;
	return len;
}

static ssize_t echo_write(struct file *filep, const char __user *buf,
			   size_t count, loff_t *offset)
{
	char kbuf[16];
	size_t to_copy;

	to_copy = min(count, sizeof(kbuf) - 1);
	if (copy_from_user(kbuf, buf, to_copy))
		return -EFAULT;
	kbuf[to_copy] = '\0';

	/* Strip trailing newline */
	if (to_copy > 0 && kbuf[to_copy - 1] == '\n')
		kbuf[to_copy - 1] = '\0';

	if (kbuf[0] == '1' || strcmp(kbuf, "on") == 0)
		led_set(1);
	else if (kbuf[0] == '0' || strcmp(kbuf, "off") == 0)
		led_set(0);
	else
		return -EINVAL;

	return count;
}

static const struct file_operations echo_fops = {
	.owner   = THIS_MODULE,
	.open    = echo_open,
	.release = echo_release,
	.read    = echo_read,
	.write   = echo_write,
};

static struct miscdevice echo_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DEVICE_NAME,
	.fops  = &echo_fops,
};

static int gpio_hw_init(void)
{
	u32 val;

	/*
	 * Pin mux for GPIO0_31 (P9_13) is already configured as mode 7
	 * (GPIO) by device tree. We only need to map GPIO0 registers
	 * and set direction to output.
	 */
	gpio0_base = ioremap(GPIO0_BASE, GPIO0_SIZE);
	if (!gpio0_base) {
		pr_err(DEVICE_NAME ": failed to ioremap GPIO0\n");
		return -ENOMEM;
	}

	/* Configure GPIO0_31 as output: clear bit 31 in OE register */
	val = readl(gpio0_base + GPIO_OE);
	val &= ~GPIO0_31_BIT;
	writel(val, gpio0_base + GPIO_OE);

	/* Start with LED off */
	led_set(0);

	return 0;
}

static void gpio_hw_cleanup(void)
{
	u32 val;

	/* Turn off LED */
	led_set(0);

	/* Restore GPIO0_31 as input */
	val = readl(gpio0_base + GPIO_OE);
	val |= GPIO0_31_BIT;
	writel(val, gpio0_base + GPIO_OE);

	iounmap(gpio0_base);
}

static int __init echo_init(void)
{
	int ret;

	ret = gpio_hw_init();
	if (ret)
		return ret;

	ret = misc_register(&echo_misc);
	if (ret) {
		pr_err(DEVICE_NAME ": misc_register failed: %d\n", ret);
		gpio_hw_cleanup();
		return ret;
	}

	pr_info(DEVICE_NAME ": registered at /dev/%s (GPIO0_31 = P9_13)\n",
		echo_misc.name);
	return 0;
}

static void __exit echo_exit(void)
{
	misc_deregister(&echo_misc);
	gpio_hw_cleanup();
	pr_info(DEVICE_NAME ": unregistered\n");
}

module_init(echo_init);
module_exit(echo_exit);
