#!/usr/bin/env bash

# Assemble the Files
nasm boot/stage1.asm -f bin -o build/stage1.bin
nasm boot/stage2/stage2entry.asm -f bin -o build/stage2.bin

# Create the image
dd if=/dev/zero of=build/test.img bs=512 count=16 status=none
dd if=build/stage1.bin of=build/test.img bs=512 conv=notrunc status=none
dd if=build/stage2.bin of=build/test.img bs=512 seek=1 conv=notrunc status=none

# Boot the Image
qemu-system-i386 -drive format=raw,file=build/test.img -serial stdio -no-reboot -d int -D build/qemu.log