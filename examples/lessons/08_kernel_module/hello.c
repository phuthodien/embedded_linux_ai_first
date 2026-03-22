/*
 * hello.c - Blink LED on GPIO0_31 using direct register access (ioremap)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO0_BASE	0x44E07000
#define GPIO_OE		0x134
#define GPIO_DATAOUT	0x13C

#define LED_PIN		(1 << 31)	/* GPIO0_31 */
#define BLINK_COUNT	10
#define BLINK_DELAY_MS	500

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("Blink LED on GPIO0_31 via ioremap");

static void __iomem *gpio0_base;

static int __init hello_init(void)
{
	uint32_t val;
	int i;

	gpio0_base = ioremap(GPIO0_BASE, SZ_4K);
	if (!gpio0_base) {
		pr_err("ioremap GPIO0 failed!\n");
		return -ENOMEM;
	}

	/* Configure GPIO0_31 as output */
	val = readl(gpio0_base + GPIO_OE);
	val &= ~LED_PIN;
	writel(val, gpio0_base + GPIO_OE);

	pr_info("Blinking GPIO0_31 %d times...\n", BLINK_COUNT);

	for (i = 0; i < BLINK_COUNT; i++) {
		/* LED ON */
		val = readl(gpio0_base + GPIO_DATAOUT);
		val |= LED_PIN;
		writel(val, gpio0_base + GPIO_DATAOUT);
		msleep(BLINK_DELAY_MS);

		/* LED OFF */
		val = readl(gpio0_base + GPIO_DATAOUT);
		val &= ~LED_PIN;
		writel(val, gpio0_base + GPIO_DATAOUT);
		msleep(BLINK_DELAY_MS);
	}

	pr_info("Blink done.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	uint32_t val;

	/* Turn off LED */
	val = readl(gpio0_base + GPIO_DATAOUT);
	val &= ~LED_PIN;
	writel(val, gpio0_base + GPIO_DATAOUT);

	iounmap(gpio0_base);
	pr_info("GPIO0_31 released.\n");
}

module_init(hello_init);
module_exit(hello_exit);
