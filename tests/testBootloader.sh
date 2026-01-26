#!/usr/bin/env bash

# Assemble the Files
nasm boot/stage1.asm -f bin -o build/stage1.bin
nasm boot/stage2/stage2entry.asm -f elf32 -o build/stage2.o

# Compile the C code
gcc -m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector -nostdlib -nostartfiles -c boot/stage2/src/c_entry.c -o build/stage2_c.o

# Link and Copy Objects
ld -m elf_i386 -nostdlib -T boot/stage2/stage2_linker.ld -o build/stage2.elf build/stage2.o build/stage2_c.o
objcopy -O binary build/stage2.elf build/stage2.bin

# Create the image
dd if=/dev/zero of=build/test.img bs=512 count=128 status=none
dd if=build/stage1.bin of=build/test.img bs=512 conv=notrunc status=none
dd if=build/stage2.bin of=build/test.img bs=512 seek=1 conv=notrunc status=none

# Boot the Image
qemu-system-i386 -cpu qemu32 -drive format=raw,file=build/test.img -serial stdio -no-reboot -d int -D build/qemu.log