#include "fff.h"
#include <stdbool.h>
#include "hal/temp_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_TEMP_HAL_FAKES_LIST(FUNC)       \
    FUNC(temp_hal_init)                 \
    FUNC(temp_hal_read)           

VALUE_FUNC(bool, temp_hal_init);
VOID_FUNC(temp_hal_read, float*);
