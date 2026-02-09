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

/**
 * Compares two memory map entries' base_address
 * Returns 0 if point1's base address is less than or equal to point2's
 * Returns 1 if point2's base address is greater than point1's
 */
int compare_map_entries(e820_entry *point1, e820_entry *point2) {
    return (point1->base_address > point2->base_address);
}

void mem_bubble_sort(e820_entry *map_start, u16 size) {
    if(size <= 1)
        return;

    e820_entry temp = {0};
    size--;
    u16 num_swaps = 0;
    do {
        for(u16 left = 0; left < size; left++) {
            // Swap the entries if right is bigger than left
            if(compare_map_entries(map_start + left, map_start + left + 1)) {
                num_swaps++;
                memcopy(&temp, map_start + left, sizeof(*map_start));
                memcopy(map_start + left, map_start + left + 1,sizeof(*map_start));
                memcopy(map_start + left + 1,&temp, sizeof(*map_start));
            }
        }

    } while(num_swaps);
}

u16 mem_merge_intervals(e820_entry *map_start, u16 size) {
    if(size <= 1)
        return size;
    
    size--;
    u16 i = 0;
    for(; i < size; i++) {
        // Look at end of i, compare it to beginning of i + 1
        u64 left_end = map_start[i].base_address + map_start[i].region_length;
        u64 right_start = map_start[i + 1].base_address;

        // If they overlap: Check if the types are the same
        // If types are the same as well, merge them
        if( right_start < left_end && \
            map_start[i].region_type == map_start[i + 1].region_type) 
        {
            // Merge intervals
            u64 right_end = map_start[i + 1].base_address + map_start[i + 1].region_length;
            u64 comb_end = (left_end >= right_end)? left_end : right_end;

            map_start[i].region_length = comb_end - map_start[i].base_address;

            // Don't memcopy if it is the last element
            if(i + 1 == size) {
                i--;
                size--;
                break;
            }

            // Copy remaining elements
            memcopy(map_start + i + 1, map_start + i + 2, sizeof(*map_start) * (size - i - 1));            

            // Decrement the size and i (Incase multiple entries overlap)
            i--;
            size--;
        }
    }

    return i;
}

/**
 * Write the entries to dst and return the number of entries
 */
u32 sort_memory_map(void *dst) {
    if(*COUNT == 0) {
        return 0;
    }
    
    // Sort the memory map
    mem_bubble_sort(E820_START, *COUNT);

    // Merge intervals
    u16 new_size = mem_merge_intervals(E820_START, *COUNT);

    // Write everything to dst
    memcopy(dst, E820_START, sizeof(*E820_START) * new_size);

    // return number of entries
    return (u32)new_size;
}