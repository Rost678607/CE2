#include <stdbool.h>
#include <stdint.h>

#include "engine/graphics/screen.h"
#include "engine/platform/terminal.h"
#include "engine/platform/time.h"

extern void game_init(void);
extern void game_loop(void);

static uint8_t tps = 60;
static bool run = true;

static void init() {
    ce2_platform_time_init();
    ce2_graphics_init();
}

void ce2_core_exit() {
    run = false;
}

void ce2_core_set_tps(uint8_t value) {
    if (value == 0) value = 1;
    tps = value;
}

uint8_t ce2_core_get_tps() {
    return tps;
}

int main() {
    init();

    game_init();

    uint64_t target_frame_ns = 0;
    uint64_t next_frame_start = ce2_platform_time_get_ns();

    while (run) {
        next_frame_start += target_frame_ns;

        ce2_screen_size_update();
        ce2_screen_buffer_clear();

        game_loop();

        ce2_screen_render();

        target_frame_ns = 1000000000ULL / tps;
        uint64_t now = ce2_platform_time_get_ns();

        if (now < next_frame_start) {
            uint64_t remaining = next_frame_start - now;
            if (remaining > 2000000ULL) {
                ce2_platform_time_sleep_ns(remaining - 2000000ULL);
                now = ce2_platform_time_get_ns();
            }
            while (now < next_frame_start) {
                ce2_platform_time_cpu_relax(); 
                now = ce2_platform_time_get_ns();
            }
        } else {
            if (now - next_frame_start > 1000000000ULL) {
                next_frame_start = now;
            }
        }
    }

    ce2_platform_terminal_shutdown();
    return 0;
}