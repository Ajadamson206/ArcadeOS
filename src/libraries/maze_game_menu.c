#include <maze_game.h>
#include <keyboard.h>
#include <graphics.h>
#include <main_menu.h>

// Menu to choose the size of the game
static u32 mg_background_color;
static u32 mg_assets_color;
static u32 mg_selected_color;

void select_game(u8 selection) {
    switch (selection) {
        // 15 x 15
        case 0:
            maze_game_entry(15, 10);
            break;
        
        // 31 x 31
        case 1:
            maze_game_entry(31, 8);
            break;

        // 55 x 55
        case 2:
            maze_game_entry(55, 6);
            break;

        // 81 x 81
        case 3:
            maze_game_entry(81, 4);
            break;

        default:
            break;
    }
}

void render_menu(u8 selection) {
    // Set background
    fill_screen(mg_background_color);

    // Static Text
    draw_text_centered("Select a difficulty", mg_assets_color, 6, 100);

    draw_rectangle(80, 200, 560, 440, 3, mg_assets_color);

    // Color the font based on the current selection
    draw_text_centered("Easy", (selection == 0)? mg_selected_color : mg_assets_color, 5, 230);
    draw_text_centered("Medium", (selection == 1)? mg_selected_color : mg_assets_color, 5, 270);
    draw_text_centered("Hard", (selection == 2)? mg_selected_color : mg_assets_color, 5, 310);
    draw_text_centered("Very Hard", (selection == 3)? mg_selected_color : mg_assets_color, 5, 350);
}

void maze_game_menu(void) {
    // Have users choose what size maze they want
    // Easy = 15 x 15
    // Medium = 31 x 31
    // Hard = 55 x 55
    // Very Hard = 81 x 81

    mg_background_color = mm_get_background_color();
    mg_assets_color = mm_get_text_color();
    mg_selected_color = mm_get_selected_text_color();

    u8 selection = 0;

    render_menu(selection);

    for(;;) {
        if(kb_check_if_pressed()) {
            // Get the key
            u16 key = kb_get_next_pressed_key();

            switch (key) {
                case KEY_ARROW_UP_PRESSED:
                    if(selection == 0)
                        selection = 3;
                    else
                        selection--;
                    break;

                case KEY_ARROW_DOWN_PRESSED:
                    selection = (selection + 1) % 4;
                    break;

                case KEY_ESC_PRESSED:
                    return;
                
                case KEY_ENTER_PRESSED:
                    select_game(selection);
                    break;

                default:
                    break;
            }

            render_menu(selection);
        } else {
            // Slow down the loop a bit when nothing is happening
            __asm__ volatile ("hlt");
        }
    }


}