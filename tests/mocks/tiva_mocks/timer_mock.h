#ifndef TIMER_MOCK_H
#define TIMER_MOCK_H

#include "fff.h"
#include <stdint.h>
#include "driverlib/timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib/interrupt.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_TIMER_FAKES_LIST(FUNC)       \
    FUNC(TimerConfigure)                 \
    FUNC(TimerLoadSet)                   \
    FUNC(TimerIntEnable)                 \
    FUNC(TimerEnable)                    \
    FUNC(TimerDisable)                   \
    FUNC(TimerIntClear)                  \
    FUNC(IntPrioritySet)                 \
    FUNC(TimerIntRegister)               

typedef void (*void_function_ptr_type)(void);

VOID_FUNC(TimerConfigure, uint32_t, uint32_t);
VOID_FUNC(TimerLoadSet, uint32_t, uint32_t, uint32_t);
VOID_FUNC(TimerIntEnable, uint32_t, uint32_t);
VOID_FUNC(TimerEnable, uint32_t, uint32_t);
VOID_FUNC(TimerDisable, uint32_t, uint32_t);
VOID_FUNC(TimerIntClear, uint32_t, uint32_t);

VOID_FUNC(IntPrioritySet, uint32_t, uint8_t);
VOID_FUNC(TimerIntRegister, uint32_t, uint32_t, void_function_ptr_type);

#endif // TIMER_MOCK_H
