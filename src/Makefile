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
CFLAGS += -O2
# CFLAGS += -nostdlib 
# CFLAGS += -nostartfiles

# Linker Flags 
LDFLAGS := -m elf_i386
LDFLAGS += -nostdlib

# Link against libgcc so I don't have to 
# reimplement 64-bit multiplication + division
LIBGCC := $(shell gcc -m32 -print-libgcc-file-name)

#
# Directories and Filenames
#

# C Code Libraries
LIB		:= libraries

# Header Files Location
INCL 	:= ../include
ASM 	:= asm
BD 		:= ../build/kernel

# Linker Script
K_LD := kernel_linker.ld

# Kernel Entry
KENTRY    := kernel-entry.asm
KENTRY_O  := $(BD)/kernel-entry.o

# C Library Sources/Objects
CLIB   := $(wildcard $(LIB)/*.c)
CLIB_O := $(patsubst $(LIB)/%.c,$(BD)/%.o,$(CLIB))

# ASM Files
KASM   := $(wildcard $(ASM)/*.asm)
KASM_O := $(patsubst $(ASM)/%.asm,$(BD)/%.o,$(KASM))

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

$(BD)/%.o: $(ASM)/%.asm | $(BD)
	$(ASBL) -f elf32 $< -o $@

# Link
$(ELF): $(CLIB_O) $(KENTRY_O) $(KASM_O) | $(BD)
	$(LD) $(LDFLAGS) -T $(K_LD) -o $@ $(KENTRY_O) $(CLIB_O) $(KASM_O) $(LIBGCC)

clean:
	rm -rf $(BD)