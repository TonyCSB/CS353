#!/usr/bin/sudo bash

make
insmod homework1.ko paramInt=1231312 paramStr='"test string"' paramArr='1,2,3'
rmmod homework1.ko
tail /var/log/kern.log
make clean