#include <main_menu.h>
#include <stdint.h>
#include <keyboard.h>
#include <unistd.h>
#include <graphics.h>
#include <interrupts.h>
#include <math.h>

// Create a ball that bounces against the wall like the tv screen savers.

#define TICK_SPEED 10000

#define BOX_START_X 320
#define BOX_START_Y 240

#define BOX_SCALE 40.0f

#define BOX_VELOCITY 45

static u32 background_color;
static u32 ball_color;

static u8 should_quit;

static float box_x, box_y; // Box's Position
static float box_vx, box_vy; // Box's Velocity Vector

static u32 prev_x1, prev_y1;
static u32 prev_x2, prev_y2;

static float box_size;

static u64 cur_ticks;
static double delta_time;

static int has_been_init = 0;

static void idle_screen_kb_hook(u16 keycode) {
    // Ignore Released Keys only press keys
    if(keycode < KEY_ESC_RELEASED)
        should_quit = 1;
}

static void idle_screen_render_loop(void) {
    delta_time = fabsf(((float)cur_ticks - (float)get_pit_ticks()) / (float)PIT_FREQ);

    // Clear the screen

    // Prevent extra lines from showing
    if(prev_x1 < 2)
        prev_x1 = 0;
    else
        prev_x1 -= 2;

    if(prev_y1 < 2)
        prev_y1 = 0;
    else
        prev_y1 -= 2;

    draw_rectangle_filled(prev_x1, prev_y1, prev_x2 + 2, prev_y2 + 2, background_color);

    // Update the position of the box
    box_x += box_vx * delta_time;
    box_y += box_vy * delta_time;

    // Check for collisions
    if(box_x <= 0.0f) {
        box_x = 0.0f;
        box_vx *= -1;
        prev_x1 = 0;
    } else if (box_x + box_size >= 639.0f) {
        box_x = 639.0f - box_size;
        box_vx *= -1;
        prev_x1 = 639 - box_size;
    } else {
        prev_x1 = (u32)box_x;
    }

    if(box_y <= 0.0f) {
        box_y = 0.0f;
        box_vy *= -1;
        prev_y1 = 0;
    } else if (box_y + box_size >= 479.0f) {
        box_y = 479.0f - box_size;
        box_vy *= -1;
        prev_y1 = 479.0f - box_size;
    } else {
        prev_y1 = (u32)box_y;
    }

    prev_x2 = prev_x1 + box_size;
    prev_y2 = prev_y1 + box_size;

    // Draw the box
    draw_rectangle_filled(prev_x1, prev_y1, prev_x2, prev_y2, ball_color);
    
    // Update Delta Time
    cur_ticks = get_pit_ticks();
}

void idle_screen_main(void) {
    // Init    
    cur_ticks = get_pit_ticks();
    should_quit = 0;
    kb_hook_add(idle_screen_kb_hook);

    background_color = mm_get_background_color();
    ball_color = mm_get_text_color();

    fill_screen(background_color);

    // Do not reset everything if it has already been
    // I had some corruption with bss memory so I've been
    // doing it like this.
    if(!has_been_init) {
        box_x = BOX_START_X;
        box_y = BOX_START_Y;

        box_vx = BOX_VELOCITY;
        box_vy = BOX_VELOCITY;

        prev_x1 = 0;
        prev_y1 = 0;
        prev_x2 = 0;
        prev_y2 = 0;

        box_size = BOX_SCALE;
        has_been_init = 1;
    }    

    delta_time = 0.0;

    kb_clear_press_buff();

    // Loop
    while(!should_quit) {
        idle_screen_render_loop();
        usleep(TICK_SPEED);
    }

    // De-Init
    kb_hook_remove();
}