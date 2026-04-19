#include <graphics.h>
#include <main_menu.h>
#include <instruction_screen.h>
#include <keyboard.h>

/* Instruction Screen for the games */
/* It will be generic so the code can be applicable to all the games */

void instr_test(void) {
    const char *test_name = "Test Game";
    char *test_instr = "This is a test for the instructions\nNext Line\nNext Line Again\nThis is a very long line YIPPE";
    instr_screen_create(test_name, test_instr);
}

static int instr_screen_loop() {
    for(;;) {
        if(kb_check_if_pressed()) {
            u16 key = kb_get_next_pressed_key();

            // Quit if ESC is pressed
            return key == KEY_ESC_PRESSED;
        }
        __asm__ volatile ("hlt");
    }

    // User will never reach here
    __builtin_unreachable();
}

int instr_screen_create(const char *game_name, char *instructions) {
    // Get the colors from the mm
    char *restart = instructions;

    // Set the background
    fill_screen(mm_get_background_color());
    
    u32 title_color = mm_get_selected_text_color();
    u32 text_color = mm_get_text_color();

    // Put the title on the top.
    draw_text_centered(game_name, title_color, 5, 20);

    // Instructions will be in a paragraph form.
    // '\n' will be used as a delimiter to move to the next line
    u32 text_start_y = 80;
    while(*instructions) {
        // Iterate until a \n or a \0 is reached
        char *start = instructions;
        for(; *instructions != '\n' && *instructions; instructions++);
        
        // If end is reached
        if(*instructions == '\0') {
            draw_text((const char *)start, text_color, 4, 5, text_start_y);
            *instructions = '\0';
        } else {
            // Temp set a null-byte
            // Reset it to \n after printing the string
            *instructions = '\0';
            draw_text((const char *)start, text_color, 4, 5, text_start_y);
            *instructions = '\n';
            instructions++;
        }
        
        text_start_y += 30;
    }

    // Final Line
    draw_text_centered("Press Any key to continue", title_color, 5, 440);

    // Loop
    return instr_screen_loop();
}