#!/usr/bin/sudo bash

make
insmod mtest.ko
echo listvma > /proc/mtest
dmesg | tail -15
# rmmod mtest


# echo findpage xxxxxxxxxxxxxxx > /proc/mtest
# dmesg | tail -1

# echo writeeval xxxxxxxxxxxxxxx x > /proc/mtest
# dmesg | tail -1