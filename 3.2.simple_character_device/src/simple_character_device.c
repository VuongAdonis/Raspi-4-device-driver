#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> // copy_to_user, copy_from_user
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adonis<vuong03062003@gmail.com>");
MODULE_DESCRIPTION("Simple character device driver!");

#define DEVICE_NAME "simplechardev"
#define BUF_SIZE 1024

static dev_t dev_number;
static struct cdev simple_cdev;
static struct class *simple_class;
static char kernel_buffer[BUF_SIZE];
static int open_count = 0;

// Function open device
static int my_simple_open(struct inode *inode, struct file *file)
{
    open_count++;
    printk(KERN_INFO "simple char dev: Device opened %d times\n", open_count);
    return 0;
}

// Function close device
static int my_simple_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "simple char dev: Device closed\n");
    return 0;
}

// Function read device
static ssize_t my_simple_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    size_t bytes_to_read;

    if(*offset >= BUF_SIZE)
    {
        return 0; // EOF
    }

    bytes_to_read = BUF_SIZE - *offset;

    if(len < bytes_to_read)
    {
        bytes_to_read = len;
    }

    if(copy_to_user(buf, kernel_buffer + *offset, bytes_to_read))
    {
        return -EFAULT;
    }

    *offset += bytes_to_read;
    printk(KERN_INFO "simplechardev: Read %zu bytes\n", bytes_to_read);
    return bytes_to_read;
}

// Function write device
static ssize_t my_simple_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    size_t bytes_to_write;

    if(*offset >= BUF_SIZE)
    {
        return -ENOMEM;
    }

    bytes_to_write = BUF_SIZE - *offset;

    if(len < bytes_to_write)
    {
        bytes_to_write = len;
    }

    if(copy_from_user(kernel_buffer + *offset, buf, bytes_to_write))
    {
        return -EFAULT;
    }

    *offset += bytes_to_write;
    printk(KERN_INFO "simplechardev: Wrote %zu bytes\n", bytes_to_write);
    return bytes_to_write;
}

// Define file operations
static struct file_operations fops = {
    .owner      = THIS_MODULE,
    .open       = my_simple_open,
    .release    = my_simple_release,
    .read       = my_simple_read,
    .write      = my_simple_write,
};

// init function
static int __init simple_init(void)
{
    int ret;

    // Register dynamic device number
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if(ret < 0)
    {
        printk(KERN_ERR "simplechardev: Failed to allocate device number\n");
        return ret;
    }

    // Init cdev
    cdev_init(&simple_cdev, &fops);
    ret = cdev_add(&simple_cdev, dev_number, 1);
    if(ret < 0)
    {
        unregister_chrdev_region(dev_number, 1);
        printk(KERN_ERR "simplechardev: Failed to add cdev\n");
        return ret;
    }

    // Create class for automatically create /dev/simplechardev
    simple_class = class_create(DEVICE_NAME);
    if(IS_ERR(simple_class))
    {
        cdev_del(&simple_cdev);
        unregister_chrdev_region(dev_number, 1);
        printk(KERN_ERR "simplechardev: Failed to create class\n");
        return -1;
    }

    if(device_create(simple_class, NULL, dev_number, NULL, DEVICE_NAME) == NULL)
    {
        class_destroy(simple_class);
        cdev_del(&simple_cdev);
        unregister_chrdev_region(dev_number, 1);
        printk(KERN_ERR "simplechardev: Failed to create device\n");
        return -1;
    }

    printk(KERN_INFO "simplechardev: Module loaded. Device /dev/%s ready\n", DEVICE_NAME);
    return 0;
}

static void __exit simple_exit(void)
{
    device_destroy(simple_class, dev_number);
    cdev_del(&simple_cdev);
    class_destroy(simple_class);
    unregister_chrdev_region(dev_number, 1);
    printk(KERN_INFO "simplechardev: Module unloaded success!\n");
}

module_init(simple_init);
module_exit(simple_exit);