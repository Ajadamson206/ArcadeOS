#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>
#include <serial.h>
#include <misc.h>

u32 default_color = LIGHT_GRAY;
u32 selected_color = YELLOW;

static volatile i8 current_option = 0;
static const i8 num_options = 3;

static const u32 menu_start = 180;

static const char* menu_options[3] = {"maze game", "color changer", "doom"};

void main_menu_draw_options(void) {
    // Draw all text
    for(i8 i = 0; i < num_options; i++) {
        draw_text_centered(menu_options[i], (current_option == i)? selected_color : default_color, 4, menu_start + (30 * i));
    }
}


void main_menu_entry(void) {
    // Load Graphics
    main_menu_background_default();

    draw_text_centered("WELCOME TO", LIGHT_GRAY, 5, 55);
    draw_text_centered("ARCADEOS", random_color(), 5, 90);

    u32 width = get_framebuffer_info()->framebuffer_width;
    u32 length = 450;
    width = (width - length) / 2;

    draw_rectangle(width, 150, width + length, 425, 1, LIGHT_GRAY);

    // Load Options
    main_menu_draw_options();
    
    // Wait for input (Up + Down Arrow)
    while(1) {
        if(kb_check_if_pressed()) {
            // Get the key
            u16 key = kb_get_next_pressed_key();
            serial_print_hex(COM1, key);


            switch (key) {
                case KEY_ARROW_UP_PRESSED:
                    current_option--;
                    if(current_option <= -1)
                        current_option = num_options - 1;

                    main_menu_draw_options();
                    break;

                case KEY_ARROW_DOWN_PRESSED:
                    current_option = (current_option + 1) % num_options;
                    main_menu_draw_options();
                    break;
                
                case KEY_ENTER_PRESSED:
                    serial_print(COM1, "Selected Game: ");
                    serial_print(COM1, menu_options[current_option]);
                    break;
                default:
                    break;
            }
        }
    }
}

void main_menu_change_text_color(u32 color) {
    default_color = color;

    main_menu_draw_options();
}