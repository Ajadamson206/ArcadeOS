#include <main_menu.h>
#include <stdint.h>
#include <keyboard.h>
#include <unistd.h>
#include <graphics.h>
#include <interrupts.h>
#include <math.h>

#define TICK_SPEED 10000

// Colors

static u32 background_color;
static u32 ball_color;

// Ball Settings

#define BALL_START_X 320
#define BALL_START_Y 240

#define BALL_SCALE 40.0f

#define BALL_VELOCITY 45

static float ball_x, ball_y;    // Ball's Position
static float ball_vx, ball_vy;  // Ball's Velocity Vector

static u32 prev_x1, prev_y1;
static u32 prev_x2, prev_y2;

static float ball_size;

// Player 1

// The X should be constant
#define PLAYER1_X 4

static float player1_y;
static float player1_vy;

static u8 player1_score;

// Player 2

// 639 is is the last pixel column so subtract 4
#define PLAYER2_X 635

static float player2_y;
static float player2_vy;

static u8 player2_score;

// Both Player
#define PLAYER_START_Y 240.0f

// Misc Variables

static u64 cur_ticks;
static double delta_time;
static u8 should_quit;

static void pong_kb_hook(u16 keycode) {
    // Arrow keys move player 2
    // WASD keys move player 1

    switch(keycode) {
        case KEY_ESC_PRESSED:
            should_quit = 1;
            break;

        case KEY_W_PRESSED:
            player1_vy = 1.0f;
            break;

        case KEY_S_PRESSED:
            player1_vy = -1.0f;
            break;

        case KEY_S_RELEASED:
        case KEY_W_RELEASED:
            player1_vy = 0.0f;
            break;

        case KEY_ARROW_UP_PRESSED:
            player2_vy = 1.0f;
            break;

        case KEY_ARROW_DOWN_PRESSED:
            player2_vy = -1.0f;
            break;

        case KEY_ARROW_DOWN_RELEASED:
        case KEY_ARROW_UP_RELEASED:
            player2_vy = 0.0f;
            break;
    }
}

static void render_text(void) {
    
}

static void pong_update_loop(void) {
    while(!should_quit) {
        // Calculate delta_time
        
        // Clear the screen



        // Slow Loop a bit, so CPU doesn't max out
        usleep(TICK_SPEED);
    }
}

static void pong_main(void) {
    // Init
    cur_ticks = get_pit_ticks();
    should_quit = 0;
    kb_hook_add(pong_kb_hook);

    background_color = mm_get_background_color();
    ball_color = mm_get_text_color();

    fill_screen(background_color);

    // Ball Init

    ball_x = BALL_START_X;
    ball_y = BALL_START_Y;
    ball_vx = BALL_VELOCITY;
    ball_vy = BALL_VELOCITY;

    // Player 1 Init

    player1_y = PLAYER_START_Y;
    player1_vy = 0.0f;
    player1_score = 0;

    // Player 2 Init

    player2_y = PLAYER_START_Y;
    player2_vy = 0.0f;
    player2_score = 0;

    delta_time = 0.0;

    kb_clear_press_buff();

    // Loop    
    pong_update_loop();

    // De-Init
    kb_hook_remove();
}