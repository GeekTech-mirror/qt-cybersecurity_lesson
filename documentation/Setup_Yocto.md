The Yocto Project
=============
Yocto is a Embedded LInux Build System. Yocto provides a flexible set of tools and configurations to help design tailored Linux images for a variety of embedded and IOT devices.

## Layers
The software stack is seperated into a series of Layers. Each layer contains a different set of scripts for building applications. Because the architecture of each Embedded Device is different, the applications must be built from source.

### *poky*
Poky is a reference distribution  of the Yocto Project. Linux distributions are Operating Systems, made from a collection of software and a Linux kernel. The specific combination of software and software install methods vary between distros. The Poky distribution can either be used by itself or serve as a base for creating your own distro.

### *meta-openembedded*
OpenEmbedded is responsible for the build system. OpenEmbedded is a widely used build framework for embedded linux. Yocto Project has incorpated OpenEmbedded into their own software stack to provide a robust build system.



## Build

// sudo pacman -S --needed rpcsvc-proto chrpath texinfo cpio diffstat

git clone --branch honister git://git.yoctoproject.org/poky.git

cd poky

git clone -b honister git://git.openembedded.org/meta-openembedded
git clone -b honister git://git.yoctoproject.org/meta-raspberrypi
git clone -b 6.2.3 git://code.qt.io/cgit/yocto/meta-qt6
git clone -b 6.3 git://code.qt.io/cgit/yocto/meta-boot2qt

meta-boot2qt/b2qt-init-build-env init --device raspberrypi4-64

source oe-init-build-env

bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-raspberrypi
bitbake-layers add-layer ../meta-qt6

// Remove line /meta-yocto-bsp from build/conf/bblayers.conf

// Add `MACHINE ?= "raspberrypi4-64"` to build/conf/local.conf

bitbake core-image-minimal

# Optionally build qemuarm64
runqemu qemuarm64


# yocto tools
+ recipetool appendfile