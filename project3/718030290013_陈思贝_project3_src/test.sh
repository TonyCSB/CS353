#!/usr/bin/sudo bash

insmod mtest.ko
echo listvma > /proc/mtest
dmesg

echo findpage xxxxxxxxxxxxxxx > /proc/mtest
dmesg | tail -1

echo writeeval xxxxxxxxxxxxxxx x > /proc/mtest
dmesg | tail -1