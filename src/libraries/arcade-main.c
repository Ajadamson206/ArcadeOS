#include <stdint.h>
#include <bootinfo.h>
#include <graphics.h>
#include <serial.h>
#include <rawio.h>
#include <keyboard.h>
#include <interrupts.h>

__attribute__((noreturn))
void kernel_main(u32 magic, void *mb_info) {
    if(magic != 0x36D76289) {
        goto error;
    }

    load_pointers(mb_info);

    set_framebuffer(get_framebuffer_info());

    main_menu_background();

    idt_init();

    PIC_init();

    sti();

error:
    for(;;) { __asm__ volatile ("hlt"); }
}