# Set COM port where RF access point is mounted
# set com "/dev/ttyACM0"
set com [lindex [glob -directory /dev/serial/by-id -types l usb-Texas_Instruments_{CC1111,eZ430-ChronosAP}_*-if00] 0]

# udev rule to assign a fixed name under /dev to the USB dongle
# TI RF USB Dongle
#KERNEL=="ttyACM*", ATTRS{idVendor}=="0451", ATTRS{idProduct}=="16a6", SYMLINK += "ti-rfdongle"
