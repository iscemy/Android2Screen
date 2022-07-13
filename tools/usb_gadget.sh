#!/sbin/sh
echo 0 > /sys/class/android_usb/android0/enable
echo 18d1 > /sys/class/android_usb/android0/idVendor
echo d02 > /sys/class/android_usb/android0/idProduct
echo \$1 > /sys/class/android_usb/android0/functions
echo 1 > /sys/class/android_usb/android0/enable
#stop adbd
#start adbd
setprop sys.usb.state \$1
setprop sys.usb.config \$1
setprop service.adb.root 1

