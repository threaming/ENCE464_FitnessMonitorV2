// display_hal.h
#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <stdint.h>
#include <stdbool.h>

void display_hal_init(void);
void display_hal_draw_string(char* str, uint32_t col, uint32_t row);

#endif // DISPLAY_HAL_H