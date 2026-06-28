#!/bin/bash
set -euo pipefail

# maybe sleeps can be deleted

MY_TOOL_PATH=/path/to/rkdeveloptool

$MY_TOOL_PATH db MiniLoaderAll.bin
sleep 1

# gpt is important, not just prm
$MY_TOOL_PATH gpt parameter.txt
sleep 1

# rk3566-usb-debian-trixie-core-6.1-arm64-20260420
$MY_TOOL_PATH wlx uboot uboot.img
$MY_TOOL_PATH wlx misc misc.img
$MY_TOOL_PATH wlx dtbo dtbo.img
$MY_TOOL_PATH wlx resource resource.img
$MY_TOOL_PATH wlx kernel kernel.img
$MY_TOOL_PATH wlx boot boot.img
$MY_TOOL_PATH wlx rootfs rootfs.img
$MY_TOOL_PATH wlx userdata userdata.img

$MY_TOOL_PATH rd
