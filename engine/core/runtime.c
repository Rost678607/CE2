#define _POSIX_C_SOURCE 199309L
#include <stdbool.h>
#include <stdint.h>
#if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(_M_IX86)
    #include <immintrin.h>
#endif

#include "engine/platform/time.h"

extern void game_init(void) __attribute__((weak));
extern void game_loop(void) __attribute__((weak));

static uint8_t tps = 60;
static bool run = true;

static void init() {
    engine_platform_time_init();
}

void engine_core_exit() {
    run = false;
}

void engine_core_set_tps(uint8_t value) {
    if (value == 0) value = 1;
    tps = value;
}

uint8_t engine_core_get_tps() {
    return tps;
}

int main() {
    init();

    if (game_init) game_init();

    if (game_loop) {
        uint64_t target_frame_ns = 0;
        uint64_t next_frame_start = engine_platform_time_get_ns();

        while (run) {
            next_frame_start += target_frame_ns;
            game_loop();
            target_frame_ns = 1000000000ULL / tps;
            uint64_t now = engine_platform_time_get_ns();

            if (now < next_frame_start) {
                uint64_t remaining = next_frame_start - now;
                if (remaining > 2000000ULL) {
                    engine_platform_time_sleep_ns(remaining - 2000000ULL);
                    now = engine_platform_time_get_ns();
                }
                while (now < next_frame_start) {
                    #if defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(_M_IX86)
                        _mm_pause(); 
                    #endif
                    now = engine_platform_time_get_ns();
                }
            } else {
                if (now - next_frame_start > 1000000000ULL) {
                    next_frame_start = now;
                }
            }
        }
    }
    return 0;
}