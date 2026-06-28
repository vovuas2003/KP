#!/bin/bash

if [ "$EUID" -ne 0 ]; then
  echo "ERROR: run me as root!" >&2
  exit 1
fi

echo "tmpfs /var/log tmpfs defaults,noatimes,size=50M 0 0" >> /etc/fstab

mkdir -p /etc/systemd/journald.conf.d
cat << 'EOF' > /etc/systemd/journald.conf.d/00-mmc-care.conf
[Journal]
Storage=volatile
MaxLevelStore=info
MaxLevelSyslog=info
EOF

rm -rf /var/log/*
reboot
