#
# Tools
#

export CC 		?= gcc 		# Compilier
export ASBL		?= nasm		# Assembler (NASM)
export LD 		?= ld		# Linker
export OBJ 		?= objcopy	# Object Copy

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

LIB		:= libraries		# C Code Libraries
INCL 	:= ../include		# Header Files Location
# ASM 	:= $(WD)/asm		# Assembly Files
BD 		?= ../build/kernel	# Output Directory

# Kernel Entry
KENTRY := kernel-entry.asm

# C Library Macros
CLIB 	:= $(wildcard $(LIB)/*.c)				    # Grab all C Files
CLIB-O  := $(patsubst $(LIB)/%.c,$(BD)/%.o,$(CLIB)) # Create Copy Filenames that end in .o $(patsubst pattern,replacement,text)

# Output Files
ELF := $(BD)/arcade.elf

.PHONY: kernel

kernel: $(ELF)