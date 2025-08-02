#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

dev_t dev = 0;
struct class *class_file_ops;
static struct cdev file_ops_cdev;

// Fucntion prototypes
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t  etx_write(struct file *file, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = etx_open,
    .release = etx_release,
    .write = etx_write,
    .read = etx_read,
};

static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Driver open function call.....!\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Driver release function call...!\n");
    return 0;
}

ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver read function call...!\n");
    return 0;
}

ssize_t etx_write(struct file *file, const char *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver write function call...!\n");
    return 0;
}

static int __init file_ops_init(void)
{
    if(alloc_chrdev_region(&dev, 0, 1, "file_ops_device") < 0)
    {
        pr_err("Allocate major and minor number failed!\n");
        return -1;
    }
    printk(KERN_INFO "Allocate successfully with major = %d, minor = %d \n", MAJOR(dev), MINOR(dev));

    // Creating cdev structure
    cdev_init(&file_ops_cdev, &fops);

    // Adding character device to the system
    if((cdev_add(&file_ops_cdev, dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system! \n");
        goto err_class;
    }

    class_file_ops = class_create("file_ops_class");
    if(IS_ERR(class_file_ops))
    {
        pr_err("Create class failed!\n");
        goto err_class;
    }
    printk(KERN_INFO "Create class successfully!\n");

    if(IS_ERR(device_create(class_file_ops, NULL, dev, NULL, "file_ops_device")))
    {
        pr_err("Create device failed!\n");
        goto err_device;
    }

    printk(KERN_INFO "Load module for file operations successfully! \n");
    return 0;

err_device:
    class_destroy(class_file_ops);

err_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit file_ops_exit(void)
{
    device_destroy(class_file_ops, dev);
    class_destroy(class_file_ops);
    cdev_del(&file_ops_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Unload module for file operations successfully! \n");
}

module_init(file_ops_init);
module_exit(file_ops_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis");
MODULE_DESCRIPTION("Module for exercise create a file operations");
MODULE_VERSION("1.0");