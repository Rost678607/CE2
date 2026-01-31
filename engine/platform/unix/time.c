#include <stdint.h>
#include <time.h>
#include <unistd.h>

void engine_platform_time_init(void) {}

uint64_t engine_platform_time_get_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void engine_platform_time_sleep_ns(uint64_t ns) {
    struct timespec req;
    req.tv_sec = ns / 1000000000ULL;
    req.tv_nsec = ns % 1000000000ULL;
    nanosleep(&req, NULL);
}