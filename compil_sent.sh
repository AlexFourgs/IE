#!/bin/bash

gcc -o serverTest serveurTest.c -w
arm-linux-gnueabi-gcc -Wall -o clientGumstix I2C/i2cbusses.c I2C/util.c clientTest.c -w
scp clientGumstix root@192.168.10.10:/home/root/aryalex
./serverTest
