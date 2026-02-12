# Makefile to build the bootloader

#
# Tools
#

# Use ?= if it has already been defined by root file
CC 		?= gcc 		# Compilier
ASBL	?= nasm		# Assembler (NASM)
LD 		?= ld		# Linker
OBJ 	?= objcopy	# Object Copy

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

CWD := .
BD ?= ../build/boot

S2_CDIR := $(CWD)/stage2/src
S2_ASM  := $(CWD)/stage2/stage2entry.asm
S2_LD   := $(CWD)/stage2/stage2_linker.ld

S2_ELF  := $(BD)/stage2.elf
S2_OUT  := $(BD)/stage2.bin

S1      := $(CWD)/stage1.asm
S1_OUT  := $(BD)/stage1.bin

S2_CSRC := $(wildcard $(S2_CDIR)/*.c)
S2_COBJ := $(patsubst $(S2_CDIR)/%.c,$(BD)/%.o,$(S2_CSRC))

S2_ASM_O := $(BD)/stage2.o

S2_PAD	:= $(BD)/stage2.pad.bin

INCL 	:= $(CWD)/stage2/include	# Header Files Location

# Build the Stage 2 Bootloader First
.PHONY: bootloader test

bootloader: $(S1_OUT)

# Ensure build dir exists
$(BD):
	mkdir -p $@

#
# Build the stage 2 binary file
#

# Assemble Stage 2 Entry
$(S2_ASM_O): $(S2_ASM) | $(BD)
	$(ASBL) -f elf32 $< -o $(S2_ASM_O)

# Compile Stage 2 C Files
$(BD)/%.o: $(S2_CDIR)/%.c | $(BD)
	$(CC) -I$(INCL) $(CFLAGS) -c $< -o $@

# Link Stage 2 Elf
$(S2_ELF): $(S2_ASM_O) $(S2_COBJ) | $(BD)
	$(LD) $(LDFLAGS) -T $(S2_LD) -o $@ $^

# Make a flat binary for stage 2
$(S2_OUT): $(S2_ELF) | $(BD)
	$(OBJ) -O binary $< $@

# Pad S2 to the Next 512 Byte Sector to make loading easier
$(S2_PAD): $(S2_OUT)
	@size=$$(stat -c '%s' $<); \
	pad_to=$$(( ((size + 511) / 512) * 512 )); \
	$(OBJ) -I binary -O binary --pad-to=$$pad_to --gap-fill=0x00 $< $@

#
# Build the stage 1 binary file
#

# Build Stage 1 binary file
$(S1_OUT): $(S1) $(S2_PAD) | $(BD)
	@stage2_bytes=$$(stat -c '%s' $(S2_PAD)); \
	stage2_sectors=$$(( (stage2_bytes + 511) / 512 )); \
	$(ASBL) $(S1) -f bin -DSTAGE2_SECTORS=$$stage2_sectors -o $(S1_OUT)


