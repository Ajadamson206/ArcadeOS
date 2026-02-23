# Low Level Arcade Machine Application

#
# Tools
#

# Compilier
export CC 		:= gcc

# Assembler
export ASBL		:= nasm

# Linker
export LD 		:= ld

# Object Copy
export OBJ 		:= objcopy

#
# Settings
#

# Compilier Flags
CFLAGS := -m32 
CFLAGS += -ffreestanding 
CFLAGS += -fno-pic 
CFLAGS += -fno-pie
CFLAGS += -fno-stack-protector
# CFLAGS += -nostdlib 
# CFLAGS += -nostartfiles

# Linker Flags 
LDFLAGS := -m elf_i386 
LDFLAGS += -nostdlib 
LDFLAGS += -nostartfiles 
LDFLAGS += -T ../linker.ld

#
# Filenames + Directories
#

BD := build

DISK := $(BD)/arcadeOS.img

#
# Scripts
#

# These should not be file names anyway, but still good to have
.PHONY: all clean run bootloader kernel

$(BD):
	mkdir -p $@

# Use "make all" to build the project
all: kernel bootloader

# Run the disk image
run: $(DISK)
	qemu-system-i386 -drive format=raw,file=$(DISK) -serial stdio -no-reboot

# Cleaning Script to delete all build files
clean:
	rm -rf $(BD)

bootloader: $(BD)
	$(MAKE) -C boot bootloader BD=../$(BD)/boot --warn-undefined-variables

kernel: $(BD)
	$(MAKE) -C src kernel BD=../$(BD)/kernel --warn-undefined-variables

$(DISK):
	dd if=/dev/zero of=$@ bs=512 count=128 status=none;\
	dd if=build/boot/stage1.bin of=$@ bs=512 conv=notrunc status=none;\ 
	dd if=build/boot/stage2.pad.bin of=$@ bs=512 seek=1 conv=notrunc status=none;\ 
	dd if=build/kernel/arcade.elf of=$@ bs=512 seek=12 conv=notrunc status=none