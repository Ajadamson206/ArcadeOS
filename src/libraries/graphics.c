
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