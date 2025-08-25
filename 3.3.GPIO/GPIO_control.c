#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/err.h>

static struct gpio_desc *led, *button;

#define IO_LED 21
#define IO_BUTTON 20

#define IO_OFFSET 512

// GPIO_06_IN value toggle
unsigned int led_toggle = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

/* Driver function */
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *file, const char *buf, size_t len, loff_t *off);
/****************** */

/* File operation structure */
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = etx_read,
    .write      = etx_write,
    .open       = etx_open,
    .release    = etx_release,
};

static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!\n");
    return 0;
}   

static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!\n");
    return 0;
}

static ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    uint8_t gpio_state = 0;

    // Reading GPIO value
    gpio_state = gpiod_get_value(led);

    // Write to user
    len = 1;
    if( copy_to_user(buf, &gpio_state, len) > 0)
    {
        pr_err("[ERROR]: Not all the bytes have been copied to user\n");
    }

    pr_info("[INFO]: Read function: GPIO_04 = %d \n", gpio_state);

    printk("Value button is: %d \n", gpiod_get_value(button));

    return 0;
}

static ssize_t etx_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    uint8_t rec_buf[10] = {0};

    if( copy_from_user(rec_buf, buf, len) > 0 )
    {
        pr_err("[ERROR]: Not all bytes have been copied from userr\n");
    }
    pr_info("[INFO]: Write Function: GPIO_04 Set = %c\n", rec_buf[0]);

    if( rec_buf[0] == '1' )
    {
        // Set the GPIO value to HIGH
        gpiod_set_value(led, 1);
    }
    else if ( rec_buf[0] == '0' )
    {
        // set the GPIO value to LOW
        gpiod_set_value(led, 1);
    }
    else
    {
        pr_err("[ERROR]: Unknown command: Please provide either 1 or 0 \n");
    }

    return len;
}

static int __init etx_driver_init(void)
{
    if( (alloc_chrdev_region(&dev, 0, 1, "etx_Dev") < 0) )
    {
        pr_err("[ERROR]: Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("[INFO]: Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);

    if( (cdev_add(&etx_cdev, dev, 1)) < 0 )
    {
        pr_err("[ERROR]: Cannot add the device to the system\n");
        goto r_del;
    }

    if( IS_ERR(dev_class = class_create("etx_class")) )
    {
        pr_err("[ERROR]: Cannot create the struct class\n");
        goto r_class;
    }

    if( IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device")) )
    {
        pr_err("[ERROR]: Cannot create the device\n");
        goto r_device;
    }

    led = gpio_to_desc(IO_LED + IO_OFFSET);
	if (!led) {
		printk("gpioctrl - Error getting pin 21\n");
        goto r_device;
		// return -ENODEV;
	}

	button = gpio_to_desc(IO_BUTTON + IO_OFFSET);
	if (!button) {
		printk("gpioctrl - Error getting pin 20\n");
        goto r_device;
		// return -ENODEV;
	}

	status = gpiod_direction_output(led, 0);
	if (status) {
		printk("gpioctrl - Error setting pin 20 to output\n");
		goto r_device;
        // return status;
	}

	status = gpiod_direction_input(button);
	if (status) {
		printk("gpioctrl - Error setting pin 21 to input\n");
        goto r_device;
		// return status;
	}

    gpiod_set_value(led, 1);

    printk("gpioctrl - Button is %spressed\n", gpiod_get_value(button) ? "" : "not ");

    pr_info("[INFO]: Device driver insert ... Done!\n");
    return 0;

r_device:
    device_destroy(dev_class, dev);
r_class:
    class_destroy(dev_class);
r_del:
    cdev_del(&etx_cdev);
r_unreg:
    unregister_chrdev_region(dev, 1);

    return -1;
}

static void __exit etx_driver_exit(void)
{
    gpiod_set_value(led, 0);
    device_destroy(dev_class, dev);
    class_destroy(dev_class); 
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("[INFO]: Device driver remove...Done!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver (GPIO Interrupt)");
MODULE_VERSION("1.0");

