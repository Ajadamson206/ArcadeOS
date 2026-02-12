#include <bootinfo.h>

// Store pointers to the boot information
static volatile void *tags[10] = {NULL};

void load_pointers(volatile void *mb_info) {
    u32 total_size = ((multiboot_header*)mb_info)->total_size - sizeof(multiboot_header);
    volatile generic_tag *cur_tag = mb_info + sizeof(multiboot_header);
    u32 original_size = total_size;

    if(total_size == 0)
        return;

    while( total_size != 0 && original_size >= total_size ) {
        u32 tag_num = cur_tag->type;
        if(tag_num < 10) {
            tags[tag_num] = cur_tag;
        }
    
        total_size -= cur_tag->size;
        cur_tag = ((void *)cur_tag) + cur_tag->size;
    }    
}

tag_type_1 *get_bootcml(void) {
    return (tag_type_1*)tags[1];
}

tag_type_2 *get_bootloader_name(void) {
    return (tag_type_2*)tags[2];
}

tag_type_4 *get_mem_info(void) {
    return (tag_type_4*)tags[4];
}

tag_type_5 *get_bios_boot_dev(void) {
    return (tag_type_5*)tags[5];
}

tag_type_6 *get_memory_map(void) {
    return (tag_type_6*)tags[6];
}

tag_type_7 *get_vbe_info(void) {
    return (tag_type_7*)tags[7];
}

tag_type_8 *get_framebuffer_info(void) {
    return (tag_type_8*)tags[8];
}

tag_type_9 *get_elf_symbols(void) {
    return (tag_type_9*)tags[9];
}
