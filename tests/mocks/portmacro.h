#ifndef PORTMACRO_H
#define PORTMACRO_H

// Define basic types expected by FreeRTOS
#include <stdint.h>

typedef int32_t BaseType_t;     // Define as a basic integer type
typedef uint32_t UBaseType_t;   // Unsigned base type
typedef uint32_t TickType_t;    // Define TickType_t for the tick count

#define portSTACK_TYPE uint32_t // Define StackType_t as a uint32_t
#define StackType_t uint32_t

// Define a valid portBYTE_ALIGNMENT
#define portBYTE_ALIGNMENT 8

// Define the maximum delay value
#define portMAX_DELAY ((TickType_t)0xffffffffUL)

// Define empty macros for the critical sections and task management
#define portYIELD_FROM_ISR(x) do {} while (0)
#define portYIELD() do {} while (0)
#define portEND_SWITCHING_ISR(x) do {} while (0)
#define portENTER_CRITICAL() do {} while (0)
#define portEXIT_CRITICAL() do {} while (0)

// Other necessary FreeRTOS macros
#define portDISABLE_INTERRUPTS() do {} while (0)
#define portENABLE_INTERRUPTS() do {} while (0)
#define portNOP() do {} while (0)

#endif // PORTMACRO_H
