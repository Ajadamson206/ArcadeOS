#include <stdint.h>
#include <memcopy.h>
#include <e820sort.h>
#include <multiboot.h>
#include <kernel_parse.h>

#define TAG_START   u32 type; \
                    u32 size

const char passed_cmdl[] = "";

void *tag_boot_cmdl(void* struct_pos) {
    tag_type_1* t1 = struct_pos;

    t1->type = 1;
    t1->size = 8 + sizeof(passed_cmdl);
    
    memcopy(t1->string, passed_cmdl, sizeof(passed_cmdl));

    return struct_pos + t1->size;
}

const char bootloader_name[] = "ArcadeOSBootloader";

void *tag_bootloader_name(void* struct_pos) {
    tag_type_2* t2 = struct_pos;

    t2->type = 1;
    t2->size = 8 + sizeof(bootloader_name);
    
    memcopy(t2->string, bootloader_name, sizeof(bootloader_name));

    return struct_pos + t2->size;
}

extern u16 lower_mem_size;

void *tag_memory_info(void *struct_pos) {
    tag_type_4* t4 = struct_pos;
    t4->type = 4;
    t4->size = 16;
   
    t4->mem_lower = lower_mem_size; 

    return struct_pos + t4->size;
}

// Defined in "magicNumbers.asm"
extern const u8 boot_drive;

void *tag_boot_info(void *struct_pos) {
    tag_type_5* t5 = struct_pos;
    t5->type = 5;
    t5->size = 20;
    
    t5->biosdev = boot_drive;
    t5->partition = 0;
    t5->subpartition = 0;

    return struct_pos + t5->size;
}

// ** sort_memory_map needs testing
void *tag_memory_map(void *struct_pos) {
    tag_type_6* t6 = struct_pos;
    t6->type = 6;
    t6->entry_size = 24;
    t6->entry_version = 0;

    u32 num_entries = sort_memory_map(t6->entries);

    t6->size = (sizeof(e820_entry) * num_entries) + 16;
    return struct_pos + t6->size;
}

extern vbe_info_structure vbe_info_struct;
extern vbe_mode_info_structure vbe_mode_info_struct;
extern u16 vbe_current_mode;

void *tag_vbe(void *struct_pos) {
    tag_type_7* t7 = struct_pos;
    t7->type = 7;
    t7->size = sizeof(*t7);
    
    // No protected mode table
    t7->vbe_seg = 0;
    t7->vbe_interface_off = 0;
    t7->vbe_interface_len = 0;
    
    memcopy(t7->vbe_mode_info, &vbe_mode_info_struct, sizeof(vbe_mode_info_struct));
    memcopy(t7->vbe_control_info, &vbe_info_struct, sizeof(vbe_info_struct));

    t7->vbe_mode = vbe_current_mode;

    return struct_pos + t7->size;
}

void *tag_framebuffer(void *struct_pos) {
    tag_type_8* t8 = struct_pos;
    t8->type = 8;
    t8->size = 12;

    t8->framebuffer_addr = (u64)vbe_mode_info_struct.framebuffer;
    t8->framebuffer_bpp = vbe_mode_info_struct.bpp;
    t8->framebuffer_height = vbe_mode_info_struct.height;
    t8->framebuffer_pitch = vbe_mode_info_struct.pitch;
    t8->framebuffer_width = vbe_mode_info_struct.width;
    t8->framebuffer_type = 1; // Direct RGB
    t8->reserved = 0;

    // FIX: Incorrect Syntax
    memcopy(t8->color_info, &vbe_mode_info_struct.red_mask, &vbe_mode_info_struct.blue_position - &vbe_mode_info_struct.red_mask);

    return struct_pos + t8->size;
}

// WIP: Parse ELF Header
void *tag_kernel_elf(void *struct_pos, void **e_entry) {
    tag_type_9* t9 = struct_pos;
    t9->type = 9;

    u32 num_elements = parse_elf_tag(t9, e_entry);

    return struct_pos + t9->size;
}

void *create_tags(u32 flags) {
    u8 *multiboot_structure = (u8 *)0x00100000u;
    volatile multiboot_header* start = (volatile multiboot_header*)0x00100000u;
    start->reserved = 0;
    multiboot_structure += sizeof(*start); // Increment the multiboot struct

    // Tag 1
    if(flags & 1<<1) {
        multiboot_structure = tag_boot_cmdl(multiboot_structure);
    }

    // Tag 2
    if(flags & 1<<2) {
        multiboot_structure = tag_bootloader_name(multiboot_structure);
    }

    // Tag 4
    if(flags & 1<<4) {
        multiboot_structure = tag_memory_info(multiboot_structure);
    }

    // Tag 5
    if(flags & 1<<5) {
        multiboot_structure = tag_boot_info(multiboot_structure);
    }

    // Tag 6
    if(flags & 1<<6) { // Causing Issues
        multiboot_structure = tag_memory_map(multiboot_structure);
    }

    // Tag 7
    if(flags & 1<<7) {
        multiboot_structure = tag_vbe(multiboot_structure);
    }

    // Tag 8
    if(flags & 1<<8) {
        multiboot_structure = tag_framebuffer(multiboot_structure);
    }

    // Tag 9
    if(flags & 1<<9) {
        multiboot_structure = tag_kernel_elf(multiboot_structure, (void*)0x0000A000);
    }

    // Calculate Size by finding the length of memory used
    start->total_size = (void *)multiboot_structure - (void *)start; 

    return start;
}