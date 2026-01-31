#include <stdint.h>

void        engine_platform_time_init(void);
uint64_t    engine_platform_time_get_ns(void);
void        engine_platform_time_sleep_ns(uint64_t);