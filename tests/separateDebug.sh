#!/usr/bin/env bash
set -e

if ! [ -f "build/kernel/arcade.debug" ]; then
    objcopy --only-keep-debug build/kernel/arcade.elf build/kernel/arcade.debug
    objcopy --strip-debug --add-gnu-debuglink=build/kernel/arcade.debug build/kernel/arcade.elf
fi

dd if=/dev/zero of=build/arcadeOS.img bs=512 count=128 status=none
dd if=build/boot/stage1.bin of=build/arcadeOS.img bs=512 conv=notrunc status=none
dd if=build/boot/stage2.pad.bin of=build/arcadeOS.img bs=512 seek=1 conv=notrunc status=none
dd if=build/kernel/arcade.elf of=build/arcadeOS.img bs=512 seek=12 conv=notrunc status=none
