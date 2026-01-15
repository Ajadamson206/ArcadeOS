#!/usr/bin/env bash
set -e

if [ -f $1  ]; then
    echo "Usage: ./loadImage.sh [imageFile]"
fi

qemu-system-i386 -drive format=raw,file=$1 -no-reboot -d int
