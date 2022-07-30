The Yocto Project
=============
Yocto is a Embedded LInux Build System. Yocto provides a flexible set of tools and configurations to help design tailored Linux images for a variety of embedded and IOT devices.

Layers
=============
The software stack is seperated into a series of Layers. Like a cookbook, each layer contains a series of recipes. Recipes tells the compiler how to build applications. Because the architecture of each Embedded Device is different, the applications must be built differently for each device. Yocto handles the device specific configuration for building applications.

### *Poky*
Poky is a reference distribution designed for the Yocto Project. Linux is a family of open-source Unix-like Operating Systems. Each member of the Linux family is called a distribution.  Each distribution contains a Linux kernel and package manager. The package manager handles system and app updates. Each distro handles updates differently. The Poky distribution contains sane defaults that can be used by themselves or used to create another distribution.

### *OpenEmbedded*
OpenEmbedded is a robust build system designed for comiling applications for Embedded Systems. Yocto Project has incorporated OpenEmbedded into their own software stack.

### *Raspberry Pi*
This layer is a Board Support Package or BSP. The layer contains information about supported features on the Raspberry Pi. The layer also contains additional changes to other application recipes to help build proper support for the Raspberry Pi.

### *Qt6*
Qt is a modern UX design tool with cross-platform support for Linux, Mac-OS, Winodws, and Android. The Qt layer provides support for the Qt framework in yocto. 

### *Cybercraft*
Cybercraft is a layer designed by GeekTech to provide support for the Cybercraft STEM Learning Course. The course teaches common cyber attacks and the importance of online security.

Build
=============
## *Dependencies*
- Git 1.8.3.1+
- tar 1.28+
- Python 3.6.0+
- gcc 5.0+

Install the following development host packages. The required packages will vary depending on the host distribution.

- Ubuntu / Debian / openSUSE / CentOS / Linux Mint
    - `sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio 
	python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git 
	python3-jinja2 libegl1-mesa libsdl1.2-dev pylint3 xterm python3-subunit mesa-common-
	dev zstd liblz4-tool`
- Archlinux / Manjaro
    - `sudo pacman -S  --needed git diffstat unzip texinfo python chrpath wget xterm sdl
	rpcsvc-proto socat cpio inetutils`
    - Enable the multilib repository and install multilib-devel

git clone --branch honister git://git.yoctoproject.org/poky.git

cd poky

git clone -b kirkstone git://git.openembedded.org/meta-openembedded
git clone -b kirkstone git://git.yoctoproject.org/meta-raspberrypi
git clone -b 6.3 git://code.qt.io/yocto/meta-qt6.git
git clone https://invent.kde.org/packaging/yocto-meta-kf5.git
>git clone -b 6.3 git://code.qt.io/cgit/yocto/meta-boot2qt

>meta-boot2qt/b2qt-init-build-env init --device raspberrypi4-64

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