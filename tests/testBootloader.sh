#!/usr/bin/env bash

# Call Makefile
cd boot && make bootloader
cd ..

# Create the image
dd if=/dev/zero of=build/test.img bs=512 count=128 status=none
dd if=build/boot/stage1.bin of=build/test.img bs=512 conv=notrunc status=none
dd if=build/boot/stage2.pad.bin of=build/test.img bs=512 seek=1 conv=notrunc status=none

# Boot the Image
qemu-system-i386 -cpu qemu32 -drive format=raw,file=build/test.img -serial stdio -no-reboot -d int -D build/qemu.log