#!/usr/bin/sudo bash

make
insmod homework1.ko
rmmod homework1.ko
tail /var/log/kern.log
make clean