#pragma once

#include <stdint.h>
#include <e820sort.h>

/**
 * Magic value to be stored in EAX when control is passed to Kernel
 */
#define MAGIC 0x36D76289

/**
 * Where the MB Structure will be written in memory
 */
#define MB_MEM_LOC 0x00007C00

#define TAG_START   u32 type; \
                    u32 size

struct __multiboot_header {
    u32 total_size;
    u32 reserved;   
} __attribute__ ((packed));


typedef struct __multiboot_header multiboot_header;

/**
 * Type 1 Struct:
 *      u32   type = 1;
 *      u32   size
 *      u8[n] string
 * Details: Boot Command Line -> String Contains the cmdl where String is null terminated
 */
struct __tag_type_1 {
    TAG_START;

    u8 string[];
} __attribute__ ((packed));

typedef struct __tag_type_1 tag_type_1;

extern void *tag_boot_cmdl(void* struct_pos);

/**
 * Type 2 Struct:
 *      u32     type = 2;
 *      u32     size
 *      u8[n]   string
 * Details: String is the name of the bootloader and is null terminated
 */
struct __tag_type_2 {
    TAG_START;

    u8 string[];
} __attribute__ ((packed));

typedef struct __tag_type_2 tag_type_2;

extern void *tag_bootloader_name(void* struct_pos);

/**
 * Skipping Type 3: No boot modules will be loaded
 */

/**
 * Type 4 Struct:
 *      u32     type    = 4
 *      u32     size    = 16
 *      u32     mem_lower
 *      u32     mem_upper
 * Details: Indicates the amount of lower and upper memory in KB.
 *          Lower Memory Starts at address 0
 *          Upper Memory Starts at address 1 MB
 */

struct __tag_type_4 {
    TAG_START;

    u32 mem_lower;
    u32 mem_upper;
} __attribute__ ((packed));

typedef struct __tag_type_4 tag_type_4;

extern void *tag_memory_info(void *struct_pos);

/**
 * Type 5 Struct:
 *      u32 type = 5
 *      u32 size = 20
 * 
 *      u32 biosdev
 *      u32 partition
 *      u32 subpartition
 * Details: biosdev contains the contains the BIOS drive number from INT 0x13
 *          partition + subpartition will be ignored for this project
 */
struct __tag_type_5 {
    TAG_START;

    u32 biosdev;
    u32 partition;
    u32 subpartition;
} __attribute__ ((packed));

typedef struct __tag_type_5 tag_type_5;

extern void *tag_boot_info(void *struct_pos);

/**
 * Type 6 Struct:
 *      u32 type = 6
 *      u32 size 
 *      u32 entry_size = 24
 *      u32 entry_version = 0
 * 
 *      e820_entry[n] entries
 * Details: Formatted Version of the e820 map
 */
struct __tag_type_6 {
    TAG_START;

    u32 entry_size;
    u32 entry_version;
    e820_entry entries[];
} __attribute__ ((packed));

typedef struct __tag_type_6 tag_type_6;

// WIP: Fix sort_memory_map
extern void *tag_memory_map(void *struct_pos);

/**
 * Type 7 Struct:
 *      u32     type = 7
 *      u32     size = 784
 *      u16     vbe_mode
 *      u16     vbe_seg
 *      u16     vbe_interface_off
 *      u16     vbe_interface_len
 *      u8[512] vbe_control_info
 *      u8[256] vbe_mode_info
 * Details: VESA BIOS Extensions (VBE) - Defines the interface that can be 
 *          used by software to access video boards
 */
struct __tag_type_7 {
    TAG_START;

    u16 vbe_mode;
    u16 vbe_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;
    u8  vbe_control_info[512];
    u8  vbe_mode_info[256];
} __attribute__ ((packed));

typedef struct __tag_type_7 tag_type_7;

// WIP
extern void *tag_vbe(void *struct_pos);

/**
 * Type 8 Struct:
 *      u32    type = 8          
 *      u32    size              
 *      u64    framebuffer_addr  
 *      u32    framebuffer_pitch 
 *      u32    framebuffer_width 
 *      u32    framebuffer_height
 *      u8     framebuffer_bpp   
 *      u8     framebuffer_type  
 *      u8     reserved          
 *      varies color_info
 * Details: Contains the information to specify how to modify the output display 
 */
struct __tag_type_8 {
    TAG_START;

    u64    framebuffer_addr;
    u32    framebuffer_pitch;
    u32    framebuffer_width;
    u32    framebuffer_heigh;
    u8     framebuffer_bpp;
    u8     framebuffer_type;
    u8     reserved;
    u8     color_info[];
} __attribute__ ((packed));

typedef struct __tag_type_8 tag_type_8;

// WIP
extern void *tag_framebuffer(void *struct_pos);

/**
 * Type 9 Struct:
 *      u32 type = 9
 *      u32 size
 *      u16 num
 *      u16 entsize
 *      u16 shndx
 *      u16 reserved
 *      varies section_headers
 * Details: Contains the info from the kernel's ELF Header
 */
struct __tag_type_9 {
    TAG_START;

    u16 num;
    u16 entsize;
    u16 shndx;
    u16 reserved;
    u8  section_headers[];
} __attribute__ ((packed));

typedef struct __tag_type_9 tag_type_9;

// WIP: Parse ELF Header
extern void *tag_kernel_elf(void *struct_pos);

/* No More Tags will be defined after Tag 9 */

/**
 * @brief Create all of the tags requested by the Kernel
 * @param (u32) flags: A bitmask of which tags were requested by the Kernel
 * @return (void *) Pointer to where the tags are stored in memory
 */
extern void *create_tags(u32 flags);

__attribute__((noreturn)) 
static inline void kernel_jump() {    
    __asm__ volatile(
        "mov eax, %0\n"
        "mov ebx, %1\n"
        "jmp 0x00100000"
        :  // No Output
        : "r" (MAGIC), "r" (MB_MEM_LOC)
        : "eax", "ebx", "memory"
    );

    __builtin_unreachable();
}

void *create_tags(u32 flags);

u32 parse_kernel(void);