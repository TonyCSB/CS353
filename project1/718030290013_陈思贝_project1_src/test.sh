#!/usr/bin/sudo bash

make
insmod homework1.ko paramInt=1231312 paramStr='"test string"' paramArr='1,2,3'

if [ -n $1 ]; then
    rmmod homework1.ko
    make clean
fi

dmesg | tail -10