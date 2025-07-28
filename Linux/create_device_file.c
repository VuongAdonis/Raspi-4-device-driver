#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/err.h>

dev_t dev = 0;
static struct class *st_dev_class;

static int __init st_create_device_init(void)
{
    if(alloc_chrdev_region(&dev, 0, 1, "dynamic_alloc") < 0)
    {
        printk(KERN_INFO "Dynamic Allocate failed! \n");
        return -1;
    }
    printk("Dynamic allocate successfully with Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));
    
    // Newer kernels (6.4 and later): class_create now takes only 1 argument - the class name string.
    st_dev_class = class_create("etx_class");
    if(IS_ERR(st_dev_class))
    {
        pr_err("Cannot create struct class for this device! \n");
        goto r_class;
    }
    printk(KERN_INFO "Create struct class successfully for this device");

    if(IS_ERR(device_create(st_dev_class, NULL, dev, NULL, "etx_device")))
    {
        pr_err("Cannot create device file! \n");
        goto r_device;
    }
    printk(KERN_INFO "Create device file successfuly! \n");

    printk(KERN_INFO "Load module create device file successfully! \n");
    return 0;

r_device:
    class_destroy(st_dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit st_create_device_exit(void)
{
    device_destroy(st_dev_class, dev);
    class_destroy(st_dev_class);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Unloaded module create device file successfully! \n");
}

module_init(st_create_device_init);
module_exit(st_create_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis");
MODULE_DESCRIPTION("This module used to create device file.");
MODULE_VERSION("1.1");