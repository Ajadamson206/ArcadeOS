#
# Tools
#

# Compilier
CC 		?= gcc

# Assembler (NASM)
ASBL	?= nasm

# Linker
LD 		?= ld

#
# Settings
#

# Compilier Flags
CFLAGS := -m32 
CFLAGS += -ffreestanding 
CFLAGS += -fno-pic 
CFLAGS += -fno-pie
CFLAGS += -fno-stack-protector
CFLAGS += -masm=intel
# CFLAGS += -nostdlib 
# CFLAGS += -nostartfiles

# Linker Flags 
LDFLAGS := -m elf_i386
LDFLAGS += -nostdlib

#
# Directories and Filenames
#

# C Code Libraries
LIB		:= libraries

# Header Files Location
INCL 	:= ../include
# ASM 	:= $(WD)/asm		# Assembly Files
BD 		:= ../build/kernel# Output Directory

# Linker Script
K_LD := kernel_linker.ld

# Kernel Entry
KENTRY    := kernel-entry.asm
KENTRY_O  := $(BD)/kernel-entry.o

# C Library Sources/Objects
CLIB   := $(wildcard $(LIB)/*.c)
CLIB_O := $(patsubst $(LIB)/%.c,$(BD)/%.o,$(CLIB))

# Output
ELF := $(BD)/arcade.elf
.PHONY: kernel

kernel: $(ELF)

$(BD):
	mkdir -p $@

# Assemble kernel entry -> object in BD
$(KENTRY_O): $(KENTRY) | $(BD)
	$(ASBL) -f elf32 $< -o $@

# Compile libraries/*.c -> BD/*.o
$(BD)/%.o: $(LIB)/%.c | $(BD)
	$(CC) -I$(INCL) $(CFLAGS) -c $< -o $@

# Compile arcade-main

# Link
$(ELF): $(CLIB_O) $(KENTRY_O) | $(BD)
	$(LD) $(LDFLAGS) -T $(K_LD) -o $@ $(KENTRY_O) $(CLIB_O)

clean:
	rm -rf $(BD)