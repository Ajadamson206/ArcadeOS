#pragma once

#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    void *mem_start;
    u32 mem_size;
} memory_spot;

typedef struct {
    u8 type; // Type of Memory Block
    u8 header_size; // Size of this header
    u16 chunk_size; // MAX_CHUNK_SIZE
    u32 max_chunks; // Number of MAX_CHUNK_SIZE entries
    u32 free_idx; // Index to queue_start where freeing starts
    u32 malloc_idx; // Index to queue_start where mallocing starts
    void *mem_start;
    void *queue_start[];
} block_start;

#define MAX_MEM_SPOTS 20

// Max size in bytes a chunk can be
#define MAX_CHUNK_SIZE 512

// Approximate Kernel Size in Bytes
#define KERNEL_SIZE 52532 * 2

extern void init_memory(tag_type_6 *memory_info);

extern void init_mem_block(memory_spot block);

extern void memory_free(void *memory_chunk);

extern void *memory_alloc(u32 size);
