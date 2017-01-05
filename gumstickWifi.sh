#!/bin/bash

iwconfig wlan0 essid "aryalex" channel 2 mode Ad-hoc
ifconfig wlan0 192.168.10.10
