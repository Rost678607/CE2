#include "engine/core/runtime.h"
#include "engine/graphics/screen.h"
#include "engine/platform/time.h"
#include "engine/platform/terminal.h"

#include <stdio.h>

int i = 0;

void game_init() {
    ce2_core_set_tps(1);
}

void game_loop() {
    uint64_t time = ce2_platform_time_get_ns();
    screen_size_t screen_size = ce2_screen_size_get();

    ce2_screen_fill(':');
    for (int j = 0; j < i; j++) {
        ce2_screen_set_pixel(3 + j, 5, '#');
    }

    if (i > 10) ce2_core_exit();
    i++;
}