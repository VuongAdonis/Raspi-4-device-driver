# cdev structure

-> In linux kernel, `struct inode` is used to represent files.

-> There can be numerous of file structures representing multiple open descriptors on a single file, but they all point to a single inode structure.

1. `struct cdev` is one of the elements of the inode structure. An inode structure is used by the kernel internally to represent files.

2. `struct cdev` is the kernel's internal structure that represents char devices.

3. `cdev structure:`

struct cdev {

    struct kobject kobj ;
    struct module *owner;
    const struct file_operations *ops;
    struct list_head list;
    dev_t dev;
    unsigned int count;

}

-> We need to fill 2 fields:

- file_operation 

- owner (This should be THIS_MODULE)

-----------------------------------------------------------------

2 ways to allocating and initializing cdev structure:

1. Runtime Allocation

2. Own allocation

---

1. Runtime Allocation:

struct cdev *my_dev = cdev_alloc();

my_cdev->ops = &my_fops;

2. OWn allocation:

Using these function:

`void cdev_init(struct cdev *cdev, struct file_operations *fops);`

`int cdev_add(struct cdev *dev, dev_t num, unsigned int count);`

---

# File operations

1. Some fields explaination:

    1.1. struct module *owner:

- Almost all the time, it is simply initialized to THIS_MODULE, a macro defined in <linux/module.h>

    1.2. read

`ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);`

- A Non-negative return value represents the number of bytes successfully read (the return value is  a 'signed size' type, usually the native integer type for the target platform).

    1.3. write

`ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);`

- The return value, if non-negative, represents the number of bytes successfully written.

    1.4. ioctl

`int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);`

- Unlike standard system calls such as read(), write(), ioctl is used for controlling device behavior or performing special tasks specific to the device.

    1.5. open

`int (*open) (struct inode *, struct file *);`

    1.6. release

`int (*release) (struct innode *, struct file *);`

Example file operations:

static struct file_operations fops = 

{

    .owner      = THIS_MODULE,
    .read       = etx_read,
    .write      = etx_write,
    .open       = etx_open,
    .release    = etx_release,

};

