#ifndef TIMER_HAL_H
#define TIMER_HAL_H

#include <stdint.h>
#include <stdbool.h>

// Function to initialize the timer
void timer_hal_init(uint8_t timer_period_seconds);

// Function to start the timer task
void timer_hal_start_task(void);

// Function to reset the timer
void timer_hal_reset(uint8_t timer_period_seconds);

// External handler function declaration
extern void act_on_prompt_to_move(void);

#endif // TIMER_HAL_TIVA_H