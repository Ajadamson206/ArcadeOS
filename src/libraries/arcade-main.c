#include <stdint.h>
#include <bootinfo.h>
#include <graphics.h>

__attribute__((noreturn))
void kernel_main(u32 magic, void *mb_info) {
    if(magic != 0x36D76289) {
        goto error;
    }

    load_pointers(mb_info);

    set_framebuffer(get_framebuffer_info());

    clear_screen();

error:
    for(;;) { __asm__ volatile ("hlt"); }
}