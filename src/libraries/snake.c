#include <stdint.h>
#include <keyboard.h>
#include <main_menu.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <rawio.h>
#include <graphics.h>
#include <serial.h>
#include <interrupts.h>
#include <instruction_screen.h>
#include <unistd.h>

// Directions

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

// Settings
#define MAP_SIZE 17
#define MAP_SCALE 11
#define SNAKE_TICK_SPEED 100000

#define SNAKE_HEAD_START_X 3
#define SNAKE_HEAD_START_Y 8
#define SNAKE_HEAD_START_LEN 3
#define SNAKE_MAX_LEN MAP_SIZE * MAP_SIZE

// Map Objects
#define MAP_NOTHING 0
#define MAP_SNAKE 1
#define MAP_APPLE 2

// Game Over Screen
#define SNAKE_OVER_BOX_X1 200
#define SNAKE_OVER_BOX_Y1 170
#define SNAKE_OVER_BOX_X2 440
#define SNAKE_OVER_BOX_Y2 310

static u8 map[MAP_SIZE][MAP_SIZE];
static u16 snake_arr[SNAKE_MAX_LEN];
static u32 snake_length;
static screen_info screen_data;

static u8 snake_next_x;
static u8 snake_next_y;

static u8 snake_direction = 1;
static int should_quit = 0;

static volatile u32 *lfb;

static u32 high_score;
static u32 current_score;

// Colors
static u32 snake_background_color;
static u32 snake_wall_color;
static u32 snake_snake_color;
static u32 snake_apple_color;
static u32 snake_floor_color;

static inline u16 snake_encode_pos(u8 x, u8 y) {
    return ((u16)(x))<<8 | y;
}

static inline u8 snake_decode_x(u16 pos) {
    return (u8)(pos>>8);
}

static inline u8 snake_decode_y(u16 pos) {
    return (u8)(pos & 0x00FF);
}

static void snake_print_map(void) {
    // Print the border
    for(int i = 0; i < MAP_SIZE + 2; i++)
        serial_print(COM1, "#");
    serial_print(COM1, "\n");


    for(int i = 0; i < MAP_SIZE; i++) {
        serial_print(COM1, "#");
        for(int e = 0; e < MAP_SIZE; e++) {
            u8 obj = map[i][e];
            if(obj == MAP_NOTHING) {
                serial_print(COM1, " ");
            } else if(obj == MAP_APPLE) {
                serial_print(COM1, "2");
            } else if(obj == MAP_SNAKE) {
                serial_print(COM1, "1");
            }
        }
        //printf("\n");
        serial_print(COM1, "#\n");
    }
    
    for(int i = 0; i < MAP_SIZE + 2; i++)
        serial_print(COM1, "#");
    serial_print(COM1, "\n");
}

static void snake_kb_handler(u16 keycode) {
    //serial_print(COM1, "Handler Called\n");
    switch (keycode) {
        case KEY_ARROW_LEFT_PRESSED:
            if(snake_direction != MOVE_RIGHT)
                snake_direction = MOVE_LEFT; 
            break;

        case KEY_ARROW_RIGHT_PRESSED:
            if(snake_direction != MOVE_LEFT)
                snake_direction = MOVE_RIGHT;
            break;

        case KEY_ARROW_DOWN_PRESSED:
            if(snake_direction != MOVE_UP)
                snake_direction = MOVE_DOWN;
            break;

        case KEY_ARROW_UP_PRESSED:
            if(snake_direction != MOVE_DOWN)
                snake_direction = MOVE_UP;
            break;

        case KEY_ESC_PRESSED:
            should_quit = 1;
            kb_clear_press_buff();
            break;

        case KEY_R_PRESSED:
            break;

        case KEY_P_PRESSED:
            snake_print_map();
            break;

        default:
            break;
    }    
}

static u8 game_state = 0;

static void snake_game_over_menu(u16 keycode) {
    if(keycode == KEY_ESC_PRESSED)
        game_state = 1;
    else 
        game_state = 2;
}

static int snake_game_over(void) {
    game_state = 0;

    // Draw Box
    draw_rectangle_filled(SNAKE_OVER_BOX_X1, SNAKE_OVER_BOX_Y1, SNAKE_OVER_BOX_X2, SNAKE_OVER_BOX_Y2, snake_background_color);
    draw_rectangle(SNAKE_OVER_BOX_X1, SNAKE_OVER_BOX_Y1, SNAKE_OVER_BOX_X2, SNAKE_OVER_BOX_Y2, 3, snake_wall_color);

    // Draw Text
    draw_text_centered("GAME OVER", snake_snake_color, 4, SNAKE_OVER_BOX_Y1 + 20);
    draw_text_centered("Press ESC to QUIT", snake_snake_color, 2, SNAKE_OVER_BOX_Y1 + 80);
    draw_text_centered("Or any key to restart", snake_snake_color, 2, SNAKE_OVER_BOX_Y1 + 110);

    kb_clear_press_buff();

    // Wait 1 Second
    sleep(1);

    // Change Handler
    kb_hook_add(snake_game_over_menu);

    while(!game_state) {
        __asm__ volatile ("hlt");
    }

    // Reset handler
    kb_hook_add(snake_kb_handler);

    // Return true if game should be over
    return game_state == 1;
}

static void snake_render_text(void) {
    // Clear it first
    draw_rectangle_filled(0, 0, 639, 100, snake_background_color);

    // Score
    char text[] = "Score 000";
    u32 score_copy = current_score;

    text[8] += score_copy % 10;
    score_copy /= 10;

    text[7] += score_copy % 10;
    score_copy /= 10;

    text[6] += score_copy % 10;

    draw_text_centered(text, snake_snake_color, 6, 40);

    // High Score
    char text2[] = "High Score 000";
    score_copy = (high_score > current_score)? high_score : current_score;

    text2[13] += score_copy % 10;
    score_copy /= 10;

    text2[12] += score_copy % 10;
    score_copy /= 10;

    text2[11] += score_copy % 10;

    draw_text_centered(text2, snake_snake_color, 3, 80);
}

static void snake_render_map(void) {
    volatile u8* fb = (volatile u8*)lfb;

    // Calculate scaled size with border
    u32 maze_hieght = (MAP_SCALE * MAP_SIZE) + (MAP_SCALE * 2);
    if(maze_hieght >= screen_data.framebuffer_height)
        return;

    u32 maze_width = (MAP_SCALE * MAP_SIZE) + (MAP_SCALE * 2);
    if(maze_width >= screen_data.framebuffer_width)
        return;

    u32 start_x = (screen_data.framebuffer_width  - maze_width)  / 2;
    u32 start_y = (screen_data.framebuffer_height - maze_hieght) / 2;

    volatile u32 *map_start =
        (volatile u32 *)(fb + start_y * screen_data.framebuffer_pitch + start_x * sizeof(*map_start)); 
    
    //
    // Top Row (Border)
    //

    for(u32 x = 0; x < maze_width; x++) {
        map_start[x] = snake_wall_color;
    }

    // Copy border 'scale' times
    for(u8 i = 1; i < MAP_SCALE; i++) {
        volatile u32 *row =
            (volatile u32 *)((volatile u8 *)map_start + i * screen_data.framebuffer_pitch);
        memcpy((void *)row, (const void *)map_start, maze_width * sizeof(*row));
    }

    //
    // Map Body
    //

    for(u32 y = 0; y < MAP_SIZE * MAP_SCALE; y += MAP_SCALE) {
        /* Draw Row */
        
        // Row is the next row after a the 'scale' size border
        volatile u32 *row = 
            (volatile u32 *)((volatile u8 *)map_start + (MAP_SCALE + y) * screen_data.framebuffer_pitch);

        // Left border
        for(u8 i = 0; i < MAP_SCALE; i++)
            row[i] = snake_wall_color;
        
        // Copy Maze Elements
        u32 index = MAP_SCALE;
        for(u8 x = 0; x < MAP_SIZE; x++) {
            u8 option = map[y / MAP_SCALE][x];
            u32 color = snake_floor_color;

            if(option == MAP_APPLE)
                color = snake_apple_color;
            else if(option == MAP_SNAKE)
                color = snake_snake_color;
 
            for(u8 i = 0; i < MAP_SCALE; i++)
                row[index + i] = color;         
        
            index += MAP_SCALE;
        }

        // Right border
        for(u8 i = 0 ; i < MAP_SCALE; i++)
            row[index + i] = snake_wall_color;
        
        /* Copy Row 'Scale' Times */

        for(u8 i = 1; i < MAP_SCALE; i++) {
            volatile u32 *row2 = 
                (volatile u32*)((volatile u8*)row + i * screen_data.framebuffer_pitch);
            memcpy((void *)row2, (const void *)row, maze_width * sizeof(*row));
        }
    }

    //
    // Draw Bottom Border
    //
    volatile u32 *row = 
        (volatile u32*)((volatile u8*)map_start + (MAP_SCALE + MAP_SIZE * MAP_SCALE) * screen_data.framebuffer_pitch);
    for(u32 x = 0; x < maze_width; x++) {
        row[x] = snake_wall_color;
    }

    // Copy border 'scale' times
    for(u8 i = 1; i < MAP_SCALE; i++) {
        volatile u32 *row2 = 
            (volatile u32*)((volatile u8*)row + i * screen_data.framebuffer_pitch);
        memcpy((void *)row2, (const void *)row, maze_width * sizeof(*row2));
    }
}

// Check if the snake can move to the next position
// Returns 1 if it can move
// Returns 0 if it cannot move
static int snake_can_move(void) {
    u16 snake_pos = snake_arr[SNAKE_MAX_LEN - snake_length];
    u8 snake_head_x = snake_decode_x(snake_pos);
    u8 snake_head_y = snake_decode_y(snake_pos);

    // Check Horizontal OOB
    if((snake_head_x == 0 && snake_direction == MOVE_LEFT) || (snake_head_x == MAP_SIZE - 1 && snake_direction == MOVE_RIGHT))
        return 0;

    // Check Vertical OOB
    if((snake_head_y == 0 && snake_direction == MOVE_UP) || (snake_head_y == MAP_SIZE - 1 && snake_direction == MOVE_DOWN))
        return 0;

    // Check if it will move into itself
    snake_next_x = snake_head_x;
    snake_next_y = snake_head_y;
    
    if(snake_direction == MOVE_LEFT)
        snake_next_x--;
    else if(snake_direction == MOVE_RIGHT)
        snake_next_x++;
    else if(snake_direction == MOVE_UP)
        snake_next_y--;
    else if(snake_direction == MOVE_DOWN)
        snake_next_y++;

    // Rare Exception
    u16 snake_tail = snake_arr[SNAKE_MAX_LEN - 1];
    u8 snake_tail_x = snake_decode_x(snake_tail);
    u8 snake_tail_y = snake_decode_y(snake_tail);
    if(snake_next_x == snake_tail_x && snake_next_y == snake_tail_y && map[snake_next_y][snake_next_x] != MAP_APPLE)
        return 1;    
    
    return map[snake_next_y][snake_next_x] != MAP_SNAKE;
}

static int snake_check_if_row_free(u8 row_index) {
    for(u8 i = 0; i < MAP_SIZE; i++) {
        if(map[row_index][i] == MAP_NOTHING)
            return 1;
    }

    return 0;
}

static void snake_spawn_apple(void) {
    u8 avail_spots[MAP_SIZE] = {0};
    u8 index = 0;

    // Load array with available Y axis
    for(u8 i = 0; i < MAP_SIZE; i++) {
        if(snake_check_if_row_free(i)) {
            avail_spots[index++] = i;
        }
    }

    // Pick a Random Row
    if(index == 0)
        return;

    // Grab a random row, and store it inside of 'row'
    u8 *row = map[avail_spots[((u8)(rand()) % index)]];
    index = 0;

    // Load array with available columns
    for(u8 i = 0; i < MAP_SIZE; i++) {
        if(row[i] == MAP_NOTHING)
            avail_spots[index++] = i;
    }

    // Should be impossible but you never know
    if(index == 0)
        return;

    u8 col = avail_spots[((u8)(rand()) % index)];
    row[col] = MAP_APPLE;
}

// Move the snake (Assumes the snake can move already)
static void snake_move(void) {
    // Start with changing the tail
    u16 tail = snake_arr[SNAKE_MAX_LEN - 1];
    map[snake_decode_y(tail)][snake_decode_x(tail)] = MAP_NOTHING;

    // Start at the end of the array and set each point
    u32 stoping_point = SNAKE_MAX_LEN - snake_length;
    for(u32 i = SNAKE_MAX_LEN - 1; i > stoping_point; i--) {
        snake_arr[i] = snake_arr[i - 1];
    }

    // snake_next_x and snake_next_y should already be set
    snake_arr[SNAKE_MAX_LEN - snake_length] = snake_encode_pos(snake_next_x, snake_next_y);

    // Set the map as well
    map[snake_next_y][snake_next_x] = MAP_SNAKE;
}

static void snake_grow(void) {
    snake_length++;
    snake_arr[SNAKE_MAX_LEN - snake_length] = snake_encode_pos(snake_next_x, snake_next_y);
    map[snake_next_y][snake_next_x] = MAP_SNAKE;
    current_score++;

    snake_render_text();
}

// Check if the snake is going to eat the apple
static int snake_will_eat_apple(void) {
    return map[snake_next_y][snake_next_x] == MAP_APPLE;
}


static void snake_init(void) {
    // Clear the screen
    fill_screen(snake_background_color);

    snake_direction = MOVE_RIGHT;
    should_quit = 0;
    snake_length = SNAKE_HEAD_START_LEN;
    current_score = 0;

    // Clear Map
    for(u8 i = 0; i < MAP_SIZE; i++) {
        memset(map[i], 0, MAP_SIZE);
    }

    // Clear Snake Array
    memset(snake_arr, 0, SNAKE_MAX_LEN * sizeof(*snake_arr));

    // Place the head + tails on the array
    for(u8 i = SNAKE_HEAD_START_LEN; i > 0; i--) {
        snake_arr[SNAKE_MAX_LEN - i] = snake_encode_pos(SNAKE_HEAD_START_X - (SNAKE_HEAD_START_X - i), SNAKE_HEAD_START_Y);
        map[SNAKE_HEAD_START_Y][SNAKE_HEAD_START_X - (SNAKE_HEAD_START_X - i)] = MAP_SNAKE;
    }

    // Spawn the apple
    snake_spawn_apple();

    // Draw the Text
    snake_render_text();

    // Should be a decent seed
    srand(rdtsc());
}

static void snake_loop(void) {
restart:
    int snake_fail = 0;

    while(!should_quit) {
        if(!snake_can_move()) {
            high_score = (current_score > high_score)? current_score : high_score;
            snake_fail = 1;
            break;
        }

        if(snake_will_eat_apple()) {
            snake_grow();

            // Move apple
            snake_spawn_apple();
        } else {
            snake_move();
        }
        
        snake_render_map();

        usleep(SNAKE_TICK_SPEED);
    }

    if(snake_fail) {
        int result = snake_game_over();

        // True means we should quit
        if(result)
            return;

        snake_init();
        goto restart;
    }
}

const char *snake_game_name = "snake";
char *snake_instructions = "\
Welcome to Snake\n\n\
Move the snake with the arrow keys\n\n\
Collect the apples to grow the\n\
snake and increase the score\n\n\
Lose by crashing into yourself or\n\
the wall";

void snake_main(void) {
    // Load the instruction Screen
    if(instr_screen_create(snake_game_name, snake_instructions)) {
        return;
    }

    // Add Keyboard Hook
    kb_hook_add(snake_kb_handler);

    // Get the colors
    snake_background_color = mm_get_background_color();
    snake_floor_color = mm_get_text_color();
    snake_snake_color = mm_get_selected_text_color();

    // Extra Colors
    snake_wall_color = (snake_floor_color + snake_background_color) / 2;
    snake_apple_color = (snake_floor_color + snake_snake_color) / 2;

    // Get the lfb
    lfb = get_lfb();
    screen_data = get_screen_info();

    // Reset High Score
    high_score = 0;

    snake_init();

    snake_loop();

    snake_render_map();

    kb_hook_remove();
}