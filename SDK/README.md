# This file guide step to build sdk for cross compile device_driver for rasp-pi4 with Yocto.

## If you don't has the toolchain or you want to build it yourself, you can use Yocto Project.

1. Source the environment:
- `$ . oe-init-build-env`

1. build the sdk:

- `$ bitbake core-image-base -c populate_sdk`

2. Go to the `sdk` folder:

-  `$ cd tmp/deploy/sdk`


3. Set up SDK/toolchain:

- `$ ./poky-glibc-x86_64-core-image-base-cortexa7t2hf-neon-vfpv4-raspberrypi4-toolchain-5.2.1.sh`

- You can choose the path to setup your toolchain when run the command above.

- `Enter target directory for SDK (default: /opt/poky/5.2.1):`

4. If this is the first time you build, prepare for the source SDK:

- `<Path in the previous step>/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/usr/src/kernel`
- `$ make ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- prepare`
- `$ make ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- scripts`

5. Go to the folder you setup toolchain

6. Source the environment for sdk

- `$ source environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi` 

7. Then build your application with the cross-compiler environment active.