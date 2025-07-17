# This file guide step to build sdk for cross compile device_driver for rasp-pi4 with Yocto.

1. build the sdk:

- `$ bitbake core-image-base -c populate_sdk`

2. Go to the `sdk` folder:

-  `$ cd tmp/deploy/sdk`

3. Set up SDK/toolchain:

- `$ ./poky-glib`

4. Source the environment for sdk

- `$ source ` 