/**
 * TO DO:
 *      Parse Kernel Headers
 *      Load Parsed Kernel into Memory
 *      Generate Multiboot2 Struct
 *      
 * Steps for Entering Kernel
 *      EAX: 0x36D76289
 *      EBX: Pointer to Multiboot2 Struct
 */

#include <e820sort.h>
#include <multiboot.h>
#include <kernel_parse.h>

__attribute__((noreturn))
void stage2_c_main(void) {
    void *kernel_mb2_header = find_multiboot2_header();
    if(kernel_mb2_header == NULL) {
        goto error;
    }

    u32 tags = parse_mb2_header(kernel_mb2_header);

    void **kernel_start; 
    void *tag_ptr = create_tags(tags, kernel_start);

    kernel_jump(tag_ptr, *kernel_start);

error:
    for(;;) { __asm__ volatile ("hlt"); }
}