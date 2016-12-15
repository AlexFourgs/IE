#!/bin/bash

sudo service network-manager stop
sudo ifconfig wlan0 up
sudo iwconfig wlan0 essid "aryalex" mode Ad-hoc channel 2
sudo ifconfig wlan0 192.168.10.2

ping 192.168.10.10
