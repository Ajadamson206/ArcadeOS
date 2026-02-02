#include <elf_parse.h>

// Kernel was written at 0x00200000
#define KERNEL_STAGE_LOC 0x00200000

Elf32_Ehdr *kernel_start = (Elf32_Ehdr *)KERNEL_STAGE_LOC;