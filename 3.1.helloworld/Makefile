ARCH ?= arm
CROSS_COMPILE ?= arm-poky-linux-gnueabi-

KERNEL_SRC := /home/adonisking/Learning_Linux/3.Device_driver/SDK/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/usr/src/kernel
SRC_DIR := $(shell pwd)/src

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	cd src
	find . -maxdepth 1 ! -name 'helloworld.c' ! -name '.' -exec rm -v {} +

	

