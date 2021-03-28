#!/usr/bin/sudo bash

make
insmod homework1.ko paramInt=1231312 paramStr='"test string"' paramArr='1,2,3'
cat /proc/test
cat /proc/hello_dir/test
echo 'nice' > /proc/hello_dir/test
cat /proc/hello_dir/test
rmmod homework1.ko
make clean


dmesg | tail -10