#pragma once

#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    void *mem_start;
    u32 mem_size;
} memory_spot;

typedef struct {
    u8 type;
    u8 header_size;
    u16 chunk_size;
    u32 max_chunks;
    u32 free_idx;
    u32 malloc_idx;
    u32 queue_size;
    void *mem_start;
    void *queue_start[];
} __attribute__ ((packed)) block_start;

#define MAX_MEM_SPOTS 20

// Max size in bytes a chunk can be
#define MAX_CHUNK_SIZE 512

// Store different Type 2 Memory locations
memory_spot memory_places[MAX_MEM_SPOTS];
u8 num_places = 0;

extern void init_memory(tag_type_6 *memory_info);