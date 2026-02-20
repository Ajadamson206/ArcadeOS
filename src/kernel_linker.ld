OUTPUT_FORMAT(elf32-i386)
ENTRY(_kernel_start)

SECTIONS
{
    . = 0x00200000;

    .multiboot2 ALIGN(8) : { 
        KEEP(*(.multiboot2)) 
    }
     
    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
}