#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>
#include <serial.h>
#include <unistd.h>
#include <color_changer.h>
#include <stdlib.h>
#include <maze_game_menu.h>
#include <snake.h>
#include <tv-screen.h>
#include <pong.h>
#include <interrupts.h>
#include <instruction_screen.h>

static u32 default_color = LIGHT_GRAY;
static u32 selected_color = YELLOW;
static u32 background_color = BLACK;

static volatile i8 current_option = 0;
static const i8 num_options = 6;

static const u32 menu_start = 180;

static u64 timer_ticks = 0;
static u64 timer_end = 0;

#define IDLE_WAIT_TIME_SEC 10

// static void draw_test(void) {
//     fill_screen(background_color);
// 
//     draw_circle(320, 240, 25, default_color);
// 
//     for(;;) {
//         __asm__ volatile("hlt");
//     }
// } 

static const char* menu_options[] = {"maze game", "color changer", "doom", "snake", "idle", "pong"};
static void (*menu_entry[])(void) = {maze_game_menu, color_changer_entry, instr_test, snake_main, idle_screen_main, pong_main};

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

    timer_ticks = get_pit_ticks();
    timer_end = timer_ticks + ((u64)IDLE_WAIT_TIME_SEC * PIT_FREQ);
    
    // Wait for input (Up + Down Arrow)
    while(1) {
        // Go to an idle screen if idle for too long
        if(get_pit_ticks() > timer_end) {
            idle_screen_main();
            goto restart;
        }

        if(kb_check_if_pressed()) {
            // Reset Timer
            timer_ticks = get_pit_ticks();
            timer_end = timer_ticks + ((u64)IDLE_WAIT_TIME_SEC * PIT_FREQ);

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
                    // serial_print(COM1, "Selected Game: ");
                    // serial_print(COM1, menu_options[current_option]);
                    menu_entry[current_option]();
                    kb_clear_press_buff();
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