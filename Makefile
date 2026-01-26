# Low Level Arcade Machine Application

#
# Tools
#

export CC 		:= gcc 		# Compilier
export ASBL	:= nasm		# Assembler (NASM)
export LD 		:= ld		# Linker
export OBJ 	:= objcopy	# Object Copy

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

WD 		:= src				# Main Code Directory
INCL 	:= $(WD)/include	# Header Files Location
BOOT 	:= boot				# Where the bootloader is located
ASM 	:= $(WD)/asm		# Assembly Files
BD 		:= build			# Output Directory

# Stage 1 Bootloader
STAGE1_SRC := $(BOOT)/stage1.asm
STAGE1_BIN := $(BD)/stage1.bin

# Assembly Application Code Macros
ASMSRC := $(wildcard $(ASM)/*.asm) # Grab all ASM Files 
ASMOBJ := $(patsubst $(ASM)/%.asm,$(BD)/%.o,$(ASMSRC)) # Create Copy filenames that end in .o $(patsubst pattern,replacement,text)

# C Code Macros
CSRC := $(wildcard $(WD)/*.c)		# Grab all C Files
COBJ := $(patsubst $(WD)/%.c,$(BD)/%.o,$(CSRC)) # Create Copy Filenames that end in .o $(patsubst pattern,replacement,text)

# Output Files
ELF := $(BD)/arcade.elf
BIN := $(BD)/arcade.bin

# Final img File
DISK := $(BD)/arcade.img

#
# Scripts
#

# These should not be file names anyway, but still good to have
.PHONY: all clean run bootloader

# Use "make all" to build the project
all: $(DISK)

# Make build directory
$(BD):
	mkdir -p $(BD)

# Assemble Kernel Assembly Objects
$(BD)/%.o: $(ASM)/%.asm | $(BD)
	$(ASBL) -f elf32 $< -o $@

# Compile the C Code
$(BD)/%.o: $(WD)/%.c | $(BD)
	$(CC) $(CFLAGS) -I$(INCL) -c $< -o $@

# Link everything together
$(ELF): $(ASMOBJ) $(COBJ)
	cd $(BD) && $(LD) $(LDFLAGS) -o $(notdir $(ELF)) $(notdir $(ASMOBJ)) $(notdir $(COBJ))

# Make the binary image
$(BIN): $(ELF)
	$(OBJ) -O binary $< $@

# Create the disk image
$(DISK): $(STAGE1_BIN) $(BIN) | $(BD)
	dd if=/dev/zero of=$@ bs=1M count=16 status=none
	dd if=$(STAGE1_BIN) of=$@ bs=512 conv=notrunc status=none
	dd if=$(BIN) of=$@ bs=512 seek=1 conv=notrunc status=none

# Run the disk image
run: $(DISK)
	qemu-system-i386 -drive format=raw,file=$(DISK) -serial stdio -no-reboot

# Cleaning Script to delete all build files
clean:
	rm -rf $(BD)

bootloader: $(BD)
	$(MAKE) -C boot bootloader BD=../$(BD) -n --warn-undefined-variables