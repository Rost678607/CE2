#pragma once

#include <stdint.h>

typedef struct {
    uint16_t width;
    uint16_t height;
} screen_size_t;

void ce2_graphics_update_screen_size(void);
screen_size_t ce2_graphics_get_screen_size(void);