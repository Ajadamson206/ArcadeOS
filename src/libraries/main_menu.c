#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>
#include <serial.h>
#include <misc.h>
#include <color_changer.h>
#include <stdlib.h>
#include <maze_game_menu.h>

static u32 default_color = LIGHT_GRAY;
static u32 selected_color = YELLOW;
static u32 background_color = BLACK;

static volatile i8 current_option = 0;
static const i8 num_options = 3;

static const u32 menu_start = 180;

static const char* menu_options[3] = {"maze game", "color changer", "doom"};
static void (*menu_entry[])(void) = {maze_game_menu, color_changer_entry, color_changer_entry};

void main_menu_draw_options(void) {
    // Draw all text
    for(i8 i = 0; i < num_options; i++) {
        draw_text_centered(menu_options[i], (current_option == i)? selected_color : default_color, 4, menu_start + (30 * i));
    }
}

void mm_change_text_color(u32 color) {
    default_color = color;
}

void mm_change_selected_text_color(u32 color) {
    selected_color = color;
}

void mm_change_background_color(u32 color) {
    background_color = color;
}

u32 mm_get_text_color(void) {
    return default_color;
}

u32 mm_get_selected_text_color(void) {
    return selected_color;
}

u32 mm_get_background_color(void) {
    return background_color;
}

void main_menu_entry(void) {
    background_color = BLACK;
    current_option = 0;

    menu_entry[0] = maze_game_menu;
    menu_entry[1] = color_changer_entry;
    menu_entry[2] = color_changer_entry;

restart:
    // Load Graphics
    main_menu_background_custom(background_color, default_color);

    draw_text_centered("WELCOME TO", default_color, 5, 55);
    draw_text_centered("ARCADEOS", default_color, 5, 90);

    u32 width = get_framebuffer_info()->framebuffer_width;
    u32 length = 450;
    width = (width - length) / 2;

    draw_rectangle(width, 150, width + length, 425, 1, default_color);

    // Load Options
    main_menu_draw_options();
    
    // Wait for input (Up + Down Arrow)
    while(1) {
        if(kb_check_if_pressed()) {
            // Get the key
            u16 key = kb_get_next_pressed_key();

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
                    menu_entry[current_option]();
                    goto restart;
                    break;

                case KEY_ESC_PRESSED:
                    abort();
                    
                default:
                    break;
            }
        } else {
            // Slow down the loop a bit when nothing is happening
            __asm__ volatile ("hlt");
        }
    }
}