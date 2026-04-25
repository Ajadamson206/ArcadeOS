
/**
 * To Do:
 *  1. Grab LFB Pointer
 *  2. Create Function for drawing a pixel to it
 */

#include <graphics.h>
#include <bootinfo.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static screen_info frame_buffer_info;
static volatile u32* lfb_start;

void set_framebuffer(tag_type_8* tag_8) {
    frame_buffer_info.framebuffer_bpp = tag_8->framebuffer_bpp;
    frame_buffer_info.framebuffer_height = tag_8->framebuffer_height;
    frame_buffer_info.framebuffer_pitch = tag_8->framebuffer_pitch;
    frame_buffer_info.framebuffer_width = tag_8->framebuffer_width;

    lfb_start = (volatile u32*)(u32)(tag_8->framebuffer_addr);
}

screen_info get_screen_info(void) {
    return frame_buffer_info;
}

volatile u32 *get_lfb(void) {
    return lfb_start;
}

void clear_screen(void) {
    volatile u32 *lfb = lfb_start;
    for(u32 height = 0; height < frame_buffer_info.framebuffer_height; height++) {
        for(u32 width = 0; width < frame_buffer_info.framebuffer_width; width++) {
            *lfb = 0;
            lfb++;   
        }
    }
}

void fill_screen(u32 color) {
    volatile u8* fb = (volatile u8*)lfb_start;

    for (u32 y = 0; y < frame_buffer_info.framebuffer_height; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info.framebuffer_width; x++) {
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
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info.framebuffer_width; x++) {
            row[x] = bg_color;
        }
    }

    // 8th row is gray with 7 pixels of black on both sides
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = bg_color;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info.framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = text_color;
        }

        for (; x < frame_buffer_info.framebuffer_width; x++) {
            row[x] = bg_color;
        }

        y++;
    }

    // Rest of the rows are black in middle with the two offset gray pixels
    u32 end = frame_buffer_info.framebuffer_height - 8;
    for(; y <= end; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        // Set x to be black
        for(u32 x = 0; x < frame_buffer_info.framebuffer_width; x++) {
            row[x] = bg_color;
        }
    
        // Set two gray pixels
        row[7] = text_color;
        row[frame_buffer_info.framebuffer_width - 8] = text_color;
    }


    // Final Gray Row
    {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        // 1st Seven Black Pixels
        u32 x = 0;   
        for (; x <= 7; x++) {
            row[x] = bg_color;
        }

        // Fill in with Gray Until last 7
        u32 end = frame_buffer_info.framebuffer_width - 8;
        for (; x <= end; x++) {
            row[x] = text_color;
        }

        for (; x < frame_buffer_info.framebuffer_width; x++) {
            row[x] = bg_color;
        }

        y++;
    }

    // Final 7 Rows
    for(; y <= frame_buffer_info.framebuffer_height; y++) {
        volatile u32* row = (volatile u32*)(fb + y * frame_buffer_info.framebuffer_pitch);
        for (u32 x = 0; x < frame_buffer_info.framebuffer_width; x++) {
            row[x] = bg_color;
        }
    }

}

const u8 font[5][185] = {
    {0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0},
    {1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0},
    {1,1,1,1,0,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0},
    {1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0},
    {1,0,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,1,1,0,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0}
};

// const u8 alpha_font_len[26] = {4,4,4 ,4 ,4 ,4 ,4 ,4 ,3 ,4 ,4 ,4 ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,4  ,5  ,5  ,4  ,5  ,4};
// const u8 alpha_font_lenr[26]= {0,5,10,15,20,25,30,35,40,44,49,54,59,65,70,75,80,85,90,95,101,106,112,118,123,129};

// Merged version of alpha_font_len and alpha_font_lenr
const u16 alpha_font_info[26] = {(u16)0x0400U,(u16)0x0405U,(u16)0x040AU,(u16)0x040FU,(u16)0x0414U,(u16)0x0419U,(u16)0x041EU,(u16)0x0423U,(u16)0x0328U,(u16)0x042CU,(u16)0x0431U,(u16)0x0436U,(u16)0x053BU,(u16)0x0441U,(u16)0x0446U,(u16)0x044BU,(u16)0x0450U,(u16)0x0455U,(u16)0x045AU,(u16)0x055FU,(u16)0x0465U,(u16)0x056AU,(u16)0x0570U,(u16)0x0476U,(u16)0x057BU,(u16)0x0481U};

// const u8 num_font_len[] = {4  ,3  ,  4,  4,  4,  4,  4,  4,  4,4};
// const u8 num_font_lenr[] ={134,139,143,148,153,158,163,168,173,178};

// Merged version of num_font_len and num_font_lenr
const u16 num_font_info[10] = {(u16)0x0486U,(u16)0x038BU,(u16)0x048FU,(u16)0x0494U,(u16)0x0499U,(u16)0x049EU,(u16)0x04A3U,(u16)0x04A8U,(u16)0x04ADU,(u16)0x04B2U};

u16 get_font_info(char c) {
    if(c == ' '){
        return (1<<8) | ((u8)182u); // Space char is 2 blocks long and at position 181
    }

    if(c >= '0' && c <= '9') {
        return num_font_info[c - '0'];
    }

    u8 mod = c % 32;
    if(mod >= 1 && mod <= 26) {
        return alpha_font_info[mod - 1];
    }

    return 0;
}

void draw_text(const char *text, u32 text_color, u8 font_size, u32 x, u32 y){
    if(font_size == 0)
        return;
    
    volatile u8* fb = (volatile u8*)lfb_start;
    
    u32 row_end = ((font_size * 5) + y) < frame_buffer_info.framebuffer_height? (font_size * 5) + y : frame_buffer_info.framebuffer_height - 1;
    u32 num_pixels = 0;

    for(u32 row = y; row < row_end; row += font_size) {
        // Row
        volatile u32 *lfb_row = (volatile u32 *)(fb + row * frame_buffer_info.framebuffer_pitch);
        u32 col = x;
        
        // Print the text
        const char *text2 = text;
        while(*text2) {
            u16 font_info = get_font_info(*text2);
            if(font_info == 0) {
                text2++;
                continue;
            }

            u8 font_loc = (u8)(font_info & 0x00FF);
            u8 font_len = (u8)(font_info>>8);

            // Check if the character will go out of bounds
            if(col + (font_len * font_size) >= frame_buffer_info.framebuffer_width) {
                break;
            }
            
            u32 font_row   = ((row - y) / font_size);
            const u8 *font_ptr = &font[font_row][font_loc];

            // Print the text row
            for(u8 i = 0; i < font_len; i++) {
                // 0 Means that we should just stick with the bg color
                if(font_ptr[i] == 0) {
                    col += font_size;
                    continue;
                }

                // Print the byte and scale it with font size
                for(u8 i2 = 0; i2 < font_size; i2++) {
                    lfb_row[col] = text_color;                
                    col++;
                }
            }

            text2++;
            
            // Don't add a space if it was the last character
            if(*text2) {
                col += font_size;
            }

        }
        
        // Copy the row 'font_size' times
        for(u8 i = 1; i < font_size; i++) {
            if(row + i >= frame_buffer_info.framebuffer_height) {
                break;
            }

            volatile u32 *lfb_row2 = (volatile u32 *)(fb + ((row + i) * frame_buffer_info.framebuffer_pitch));
            memcpy((void *)&(lfb_row2[x]), (const void *)&(lfb_row[x]), (col - x) * sizeof(u32));
        }
    }
}

u32 projected_text_length(const char *text, u8 font_size) {
    u32 length = 0;
    
    while(*text) {
        u16 font_info = get_font_info(*text);
        if(font_info == 0) {
            text++;
            continue;
        }

        length += ((font_info>>8) + 1) * font_size;
        text++;
    }

    // Between each character is a space; subtract the extra added space
    return length - ((length != 0) * font_size);
}

void draw_text_centered(const char *text, u32 text_color, u8 font_size, u32 y) {
    // Get the projected length of the text
    u32 length = projected_text_length(text, font_size);
    if(length > frame_buffer_info.framebuffer_width)
        return;

    // Find the x
    u32 x = (frame_buffer_info.framebuffer_width - length) / 2;

    draw_text(text, text_color, font_size, x, y);
}

void draw_rectangle(u32 x1, u32 y1, u32 x2, u32 y2, u8 depth, u32 color) {
    if(depth == 0)
        return;
    
    if(y1 >= frame_buffer_info.framebuffer_height)
        return;

    if(x1 >= frame_buffer_info.framebuffer_width)
        return;


    volatile u8* fb = (volatile u8*)lfb_start;

    // Draw the top line
    volatile u32* row = (volatile u32*)(fb + y1 * frame_buffer_info.framebuffer_pitch);
    for(u32 i = x1; i <= x2; i++) {
        if(i >= frame_buffer_info.framebuffer_width)
            break;

        row[i] = color;
    }
    
    u32 i = 1;
    for(; i < depth; i++) {
        if(y1 + i >= frame_buffer_info.framebuffer_height)
            break;
        
        volatile u32* row2 = (volatile u32*)(fb + (y1 + i) * frame_buffer_info.framebuffer_pitch);
        memcpy((void *)&(row2[x1]), (const void *)&(row[x1]), (x2 - x1 + 1) * sizeof(u32));
    }

    // Calculate how many normal sides have to be done
    i32 num_sides = (i32)(y2 - y1 + 1) - 2 * depth;
    if(num_sides <= 0) {
        goto bottom; // Skip the middle
    }

    // Middle Sides
    for(; i + y1 < y2 - depth; i++) {
        if(y1 + i >= frame_buffer_info.framebuffer_height)
            break;

        volatile u32* row2 = (volatile u32*)(fb + (y1 + i) * frame_buffer_info.framebuffer_pitch);
        // Draw Left Side
        for(u32 e = 0; e < depth; e++) {
            if(x1 + e > frame_buffer_info.framebuffer_width)
                break;
            row2[x1 + e] = color;            
        }

        // Draw Right Side
        for(u32 e = depth; e > 0; e--) {
            if(x2 - e > frame_buffer_info.framebuffer_width)
                break;
            row2[x2 - e] = color;
        }
    }

    // Bottom Portion
bottom:
    if(i >= frame_buffer_info.framebuffer_height)
        return;

    row = (volatile u32*)(fb + (y1 + i) * frame_buffer_info.framebuffer_pitch);
    // Draw First Part
    for(u32 i = x1; i <= x2; i++) {
        row[i] = color;
    }

    // Copy the rest
    for(; y1 + i <= y2; i++) {
        if(y1 + i >= frame_buffer_info.framebuffer_height)
            break;

        volatile u32* row2 = (volatile u32*)(fb + (y1 + i) * frame_buffer_info.framebuffer_pitch);
        memcpy((void *)&(row2[x1]), (const void *)&(row[x1]), (x2 - x1) * sizeof(u32));
    }

}

void draw_rectangle_filled(u32 x1, u32 y1, u32 x2, u32 y2, u32 color) {
    if(y1 >= frame_buffer_info.framebuffer_height)
        return;

    if(x1 >= frame_buffer_info.framebuffer_width)
        return;

    if(x2 >= frame_buffer_info.framebuffer_width)
        x2 = frame_buffer_info.framebuffer_width - 1;
    
    if(y2 >= frame_buffer_info.framebuffer_height)
        y2 = frame_buffer_info.framebuffer_height - 1;

    volatile u8* fb = (volatile u8*)lfb_start;

    // Draw the lines
    for(u32 i = y1; i <= y2; i++) {
        volatile u32* row = (volatile u32*)(fb + (x1 * 4) + i * frame_buffer_info.framebuffer_pitch);
        memset32((void *)row, (int)color, sizeof(*row) * (x2 - x1 + 1));
    }
}

void draw_horizontal_line(u32 x1, u32 y, u32 x2, u32 color) {
    if(y >= frame_buffer_info.framebuffer_height)
        return;

    if(x1 >= frame_buffer_info.framebuffer_width || x2 >= frame_buffer_info.framebuffer_width)
        return;

    if(x1 > x2) {
        x2 = x1;   
    }

    volatile u32* row = (volatile u32*)((volatile u8*)lfb_start + (x1 * 4) + y * frame_buffer_info.framebuffer_pitch);
    memset32((void *)row, (int)color, sizeof(*row) * (x2 - x1 + 1));
}

void draw_circle_filled(u32 x, u32 y, u32 radius, u32 color) {
    if(radius == 0)
        return;

    if(x < radius || x + radius >= frame_buffer_info.framebuffer_width)
        return;

    if(y < radius || y + radius >= frame_buffer_info.framebuffer_height)
        return;

    volatile u8* fb = (volatile u8*)lfb_start;
    
    // Draw the Middle Row since it will be completely filled
    volatile u32* middle_row = (volatile u32*)(fb + ((x - radius) * 4) + y * frame_buffer_info.framebuffer_pitch);
    memset32((void *)middle_row, (int)color, sizeof(*middle_row) * (2 * radius + 1));

    u32 inner_x = radius;

    // Draw the Upper and Middle at the Same Time
    for(u32 r = 1; r <= radius; r++) {
        volatile u32* top_row = (volatile u32*)(fb + ((x - radius) * 4) + (y - r) * frame_buffer_info.framebuffer_pitch);
        volatile u32* bottom_row = (volatile u32*)(fb + ((x - radius) * 4) + (y + r) * frame_buffer_info.framebuffer_pitch);
    
        // Check Which X value to start with 
        while(inner_x > 0) {
            // We can draw the pixels
            if(r * r + inner_x * inner_x <= radius * radius) {
                memset32((void *)(top_row + (radius - inner_x)), (int)color, sizeof(*top_row) * inner_x * 2 + 1);
                memset32((void *)(bottom_row + (radius - inner_x)), (int)color, sizeof(*top_row) * inner_x * 2 + 1);
                break;
            }

            inner_x--;
        }     
    }

}