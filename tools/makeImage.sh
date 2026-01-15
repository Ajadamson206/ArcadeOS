#!/usr/bin/env bash
set -e

if [ -zf $1  ] || [ -z $2 ]; then
    echo "Usage: ./makeImage.sh [binaryFile] [outputName]"
fi

# Create Empty Image
dd if=/dev/zero of=$2.img bs=512 count=16

# Copy Binary File
dd if=$1 of=$2.img bs=512 count=1 conv=notrunc