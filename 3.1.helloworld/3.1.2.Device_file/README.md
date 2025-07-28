# Check device file:

`ls -l /dev/ | grep "etx_device"`

--------------------------------------------------

# There are two ways to create a device file

1. Manually

2. Automatically

--------------------------------------------------

1. Manually:

- Using mknod.

`mknod -m <permissions> <name> <device type> <major> <minor>`

- <name>: Your device file name that should have a full path (/dev/name)
- <device type>: Put c or b

c - Character device - Transfer data one byte (or character) at a time.

b - Block device - Transfer data in fixed-size blocks, typically 512 bytes or multiples thereof

- <major>: major number of your device

- <minor>: minor number of your device

- -m <permission>: optional argument that sets the permission bits of the new device file to permissions.

Example: sudo mknod -m 666 /dev/etx_device c 246 0


------------------------------------------------

2. Automatically:

- Follow the below steps:

2.1. Include the header file `linux/device.h` and `linux/kdev_t.h`

2.2. Create the struct class:

- `struct class *class_create(struct module *owner, consnt char* name);`

[in] owner - pointer to the module that is to 'owm' this struct class

[in] name - pointer to the string for the name of this class

-> Destroy class with `void class_destroy(struct class *cls);`

2.3. Create device:

- `struct device *device_create(struct *class, struct device *parent, dev_t dev, void *drvdata, const char *fmt, ...);`

[in] class - pointer to the struct class that this device should register to.

[in] parent - pointer to the parent struct device of this device, if any.

[in] dev - the dev_t for the char device to be added.

[in] drvdata - the data to be added to the device for callbacks.

[in] fmt - string for the device's name.

... - variable arguments

--> Destroy device with `void device_destroy(struct class *class, dev_t *dev);`

---------------------------------------------------------------------------
