#pragma once

#include <stdint.h>

void        ce2_platform_time_init(void);
uint64_t    ce2_platform_time_get_ns(void);
void        ce2_platform_time_sleep_ns(uint64_t);
void        ce2_platform_time_cpu_relax(void);