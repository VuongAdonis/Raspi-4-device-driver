obj-m += helloworld.o

# Default ARCH to empty if not defined
ARCH ?=

# Set KERNEL_SRC and CROSS_COMPILE based on ARCH value
ifeq ($(ARCH),arm64)
    # Raspberry Pi 4 64-bit kernel source and cross-compile prefix
    KERNEL_SRC ?= /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/cortexa72-poky-linux/usr/src/kernel
    CROSS_COMPILE ?= aarch64-poky-linux-gnu-
else
    # Default: native Linux kernel build directory and no cross-compiler
    KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
    CROSS_COMPILE :=
endif

all:
	@echo "Building for ARCH=$(ARCH)"
	@echo "Using Kernel source: $(KERNEL_SRC)"
	@echo "Cross compile prefix: $(CROSS_COMPILE)"
	$(MAKE) -C $(KERNEL_SRC) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(shell pwd) clean
