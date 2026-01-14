# Low Level Arcade Machine Application

#
# Tools
#

# Compilier
CC := gcc

# Linker
LD := ld

# Object Copy
OBJ := objcopy

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
# Directories and Filenames
#

# Working Directory
WD := src

# Assembly Directory
ASM := $(WD)/asm

# Build Directory
BD := build

# Assembly Source Code Macros
ASMSRC := $(wildcard $(ASM)/*.S)	# Grab all ASM Files
ASMOBJ := $(patsubst $(ASM)/%.S,$(BD)/%.o,$(ASMSRC)) # Create Copy filenames that end in .o $(patsubst pattern,replacement,text)

# C Code Macros
CSRC   := $(wildcard $(WD)/*.c)		# Grab all C Files
COBJ   := $(patsubst $(WD)/%.c,$(BD)/%.o,$(CSRC)) # Create Copy Filenames that end in .o $(patsubst pattern,replacement,text)

# Output Files
ELF := $(BD)/arcade.elf
BIN := $(BD)/arcade.bin

#
# Scripts
#

# These should not be file names anyway, but still good to have
.PHONY: all clean

# Use "make all" to build the project
all: $(BIN)

# Make build directory
$(BD):
	mkdir -p $(BD)

# Assemble all Asseembly
$(BD)/%.o: $(ASM)/%.S | $(BD)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the C Code
$(BD)/%.o: $(WD)/%.c | $(BD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything together
$(ELF): $(ASMOBJ) $(COBJ)
	cd $(BD) && $(LD) $(LDFLAGS) -o $(notdir $(ELF)) $(notdir $(ASMOBJ)) $(notdir $(COBJ))

# Make the binary image
$(BIN): $(ELF)
	$(OBJ) -O binary $(ELF) $(BIN)

# Cleaning Script to delete all build files
clean:
	rm -rf $(BD)