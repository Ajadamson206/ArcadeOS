#include <kernel_parse.h>
#include <multiboot.h>
#include <memcopy.h>

// Kernel was written at 0x00200000
#define KERNEL_STAGE_LOC 0x00200000

volatile Elf32_Ehdr *kernel_start = (Elf32_Ehdr *)KERNEL_STAGE_LOC;

// Parse the kernel elf
// Write it into mb_tag
// Return the number of bytes written
u32 parse_elf_tag(tag_type_9* mb_tag) {
    mb_tag->num = kernel_start->e_shnum;
    mb_tag->entsize = kernel_start->e_shentsize;
    mb_tag->shndx = kernel_start->e_shstrndx;
    mb_tag->reserved = 0;

    // Byte-for-Byte copy of the ELF Section Header Table
    // mb_tag->section_headers
    if(kernel_start->e_shoff == 0) {
        return sizeof(*mb_tag); // Ignores the flexible array member
    }

    volatile Elf32_Shdr *section_header = (volatile Elf32_Shdr *)(KERNEL_STAGE_LOC + kernel_start->e_shoff);

    memcopy(mb_tag->section_headers, \
            section_header, \
            kernel_start->e_shentsize * kernel_start->e_shnum);
    
    return sizeof(*mb_tag) + (kernel_start->e_shentsize * kernel_start->e_shnum);
}

void* calculate_start(void) {
    return NULL;    
}

void *find_multiboot2_header(void) {
    volatile u32 *magic = (volatile u32 *)kernel_start;

    // Kernel header should start below 32768 Bytes
    volatile u32 *mb2_end = magic + (MB2_MAX_DEPTH / 4); // Check 4 Bytes at a time

    while(magic <= mb2_end) {
        if(*magic == KERNEL_MAGIC) {
            return magic;
        }

        magic++;
    }

    return NULL;
}

u32 parse_tags(volatile mb2_kernel_info_req* tag) {
    // Subtract type/flags/size, divide by 4 since each tag is a u32
    u32 num_elements = (tag->size - 8) / 4;

    u32 requested = 0;
    volatile u32* req_tags = tag->mbi_tag_types;

    for(u16 i = 0; i < num_elements; i++) {
        if(req_tags[i] >= 32)
            continue;;

        requested |= 1<<req_tags[i];
    }

    return requested;
}

u32 parse_mb2_header(volatile mb2_kernel_header *header_start) {
    if(header_start->magic != KERNEL_MAGIC) {
        return 0xFFFFFFFF; // Give up and just request all tags
    }

    u32 header_length = header_start->header_length;

    // Verify Checksum
    u32 check = header_length + header_start->architecture + \
                header_start->checksum + header_start->magic;

    if(check) {
        return 0xFFFFFFFF; // Give up and just request all tags
    }

    const void *mb2_header_end = ((void *)header_start) + header_length;
    volatile void *tags = ((void*)header_start) + sizeof(*header_start);

    while(tags < mb2_header_end) {
        volatile mb2_kernel_generic_tag *cur_tag = tags;
        
        // For now I really only care about tag 1
        if(cur_tag->type == 1) {
            return parse_tags((volatile mb2_kernel_info_req *)cur_tag);
        }
    
        tags += cur_tag->size;
    }

}