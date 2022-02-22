// sudo pacman -S --needed rpcsvc-proto chrpath texinfo cpio diffstat

git clone --branch honister git://git.yoctoproject.org/poky.git

cd poky

git clone -b honister git://git.yoctoproject.org/meta-raspberrypi
git clone -b honister git://git.openembedded.org/meta-openembedded
git clone -b 6.2.3 https://code.qt.io/cgit/yocto/meta-qt6.git

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