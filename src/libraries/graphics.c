
/**
 * To Do:
 *  1. Grab LFB Pointer
 *  2. Create Function for drawing a pixel to it
 */

#include <graphics.h>
#include <bootinfo.h>
#include <stdint.h>
#include <stdlib.h>

static tag_type_8* frame_buffer_info;
static volatile u32* lfb_start;


void set_framebuffer(tag_type_8* tag_8) {
    frame_buffer_info = tag_8;

    lfb_start = (volatile u32*)(u32)(tag_8->framebuffer_addr);
}

void clear_screen(void) {
    volatile u32 *lfb = lfb_start;
    for(u32 height = 0; height < frame_buffer_info->framebuffer_height; height++) {
        for(u32 width = 0; width < frame_buffer_info->framebuffer_width; width++) {
            *lfb = 0;
            lfb++;   
        }
    }
}

void fill_screen(u32 color) {
    volatile u8* fb = (volatile u8*)lfb_start;

    for (u32 y = 0; y < frame_buffer_info->framebuffer_height; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = color;
        }
    }
}

u32 random_color(void) {
    return ((u32)rand()) & 0x00FFFFFF;
}

void main_menu_background_custom(u32 bg_color, u32 text_color) {
    volatile u8* fb = (volatile u8*)lfb_start;

    // First 7 Rows are black
    u32 y = 0;
    for(; y <= 7; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = bg_color;
        }
    }

    // 8th row is gray with 7 pixels of black on both sides
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = bg_color;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info->framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = text_color;
        }

        for (; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = bg_color;
        }

        y++;
    }

    // Rest of the rows are black in middle with the two offset gray pixels
    u32 end = frame_buffer_info->framebuffer_height - 8;
    for(; y <= end; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // Set x to be black
        for(u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = bg_color;
        }
    
        // Set two gray pixels
        row[7] = text_color;
        row[frame_buffer_info->framebuffer_width - 8] = LIGHT_GRAY;
    }


    // Final Gray Row
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = bg_color;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info->framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = text_color;
        }

        for (; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = bg_color;
        }

        y++;
    }

    // Final 7 Rows
    for(; y <= frame_buffer_info->framebuffer_height; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = bg_color;
        }
    }

    main_menu_text(bg_color, text_color, 5);
}

u8 text[5][91] = {
    {1,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1},
    {1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,0,1,1,0},
    {1,1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,1,0,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,1,1,1,1,0,0,1,1,0,0,1,1,1,0}
};

void main_menu_text(u32 bg_color, u32 text_color, u8 font_size) {
    // Top starts at (93, 55) and ends at (547, 55)
    // Bottom starts at (93, 79) and ends at (547, 79)
    // Font Size is 5 (Each 1 in text is a 5x5 of color)
    volatile u8* fb = (volatile u8*)lfb_start;
    
    for(u32 y = 55; y <= 79; y++) {
        // Row
        volatile u32 *row = (volatile u32 *)(fb + y * frame_buffer_info->framebuffer_pitch);
        for(u32 i = 93; i <= 547; i += font_size) {
            u32 text_index = (i / 5) - 18;
            if(text[(y / 5) - 11][text_index]) {
                volatile u32 *start = (volatile u32 *)(row + i);
                start[0] = text_color;
                start[1] = text_color;
                start[2] = text_color;
                start[3] = text_color;
                start[4] = text_color;
            }
        }
    }
}