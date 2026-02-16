#pragma once

#include <stdint.h>
#include <bootinfo.h>

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} pixel;

void set_framebuffer(tag_type_8* tag_8);

void clear_screen(void);
