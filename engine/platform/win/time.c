#include <windows.h>

static HANDLE timer = NULL;
static LARGE_INTEGER frequency;
static double frequency_inv_ns;

void ce2_platform_time_init(void) {
    QueryPerformanceFrequency(&frequency);
    frequency_inv_ns = 1000000000.0 / (double)frequency.QuadPart;
    timer = CreateWaitableTimerExW(NULL, NULL, 0x02, TIMER_ALL_ACCESS);
    if (!timer) {
        timer = CreateWaitableTimer(NULL, TRUE, NULL);
    }
}

uint64_t ce2_platform_time_get_ns(void) {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return (uint64_t)(count.QuadPart * frequency_inv_ns);
}

void ce2_platform_time_sleep_ns(uint64_t ns) {
    if (ns < 1000) return; 

    if (timer) {
        LARGE_INTEGER due_time;
        due_time.QuadPart = -(int64_t)(ns / 100);
        
        if (SetWaitableTimer(timer, &due_time, 0, NULL, NULL, FALSE)) {
            WaitForSingleObject(timer, INFINITE);
            return;
        }
    }
    Sleep((DWORD)(ns / 1000000ULL));
}

void ce2_platform_cpu_relax(void) {
    YieldProcessor();
}