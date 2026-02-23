#pragma once

#include <stdint.h>
#include <e820sort.h>

/**
 * Magic value to be stored in EAX when control is passed to Kernel
 */
#define MAGIC 0x36D76289

/**
 * Magic value stored in the kernel to show that it is a multiboot2
 * compliant kernel
 */
#define KERNEL_MAGIC 0xE85250D6

/**
 * Where the MB2 Structure will be written in memory
 */
#define MB2_MEM_LOC 0x00100000

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

struct __vbe_info_structure {
	char signature[4];      // must be "VESA" to indicate valid VBE support
	u16 version;			// VBE version; high byte is major version, low byte is minor version
	u32 oem;			    // segment:offset pointer to OEM
	u32 capabilities;		// bitfield that describes card capabilities
	u32 video_modes;		// segment:offset pointer to list of supported video modes
	u16 video_memory;		// amount of video memory in 64KB blocks
	u16 software_rev;		// software revision
	u32 vendor;			    // segment:offset to card vendor string
	u32 product_name;		// segment:offset to card model name
	u32 product_rev;		// segment:offset pointer to product revision
	char reserved[222];		// reserved for future expansion
	char oem_data[256];		// OEM BIOSes store their strings in this area
} __attribute__ ((packed));

typedef struct __vbe_info_structure vbe_info_structure;

_Static_assert(sizeof(struct __vbe_info_structure) == 512, "vbe_info_structure is not 512 bytes");

struct __vbe_mode_info_structure {
	u16 attributes;
	u8 window_a;
	u8 window_b;			
	u16 granularity;
	u16 window_size;
	u16 segment_a;
	u16 segment_b;
	u32 win_func_ptr;
	u16 pitch;			// number of bytes per horizontal line
	u16 width;			// width in pixels
	u16 height;			// height in pixels
	u8 w_char;
	u8 y_char;
	u8 planes;
	u8 bpp;			    // bits per pixel in this mode
	u8 banks;			
	u8 memory_model;
	u8 bank_size;
	u8 image_pages;
	u8 reserved0;

	u8 red_mask;
	u8 red_position;
	u8 green_mask;
	u8 green_position;
	u8 blue_mask;
	u8 blue_position;
	u8 reserved_mask;
	u8 reserved_position;
	u8 direct_color_attribute;
	u32 framebuffer;		    // physical address of the linear frame buffer
	u32 off_screen_mem_off;
	u16 off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	u8 reserved1[206];
} __attribute__ ((packed));

typedef struct __vbe_mode_info_structure vbe_mode_info_structure;

_Static_assert(sizeof(struct __vbe_mode_info_structure) == 256, "vbe_mode_info_structure is not 256 Bytes");

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
    u32    framebuffer_height;
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
extern void *tag_kernel_elf(void *struct_pos, void **e_entry);

/* No More Tags will be defined after Tag 9 */

/**
 * @brief Create all of the tags requested by the Kernel
 * @param (u32) flags: A bitmask of which tags were requested by the Kernel
 * @param (void **) e_entry: A pointer to where the entry address will be stored
 * @return (void *) Pointer to where the tags are stored in memory
 */
extern void *create_tags(u32 flags, void **e_entry);

__attribute__((noreturn)) 
static inline void kernel_jump
(   volatile void* mb2_req_struct, 
    volatile void* kernel_start ) 
{    
    __asm__ volatile(
        "mov eax, %0\n"
        "mov ebx, %1\n"
        "mov ecx, %2\n"
        "jmp ecx\n"
        :  // No Output
        : "r" (MAGIC), "r" (mb2_req_struct), "r" (kernel_start)
        : "eax", "ebx", "ecx", "memory"
    );

    __builtin_unreachable();
}