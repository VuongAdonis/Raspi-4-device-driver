#include <linux/init.h>     //  for __init, __exit, __initdata
#include <linux/module.h>   // for MODULE_LICENSE(), MODULE_AUTHOR(), module_init(), module_exit()
#include <linux/kernel.h>   // for printk()
#include <linux/moduleparam.h>

int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

module_param(valueETX, int, S_IRUSR|S_IWUSR);                   // integer value
module_param(nameETX, charp, S_IRUSR|S_IWUSR);                  // String value
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR);   // Array of integer

/*------------------Module_param_cb()--------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
    int res = param_set_int(val, kp);   // use helper for write variable
    if( res == 0 )
    {
        printk(KERN_INFO "Callback function called...\n");
        printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
        return 0;
    }
    return -1;
}

const struct kernel_param_ops my_param_ops =
{
    .set = &notify_param,   // use our setter ...
    .get = &param_get_int,  // ... and standard getter
};

module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUSR|S_IWUSR);

static int __init hello_init(void) {
    printk(KERN_INFO "Helloworld! Simple Device Driver loaded.\n");
    int i;
    printk(KERN_INFO "ValueETX = %d \n", valueETX);
    printk(KERN_INFO "Value cb_valueETX = %d \n", cb_valueETX);
    printk(KERN_INFO "Value NameETX = %s \n", nameETX);
    for (i = 0; i<sizeof(arr_valueETX)/sizeof(int); i++)
    {
        printk(KERN_INFO "Arr_value[%d] = %d \n", i, arr_valueETX[i]);
    }
    printk(KERN_INFO "Kernel module inserted successfully... \n");
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