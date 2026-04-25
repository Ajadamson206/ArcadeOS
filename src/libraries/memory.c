#include <bootinfo.h>
#include <memory.h>

#define KERNEL_STAGING_ADDR 0x00200000u

static inline int contains_kernel(e820_entry entry) {
    return (entry.base_address > KERNEL_STAGING_ADDR) && ((entry.base_address + entry.region_length) < KERNEL_STAGING_ADDR);
}

void init_memory(tag_type_6 *memory_info) {
    num_places = 0;
    u32 num_entries = memory_info->size - sizeof(*memory_info) / memory_info->entry_size;
    e820_entry *entries = memory_info->entries;

    for (u32 i = 0; i < num_entries; i++) {
        if(entries[i].region_type == 2 && !contains_kernel(entries[i])) {
            memory_places[num_places].mem_start = (void *)entries[i].base_address;
            memory_places[num_places].mem_size = entries[i].region_length;
            num_places++;
        }
 
        // We can only fit this memory memory locations
        if(num_places == MAX_MEM_SPOTS) {
            break;
        }
    }
}

void init_mem_block(memory_spot block) {
    // Calculate the max number of chunks that can fit
    u32 max_chunks = block.mem_size / MAX_CHUNK_SIZE;

    // We will allocate a chunk by having a fixed queue at the beginning

    // Calculate the size of the queue in bytes
    u32 queue_size = max_chunks * (sizeof(void*));

    // Subtract the number of queue_size Ceiled from max chunks
    // (We will wind up not using all of the memory but it is what it is)
    max_chunks -= (queue_size / MAX_CHUNK_SIZE) + 1;
    queue_size = max_chunks * (sizeof(void*));

    // Things to keep track of in the block
    // u8 type = 1
    // u8 header size = 20
    // u16 chunk_size = MAX_CHUNK_SIZE
    // u32 max_chunks
    // u32 free_ptr
    // u32 malloc_ptr 
    // u32 queue_size
    // void *mem_start
    // void *queue_start

    // Get the ptr to the block start
    volatile block_start* block_write = block.mem_start;

    // Type + Header Size + Chunk Size
    block_write->type = 1;
    block_write->header_size = sizeof(block_start);
    block_write->chunk_size = MAX_CHUNK_SIZE;

    // Free ptr and malloc ptr point to the beginning of the queue
    block_write->free_idx = 0;
    block_write->malloc_idx = 0;

    // Where the memory chunks start (Move past the header)
    block_write->mem_start = (void *)(block.mem_start + block_write->header_size + queue_size);
    block_write->queue_size = queue_size;

    // Start filling out the queue
    void* mem_start = (void*)block_write->mem_start;
    for(u32 i = 0; i < queue_size; i++) {
        block_write->queue_start[i] = mem_start;
        mem_start = ((u8 *)mem_start) + MAX_CHUNK_SIZE;
    }
}

void memory_free(void *memory_chunk) {
    // Find what memory block the chunk is in
    u8 i = 0;
    for(; i < num_places; i++) {
        if((u32)memory_chunk > (u32)(memory_places[i].mem_start) && (u32)memory_chunk < (u32)(memory_places[i].mem_start + memory_places[i].mem_size)) {
            break;
        }
    }

    if(i == num_places)
        return;
    
    block_start *header = memory_places[i].mem_start;
    // If free_ptr is pointing to a nonzero entry
    // Ignore the free

    if(header->queue_start[header->free_idx] != NULL)
        return;

    // Write the memory chunk to the free_ptr queue and
    // Increment it
    header->queue_start[header->free_idx++] = memory_chunk;
    header->free_idx %= header->queue_size;
}

void *memory_alloc(u32 size) {
    if(size > MAX_CHUNK_SIZE)
        return NULL;

    // For each block
    for(u8 i = 0; i < num_places; i++) {
        block_start *head = memory_places[i].mem_start;

        if(head->queue_start[head->malloc_idx] == NULL) {
            continue;
        }

        // If the block has an empty chunk (malloc_ptr doesn't point to NULL)
        // Grab the ptr pointed to by malloc_ptr.
        void *ret = head->queue_start[head->malloc_idx];
        
        // Increment malloc_ptr and mod it by queue size
        head->queue_start[head->malloc_idx++] = NULL;
        head->malloc_idx %= head->queue_size;

        return ret;
    }

    // No valid memory found
    return NULL;
}
