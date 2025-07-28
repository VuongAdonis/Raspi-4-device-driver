#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/init.h>
#include<linux/fs.h>

dev_t dev = 0;

static int __init dynamic_allocate_init(void)
{
    printk(KERN_INFO "Load dynamic allocate major minor successfully. \n");
    if(alloc_chrdev_region(&dev, 0, 1, "Dynamic allocate") < 0)
    {
        printk(KERN_INFO "Allocate failed! \n");
        return -1;
    }
    printk(KERN_INFO "Dynamic allocate successfully with Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));
    return 0;
}

static void __exit dynamic_allocate_exit(void)
{
    printk(KERN_INFO "Unload dynamic allocate major minor successfully. \n");
    unregister_chrdev_region(dev, 1);
}

module_init(dynamic_allocate_init);
module_exit(dynamic_allocate_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis");
MODULE_DESCRIPTION("Module to dynamic allocate major and minor number");