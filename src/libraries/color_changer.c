#include <graphics.h>
#include <stdint.h>
#include <keyboard.h>
#include <main_menu.h>
#include <color_changer.h>
#include <serial.h>

const static u32 cc_bg_color = LIGHT_GRAY;

static const u8 cc_num_entries = 3;
static i8 cc_current_option = 0;
const char* cc_options[] = {"Background", "Text", "Selected Text"};

void color_change_selections(void) {
    fill_screen(cc_bg_color);
    
    draw_text_centered("Select what to change", BLACK, 5, 40);
    draw_text_centered("Press ENTER to select or ESC to cancel", BLACK, 5, 75);
}

void color_change_dynamic_selections(void) {
    for(u8 i = 0; i < cc_num_entries; i++) {
        draw_text_centered(cc_options[i], (i == cc_current_option)? YELLOW : BLACK, 4, 200 + (i * 30));
    }
}

void color_changer_entry(void) {
    cc_current_option = 0;

cc_restart:
    
    color_change_selections();
    color_change_dynamic_selections();

    for(;;) {
        if (kb_check_if_pressed()) {
            // handle input
            u16 pressed_key = kb_get_next_pressed_key();

            switch (pressed_key)
            {
                case KEY_ARROW_UP_PRESSED:
                    cc_current_option--;
                    if(cc_current_option < 0) {
                        cc_current_option = (i8)(cc_num_entries - 1);
                    }

                    break;

                case KEY_ARROW_DOWN_PRESSED:
                    cc_current_option = (cc_current_option + 1) % cc_num_entries;                    
                    break;

                case KEY_ENTER_PRESSED:
                    serial_print(COM1, "Selected: ");
                    serial_print(COM1, cc_options[cc_current_option]);
                    color_changer_change_color();
                    goto cc_restart;
                    
                case KEY_ESC_PRESSED:
                    return;

                default:
                    continue;
            }

            color_change_dynamic_selections();
        } else {
            __asm__ volatile ("hlt");
        }
    }
}

// R = 0, G = 1, B = 2
static u8 rgb[3] = {0};
static i8 cc_changer_option = 0;

void color_changer_draw_modifier_static_assets(void) {
    // Set background color
    fill_screen(cc_bg_color);

    // Text
    draw_text_centered("Changing the color of", BLACK, 5, 40);
    draw_text_centered(cc_options[cc_current_option], RED, 5, 75);
    draw_text_centered("Press Plus or Minus to change color by 10 points", BLACK, 3, 355);
    draw_text_centered("Press Up or Down arrow to change by 1 point", BLACK, 3, 390);
    draw_text_centered("Press Left or Right to change selection", BLACK, 3, 425);
    draw_text_centered("Press Enter or ESC to accept or decline changes", BLACK, 3, 460);

    // Get the current color
    u32 current_color = 0;

    // Square with current color + black border
    if(cc_current_option == 0)
        current_color = mm_get_background_color();
    else if(cc_current_option == 1)
        current_color = mm_get_text_color();
    else
        current_color = mm_get_selected_text_color();

    rgb[0] = (u8)((current_color & 0x00FFFFFF) >> 16); // R
    rgb[1] = (u8)((current_color & 0x0000FFFF) >> 8); // G
    rgb[2] = (u8)(current_color & 0x000000FF); // B
}

static void conv_channel(char* buffer, u8 channel) {
    buffer[2] = channel % 10 + '0';
    channel /= 10;
    buffer[1] = channel % 10 + '0';
    channel /= 10;
    buffer[0] = channel % 10 + '0';
}

void color_changer_draw_modifier_dynam_assets(void) {
    // Draw centered rectangle
    draw_rectangle_filled(245, 145, 395, 295, create_color(rgb[0], rgb[1], rgb[2], 0));

    // Draw border
    draw_rectangle(245, 145, 395, 295, 2, BLACK);

    // Clear the text
    draw_rectangle_filled(128, 320, 520, 340, LIGHT_GRAY);


    // RGB
    char buffer[4] = {0, 0, 0, 0};
    conv_channel(buffer, rgb[0]);
    draw_text("HHHHH", LIGHT_GRAY, 6, 128, 320);
    draw_text("IIIII", LIGHT_GRAY, 6, 128, 320);
    draw_text(buffer, (cc_changer_option == 0)? YELLOW : RED, 6, 128, 320);

    conv_channel(buffer, rgb[1]);
    draw_text("HHHHH", LIGHT_GRAY, 6, 256, 320);
    draw_text("IIIII", LIGHT_GRAY, 6, 256, 320);
    draw_text(buffer, (cc_changer_option == 1)? YELLOW : GREEN, 6, 256, 320);
    
    conv_channel(buffer, rgb[2]);
    draw_text("HHHHH", LIGHT_GRAY, 6, 384, 320);
    draw_text("IIIII", LIGHT_GRAY, 6, 384, 320);
    draw_text(buffer, (cc_changer_option == 2)? YELLOW : BLUE, 6, 384, 320);
}

void color_changer_change_color(void) {
    if(cc_current_option < 0 || cc_current_option >= cc_num_entries)
        return;

    // Draw the text + assets
    color_changer_draw_modifier_static_assets();

    // Sections for RGB
    color_changer_draw_modifier_dynam_assets();

    // Control logic
    for(;;) {
        if (kb_check_if_pressed()) {
            // handle input
            u16 pressed_key = kb_get_next_pressed_key();

            switch (pressed_key)
            {
                case KEY_R_PRESSED:
                    u32 rand_color = random_color(); 
                    rgb[0] = (u8)((rand_color & 0x00FFFFFF) >> 16); // R
                    rgb[1] = (u8)((rand_color & 0x0000FFFF) >> 8); // G
                    rgb[2] = (u8)(rand_color & 0x000000FF); // B
                    break;

                case KEY_ARROW_RIGHT_PRESSED:
                    cc_changer_option = (cc_changer_option + 1) % 3;
                    break;

                case KEY_ARROW_LEFT_PRESSED:
                    cc_changer_option--;
                    if(cc_changer_option < 0)
                        cc_changer_option = 2;
                    break;

                case KEY_ARROW_UP_PRESSED:
                    rgb[cc_changer_option]++;
                    break;

                case KEY_ARROW_DOWN_PRESSED:
                    rgb[cc_changer_option]--;
                    break;

                case KEY_EQUALS_PRESSED:
                    rgb[cc_changer_option] += 10;
                    break;

                case KEY_MINUS_PRESSED:
                    rgb[cc_changer_option] -= 10;
                    break;
                
                case KEY_ENTER_PRESSED:
                    u32 new_color = create_color(rgb[0], rgb[1], rgb[2], 0);
                    if(cc_current_option == 0)
                        mm_change_background_color(new_color);
                    else if(cc_current_option == 1)
                        mm_change_text_color(new_color);
                    else
                        mm_change_selected_text_color(new_color);    
                case KEY_ESC_PRESSED:
                    return;

                default:
                    continue;
            }

            color_changer_draw_modifier_dynam_assets();
        } else {
            __asm__ volatile ("hlt");
        }
    }
}
