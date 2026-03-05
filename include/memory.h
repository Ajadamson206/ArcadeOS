#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    void *mem_start;
    u32 mem_size;
} memory_spot;

#define MAX_MEM_SPOTS 20

// Store different Type 2 Memory locations
memory_spot *memory_places[MAX_MEM_SPOTS];
u8 num_places = 0;

extern void init_memory(tag_type_6 *memory_info);