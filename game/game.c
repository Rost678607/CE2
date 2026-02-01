#include "engine/core/runtime.h"
#include "engine/graphics/screen.h"
#include "engine/platform/time.h"

#include <stdio.h>

int i = 0;

void game_init() {
    ce2_core_set_tps(1);
    printf("Hello, CE2!\n\n\r");
}

void game_loop() {
    uint64_t time = ce2_platform_time_get_ns();
    screen_size_t screen_size = ce2_graphics_get_screen_size();

    printf("%lu, %hux%hu\n\r", (unsigned long)time, screen_size.width, screen_size.height);
    i++;
    if (i > 10) ce2_core_exit();
}