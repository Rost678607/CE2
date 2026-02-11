#include "engine/graphics/screen.h"
#include "engine/platform/terminal.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static screen_size_t screen_size = {0, 0};
static char *screen_buffer = NULL;

void ce2_graphics_init() {
    ce2_platform_terminal_init();
    ce2_screen_size_update();
    ce2_screen_clear();
}

void ce2_screen_buffer_clear() {
    if (screen_buffer && screen_size.width > 0 && screen_size.height > 0) {
        memset(screen_buffer, ' ', screen_size.width * screen_size.height);
    }
}

void ce2_screen_clear() {
    ce2_platform_terminal_clear();
}

void ce2_screen_size_update() {
    uint16_t new_w, new_h;
    ce2_platform_terminal_get_size(&new_w, &new_h);

    if (new_w != screen_size.width || new_h != screen_size.height || screen_buffer == NULL) {
        screen_size.width = new_w;
        screen_size.height = new_h;

        char *new_ptr = realloc(screen_buffer, new_w * new_h * sizeof(char));
        
        if (new_ptr) {
            screen_buffer = new_ptr;
            ce2_screen_buffer_clear(); 
        } else {
            exit(1);
        }
    }
}

screen_size_t ce2_screen_size_get() {
    return screen_size;
}

void ce2_screen_set_pixel(uint16_t x, uint16_t y, char pixel) {
    if (x >= screen_size.width || y >= screen_size.height) {
        return;
    }

    size_t index = (screen_size.height - 1 - y) * screen_size.width + x;

    if (screen_buffer) {
        screen_buffer[index] = pixel;
    }
}

char ce2_screen_read_pixel(uint16_t x, uint16_t y) {
    if (x >= screen_size.width || y >= screen_size.height || !screen_buffer) {
        return ' '; 
    }

    size_t index = (screen_size.height - 1 - y) * screen_size.width + x;
    return screen_buffer[index];
}

void ce2_screen_fill(char pixel) {
    if (screen_buffer) {
        memset(screen_buffer, pixel, screen_size.width * screen_size.height);
    }
}

void ce2_screen_render() {
    if (screen_buffer) {
        ce2_platform_terminal_write(
            screen_buffer, 
            screen_size.width, 
            screen_size.height
        );
    }
}