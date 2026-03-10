#include <stdint.h>
#include <bootinfo.h>
#include <graphics.h>
#include <serial.h>
#include <rawio.h>
#include <keyboard.h>
#include <stdlib.h>
#include <interrupts.h>

__attribute__((noreturn))
void kernel_main(u32 magic, void *mb_info) {
    if(magic != 0x36D76289) {
        goto error;
    }

    // Load MB2 Struct
    load_pointers(mb_info);

    // Load the LFB + Info about it
    set_framebuffer(get_framebuffer_info());

    /* Initialize Interrupts */
    idt_init();

    PIC_init();

    sti();

    // Seed a random number for a randomized background each time
    srand(rdtsc());
    
    main_menu_background_default();

error:
    for(;;) { __asm__ volatile ("hlt"); }
}