#include <stdint.h>
#include <memcopy.h>
#include <e820sort.h>
#include <multiboot.h>

#define TAG_START   u32 type; \
                    u32 size

struct __multiboot_header {
    u32 total_size;
    u32 reserved;   
} __attribute__ ((packed));


typedef struct __multiboot_header multiboot_header;

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

void *tag_memory_info(void *struct_pos) {
    tag_type_4* t4 = struct_pos;
    t4->type = 4;
    t4->size = 16;
    


    return struct_pos + sizeof(*t4);
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

    return struct_pos + sizeof(*t5);
}

// WIP: Fix sort_memory_map
void *tag_memory_map(void *struct_pos) {
    tag_type_6* t6 = struct_pos;
    t6->type = 6;
    t6->entry_size = 24;
    t6->entry_version = 0;

    u32 num_entries = sort_memory_map(t6->entries);

    return struct_pos + (sizeof(e820_entry) * num_entries) + 16;
}

// WIP
void *tag_vbe(void *struct_pos) {
    tag_type_7* t7 = struct_pos;
    t7->type = 7;
    t7->size = 784;

    return struct_pos + sizeof(*t7);
}

// WIP
void *tag_framebuffer(void *struct_pos) {
    tag_type_8* t8 = struct_pos;
    t8->type = 8;
    t8->size = 784;

    return struct_pos + sizeof(*t8);
}

// WIP: Parse ELF Header
void *tag_kernel_elf(void *struct_pos) {
    tag_type_9* t9 = struct_pos;
    t9->type = 9;

    return struct_pos + sizeof(*t9);
}



// Store the multiboot struct where the stage1 bootloader was
volatile void* multiboot_structure = (void*)0x00007C00;



void *create_tags(u32 flags) {
    multiboot_header* start = multiboot_structure;
    start->reserved = 0;
    multiboot_structure += sizeof(*start); // Increment the multiboot struct

    // Tag 1
    if(flags & 1) {
        multiboot_structure = tag_boot_cmdl(multiboot_structure);
    }

    // Tag 2
    if(flags & 1<<1) {
        multiboot_structure = tag_bootloader_name(multiboot_structure);
    }

    // Tag 4
    if(flags & 1<<3) {
        multiboot_structure = tag_memory_info(multiboot_structure);
    }

    // Tag 5
    if(flags & 1<<4) {
        multiboot_structure = tag_boot_info(multiboot_structure);
    }

    // Tag 6
    if(flags & 1<<5) {
        multiboot_structure = tag_memory_map(multiboot_structure);
    }

    // Tag 7
    if(flags & 1<<6) {
        multiboot_structure = tag_vbe(multiboot_structure);
    }

    // Tag 8
    if(flags & 1<<7) {
        multiboot_structure = tag_framebuffer(multiboot_structure);
    }

    // Tag 9
    if(flags & 1<<8) {
        multiboot_structure = tag_kernel_elf(multiboot_structure);
    }

    // Calculate Size by finding the length of memory used
    start->total_size = multiboot_structure - (void *)start; 

    return start;
}