#!/usr/bin/env bash

set -e

dd if=/dev/zero of=build/arcadeOS.img bs=512 count=128 status=none
dd if=build/boot/stage1.bin of=build/arcadeOS.img bs=512 conv=notrunc status=none
dd if=build/boot/stage2.pad.bin of=build/arcadeOS.img bs=512 seek=1 conv=notrunc status=none
dd if=build/kernel/arcade.elf of=build/arcadeOS.img bs=512 seek=12 conv=notrunc status=none

qemu-system-i386 -cpu qemu32 -drive format=raw,file=build/arcadeOS.img -serial stdio -no-reboot -d int -D build/qemu.log