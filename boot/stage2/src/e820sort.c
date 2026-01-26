/**
 * Sort the E820 Map:
 *      Combine like ranges, make the map start from ground up, 
 * 
 * Note: E820 Map is written at 0x009C00, Number of entries is at 0x009B00
 */
#include <stdint.h>
#include <memcopy.h>

struct __e820_entry {
    u64 base_address;
    u64 region_length;
    u32 region_type;
    u32 ext_attr;
} __attribute__ ((packed));

typedef struct __e820_entry e820_entry;

struct __e820_map {
    u16         count;
    e820_entry  entries[];    
} __attribute__ ((packed));

typedef struct __e820_map e820_map;

volatile u16 *COUNT = (u16*)0x00009B00;
volatile e820_entry *E820_START = (e820_entry*)0x00009C00;

void test_check(void) {
    E820_START->region_length += 200;
}

int check_e820_order(void) {
    volatile e820_entry *map = E820_START;
    u16 count = (*COUNT) - 1;

    for(u16 start = 0; start < count; start++) {
        u64 left_end = map[start].base_address + map[start].region_length;

        if(left_end > map[start + 1].base_address) {
            return 0;
        }
    }

    return 1;
}

e820_map *sort_memory_map(void) {
    if(*COUNT == 0) {
        return NULL;
    }

    volatile e820_entry *current_map = E820_START;
    e820_map *new_map = (e820_map*)COUNT;      // Overwrite data starting at 0x00009B00 (Don't have to copy count)

    u16 entries_left = *COUNT;
    u16 current_count = 0;

    // Copy first entry to new location
    memcopy(new_map->entries, E820_START, sizeof(e820_entry));
    current_count++;
    entries_left--;

    // Compare first entries
    while(entries_left != 0) {
        // Verify that the lengths don't overlap
        e820_entry new_entry = new_map->entries[current_count - 1];
        e820_entry cur_entry = current_map[current_count];
        
        u64 memend = (new_entry.base_address + new_entry.region_length);
    
        // Merge Entries
        if(cur_entry.base_address < memend && new_entry.region_type == cur_entry.region_type) {

        }
    }    


    return new_map;
}