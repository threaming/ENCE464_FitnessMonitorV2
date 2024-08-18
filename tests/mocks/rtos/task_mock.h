#include "fff.h"
#include <stdint.h>
#include "task.h"
#include "FreeRTOSConfig.h"
#include "projdefs.h"
#include "portmacro.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_TASK_FAKES_LIST(FUNC)        \
    FUNC(xTaskCreate)                    \
    FUNC(vTaskDelay)         

VALUE_FUNC(BaseType_t, xTaskCreate, TaskFunction_t, const char * const, const configSTACK_DEPTH_TYPE, void * const, UBaseType_t, TaskHandle_t* const);
VOID_FUNC(vTaskDelay, const TickType_t);