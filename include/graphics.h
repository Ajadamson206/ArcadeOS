#pragma once

#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} pixel;

#define RED 0x00FF0000U
#define ORANGE 0x00DB6B0FU
#define YELLOW 0x00FFFF00U
#define GREEN 0x0000FF00U
#define BLUE 0x000000FFU
#define INDIGO 0x005D00FFU
#define VIOLET 0x00FF00E6U
#define BLACK 0x00000000U
#define WHITE 0x00FFFFFFU
#define LIGHT_GRAY 0x00B4B4B4

/**
 * Create a u32 value for the passed in RGBA Values
 * @param alpha (u8) Color Transparency (Ignored in most cases)
 * @param red (u8) How much red is in the color (0-255)
 * @param green (u8) How much green is in the color (0-255)
 * @param blue (u8) How much blue is in the color (0-255)
 */
static inline u32 create_color(u8 red, u8 green, u8 blue, u8 alpha) {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

/**
 * Extract information from the MB2 tag containing the graphics information
 * @param tag_8 (tag_type_8 *) Pointer to where tag 8 is on the MB2 struct
 */
extern void set_framebuffer(tag_type_8* tag_8);

/**
 * Clears all elements on the screen by setting everything to black.
 * 
 * Identical to fill_screen(BLACK)
 */
extern void clear_screen(void);

/**
 * Changes the color of the screen depending on the passed in value
 * 
 * @param color (u32) a 32-bit value with an 8-bit R/G/B/A value
 */
extern void fill_screen(u32 color);

/**
 * Replace the screen with the background of the main menu with custom
 * colors
 * @param bg_color (u32) Color of the background
 * @param text_color (u32) Color of the text + accents
 */
extern void main_menu_background_custom(u32 bg_color, u32 text_color);

/**
 * Replace the screen with the default colors for the background. 
 * Background: Black
 * Text/Accents: Light Gray
 */
#define main_menu_background_default() main_menu_background_custom(BLACK, LIGHT_GRAY)

/**
 * Returns a random color
 */
extern u32 random_color(void);

extern void main_menu_text(u32 bg_color, u32 text_color, u8 font_size);
