#pragma once

#include <stdint.h>

struct e820_entry {
    u64 base_address;
    u64 region_length;
    u32 region_type;
    u32 ext_attr;
} __attribute__ ((packed));

typedef struct e820_entry e820_entry;

struct e820_map {
    u16                 count;
    struct e820_entry   entries[];    
} __attribute__ ((packed));

typedef struct e820_map e820_map;

/**
 * @brief Check if the e820 map is sorted already
 * @return Returns 1 if it is sorted, returns 0 otherwise
 */
int check_e820_order(void);

/**
 * @brief Sort and Merge intervals for the e820 map
 * @return Returns a pointer to the new e820 map location
 */
e820_map *sort_memory_map(void);

void test_check(void);
