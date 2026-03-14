#include <graphics.h>
#include <stdint.h>
#include <keyboard.h>
#include <main_menu.h>

const static u32 cc_bg_color = LIGHT_GRAY;

void color_changer_entry(void) {
    fill_screen(cc_bg_color);
    
    for(;;) {
        if (kb_check_if_pressed()) {
            // handle input
        } else {
            __asm__ volatile ("hlt");
        }
    }
}