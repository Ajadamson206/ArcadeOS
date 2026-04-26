#include <main_menu.h>
#include <stdint.h>
#include <keyboard.h>
#include <unistd.h>
#include <graphics.h>
#include <interrupts.h>
#include <math.h>
#include <instruction_screen.h>
#include <stdlib.h>

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

#define BALL_RADIUS 15.0f

#define BALL_STARTING_VELOCITY 55.0f
static float ball_velocity;
#define BALL_VELOCITY_SCALE 5.0f

static float ball_x, ball_y;    // Ball's Position
static float ball_vx, ball_vy;  // Ball's Velocity Vector

static u32 ball_prev_x1, ball_prev_y1;
static u32 ball_prev_x2, ball_prev_y2;

static float ball_size;

static float ball_vy_rolling_scale;

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

static void pong_normalize_movement(void) {
    // Calculate Magnitude
    float magnitude = hypotf(ball_vx, ball_vy);

    // Divide by magnitude
    ball_vx /= magnitude;
    ball_vy /= magnitude;

    // Scale it up by velocity
    ball_vx *= ball_velocity;
    ball_vy *= ball_velocity;
}

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

static void pong_game_over_kb_handler(u16 key) {
    if(key == KEY_ESC_PRESSED) {
        should_quit = 1;
    } else {
        should_quit = 2;
    }
}

static int win_screen(int player) {
    draw_rectangle_filled(200, 170, 440, 310, background_color);
    draw_rectangle(200, 170, 440, 310, 3, text_color);

    draw_text_centered("GAME OVER", text_color, 4, 190);
    char text[] = "Player 1 Wins";
    if(player == 1) {
        text[7] = '2';
    }

    draw_text_centered(text, text_color, 3, 220);
    draw_text_centered("Press ESC to QUIT", text_color, 2, 250);
    draw_text_centered("Or any key to restart", text_color, 2, 280);

    kb_clear_press_buff();

    sleep(1);

    kb_hook_add(pong_game_over_kb_handler);
    
    while(!should_quit) {
        __asm__ volatile("hlt");
    }

    kb_hook_add(pong_kb_hook);

    // Should Quit: 1 == Quit, 2 == Restart
    if(should_quit == 2) {
        should_quit = 0;
        player1_score = 0;
        player2_score = 0;
        return 0;
    } else {
        return 1;   
    }
}

static void render_text(void) {
    // Clear the Text First
    draw_rectangle_filled(0, 0, 639, SCREEN_Y_END - 1, background_color);

    char* text = "LEFT 00      00 RIGHT";
    u8 p1_score = player1_score;
    u8 p2_score = player2_score;

    // Player 1 Score
    text[6] += p1_score % 10;
    p1_score /= 10;
    text[5] += p1_score % 10;

    // player 2 Score
    text[14] += p2_score % 10;
    p2_score /= 10;
    text[13] += p2_score % 10;

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

static void reset_game(void) {
    fill_screen(background_color);

    draw_horizontal_line(0, SCREEN_Y_END, 639, ball_color);

    render_text();

    srand(rdtsc());

    // Ball Init

    ball_x = BALL_START_X;
    ball_y = BALL_START_Y;

    // Ball direction is a little random
    int r = rand();
    if(r & 1)
        ball_vx = -BALL_STARTING_VELOCITY;
    else
        ball_vx = BALL_STARTING_VELOCITY;

    r = rand();
    if(r & 1)
        ball_vy = -BALL_STARTING_VELOCITY;
    else
        ball_vy = BALL_STARTING_VELOCITY;

    ball_velocity = BALL_STARTING_VELOCITY;

    // Normalize the vector
    pong_normalize_movement();

    ball_vy_rolling_scale = BALL_VELOCITY_SCALE;

    // Player 1 Init

    player1_y = PLAYER_START_Y;
    player1_vy = 0.0f;

    // Player 2 Init

    player2_y = PLAYER_START_Y;
    player2_vy = 0.0f;

    delta_time = 0.0;

    if(player1_score >= 3) {
        // Win Screen Returns 0 if we are restarting
        if(!win_screen(0)) {
            reset_game();
        }
    } else if(player2_score >= 3) {
        if(!win_screen(1)) {
            reset_game();
        }
    }
}

static void check_for_goals(void) {
    // Left sided goals
    if(ball_x - BALL_RADIUS <= 0.0f) {
        player2_score++;
        reset_game();
    } else if(ball_x + BALL_RADIUS >= 640.0f) {
        player1_score++;
        reset_game();
    }
}

// Find point on the rectangle closest to the circle center
static float clampf(float v, float min, float max) {
    if(v < min) return min;
    if(v > max) return max;
    return v;
}

static void ball_left_player_collision(void) {
    // We called this function when the radius across the x axis
    // could potentially be touching

    float closest_x = clampf(ball_x, PLAYER1_X, PLAYER1_X + PLAYER_WIDTH);
    float closest_y = clampf(ball_y, player1_y - PLAYER_LENGTH, player1_y + PLAYER_LENGTH);

    float dx = ball_x - closest_x;
    float dy = ball_y - closest_y;

    // Check for a collision
    if(dx * dx + dy * dy <= BALL_RADIUS * BALL_RADIUS) {
        // Determine direction by where ball hits
        float offset = (ball_y - player1_y) / (PLAYER_LENGTH);   // range about [-1, 1]

        // Subtract b/c the ball was moving left
        ball_vx = -(ball_vx * BALL_VELOCITY_SCALE);
        ball_vy = (offset * ball_velocity);

        ball_velocity += ball_vy_rolling_scale;

        // Normalize the movement
        pong_normalize_movement();
    }
}

static void ball_right_player_collision(void) {
    float closest_x = clampf(ball_x, PLAYER2_X, PLAYER2_X + PLAYER_WIDTH);
    float closest_y = clampf(ball_y, player2_y - PLAYER_LENGTH, player2_y + PLAYER_LENGTH);

    float dx = ball_x - closest_x;
    float dy = ball_y - closest_y;

    // Check for a collision
    if(dx * dx + dy * dy <= BALL_RADIUS * BALL_RADIUS) {
        // Determine direction by where ball hits
        float offset = (ball_y - player2_y) / (PLAYER_LENGTH);   // range about [-1, 1]

        ball_vx = -(ball_vx * BALL_VELOCITY_SCALE);
        ball_vy = (offset * ball_velocity);

        ball_velocity += ball_vy_rolling_scale;
        
        // Normalize the movement
        pong_normalize_movement();
    }
}

static void update_ball(void) {
    // Update ball's position
    ball_x += ball_vx * delta_time;
    ball_y += ball_vy * delta_time;

    // Check for Collisions (Top)
    if(ball_y - BALL_RADIUS <= SCREEN_Y_END) {
        ball_vy *= -1;
        ball_y = SCREEN_Y_END + BALL_RADIUS; // Clamp Ball to Screen
    }

    // Check for Collision (Bottom)
    if(ball_y + BALL_RADIUS >= 480.0f) {
        ball_vy *= -1;
        ball_y = 480.0f - BALL_RADIUS; // Clamp Ball to Screen
    }

    // Only Need to Check for Collisions if the Ball is near the paddle
    
    // Check for Left Player Collisions
    if(ball_x - BALL_RADIUS <= PLAYER_WIDTH) {
        ball_left_player_collision();
    }

    // Check for Right Player Collisions
    if(ball_x + BALL_RADIUS >= 640 - PLAYER_WIDTH) {
        ball_right_player_collision();
    }

    // Draw the Ball
    draw_circle_filled(ball_x, ball_y, BALL_RADIUS, ball_color);
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

const char *pong_game_name = "pong";
char *pong_instructions = "\
Welcome to pong\n\n\
Pong is a two player game\n\n\
Player 1 Controls the paddle\n\
with WASD\n\
Player 2 Controls the paddle\n\
with Arrow keys\n\n\
The first player to 3 points wins";

void pong_main(void) {
    // Info Screen
    if(instr_screen_create(pong_game_name, pong_instructions)) {
        return;
    }

    // Init
    cur_ticks = get_pit_ticks();
    should_quit = 0;
    kb_hook_add(pong_kb_hook);

    background_color = mm_get_background_color();
    ball_color = mm_get_text_color();
    text_color = mm_get_selected_text_color();

    player1_score = 0;
    player2_score = 0;

    reset_game();

    kb_clear_press_buff();

    // Loop    
    pong_update_loop();

    // De-Init
    kb_hook_remove();
}