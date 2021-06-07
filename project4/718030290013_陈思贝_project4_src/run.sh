#!/usr/bin/sudo bash

set -x

make
insmod romfs.ko hided_file_name=aa encrypted_file_name=bb exec_file_name=ft
mount -o loop test.img target
ls -l target/
ls -l source/