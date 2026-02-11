#pragma once
#include <stdint.h>

typedef struct {
    uint16_t width;
    uint16_t height;
} screen_size_t;

void ce2_graphics_init(void);
void ce2_screen_size_update(void);
screen_size_t ce2_screen_size_get(void);

void ce2_screen_set_pixel(uint16_t x, uint16_t y, char pixel);
char ce2_screen_read_pixel(uint16_t x, uint16_t y);
void ce2_screen_fill(char pixel);
void ce2_screen_buffer_clear(void);
void ce2_screen_clear(void);

void ce2_screen_render(void);