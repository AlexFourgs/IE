#!/bin/bash

sudo service network-manager stop
iwconfig wlan0 essid "aryaalex" channel 2 mode Ad-hoc
ifconfig wlan0 192.168.10.2
