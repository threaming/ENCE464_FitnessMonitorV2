#include "fff.h"
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "portmacro.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_QUEUE_FAKES_LIST(FUNC)  \
    FUNC(xQueueGenericCreate)              \
    FUNC(xQueueGenericSend)                \
    FUNC(xQueueReceive)                    

VALUE_FUNC(QueueHandle_t, xQueueGenericCreate, const UBaseType_t, const UBaseType_t, const uint8_t);
VALUE_FUNC(BaseType_t, xQueueGenericSend, QueueHandle_t, const void * const, TickType_t, const BaseType_t);
VALUE_FUNC(BaseType_t, xQueueReceive, QueueHandle_t, void * const, TickType_t);