#!/bin/bash

iwconfig wlan0 essid "aryalex" channel 2 mode Ad-hoc
ifconfig wlan0 192.168.10.10
i2cset -y 3 0x1d 0x20 0x47 b
