Layout
=============

## __*recipes-bsp*__
- __*rpi-config append*__ 
    - add config option for the Miuzei touchscreen

## __*recipes-connectivity*__
- __*dhcpcd append*__
    - add static ip for Raspberry Pi Zero access point
- __*hostapd append*__
    - add config to set the Raspberry Pi Zero as a access point
    - configure hostapd to run on start up
- __*libpcap append*__
    - build with libnl support for Deauther attack
    - libnl is required for creating the monitoring interface
- __*networkmanager append*__
    - add config for networkmanager to handle network interfaces by default
    - This ensures networkmanager can scan for access points in the War Driving attack

## __*recipes-core*__
- __*psplash append*__
    - display geek tech logo on boot

- __*systemd append*__
    - build systemd with networkd and resolved
    - allow systemd to handle dns queries by default

## __*recipes-kf5*__
- __*extra-cmake-modules*__
    - the full kde framework does not support qt6
    - the cybersecurity lesson uses NetworkManagerQt, which requires extra-cmake-modules
    - edit kf5 recipes to allow qt6

## __*recipes-qt*__
- __*qtbase append*__
    - add build options to enable eglfs and touch support
    - eglfs is responsible for displaying the app on boot
- __*qt-cybersecurity_lesson*__
    - build the cybersecurity lesson for the PenPi
    - configure the lesson to launch on boot

## __*recipes-support*__
- __*dnsmasq append*__
    - allocate ip range for Raspberry Pi Zero access point.