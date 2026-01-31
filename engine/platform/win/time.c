#include <windows.h>
#include <stdint.h>

static HANDLE timer = NULL;
static LARGE_INTEGER frequency;
static double frequency_inv_ns;

void engine_platform_time_init(void) {
    QueryPerformanceFrequency(&frequency);
    frequency_inv_ns = 1000000000.0 / (double)frequency.QuadPart;
    timer = CreateWaitableTimerExW(NULL, NULL, 
                                   CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, 
                                   TIMER_ALL_ACCESS);
}

uint64_t engine_platform_time_get_ns(void) {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return (uint64_t)(count.QuadPart * frequency_inv_ns);
}

void engine_platform_time_sleep_ns(uint64_t ns) {
    if (ns < 1000) return;

    if (timer) {
        LARGE_INTEGER due_time;
        due_time.QuadPart = -(int64_t)(ns / 100);
        
        SetWaitableTimer(timer, &due_time, 0, NULL, NULL, FALSE);
        WaitForSingleObject(timer, INFINITE);
    } else {
        Sleep((DWORD)(ns / 1000000ULL));
    }
}