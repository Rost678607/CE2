#include "engine/core/runtime.h"
#include "engine/platform/time.h"

#include <stdio.h>

void game_init() {
    engine_core_set_tps(100);
    printf("Hello, CE2!\n\n");
}

void game_loop() {
    uint64_t time_ns = engine_platform_time_get_ns();
    int time = time_ns / 100;
    printf("%d\n", time);
}
