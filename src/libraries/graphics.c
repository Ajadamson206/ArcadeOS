
/**
 * To Do:
 *  1. Grab LFB Pointer
 *  2. Create Function for drawing a pixel to it
 */

#include <graphics.h>
#include <bootinfo.h>
#include <stdint.h>

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

void main_menu_background() {
    volatile u8* fb = (volatile u8*)lfb_start;

    // First 7 Rows are black
    u32 y = 0;
    for(; y <= 7; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = BLACK;
        }
    }

    // 8th row is gray with 7 pixels of black on both sides
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = BLACK;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info->framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = LIGHT_GRAY;
        }

        for (; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = BLACK;
        }

        y++;
    }

    // Rest of the rows are black in middle with the two offset gray pixels
    u32 end = frame_buffer_info->framebuffer_height - 8;
    for(; y <= end; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // Set x to be black
        for(u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = BLACK;
        }
    
        // Set two gray pixels
        row[7] = LIGHT_GRAY;
        row[frame_buffer_info->framebuffer_width - 8] = LIGHT_GRAY;
    }


    // Final Gray Row
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = BLACK;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info->framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = LIGHT_GRAY;
        }

        for (; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = BLACK;
        }

        y++;
    }

    // Final 7 Rows
    for(; y <= frame_buffer_info->framebuffer_height; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info->framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info->framebuffer_width; x++) {
            row[x] = BLACK;
        }
    }
}