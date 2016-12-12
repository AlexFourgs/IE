#!/bin/bash

gcc -o serverTest serveurTest.c
arm-linux-gnueabi-gcc -o clientGumstix clientTest.c
scp clientGumstix root@192.168.10.10:/home/root/aryalex
./serverTest
