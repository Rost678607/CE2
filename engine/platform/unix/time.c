#include <stdint.h>
#include <time.h>
#include <unistd.h>

void ce2_platform_time_init(void) {}

uint64_t ce2_platform_time_get_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void ce2_platform_time_sleep_ns(uint64_t ns) {
    struct timespec req;
    req.tv_sec = ns / 1000000000ULL;
    req.tv_nsec = ns % 1000000000ULL;
    nanosleep(&req, NULL);
}

void ce2_platform_cpu_relax(void) {
    #if defined(__x86_64__) || defined(i386) || defined(__i386__)
        _mm_pause();
    #elif defined(__aarch64__) || defined(__arm__)
        __asm__ __volatile__("yield");
    #else
        __asm__ __volatile__("" : : : "memory");
    #endif
}