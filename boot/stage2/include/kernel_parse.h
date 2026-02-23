#pragma once

// This Code is based off of the specifications designed
// By the Linux Foundation
// More information can be found here:
// https://refspecs.linuxfoundation.org/elf/elf.pdf

#define EI_NIDENT 16

#include <stdint.h>
#include <multiboot.h>

/* ELF Headers + Variables + Structs */

typedef u32 Elf32_Addr;
typedef u16 Elf32_Half;
typedef u32 Elf32_Off;
typedef i32 Elf32_Sword;
typedef u32 Elf32_Word;

// Elf Header
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;


// Section Header
typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

// Symbol Table Entry
typedef struct {
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
} Elf32_Sym;

// Relocation Entries
typedef struct {
    Elf32_Addr r_offset;
    Elf32_Word r_info;
} Elf32_Rel;
typedef struct {
    Elf32_Addr r_offset;
    Elf32_Word r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

// Program Header
typedef struct {
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
} Elf32_Phdr;

extern u32 parse_elf_tag(tag_type_9* mb_tag, void **e_entry);

/* Mulitboot2 Kernel Headers */

// Number of Bytes that the header should start within
#define MB2_MAX_DEPTH 32768

struct __mb2_kernel_header {
    u32 magic;
    u32 architecture;
    u32 header_length;
    u32 checksum;
    u8 tags[];
} __attribute__ ((packed));

/**
 * Start of the MB2 Header Stuct
 * 
 * magic: Must have value 0xE85250D6
 * 
 * architecture: Specifies CPU instruction set
 * 
 * header_length: Length of entire header in Bytes
 * 
 * checksum: 32-bit value when added to all other fields has sum of 0
 */
typedef struct __mb2_kernel_header mb2_kernel_header;

#define KERNEL_TAG_START u16 type; \
                         u16 flags; \
                         u32 size

struct __mb2_kernel_generic_tag {
    u16 type;
    u16 flags;
    u32 size;
} __attribute__ ((packed));

typedef struct __mb2_kernel_generic_tag mb2_kernel_generic_tag;

struct __mb2_kernel_info_req {
    KERNEL_TAG_START;

    u32 mbi_tag_types[];
} __attribute__ ((packed));

typedef struct __mb2_kernel_info_req mb2_kernel_info_req;

struct __mb2_kernel_address_tag {
    KERNEL_TAG_START;

    u32 header_addr;
    u32 load_addr;
    u32 load_end_addr;
    u32 bss_end_addr;
} __attribute__ ((packed));

/**
 * Type = 2
 * NOTE: All Addresses are physical addresses, also not needed if in ELF
 * format
 * 
 * header_addr: address corresponding to the beginning of the MB2 header
 * 
 * load_addr: address corresponding to the beginning of the .txt segment
 * 
 * load_end_addr: address corresponding to the end of the data segment
 * 
 * bss_end_addr: address corresponding to the end of the .bss segment
 */
typedef struct __mb2_kernel_address_tag mb2_kernel_address_tag;

struct __mb2_kernel_entry_addr {
    KERNEL_TAG_START;

    u32 entry_addr;
} __attribute__ ((packed));

/**
 * Works for types 3, 8, and 9 (Normal / efi i386 / efi amd64)
 * Contains the physical address to where the bootloader should jump 
 * in order to start running the operating system
 */
typedef struct __mb2_kernel_entry_addr mb2_kernel_entry_addr;

struct __mb2_kernel_framebuffer {
    KERNEL_TAG_START;

    u32 width;
    u32 height;
    u32 depth;
} __attribute__ ((packed));

/**
 * Type = 5
 * 
 * Specifies the preferred graphics mode
 * 
 * width: Number of columns
 * 
 * height: Number of lines/rows
 * 
 * depth: bits per pixel
 */
typedef struct __mb2_kernel_framebuffer mb2_kernel_framebuffer;

/**
 * Search the Kernel until the multiboot2 header is found
 * @return (void *) Memory address of where the multiboot2 header starts,
 * including the 'magic' value
 */
extern void *find_multiboot2_header(void);

/**
 * Parse information from the Kernel's MB2 Struct
 * @param header_start: Pointer to where the header starts
 * @return (u32) Bitmask of required Bootloader Structs
 */
extern u32 parse_mb2_header(volatile mb2_kernel_header *header_start);

/**
 * Return the memory address of where the kernel's entry function is at
 * @return (void *) Memory address to the kernel's entry function
 */
extern void* calculate_start(void);
