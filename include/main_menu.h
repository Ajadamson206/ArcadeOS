#pragma once

#include <stdint.h>

extern void main_menu_entry(void);

extern void main_menu_draw_options(void);

/**
 * Change the color of the text + decals
 * @param color (u32) Created Color
 */
extern void mm_change_text_color(u32 color);

/**
 * Change the color of the selected text in the menu
 * @param color (u32) Created Color
 */
extern void mm_change_selected_text_color(u32 color);

/**
 * Change the color of the background
 * @param color (u32) Created Color
 */
extern void mm_change_background_color(u32 color);