# Full path of communication:

 Applications ---> Device File or Device Node ---> Major and minor number ---> Device driver ---> Hardware

 - Application will open the device file (device file was created by device driver). Then device file will find corresponding device driver using major and minor number. Then device driver will talk to the Hardware.
 
# To read major number

1. command:

    `$ cat \proc\devices`

# Minor number

- Minor number is to identify the corresponding driver. Many device using the same major numbers, so using minor number to assign for each device.

# Allocating Major and Minor number

There are 2 ways:

1. Statically allocating

2. Dynamically allocating

------------------------------------------------------------------

## 1. Staically allocating

1.1.  Usig `int register_chrdev_region(dev_t first, unsigned int count, char *name);`

[in] first: the beginning device number of the range you would like to allocate.

[in] count: the total number of contiguous device numbers you are requesting.

[in] name: name of device that should be associated with this number range. (appear in /proc/device and sysfs)

[out] return value: 0 is allocation was successfully and negative error in error case.

Note: `dev_t` type is defined in `linux/types.h` and used to hold the `major` and `minor` number.

1.2. Create `dev_t` structure variable:

1.2.1 MKDEV(int major, int minor);

1.2.2. To get major and minor number:

- MAJOR(dev_t dev);

- MINOR(dev_t dev);

Example used:

dev_t dev = MKDEV(235, 0);

register_chrdev_region(dev, 1, "Embedtronicx_Dev");

-------------------------------------------------------------

# 2. Dynamically alocating

-> In case don't want to fixed major and minor number

2.1. Using `int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);`

[in] dev: variable that store the first device number allocated if the call succeeds.

[in] firstminor: should be the requested first minor number to use; it is ussually 0;

[in] count: total number of contiguous you are requesting.

[in] name: the name of device should be associated with this number range. (appear in /proc/devices/ and sysfs)


---------------------------------------------------------------

# 3. Unrergister the major and minor number.

3.1. Using `unregister_chrdev_regionn(dev_t first, unsigned int count);`

