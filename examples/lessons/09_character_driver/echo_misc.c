/*
 * echo_misc.c - Misc character driver to control LED via device tree
 *
 * Write "1"/"on" to /dev/echo_misc to turn LED on, "0"/"off" to turn off.
 * Read from /dev/echo_misc to get current state.
 *
 * GPIO base address and pin number are read from device tree node
 * with compatible = "training,gpio-led".
 *
 * Device tree node example:
 *   training_led {
 *       compatible = "training,gpio-led";
 *       reg = <0x44e07000 0x1000>;   // GPIO0 register block
 *       gpio-bit = <31>;             // GPIO0_31 = P9_13
 *       label = "training:p9_13";
 *   };
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>

#define DEVICE_NAME	"echo_misc"

/* AM335x GPIO register offsets (TRM SPRUH73Q, ch25) */
#define GPIO_OE			0x134
#define GPIO_DATAOUT		0x13C
#define GPIO_SETDATAOUT		0x194
#define GPIO_CLEARDATAOUT	0x190

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("Misc char driver: LED control via device tree");

static void __iomem *gpio_base;
static u32 gpio_bit_mask;
static int led_state;
static const char *led_label;

static void led_set(int on)
{
	if (on)
		writel(gpio_bit_mask, gpio_base + GPIO_SETDATAOUT);
	else
		writel(gpio_bit_mask, gpio_base + GPIO_CLEARDATAOUT);
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

static int echo_led_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res;
	u32 gpio_bit;
	u32 val;
	int ret;

	/*
	 * Read "reg" property via standard platform API.
	 * Kernel auto-parses reg = <0x44e07000 0x1000> into IORESOURCE_MEM.
	 */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "no 'reg' property in device tree\n");
		return -ENODEV;
	}
	dev_info(dev, "[dt] reg: phys=0x%08llx size=0x%08llx\n",
		 (unsigned long long)res->start,
		 (unsigned long long)resource_size(res));

	/*
	 * Use devm_ioremap() instead of devm_ioremap_resource() because
	 * the GPIO0 region is already claimed by the gpio-omap driver.
	 * devm_ioremap_resource() would fail with -EBUSY.
	 */
	gpio_base = devm_ioremap(dev, res->start, resource_size(res));
	if (!gpio_base) {
		dev_err(dev, "failed to ioremap 0x%08llx\n",
			(unsigned long long)res->start);
		return -ENOMEM;
	}

	/*
	 * Read "gpio-bit" custom property via of_property_read_u32().
	 */
	ret = of_property_read_u32(dev->of_node, "gpio-bit", &gpio_bit);
	if (ret) {
		dev_err(dev, "no 'gpio-bit' property in device tree\n");
		return ret;
	}
	gpio_bit_mask = BIT(gpio_bit);
	dev_info(dev, "[dt] gpio-bit: %u (mask=0x%08x)\n",
		 gpio_bit, gpio_bit_mask);

	/*
	 * Read "label" custom string property via of_property_read_string().
	 */
	ret = of_property_read_string(dev->of_node, "label", &led_label);
	if (ret)
		led_label = "unknown";
	dev_info(dev, "[dt] label: %s\n", led_label);

	/* Configure GPIO pin as output */
	val = readl(gpio_base + GPIO_OE);
	val &= ~gpio_bit_mask;
	writel(val, gpio_base + GPIO_OE);

	led_set(0);

	ret = misc_register(&echo_misc);
	if (ret) {
		dev_err(dev, "misc_register failed: %d\n", ret);
		return ret;
	}

	dev_info(dev, "registered /dev/%s (label=%s)\n",
		 echo_misc.name, led_label);
	return 0;
}

static int echo_led_remove(struct platform_device *pdev)
{
	u32 val;

	led_set(0);

	/* Restore GPIO pin as input */
	val = readl(gpio_base + GPIO_OE);
	val |= gpio_bit_mask;
	writel(val, gpio_base + GPIO_OE);

	misc_deregister(&echo_misc);
	dev_info(&pdev->dev, "removed\n");
	return 0;
}

static const struct of_device_id echo_led_of_match[] = {
	{ .compatible = "training,gpio-led" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, echo_led_of_match);

static struct platform_driver echo_led_driver = {
	.probe	= echo_led_probe,
	.remove	= echo_led_remove,
	.driver	= {
		.name		= "echo_led",
		.of_match_table	= echo_led_of_match,
	},
};
module_platform_driver(echo_led_driver);
