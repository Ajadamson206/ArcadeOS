#pragma once

#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} pixel;

#define RED 0xFF000000U
#define ORANGE 0xDB6B0F00U
#define YELLOW 0xFFFF0000U
#define GREEN 0x00FF0000U
#define BLUE 0x0000FF00U
#define INDIGO 0x5D00FF00U
#define VIOLET 0xFF00E600U
#define BLACK 0x00000000U
#define WHITE 0xFFFFFF00U

/**
 * Create a u32 value for the passed in RGBA Values
 * @param red (u8) How much red is in the color (0-255)
 * @param green (u8) How much green is in the color (0-255)
 * @param blue (u8) How much blue is in the color (0-255)
 * @param alpha (u8) Color Transparency (Ignored in most cases)
 */
inline u32 create_color(u8 red, u8 green, u8 blue, u8 alpha) {
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

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