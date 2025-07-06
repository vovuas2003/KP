#!/bin/bash

# sudo dmesg | grep tty
# ls -l /dev/ttyUSB0
# sudo usermod -aG dialout user
# reboot

putty -serial /dev/ttyUSB0 -sercfg 1500000,8,1,n,X -fn "Monospace 12" -geometry 88x53 &
