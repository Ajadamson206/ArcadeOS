#include <main_menu.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <rawio.h>
#include <graphics.h>
#include <keyboard.h>
#include <serial.h>

static u32 mg_background_color = 0;
static u32 mg_assets_color = 0;
static u32 mg_player_color = 0;

typedef u16 point_t;

#define MAZE_X_MAX 81
#define MAZE_Y_MAX 81

static u8 MAZE_X = 81;
static u8 MAZE_Y = 81;

static u8 player_x = 0;
static u8 player_y = 0;

static u8 maze_scale = 0;

static screen_info screen_data;
static volatile u32 *lfb;

// Points are a u16 with x as the upper 8 bits and y as the lower 8 bits
static inline point_t point_create(u8 x, u8 y) {
    return (((u16)x)<<8) | y;
}

// Extracts the upper 8 bits
static inline u8 point_get_x(point_t point) {
    return (u8)(point>>8);
}

// Separates the lower 8 bits
static inline u8 point_get_y(point_t point) {
    return (u8)(point & 0x00FF);
}

point_t remove_element(point_t *array, u8 index, u8 num) {
    point_t ret = array[index];

    for(u8 i = index; i < num - 1; i++) {
        array[i] = array[i + 1];
    }

    return ret;
}

static u8 maze[MAZE_Y_MAX][MAZE_X_MAX];

static void maze_create(u8 x, u8 y) {
    if(maze[y][x] == 1)
        return;

    // Set Point to 1
    maze[y][x] = 1;

    // Store potential paths
    point_t array[4] = {0};
    u8 num = 0;

    // Add North (Ignore already visited nodes)
    if(y >= 2 && maze[y - 2][x] != 1) {
        array[num++] = point_create(x, y - 2); // Skip 2 rows since the prev row is for the path not node
    }

    // Add East
    if(x + 2 < MAZE_X && maze[y][x + 2] != 1) {
        array[num++] = point_create(x + 2, y);
    }

    // Add West
    if(x >= 2 && maze[y][x - 2] != 1) {
        array[num++] = point_create(x - 2, y);
    }

    // Add South
    if(y + 2 < MAZE_Y && maze[y + 2][x] != 1) {
        array[num++] = point_create(x, y + 2);
    }

    // Randomly pick an element and recurse the function with it
    while(num) {
        // Remove element from the array
        u8 index = (u8)((u8)(rand()) % num);
        point_t next = remove_element(array, index, 4);
        u8 next_x = point_get_x(next);        
        u8 next_y = point_get_y(next);        

        if(maze[next_y][next_x] == 1) {
            num--;
            continue;
        }

        // Create path between the current point and next
        u8 path_x = x;
        if(next_x != x)
            path_x = (next_x + x) / 2;

        u8 path_y = y;
        if(next_y != y)
            path_y = (next_y + y) / 2;
        
        maze[path_y][path_x] = 1;

        // Recurse with that chosen point
        maze_create(next_x, next_y);
        
        // Decrease num
        num--;
    }
}

void maze_render(u8 scale) {
    if(scale == 0)
        return;

    volatile u8* fb = (volatile u8*)lfb;

    // Calculate scaled size with border
    u32 maze_hieght = (scale * MAZE_Y) + (scale * 2);
    if(maze_hieght >= screen_data.framebuffer_height)
        return;

    u32 maze_width = (scale * MAZE_X) + (scale * 2);
    if(maze_width >= screen_data.framebuffer_width)
        return;

    u32 start_x = (screen_data.framebuffer_width  - maze_width)  / 2;
    u32 start_y = (screen_data.framebuffer_height - maze_hieght) / 2;

    volatile u32 *maze_start =
        (volatile u32 *)(fb + start_y * screen_data.framebuffer_pitch + start_x * sizeof(*maze_start)); 
    
    u32 border_color = (mg_background_color + mg_assets_color) / 2;

    //
    // Top Row (Border)
    //

    // Draw border
    for(u32 x = 0; x < maze_width; x++) {
        maze_start[x] = border_color;
    }

    // Copy border 'scale' times
    for(u8 i = 1; i < scale; i++) {
        volatile u32 *row =
            (volatile u32 *)((volatile u8 *)maze_start + i * screen_data.framebuffer_pitch);
        memcopy((void *)row, (const void *)maze_start, maze_width * sizeof(*row));
    }
    
    //
    // Maze Body
    //

    for(u32 y = 0; y < MAZE_Y * scale; y += scale) {
        /* Draw Row */
        
        // Row is the next row after a the 'scale' size border
        volatile u32 *row = 
            (volatile u32 *)((volatile u8 *)maze_start + (scale + y) * screen_data.framebuffer_pitch);

        // Left border
        for(u8 i = 0; i < scale; i++)
            row[i] = border_color;
        
        // Copy Maze Elements
        u32 index = scale;
        for(u8 x = 0; x < MAZE_X; x++) {
            u32 color = (maze[y / scale][x] == 1)? mg_assets_color : mg_background_color;
            
            for(u8 i = 0; i < scale; i++)
                row[index + i] = color;         
        
            index += scale;
        }

        // Right border
        for(u8 i = 0 ; i < scale; i++)
            row[index + i] = border_color;
        
        /* Copy Row 'Scale' Times */

        for(u8 i = 1; i < scale; i++) {
            volatile u32 *row2 = 
                (volatile u32*)((volatile u8*)row + i * screen_data.framebuffer_pitch);
            memcopy((void *)row2, (const void *)row, maze_width * sizeof(*row));
        }
    }
    
    //
    // Draw Bottom Border
    //
    volatile u32 *row = 
        (volatile u32*)((volatile u8*)maze_start + (scale + MAZE_Y * scale) * screen_data.framebuffer_pitch);
    for(u32 x = 0; x < maze_width; x++) {
        row[x] = border_color;
    }

    // Copy border 'scale' times
    for(u8 i = 1; i < scale; i++) {
        volatile u32 *row2 = 
            (volatile u32*)((volatile u8*)row + i * screen_data.framebuffer_pitch);
        memcopy((void *)row2, (const void *)row, maze_width * sizeof(*row2));
    }
} 

void maze_redraw_pos(u8 x, u8 y, u8 scale) {

}

void maze_color_pos(u8 x, u8 y, u8 scale, u32 color) {
    if(scale == 0 || x >= MAZE_X || y >= MAZE_Y)
        return;

    volatile u8* fb = (volatile u8*)lfb;

    // Calculate scaled size with border
    u32 maze_hieght = (scale * MAZE_Y) + (scale * 2);
    if(maze_hieght >= screen_data.framebuffer_height)
        return;

    u32 maze_width = (scale * MAZE_X) + (scale * 2);
    if(maze_width >= screen_data.framebuffer_width)
        return;

    u32 start_x = (screen_data.framebuffer_width  - maze_width)  / 2 + scale;
    u32 start_y = (screen_data.framebuffer_height - maze_hieght) / 2 + scale;

    // Ignore Border
    for(u8 i = 0; i < scale; i++) {
        volatile u32 *pos =
            (volatile u32 *)(fb + (start_y + i + (y * scale)) * screen_data.framebuffer_pitch + (start_x + (x * scale)) * sizeof(*pos));

        for(u8 e = 0; e < scale; e++) {
            *(pos + e) = color;
        }
    }
}

void player_move_up(void) {
    if(player_y == 0)
        return;

    u8 new_y = player_y - 1;
    if(maze[new_y][player_x] == 1) {
        // Reset current player tile
        maze_color_pos(player_x, player_y, maze_scale, mg_assets_color);

        // Update Player
        player_y = new_y;

        // Redraw Player
        maze_color_pos(player_x, player_y, maze_scale, mg_player_color);
    }
}

void player_move_down(void) {
    if(player_y == MAZE_Y)
        return;

    u8 new_y = player_y + 1;
    if(maze[new_y][player_x] == 1) {
        // Reset current player tile
        maze_color_pos(player_x, player_y, maze_scale, mg_assets_color);

        // Update Player
        player_y = new_y;

        // Redraw Player
        maze_color_pos(player_x, player_y, maze_scale, mg_player_color);
    }
}

void player_move_left(void) {
    if(player_x == 0)
        return;

    u8 new_x = player_x - 1;
    if(maze[player_y][new_x] == 1) {
        // Reset current player tile
        maze_color_pos(player_x, player_y, maze_scale, mg_assets_color);

        // Update Player
        player_x = new_x;

        // Redraw Player
        maze_color_pos(player_x, player_y, maze_scale, mg_player_color);
    }
}

void player_move_right(void) {
    if(player_x == MAZE_X)
        return;

    u8 new_x = player_x + 1;
    if(maze[player_y][new_x] == 1) {
        // Reset current player tile
        maze_color_pos(player_x, player_y, maze_scale, mg_assets_color);

        // Update Player
        player_x = new_x;

        // Redraw Player
        maze_color_pos(player_x, player_y, maze_scale, mg_player_color);
    }
}

void print_maze() {
    // Print the border
    for(int i = 0; i < MAZE_X + 2; i++)
        serial_print(COM1, "#");
    serial_print(COM1, "\n");


    for(int i = 0; i < MAZE_Y; i++) {
        serial_print(COM1, "#");
        for(int e = 0; e < MAZE_X; e++) {
            if(maze[i][e] == 1) {
                serial_print(COM1, " ");
            } else {
                serial_print(COM1, "#");
            }
        }
        //printf("\n");
        serial_print(COM1, "#\n");
    }
    
    for(int i = 0; i < MAZE_X + 2; i++)
        serial_print(COM1, "#");
    serial_print(COM1, "\n");
}

int maze_game_win_screen(void) {
    fill_screen(mg_background_color);
    draw_text_centered("YOU WIN", mg_player_color, 10, 100);

    draw_text_centered("Press any key to retry", mg_player_color, 5, 200);
    draw_text_centered("Or ESC to quit", mg_player_color, 5, 250);

    while(1) {
        if(kb_check_if_pressed()) {
            // Get the key
            u16 key = kb_get_next_pressed_key();

            switch (key) {
                case KEY_ESC_PRESSED:
                    return 1;
                
                default:
                    return 0;
            }
        } else {
            // Slow down the loop a bit when nothing is happening
            __asm__ volatile ("hlt");
        }
    }
}

point_t calculate_end_loc(u8 maze_size) {
    return point_create(maze_size - 1, maze_size - 1);   
}

void maze_game_entry(u8 maze_size, u8 scale) {
    // Initialize Game
    mg_background_color = mm_get_background_color();
    mg_assets_color = mm_get_text_color();
    mg_player_color = mm_get_selected_text_color();
    screen_data = get_screen_info();
    lfb = get_lfb();

restart:
    // Game Start
    fill_screen(mg_background_color);

    // Set the Maze to Zero
    for(u8 i = 0; i < MAZE_Y; i++) {
        memset(maze[i], 0, MAZE_X);
    }

    // Seed the random
    srand(rdtsc());

    // Start Point is (0,0)
    MAZE_X = maze_size;
    MAZE_Y = maze_size;
    player_x = 0;
    player_y = 0;

    maze_scale = scale;

    point_t win = calculate_end_loc(maze_size);
    u8 win_x = point_get_x(win);
    u8 win_y = point_get_y(win);
    maze_create(player_x, player_y);

    // Render the maze (Each maze node is a 4x4)
    maze_render(scale);
    maze_color_pos(player_x, player_y, maze_scale, mg_player_color);
    maze_color_pos(win_x, win_y, maze_scale, mg_player_color);

    while(1) {
        if(kb_check_if_pressed()) {
            // Get the key
            u16 key = kb_get_next_pressed_key();

            switch (key) {
                case KEY_ARROW_UP_PRESSED:
                    player_move_up();
                    break;

                case KEY_ARROW_DOWN_PRESSED:
                    player_move_down();
                    break;

                case KEY_ARROW_RIGHT_PRESSED:
                    player_move_right();
                    break;

                case KEY_ARROW_LEFT_PRESSED:
                    player_move_left();
                    break;

                case KEY_ESC_PRESSED:
                    return;
                
                case KEY_R_PRESSED:
                    goto restart;

                default:
                    break;
            }

            if(player_x == win_x && player_y == win_y)
                if(maze_game_win_screen())
                    return;
                else
                    goto restart;
        } else {
            // Slow down the loop a bit when nothing is happening
            __asm__ volatile ("hlt");
        }
    }
}