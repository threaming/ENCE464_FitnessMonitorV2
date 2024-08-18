#ifndef RTOS_WRAPPER_MOCK_H
#define RTOS_WRAPPER_MOCK_H

#include "fff.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtos_wrapper.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_RTOS_WRAPPER_FAKES_LIST(FUNC)       \
    FUNC(RTOS_TaskNotifyGiveFromISR)            \
    FUNC(RTOS_TaskCreate)                       \
    FUNC(RTOS_TaskNotifyTake)

VOID_FUNC(RTOS_TaskNotifyGiveFromISR, TaskHandle_t, BaseType_t*);
VALUE_FUNC(BaseType_t, RTOS_TaskCreate, TaskFunction_t, const char*, configSTACK_DEPTH_TYPE, void*, UBaseType_t, TaskHandle_t*);
VALUE_FUNC(uint32_t, RTOS_TaskNotifyTake, BaseType_t, TickType_t);

#endif // RTOS_WRAPPER_MOCK_H
