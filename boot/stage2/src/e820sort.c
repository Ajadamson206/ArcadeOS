/**
 * Sort the E820 Map:
 *      Combine like ranges, make the map start from ground up, 
 * 
 * Note: E820 Map is written at 0x009C00, Number of entries is at 0x009B00
 */
#include <stdint.h>
#include <memcopy.h>
#include <e820sort.h>

void test_check(void) {
    E820_START->region_length += 200;
}

int check_e820_order(void) {
    volatile e820_entry *map = E820_START;
    u16 count = (mem_map_entries) - 1;

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

static e820_entry temp;

void mem_bubble_sort(e820_entry *map_start, u16 size) {
    if(size <= 1)
        return;

    size--;
    u16 num_swaps = 0;
    do {
        num_swaps = 0;
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
    
    u16 i = 0;
    while( i < size - 1) {
        u64 left_start = map_start[i].base_address;
        u64 left_end = left_start + map_start[i].region_length;
        
        u64 right_start = map_start[i + 1].base_address;
        u64 right_end = right_start + map_start[i + 1].region_length;

        if (map_start[i].region_type == map_start[i + 1].region_type &&
            right_start <= left_end)
        {
            u64 comb_end = (left_end >= right_end) ? left_end : right_end;
            map_start[i].region_length = comb_end - left_start;

            // Shift everything after i+1 one slot left
            for (u16 j = i + 1; j < size - 1; j++) {
                map_start[j] = map_start[j + 1];
            }

            size--;

            // Do not increment i, because the newly merged interval
            // may also overlap the next one.
        } else {
            i++;
        }
    }

    return size;
}

/**
 * Write the entries to dst and return the number of entries
 */
u32 sort_memory_map(void *dst) {
    if(mem_map_entries == 0) {
        return 0;
    }
    
    // Sort the memory map
    mem_bubble_sort(E820_START, mem_map_entries); // What is causing the issue

    // Merge intervals
    u16 new_size = mem_merge_intervals(E820_START, mem_map_entries);

    // Write everything to dst
    memcopy(dst, E820_START, sizeof(*E820_START) * new_size);

    // return number of entries
    return (u32)new_size;
}