#include <bootinfo.h>
#include <memory.h>

#define KERNEL_STAGING_ADDR 0x00200000u

static inline int contains_kernel(e820_entry entry) {
    return (entry.base_address > KERNEL_STAGING_ADDR) && ((entry.base_address + entry.region_length) < KERNEL_STAGING_ADDR);
}

void init_memory(tag_type_6 *memory_info) {
    u32 num_entries = memory_info->size - sizeof(*memory_info) / memory_info->entry_size;
    e820_entry *entries = memory_info->entries;

    for (u32 i = 0; i < num_entries; i++) {
        if(entries[i].region_type == 2 && !contains_kernel(entries[i])) {
            memory_places[num_places]->mem_start = (void *)entries[i].base_address;
            memory_places[num_places]->mem_size = entries[i].region_length;
            num_places++;
        }
 
        // We can only fit this memory memory locations
        if(num_places == MAX_MEM_SPOTS) {
            break;
        }
    }
}