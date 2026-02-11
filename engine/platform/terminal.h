#pragma once

#include <stdint.h>

void ce2_platform_terminal_init(void);
void ce2_platform_terminal_shutdown(void);
void ce2_platform_terminal_get_size(uint16_t *width, uint16_t *height);
void ce2_platform_terminal_write(const char *buffer, uint16_t width, uint16_t height);
void ce2_platform_terminal_clear(void);