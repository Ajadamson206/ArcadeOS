#!/usr/bin/env bash
set -e

if [ ! -f $1  ] || [ -z $2 ]; then
    echo "Usage: ./makeImage.sh [binaryFile] [outputName]"
    exit 1
fi

# Create Empty Image
echo "Creating Image File"
dd if=/dev/zero of=$2.img bs=512 count=16

# Copy Binary File
echo "Copying the Binary File"
dd if=$1 of=$2.img bs=512 count=1 conv=notrunc