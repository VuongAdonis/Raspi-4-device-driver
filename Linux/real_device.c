#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>     // kmalloc


#define mem_size 1024

dev_t dev = 0;
struct class *class_real_device;
static struct cdev cdev_real_device;
uint8_t *kernel_buffer;

// Function prototypes
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *file, const char __user*buf, size_t len, loff_t *off); 

static struct file_operations fops =
{
    .owner  = THIS_MODULE,
    .open   = etx_open,
    .release  = etx_release,
    .read   = etx_read,
    .write  = etx_write,
};

static int etx_open(struct inode *inode, struct file *file)
{
    /* Creating physical memory */
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0) 
    {
        printk(KERN_INFO "Cannot allocate memory in kernel!\n");
        return -1;
    }
    printk(KERN_INFO "Device file opened...!\n");
    return 0;
}

static ssize_t etx_write(struct file *file, const char *buf, size_t len, loff_t *off)
{
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Data write: Done...!\n");
    return len;
}

static ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    copy_to_user(buf, kernel_buffer, mem_size);
    printk(KERN_INFO "Data read: Done...!\n");
    return mem_size;
}

static int etx_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Device close...!\n");
    return 0;
}

static int __init real_device_init(void)
{
    if(alloc_chrdev_region(&dev, 0, 1, "real_device") < 0)
    {
        pr_err("Cannot register major and minor number!\n");
        return -1;
    }
    printk(KERN_INFO "Register device success with MAJOR = %d, MINOR = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&cdev_real_device, &fops);
    if(cdev_add(&cdev_real_device, dev, 1) < 0)
    {
        pr_err("Cannot add character device! \n");
        goto err_class;
    }
    printk(KERN_INFO "Add character device successfully!\n");

    class_real_device = class_create("class_real_device_class");
    if(IS_ERR(class_real_device))
    {
        pr_err("Cannot create class!\n");
        goto err_class;
    }
    printk(KERN_INFO "Create class successfully! \n");

    if(IS_ERR(device_create(class_real_device, NULL, dev, NULL, "real_device")))
    {
        pr_err("Cannot create device!\n");
        goto err_device;
    }
    printk(KERN_INFO "Create device successfully!\n");

    printk(KERN_INFO "Load module real device successfully!\n");
    return 0;

err_device:
    class_destroy(class_real_device);
err_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit real_device_exit(void)
{
    device_destroy(class_real_device, dev);
    class_destroy(class_real_device);
    cdev_del(&cdev_real_device);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Unload module real device successfully!\n");
}

module_init(real_device_init);
module_exit(real_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis <vuong03062003@gmail.com");
MODULE_VERSION("1.0");