#include "engine/graphics/screen.h"

#include "engine/platform/terminal.h"

#include <stdint.h>

static screen_size_t screen_size;

void ce2_graphics_update_screen_size() {
    ce2_platform_terminal_get_size(&screen_size.width, &screen_size.height);
}

screen_size_t ce2_graphics_get_screen_size() {
    return screen_size;
}