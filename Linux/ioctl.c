#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

#define mem_size 1024
/* Define the IOCTL command */
/* #define "ioctl_name" _IOX("magic number", "command number", "argument type") 
    @ IO: an ioctl with no parameters
    @ IOW: an ioctl with write parameters (copy_from_user)
    @ IOR: an ioctl with read parameters (copy_to_user)
    @ IOWR: an ioctl with both read and write parameters.
*/
#define WR_VALUE _IOW('a', 'a', int32_t*)
#define RD_VALUE _IOR('a', 'b', int32_t*)

dev_t dev = 0;
struct class *class_ioctl;
static struct cdev cdev_ioctl;
uint8_t *kernel_buffer;
uint8_t value;

/* Function prototype */
static int ioctl_open(struct inode* inode, struct file *file);
static int ioctl_release(struct inode *inode, struct file *file);
static ssize_t ioctl_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t ioctl_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static long ioctl_func(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .open           = ioctl_open,
    .release        = ioctl_release,
    .read           = ioctl_read,
    .write          = ioctl_write,
    .unlocked_ioctl = ioctl_func,
};

static int ioctl_open(struct inode *inode, struct file *file)
{
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
    {
        pr_err("Cannot allocate for kernel buffer! \n");
        return -1;
    }
    printk(KERN_INFO "Allocate kernel buffer successfully! \n");
    return 0;
}

static int ioctl_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Free kernel buffer...!\n");
    return 0;
}

static ssize_t ioctl_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read data: ... ");
    copy_to_user(kernel_buffer, buf, mem_size);
    printk(KERN_INFO "Done!\n");
    return mem_size;
}

static ssize_t ioctl_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write data: ...");
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Done!\n");
    return len;
}

static long ioctl_func(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {   
        case WR_VALUE:
            if(copy_from_user(&value, (int32_t*)arg, sizeof(value)))
            {
                pr_err("Data write: Err!\n");
            }
            pr_info("Value = %d\n", value);
            break;
        case RD_VALUE:
            if(copy_to_user((int32_t *)arg, &value, sizeof(value)))
            {
                pr_err("Data read: Err!\n");
            }
            break;
        default:
            pr_info("Default!\n");
            break;
    }
    return 0;
}

static int __init ioctl_init(void)
{
    if(alloc_chrdev_region(&dev, 0, 1, "ioctl_chrdev") < 0)
    {
        pr_err("Cannot allocate register major and minor number!\n");
        return -1;
    }
    printk(KERN_INFO "Allocate success with MAJOR=%d, MINOR=%d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&cdev_ioctl, &fops);
    if(cdev_add(&cdev_ioctl, dev, 1) < 0)
    {
        pr_err("Cannot add cdev device!\n");
        goto err_cdev;
    }
    printk(KERN_INFO "Add cdev success!\n");

    class_ioctl = class_create("ioctl_class");
    if(IS_ERR(class_ioctl))
    {
        pr_err("Cannot create class!\n");
        goto err_class;
    }
    printk(KERN_INFO "Create class success!");

    if(IS_ERR(device_create(class_ioctl, NULL, dev, NULL, "ioctl_device")))
    {
        pr_err("Cannot create device!\n");
        goto err_device;
    }
    printk(KERN_INFO "Create device success!\n");
    
    printk(KERN_INFO "Load module for ioctl success!\n");

    return 0;

err_device:
    class_destroy(class_ioctl);
err_class:
    cdev_del(&cdev_ioctl);
err_cdev:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit ioctl_exit(void)
{
    device_destroy(class_ioctl, dev);
    class_destroy(class_ioctl);
    cdev_del(&cdev_ioctl);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "Unload module for ioctl success!\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis <vuong03062003@gmail.com");
MODULE_DESCRIPTION("Module for ioctl");
MODULE_VERSION("1.0");