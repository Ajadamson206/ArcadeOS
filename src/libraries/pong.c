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
static u32 text_color;

// Screen Settings

#define SCREEN_Y_END 100

// Ball Settings

#define BALL_START_X 320
#define BALL_START_Y 240

#define BALL_RADIUS 20.0f

#define BALL_VELOCITY 45

static float ball_x, ball_y;    // Ball's Position
static float ball_vx, ball_vy;  // Ball's Velocity Vector

static u32 ball_prev_x1, ball_prev_y1;
static u32 ball_prev_x2, ball_prev_y2;

static float ball_size;

// Player 1

// The X should be constant
#define PLAYER1_X 4

static float player1_y;
static float player1_prev_y;
static float player1_vy;

static u8 player1_score;

// Player 2

// 639 is is the last pixel column so subtract 4
#define PLAYER2_X 635

static float player2_y;
static float player2_prev_y;
static float player2_vy;

static u8 player2_score;

// Both Player
#define PLAYER_START_Y 240.0f

// In both directions (UP + DOWN)
#define PLAYER_LENGTH 20.0f
#define PLAYER_WIDTH 4.0f
#define PLAYER_VELOCITY 100.0f
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
            player1_vy = -PLAYER_VELOCITY;
            break;

        case KEY_S_PRESSED:
            player1_vy = PLAYER_VELOCITY;
            break;

        case KEY_S_RELEASED:
        case KEY_W_RELEASED:
            player1_vy = 0.0f;
            break;

        case KEY_ARROW_UP_PRESSED:
            player2_vy = -PLAYER_VELOCITY;
            break;

        case KEY_ARROW_DOWN_PRESSED:
            player2_vy = PLAYER_VELOCITY;
            break;

        case KEY_ARROW_DOWN_RELEASED:
        case KEY_ARROW_UP_RELEASED:
            player2_vy = 0.0f;
            break;
    }
}

// Player 1 Goal = 0, Player 2 Goal = 1,
// Ignore anything else
static void on_goal_score(int player) {

}

static void render_text(void) {
    // Clear the Text First
    draw_rectangle_filled(0, 0, 639, SCREEN_Y_END - 1, background_color);

    char* text = "LEFT 000     000 RIGHT";
    draw_text_centered(text, text_color, 7, 30);
}

static void clear_objects(void) {

    // Players
    draw_rectangle_filled(0, (u32)(player1_y - PLAYER_LENGTH) - 1, (u32)PLAYER_WIDTH + 1, (u32)(player1_y + PLAYER_LENGTH) + 1, background_color);
    draw_rectangle_filled(639 - (u32)PLAYER_WIDTH - 1, (u32)(player2_y - PLAYER_LENGTH) - 1, 639, (u32)(player2_y + PLAYER_LENGTH) + 1, background_color);

    // Ball
    draw_rectangle_filled((u32)(ball_x - BALL_RADIUS), (u32)(ball_y - BALL_RADIUS), (u32)(ball_x + BALL_RADIUS), (u32)(ball_y + BALL_RADIUS), background_color);
}

static void update_player_1(void) {
    player1_y += player1_vy * delta_time;

    if(player1_y - PLAYER_LENGTH <= SCREEN_Y_END) {
        player1_y = (float)(SCREEN_Y_END) + PLAYER_LENGTH;
    } else if (player1_y + PLAYER_LENGTH >= 479.0f) {
        player1_y = 479.0f - PLAYER_LENGTH;
    }

    draw_rectangle_filled(0, (u32)(player1_y - PLAYER_LENGTH), (u32)PLAYER_WIDTH, (u32)(player1_y + PLAYER_LENGTH), ball_color);
}

static void update_player_2(void) {
    player2_y += player2_vy * delta_time;

    if(player2_y - PLAYER_LENGTH <= SCREEN_Y_END) {
        player2_y = (float)(SCREEN_Y_END) + PLAYER_LENGTH;
    } else if (player2_y + PLAYER_LENGTH >= 479.0f) {
        player2_y = 479.0f - PLAYER_LENGTH;
    }

    draw_rectangle_filled(639 - (u32)PLAYER_WIDTH, player2_y - PLAYER_LENGTH, 639, player2_y + PLAYER_LENGTH, ball_color);
}

// Find point on the rectangle closest to the circle center
static float clampf(float v, float min, float max) {
    if(v < min) return min;
    if(v > max) return max;
    return v;
}

static void update_ball(void) {
    // Update ball's position
    ball_x += ball_vx * delta_time;
    ball_y += ball_vy * delta_time;

    // Check for Collisions (Top)
    if(ball_y - BALL_RADIUS <= SCREEN_Y_END) {
        ball_vy *= -1;
    }

    // Check for Collision (Bottom)
    if(ball_y + BALL_RADIUS >= 480.0f) {
        ball_vy *= -1;
    }

    // Only Need to Check for Collisions if the Ball is near the paddle

    // Check for Potential Collisions (Left Player)


    float closest_x = clampf(ball_x, 0.0f, 0.0f + PLAYER_WIDTH);
    float closest_y = clampf(ball_y, 0.0f, 0.0f + PLAYER_WIDTH);


    // Check for Collisions (Right Player)

}

static int check_for_goals(void) {

}

static void pong_update_loop(void) {
    while(!should_quit) {
        // Calculate delta_time
        delta_time = fabsf(((float)cur_ticks - (float)get_pit_ticks()) / (float)PIT_FREQ);
        
        // Clear the screen
        //draw_rectangle_filled(0, SCREEN_Y_END + 1, 639, 479, background_color);

        // Update player1, player2
        clear_objects();
        update_player_1();
        update_player_2();
        
        // Update the ball
        update_ball();

        // Redraw the line incase it got partially destroyed
        draw_horizontal_line(0, SCREEN_Y_END, 639, ball_color);

        // Check for goals, it calls on_goal_score if there was a goal
        check_for_goals();

        cur_ticks = get_pit_ticks();

        // Slow Loop a bit, so CPU doesn't max out
        usleep(TICK_SPEED);
    }
}

void pong_main(void) {
    // Init
    cur_ticks = get_pit_ticks();
    should_quit = 0;
    kb_hook_add(pong_kb_hook);

    background_color = mm_get_background_color();
    ball_color = mm_get_text_color();
    text_color = mm_get_selected_text_color();

    fill_screen(background_color);

    draw_horizontal_line(0, SCREEN_Y_END, 639, ball_color);

    render_text();

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