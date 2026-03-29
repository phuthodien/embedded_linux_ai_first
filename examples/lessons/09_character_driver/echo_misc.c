/*
 * echo_misc.c - Misc character driver with open/read/write/release
 *
 * Write data to /dev/echo_misc, read it back. Demonstrates all four
 * basic file_operations using the misc device framework.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define BUF_SIZE	4096
#define DEVICE_NAME	"echo_misc"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training");
MODULE_DESCRIPTION("Misc char driver demo: open/read/write/release");

struct echo_dev {
	char *buf;
	size_t len;
};

static struct echo_dev edev;

static int echo_open(struct inode *inode, struct file *filep)
{
	filep->private_data = &edev;
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
	struct echo_dev *dev = filep->private_data;
	size_t to_read;

	if (*offset >= dev->len)
		return 0;

	to_read = min(count, dev->len - (size_t)*offset);

	if (copy_to_user(buf, dev->buf + *offset, to_read))
		return -EFAULT;

	*offset += to_read;
	return to_read;
}

static ssize_t echo_write(struct file *filep, const char __user *buf,
			   size_t count, loff_t *offset)
{
	struct echo_dev *dev = filep->private_data;
	size_t to_write;

	to_write = min(count, (size_t)BUF_SIZE);

	if (copy_from_user(dev->buf, buf, to_write))
		return -EFAULT;

	dev->len = to_write;
	return to_write;
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

static int __init echo_init(void)
{
	int ret;

	edev.buf = kzalloc(BUF_SIZE, GFP_KERNEL);
	if (!edev.buf)
		return -ENOMEM;
	edev.len = 0;

	ret = misc_register(&echo_misc);
	if (ret) {
		pr_err(DEVICE_NAME ": misc_register failed: %d\n", ret);
		kfree(edev.buf);
		return ret;
	}

	pr_info(DEVICE_NAME ": registered at /dev/%s\n", echo_misc.name);
	return 0;
}

static void __exit echo_exit(void)
{
	misc_deregister(&echo_misc);
	kfree(edev.buf);
	pr_info(DEVICE_NAME ": unregistered\n");
}

module_init(echo_init);
module_exit(echo_exit);
