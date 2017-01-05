#!/bin/bash

arm-linux-gnueabi-gcc -o acceleroTest acceleroTest.c
scp acceleroTest root@192.168.10.10:/home/root/aryalex
