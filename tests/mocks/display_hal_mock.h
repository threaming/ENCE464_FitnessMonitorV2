#include "fff.h"
#include <stdint.h>
#include "hal/display_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_I2C_HAL_FAKES_LIST(FUNC)    \
    FUNC(display_hal_init)              \
    FUNC(display_hal_draw_string)

VOID_FUNC(display_hal_init);
VOID_FUNC(display_hal_draw_string, char *, uint32_t, uint32_t);