#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
    printk(KERN_INFO "Helloworld! Simple Device Driver loaded.\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Helloworld! Simple Device Driver unloaded!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis");
MODULE_DESCRIPTION("A simple helloworld device driver");
MODULE_VERSION("0.1");