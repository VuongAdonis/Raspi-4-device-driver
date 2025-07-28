#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>

dev_t dev = MKDEV(2355, 0);

static int __init init_static_alloc(void)
{
    printk(KERN_INFO "This is the module to statically allocate the major and minor number. \n");
    if (register_chrdev_region(dev, 1, "Static allocate") == 0)
    {
        printk(KERN_INFO "Statically allocate successfully! \n");
        printk(KERN_INFO "MAJOR number = %d, MINOR number = %d \n", MAJOR(dev), MINOR(dev));
    }
    else {
        printk(KERN_INFO "Statically allocate failed! \n");
    }
    return 0;
}

static void __exit exit_static_alloc(void)
{
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Unloaded the module statically allocate successfully! \n");
}

module_init(init_static_alloc);
module_exit(exit_static_alloc);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis");
MODULE_DESCRIPTION("Module to statically allocate the major and minor number!");

