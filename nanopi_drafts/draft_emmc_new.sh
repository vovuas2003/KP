#!/bin/bash

if [ "$EUID" -ne 0 ]; then
  echo "ERROR: run me as root!" >&2
  exit 1
fi

# check (and off) swap
# check /etc/fstab:
## MAYBE /tmp, /var/tmp to tmpfs defaults,noatime,nosuid,nodev,size=256M  0  0
##noatime anywhere
# /etc/systemd/journald.conf - [Journal] Storage=volatile RuntimeMaxUse=32M
# use overlayfs:
##mkdir -p /tmp/log_rw /tmp/log_work
## in /etc/fstab
##tmpfs   /tmp   tmpfs   defaults,noatime,nosuid,nodev,size=256M   0   0
##overlay /var/log overlay noatime,lowerdir=/var/log,upperdir=/tmp/log_rw,workdir=/tmp/log_work 0 0

# clean all logs (maybe it is possible to do better)
#find /var/log -type f -name "*.gz" -delete
#find /var/log -type f -name "*.1" -delete
#find /var/log -type f -exec truncate -s 0 {} +
# ??? Maybe edit image file (/var/log to tmpfs before first boot) and flash already prepared image (to clean system) !!!

#reboot
