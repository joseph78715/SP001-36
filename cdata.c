#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define CDATA_MAJOR 121
#define	BUF_SIZE	32

struct cdata_t {
	char buf[BUF_SIZE];
	int  idx;
};

static int cdata_open(struct inode *inode, struct file *filp)
{
	struct cdata_t *cdata;

	printk(KERN_ALERT "cdata in open: filp = %p\n", filp);

	cdata = kmalloc(sizeof(*cdata), GFP_KERNEL);
	cdata->idx = 0;

	filp->private_data = (void *)cdata;

	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "cdata in close\n");
	return 0;
}

static ssize_t cdata_read(struct file *filp, const char __user *user, 
	size_t size, loff_t *off)
{
	printk(KERN_ALERT "cdata in read\n");
	return 0;
}

static ssize_t cdata_write(struct file *filp, const char __user *user, 
	size_t size, loff_t *off)
{
	struct cdata_t *cdata = (struct cdata_t *)filp->private_data;
	int i;
	int idx;

	idx = cdata->idx;

	for (i = 0; i < size; i++) {
		cdata->buf[idx] = user[i];
		idx++;
	}

	cdata->idx = idx;

	return 0;
}

static long cdata_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_ALERT "cdata in ioctl\n");
	return 0;
}

static struct file_operations cdata_fops = {
    owner:      THIS_MODULE,

    open:		cdata_open,
    read:		cdata_read,
    write:		cdata_write,
    unlocked_ioctl:	cdata_ioctl,
    release:    	cdata_close
};

int cdata_init_module(void)
{
	if (register_chrdev(CDATA_MAJOR, "cdata", &cdata_fops)) {
	    printk(KERN_ALERT "cdata module: can't registered.\n");
	    return -1;
    	}
	printk(KERN_ALERT "cdata module: registerd!\n");
	return 0;
}

void cdata_cleanup_module(void)
{
	unregister_chrdev(CDATA_MAJOR, "cdata");
	printk(KERN_ALERT "cdata module: unregisterd.\n");
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
