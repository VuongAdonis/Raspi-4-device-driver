#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/proc_fs.h>

#define mem_size 1024
#define WR_VALUE    _IOW('a', 'a', int32_t*)
#define RD_VALUE    _IOR('a', 'b', int32_t*)

char procfs_array[20] = "try_proc_array\n";
static int len = 1;

dev_t dev = 0;
struct class *class_procfs;
struct cdev cdev_procfs;
struct proc_dir_entry *parent;
uint8_t *kernel_buffer;
uint8_t value = 0;

static int file_open(struct inode *innode, struct file *file);
static int file_release(struct inode *inode, struct file *file);
static ssize_t file_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t file_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/* Procfs function */
static int open_procfs(struct inode *inode, struct file *file);
static int release_procfs(struct inode *inode, struct file *file);
static ssize_t read_procfs(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t write_procfs(struct file *file, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops =
{
    .owner              =   THIS_MODULE,
    .open               =   file_open,
    .release            =   file_release,
    .read               =   file_read,
    .write              =   file_write,
    .unlocked_ioctl     =   file_ioctl,  
};

static struct proc_ops proc_fops =
{
    .proc_open      =   open_procfs,
    .proc_release   =   release_procfs,
    .proc_read      =   read_procfs,
    .proc_write     =   write_procfs,
};

static int file_open(struct inode *inode, struct file * file)
{
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
    {
        pr_err("Cannot allocate kernel buffer!\n");
        return -1;
    }
    printk(KERN_INFO "Allocate success, file open!\n");
    return 0;
}

static int file_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Free kernel buffer, file close!\n");
    return 0;
}

static ssize_t file_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Reading data ...");
    copy_to_user(buf, kernel_buffer, mem_size);
    printk(KERN_INFO "Done!\n");
    return mem_size;
}

static ssize_t file_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Writting data ...");
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Done!\n");
    return len;
}

static long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case WR_VALUE:
            if(copy_from_user(&value, (int32_t*)arg, sizeof(value)))
            {
                pr_err("Data write err!\n");
            }
            printk(KERN_INFO "IOCTL Data write: value = %d \n", value);
            break;
        case RD_VALUE:
            if(copy_to_user((int32_t*)arg, &value, sizeof(value)))
            {
                pr_err("Data Read err!\n");
            }
            printk(KERN_INFO "IOCTL Data read: Done!\n");
            break;
        default:
            printk(KERN_INFO "IOCTL Default!\n");
            break;
    }
    return 0;
}

static int open_procfs(struct inode *inode, struct file *file)
{
    pr_info("Proc file open .... !\n");
    return 0;
}

static int release_procfs(struct inode *inode, struct file *file)
{
    pr_info("Proc file release ... !\n");
    return 0;
}

static ssize_t read_procfs(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("proc file read ... \n");
    if(len)
    {
        len = 0;
    }
    else 
    {
        len = 1;
        return 0;
    }
    
    if(copy_to_user(buf, procfs_array, 20))
    {
        pr_err("Data send: Err!\n");
        return -EFAULT;
    }
    return len;
}

static ssize_t write_procfs(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("proc file write ... !\n");
    
    if(copy_from_user(procfs_array, buf, len))
    {
        pr_err("Data write: Err!\n");
        return -EFAULT;
    }
    return len;
}

static int __init module_driver_init(void)
{
    if(alloc_chrdev_region(&dev, 0, 1, "character_device") < 0)
    {
        pr_err("Cannot create character device!\n");
        return -1;
    }
    printk(KERN_INFO "Create character device with MAJOR=%d, MINOR=%d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&cdev_procfs, &fops);
    if(cdev_add(&cdev_procfs, dev, 1) < 0)
    {
        pr_err("Cannot add character device!\n");
        goto err_cdev;
    }
    printk(KERN_INFO "Add character device success!\n");

    class_procfs = class_create("class_device");
    if(IS_ERR(class_procfs))
    {
        pr_err("Cannot create class!\n");
        goto err_class;
    }
    printk(KERN_INFO "Create class success with name: class_device!\n");

    if(IS_ERR(device_create(class_procfs, NULL, dev, NULL, "device_procfs")))
    {
        pr_err("Cannot create device!\n");
        goto err_device;
    }
    pr_info("Create device success!\n");

    /* Create proc directory. It will create a directory under "/proc" */
    parent = proc_mkdir("etx", NULL);

    if(parent == NULL)
    {
        pr_info("Error creating proc entry");
        goto err_proc;
    }

    /* Creating proc entry under "/proc/etx/" */
    proc_create("etx_proc", 0666, parent, &proc_fops);

    pr_info("Load module success!\n");
    return 0;

err_proc:
    device_destroy(class_procfs, dev);
err_device:
    class_destroy(class_procfs);
err_class:
    cdev_del(&cdev_procfs);
err_cdev:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit module_driver_exit(void)
{
    /* Remove single proc entry */
    // remove_proc_entry("/etx/etx_proc", parent);

    /* Remove complete /proc/etx*/
    proc_remove(parent);
    device_destroy(class_procfs, dev);
    class_destroy(class_procfs);
    cdev_del(&cdev_procfs);
    unregister_chrdev_region(dev, 1);
    pr_info("Unload module success!\n");
}

module_init(module_driver_init);
module_exit(module_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis <vuong03062003@gmail.com>");
MODULE_DESCRIPTION("Module for procfs");
MODULE_VERSION("1.0");

