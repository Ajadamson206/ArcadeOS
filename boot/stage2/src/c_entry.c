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


__attribute__((noreturn))
void stage2_c_main(void) {
    volatile unsigned short *vga = (unsigned short*)0xB8000;
    // vga[2] = 0x2F43; // 'C'

    if(check_e820_order()) {
        vga[2] = 0x1F43;
    }    

    for(;;) { __asm__ volatile ("hlt"); }


    // kernel_jump();
}