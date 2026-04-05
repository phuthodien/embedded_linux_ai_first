// SPDX-License-Identifier: GPL-2.0
/*
 * hello_dt.c - Sample driver demonstrating device tree property parsing.
 *
 * Matches "training,hello-world" compatible node and reads:
 *   - Standard properties (reg, interrupts, clocks) via kernel platform API
 *   - Custom properties (greeting, version, lucky-number) via of_property_read_*()
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/clk.h>

static int hello_dt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct resource *res;
	const char *greeting;
	u32 version;
	u32 lucky[3];
	int irq;
	int ret;
	int i;

	dev_info(dev, "=== hello_dt: probe called ===\n");

	/*
	 * 1. Read "reg" property via platform_get_resource()
	 *    Kernel auto-parses "reg" into IORESOURCE_MEM resources.
	 */
	for (i = 0; ; i++) {
		res = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (!res)
			break;
		dev_info(dev, "[reg] region %d: phys=0x%08llx size=0x%08llx\n",
			 i, (unsigned long long)res->start,
			 (unsigned long long)resource_size(res));
	}

	/*
	 * 2. Read "interrupts" property via platform_get_irq()
	 *    Kernel auto-parses "interrupts" into IRQ resources.
	 */
	for (i = 0; ; i++) {
		irq = platform_get_irq_optional(pdev, i);
		if (irq < 0)
			break;
		dev_info(dev, "[interrupts] irq %d: linux_irq=%d\n", i, irq);
	}

	/*
	 * 3. Read "clocks" + "clock-names" via devm_clk_get()
	 *    Kernel resolves the phandle and returns a clk handle.
	 */
	{
		struct clk *fclk;

		fclk = devm_clk_get(dev, "fclk");
		if (IS_ERR(fclk))
			dev_warn(dev, "[clocks] failed to get 'fclk': %ld\n",
				 PTR_ERR(fclk));
		else
			dev_info(dev, "[clocks] got 'fclk', rate=%lu Hz\n",
				 clk_get_rate(fclk));
	}

	/*
	 * 4. Read custom string property "greeting"
	 */
	ret = of_property_read_string(np, "greeting", &greeting);
	if (ret)
		dev_warn(dev, "[custom] 'greeting' not found\n");
	else
		dev_info(dev, "[custom] greeting = \"%s\"\n", greeting);

	/*
	 * 5. Read custom u32 property "version"
	 */
	ret = of_property_read_u32(np, "version", &version);
	if (ret)
		dev_warn(dev, "[custom] 'version' not found\n");
	else
		dev_info(dev, "[custom] version = %u\n", version);

	/*
	 * 6. Read custom u32 array property "lucky-number"
	 */
	ret = of_property_read_u32_array(np, "lucky-number", lucky,
					 ARRAY_SIZE(lucky));
	if (ret)
		dev_warn(dev, "[custom] 'lucky-number' not found\n");
	else
		dev_info(dev, "[custom] lucky-number = <%u %u %u>\n",
			 lucky[0], lucky[1], lucky[2]);

	dev_info(dev, "=== hello_dt: probe done ===\n");
	return 0;
}

static int hello_dt_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "hello_dt: removed\n");
	return 0;
}

static const struct of_device_id hello_dt_of_match[] = {
	{ .compatible = "training,hello-world" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, hello_dt_of_match);

static struct platform_driver hello_dt_driver = {
	.probe	= hello_dt_probe,
	.remove	= hello_dt_remove,
	.driver	= {
		.name		= "hello_dt",
		.of_match_table	= hello_dt_of_match,
	},
};
module_platform_driver(hello_dt_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("Sample driver to read device tree properties");
