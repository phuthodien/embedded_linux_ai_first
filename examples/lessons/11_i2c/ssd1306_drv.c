/*
 * ssd1306_drv.c - I2C driver for SSD1306 OLED 128x64
 *
 * Usage:
 *   echo "A" > /dev/ssd1306
 *   echo "Hello" > /dev/ssd1306
 *   echo "" > /dev/ssd1306          (clear screen)
 *
 * Device tree node example (child of i2c bus):
 *   &i2c2 {
 *       ssd1306@3c {
 *           compatible = "training,ssd1306";
 *           reg = <0x3c>;
 *       };
 *   };
 *
 * Or register at runtime via sysfs:
 *   echo training_ssd1306 0x3c > /sys/bus/i2c/devices/i2c-2/new_device
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/slab.h>

#define DEVICE_NAME	"ssd1306"

#define SSD1306_WIDTH		128
#define SSD1306_HEIGHT		64
#define SSD1306_PAGES		(SSD1306_HEIGHT / 8)

/* Control bytes */
#define SSD1306_CTRL_CMD	0x00
#define SSD1306_CTRL_DATA	0x40

/* Font: 5x8 pixels, printable ASCII 0x20–0x7E */
#define FONT_WIDTH	5
#define FONT_HEIGHT	8
#define FONT_FIRST	0x20
#define FONT_LAST	0x7E

/*
 * Standard 5x8 font table — each character is 5 bytes (columns),
 * each byte represents 8 vertical pixels (1 page).
 */
static const u8 font5x8[][FONT_WIDTH] = {
	/* 0x20 ' ' */ {0x00, 0x00, 0x00, 0x00, 0x00},
	/* 0x21 '!' */ {0x00, 0x00, 0x5F, 0x00, 0x00},
	/* 0x22 '"' */ {0x00, 0x07, 0x00, 0x07, 0x00},
	/* 0x23 '#' */ {0x14, 0x7F, 0x14, 0x7F, 0x14},
	/* 0x24 '$' */ {0x24, 0x2A, 0x7F, 0x2A, 0x12},
	/* 0x25 '%' */ {0x23, 0x13, 0x08, 0x64, 0x62},
	/* 0x26 '&' */ {0x36, 0x49, 0x55, 0x22, 0x50},
	/* 0x27 ''' */ {0x00, 0x05, 0x03, 0x00, 0x00},
	/* 0x28 '(' */ {0x00, 0x1C, 0x22, 0x41, 0x00},
	/* 0x29 ')' */ {0x00, 0x41, 0x22, 0x1C, 0x00},
	/* 0x2A '*' */ {0x08, 0x2A, 0x1C, 0x2A, 0x08},
	/* 0x2B '+' */ {0x08, 0x08, 0x3E, 0x08, 0x08},
	/* 0x2C ',' */ {0x00, 0x50, 0x30, 0x00, 0x00},
	/* 0x2D '-' */ {0x08, 0x08, 0x08, 0x08, 0x08},
	/* 0x2E '.' */ {0x00, 0x60, 0x60, 0x00, 0x00},
	/* 0x2F '/' */ {0x20, 0x10, 0x08, 0x04, 0x02},
	/* 0x30 '0' */ {0x3E, 0x51, 0x49, 0x45, 0x3E},
	/* 0x31 '1' */ {0x00, 0x42, 0x7F, 0x40, 0x00},
	/* 0x32 '2' */ {0x42, 0x61, 0x51, 0x49, 0x46},
	/* 0x33 '3' */ {0x21, 0x41, 0x45, 0x4B, 0x31},
	/* 0x34 '4' */ {0x18, 0x14, 0x12, 0x7F, 0x10},
	/* 0x35 '5' */ {0x27, 0x45, 0x45, 0x45, 0x39},
	/* 0x36 '6' */ {0x3C, 0x4A, 0x49, 0x49, 0x30},
	/* 0x37 '7' */ {0x01, 0x71, 0x09, 0x05, 0x03},
	/* 0x38 '8' */ {0x36, 0x49, 0x49, 0x49, 0x36},
	/* 0x39 '9' */ {0x06, 0x49, 0x49, 0x29, 0x1E},
	/* 0x3A ':' */ {0x00, 0x36, 0x36, 0x00, 0x00},
	/* 0x3B ';' */ {0x00, 0x56, 0x36, 0x00, 0x00},
	/* 0x3C '<' */ {0x00, 0x08, 0x14, 0x22, 0x41},
	/* 0x3D '=' */ {0x14, 0x14, 0x14, 0x14, 0x14},
	/* 0x3E '>' */ {0x41, 0x22, 0x14, 0x08, 0x00},
	/* 0x3F '?' */ {0x02, 0x01, 0x51, 0x09, 0x06},
	/* 0x40 '@' */ {0x32, 0x49, 0x79, 0x41, 0x3E},
	/* 0x41 'A' */ {0x7E, 0x11, 0x11, 0x11, 0x7E},
	/* 0x42 'B' */ {0x7F, 0x49, 0x49, 0x49, 0x36},
	/* 0x43 'C' */ {0x3E, 0x41, 0x41, 0x41, 0x22},
	/* 0x44 'D' */ {0x7F, 0x41, 0x41, 0x22, 0x1C},
	/* 0x45 'E' */ {0x7F, 0x49, 0x49, 0x49, 0x41},
	/* 0x46 'F' */ {0x7F, 0x09, 0x09, 0x01, 0x01},
	/* 0x47 'G' */ {0x3E, 0x41, 0x41, 0x51, 0x32},
	/* 0x48 'H' */ {0x7F, 0x08, 0x08, 0x08, 0x7F},
	/* 0x49 'I' */ {0x00, 0x41, 0x7F, 0x41, 0x00},
	/* 0x4A 'J' */ {0x20, 0x40, 0x41, 0x3F, 0x01},
	/* 0x4B 'K' */ {0x7F, 0x08, 0x14, 0x22, 0x41},
	/* 0x4C 'L' */ {0x7F, 0x40, 0x40, 0x40, 0x40},
	/* 0x4D 'M' */ {0x7F, 0x02, 0x04, 0x02, 0x7F},
	/* 0x4E 'N' */ {0x7F, 0x04, 0x08, 0x10, 0x7F},
	/* 0x4F 'O' */ {0x3E, 0x41, 0x41, 0x41, 0x3E},
	/* 0x50 'P' */ {0x7F, 0x09, 0x09, 0x09, 0x06},
	/* 0x51 'Q' */ {0x3E, 0x41, 0x51, 0x21, 0x5E},
	/* 0x52 'R' */ {0x7F, 0x09, 0x19, 0x29, 0x46},
	/* 0x53 'S' */ {0x46, 0x49, 0x49, 0x49, 0x31},
	/* 0x54 'T' */ {0x01, 0x01, 0x7F, 0x01, 0x01},
	/* 0x55 'U' */ {0x3F, 0x40, 0x40, 0x40, 0x3F},
	/* 0x56 'V' */ {0x1F, 0x20, 0x40, 0x20, 0x1F},
	/* 0x57 'W' */ {0x7F, 0x20, 0x18, 0x20, 0x7F},
	/* 0x58 'X' */ {0x63, 0x14, 0x08, 0x14, 0x63},
	/* 0x59 'Y' */ {0x03, 0x04, 0x78, 0x04, 0x03},
	/* 0x5A 'Z' */ {0x61, 0x51, 0x49, 0x45, 0x43},
	/* 0x5B '[' */ {0x00, 0x00, 0x7F, 0x41, 0x41},
	/* 0x5C '\' */ {0x02, 0x04, 0x08, 0x10, 0x20},
	/* 0x5D ']' */ {0x41, 0x41, 0x7F, 0x00, 0x00},
	/* 0x5E '^' */ {0x04, 0x02, 0x01, 0x02, 0x04},
	/* 0x5F '_' */ {0x40, 0x40, 0x40, 0x40, 0x40},
	/* 0x60 '`' */ {0x00, 0x01, 0x02, 0x04, 0x00},
	/* 0x61 'a' */ {0x20, 0x54, 0x54, 0x54, 0x78},
	/* 0x62 'b' */ {0x7F, 0x48, 0x44, 0x44, 0x38},
	/* 0x63 'c' */ {0x38, 0x44, 0x44, 0x44, 0x20},
	/* 0x64 'd' */ {0x38, 0x44, 0x44, 0x48, 0x7F},
	/* 0x65 'e' */ {0x38, 0x54, 0x54, 0x54, 0x18},
	/* 0x66 'f' */ {0x08, 0x7E, 0x09, 0x01, 0x02},
	/* 0x67 'g' */ {0x08, 0x14, 0x54, 0x54, 0x3C},
	/* 0x68 'h' */ {0x7F, 0x08, 0x04, 0x04, 0x78},
	/* 0x69 'i' */ {0x00, 0x44, 0x7D, 0x40, 0x00},
	/* 0x6A 'j' */ {0x20, 0x40, 0x44, 0x3D, 0x00},
	/* 0x6B 'k' */ {0x00, 0x7F, 0x10, 0x28, 0x44},
	/* 0x6C 'l' */ {0x00, 0x41, 0x7F, 0x40, 0x00},
	/* 0x6D 'm' */ {0x7C, 0x04, 0x18, 0x04, 0x78},
	/* 0x6E 'n' */ {0x7C, 0x08, 0x04, 0x04, 0x78},
	/* 0x6F 'o' */ {0x38, 0x44, 0x44, 0x44, 0x38},
	/* 0x70 'p' */ {0x7C, 0x14, 0x14, 0x14, 0x08},
	/* 0x71 'q' */ {0x08, 0x14, 0x14, 0x18, 0x7C},
	/* 0x72 'r' */ {0x7C, 0x08, 0x04, 0x04, 0x08},
	/* 0x73 's' */ {0x48, 0x54, 0x54, 0x54, 0x20},
	/* 0x74 't' */ {0x04, 0x3F, 0x44, 0x40, 0x20},
	/* 0x75 'u' */ {0x3C, 0x40, 0x40, 0x20, 0x7C},
	/* 0x76 'v' */ {0x1C, 0x20, 0x40, 0x20, 0x1C},
	/* 0x77 'w' */ {0x3C, 0x40, 0x30, 0x40, 0x3C},
	/* 0x78 'x' */ {0x44, 0x28, 0x10, 0x28, 0x44},
	/* 0x79 'y' */ {0x0C, 0x50, 0x50, 0x50, 0x3C},
	/* 0x7A 'z' */ {0x44, 0x64, 0x54, 0x4C, 0x44},
	/* 0x7B '{' */ {0x00, 0x08, 0x36, 0x41, 0x00},
	/* 0x7C '|' */ {0x00, 0x00, 0x7F, 0x00, 0x00},
	/* 0x7D '}' */ {0x00, 0x41, 0x36, 0x08, 0x00},
	/* 0x7E '~' */ {0x08, 0x04, 0x08, 0x10, 0x08},
};

struct ssd1306_dev {
	struct i2c_client *client;
	struct miscdevice misc;
	u8 cursor_page;
	u8 cursor_col;
};

/* ---- Low-level I2C helpers ---- */

static int ssd1306_write_cmd(struct i2c_client *client, u8 cmd)
{
	u8 buf[2] = {SSD1306_CTRL_CMD, cmd};

	return i2c_master_send(client, buf, 2);
}

static int ssd1306_write_data(struct i2c_client *client,
			      const u8 *data, int len)
{
	u8 *buf;
	int ret;

	buf = kmalloc(len + 1, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	buf[0] = SSD1306_CTRL_DATA;
	memcpy(buf + 1, data, len);
	ret = i2c_master_send(client, buf, len + 1);
	kfree(buf);
	return ret;
}

/* ---- SSD1306 display control ---- */

static int ssd1306_set_cursor(struct ssd1306_dev *dev, u8 page, u8 col)
{
	struct i2c_client *client = dev->client;
	int ret;

	/* Set page address */
	ret = ssd1306_write_cmd(client, 0xB0 | (page & 0x07));
	if (ret < 0)
		return ret;

	/* Set column address low nibble */
	ret = ssd1306_write_cmd(client, col & 0x0F);
	if (ret < 0)
		return ret;

	/* Set column address high nibble */
	ret = ssd1306_write_cmd(client, 0x10 | ((col >> 4) & 0x0F));
	if (ret < 0)
		return ret;

	dev->cursor_page = page;
	dev->cursor_col = col;
	return 0;
}

static int ssd1306_clear(struct ssd1306_dev *dev)
{
	u8 zeros[SSD1306_WIDTH];
	int page, ret;

	memset(zeros, 0x00, sizeof(zeros));

	for (page = 0; page < SSD1306_PAGES; page++) {
		ret = ssd1306_set_cursor(dev, page, 0);
		if (ret < 0)
			return ret;
		ret = ssd1306_write_data(dev->client, zeros,
					 SSD1306_WIDTH);
		if (ret < 0)
			return ret;
	}

	return ssd1306_set_cursor(dev, 0, 0);
}

static int ssd1306_putchar(struct ssd1306_dev *dev, char ch)
{
	const u8 *glyph;
	u8 space = 0x00;
	int ret;

	if (ch == '\n') {
		dev->cursor_page++;
		if (dev->cursor_page >= SSD1306_PAGES)
			dev->cursor_page = 0;
		dev->cursor_col = 0;
		return ssd1306_set_cursor(dev, dev->cursor_page,
					  dev->cursor_col);
	}

	if (ch < FONT_FIRST || ch > FONT_LAST)
		ch = '?';

	/* Check if character fits on current line */
	if (dev->cursor_col + FONT_WIDTH + 1 > SSD1306_WIDTH) {
		dev->cursor_page++;
		if (dev->cursor_page >= SSD1306_PAGES)
			dev->cursor_page = 0;
		dev->cursor_col = 0;
		ret = ssd1306_set_cursor(dev, dev->cursor_page,
					 dev->cursor_col);
		if (ret < 0)
			return ret;
	}

	glyph = font5x8[ch - FONT_FIRST];
	ret = ssd1306_write_data(dev->client, glyph, FONT_WIDTH);
	if (ret < 0)
		return ret;

	/* 1 pixel spacing between characters */
	ret = ssd1306_write_data(dev->client, &space, 1);
	if (ret < 0)
		return ret;

	dev->cursor_col += FONT_WIDTH + 1;
	return 0;
}

/*
 * SSD1306 initialization sequence
 * Reference: SSD1306 datasheet, section 3 (Command Table)
 */
static const u8 ssd1306_init_cmds[] = {
	0xAE,		/* Display OFF */
	0xD5, 0x80,	/* Set clock div ratio / oscillator freq */
	0xA8, 0x3F,	/* Set multiplex ratio: 64 lines */
	0xD3, 0x00,	/* Set display offset: none */
	0x40,		/* Set display start line: 0 */
	0x8D, 0x14,	/* Enable charge pump */
	0x20, 0x02,	/* Memory addressing mode: page */
	0xA1,		/* Segment re-map: col 127 = SEG0 */
	0xC8,		/* COM scan direction: remapped */
	0xDA, 0x12,	/* Set COM pins config: alternative */
	0x81, 0xCF,	/* Set contrast: 0xCF */
	0xD9, 0xF1,	/* Set pre-charge period */
	0xDB, 0x40,	/* Set VCOMH deselect level */
	0xA4,		/* Display from RAM */
	0xA6,		/* Normal display (not inverted) */
	0x2E,		/* Deactivate scroll */
	0xAF,		/* Display ON */
};

static int ssd1306_init_display(struct ssd1306_dev *dev)
{
	int i, ret;

	for (i = 0; i < sizeof(ssd1306_init_cmds); i++) {
		ret = ssd1306_write_cmd(dev->client, ssd1306_init_cmds[i]);
		if (ret < 0)
			return ret;
	}

	return ssd1306_clear(dev);
}

/* ---- File operations ---- */

static ssize_t ssd1306_fop_write(struct file *filep,
				 const char __user *buf,
				 size_t count, loff_t *offset)
{
	struct ssd1306_dev *dev = container_of(filep->private_data,
					       struct ssd1306_dev, misc);
	char *kbuf;
	size_t len;
	int i, ret;

	if (count == 0)
		return 0;

	kbuf = kmalloc(count + 1, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, buf, count)) {
		kfree(kbuf);
		return -EFAULT;
	}
	kbuf[count] = '\0';

	/* Strip trailing newline from echo */
	len = count;
	while (len > 0 && kbuf[len - 1] == '\n')
		len--;

	/* Empty string (or only newlines) => clear screen */
	if (len == 0) {
		ret = ssd1306_clear(dev);
		kfree(kbuf);
		return ret < 0 ? ret : count;
	}

	/* Clear screen and print from top-left */
	ret = ssd1306_clear(dev);
	if (ret < 0) {
		kfree(kbuf);
		return ret;
	}

	for (i = 0; i < len; i++) {
		ret = ssd1306_putchar(dev, kbuf[i]);
		if (ret < 0) {
			kfree(kbuf);
			return ret;
		}
	}

	kfree(kbuf);
	return count;
}

static const struct file_operations ssd1306_fops = {
	.owner	= THIS_MODULE,
	.write	= ssd1306_fop_write,
};

/* ---- I2C driver ---- */

static int ssd1306_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct ssd1306_dev *dev;
	int ret;

	dev = devm_kzalloc(&client->dev, sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->client = client;
	i2c_set_clientdata(client, dev);

	ret = ssd1306_init_display(dev);
	if (ret < 0) {
		dev_err(&client->dev, "display init failed: %d\n", ret);
		return ret;
	}

	dev->misc.minor = MISC_DYNAMIC_MINOR;
	dev->misc.name = DEVICE_NAME;
	dev->misc.fops = &ssd1306_fops;

	ret = misc_register(&dev->misc);
	if (ret) {
		dev_err(&client->dev, "misc_register failed: %d\n", ret);
		return ret;
	}

	dev_info(&client->dev,
		 "SSD1306 OLED probed at 0x%02x, /dev/%s created\n",
		 client->addr, DEVICE_NAME);
	return 0;
}

static int ssd1306_remove(struct i2c_client *client)
{
	struct ssd1306_dev *dev = i2c_get_clientdata(client);

	ssd1306_clear(dev);
	ssd1306_write_cmd(client, 0xAE); /* Display OFF */
	misc_deregister(&dev->misc);
	dev_info(&client->dev, "SSD1306 removed\n");
	return 0;
}

static const struct of_device_id ssd1306_of_match[] = {
	{ .compatible = "training,ssd1306" },
	{ }
};
MODULE_DEVICE_TABLE(of, ssd1306_of_match);

static const struct i2c_device_id ssd1306_id[] = {
	{ "training_ssd1306", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ssd1306_id);

static struct i2c_driver ssd1306_driver = {
	.driver = {
		.name		= DEVICE_NAME,
		.of_match_table	= ssd1306_of_match,
	},
	.probe		= ssd1306_probe,
	.remove		= ssd1306_remove,
	.id_table	= ssd1306_id,
};
module_i2c_driver(ssd1306_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("SSD1306 OLED I2C driver - display characters via /dev/ssd1306");
