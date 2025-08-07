#!/bin/bash

# find tty name and give access to user without sudo -s
#
# sudo dmesg | grep tty
# ls -l /dev/ttyUSB0
# sudo usermod -aG dialout user
# sudo reboot

# after fresh putty install (better use N instead of X in sercfg)
# adjust window size and font size
#putty -serial /dev/ttyUSB0 -sercfg 1500000,8,1,n,X -fn "Monospace 12" -geometry 88x53 &

# or edit Default Settings in putty Saved Sessions (and add some extra settings)
# Session: Default Settings, Load
# Window: Columns 88, Rows 53, Lines of scrollback 1000000
# Window -> Selection: disable Auto-copy selected text to CLIPBOARD, set Mouse paste action to No action, CtrlShith+Ins to CLIPBOARD and Ctrl+Shift+CV to PRIMARY
# Window -> Fonts: Font used for ordinary text -> Change -> Monospace,Regular,12
# Session: Default Settings, Save
# make sure that default settings was edited: reopen putty and check it
# so, specify only serial (UART) settings, other settings will be loaded from Default Settings
# NOTE: on my Ubuntu I can't paste text from putty console to "Text Editor" on PC (and copy to console from PC), so use "gedit"; settings above allows to have 2 buffers:
# 1) Ctrl+Ins and Shift+Ins is CLIPBOARD, same behaviour and buffer in putty and PC
# 2) PRIMARY: in putty select text to copy (Ctrl+Shift+C too, but you need to select before, so it is overhead) and Ctrl+Shift+V to paste; on PC press mouse wheel (middle button) to paste
putty -serial /dev/ttyUSB0 -sercfg 1500000,8,1,n,N &
