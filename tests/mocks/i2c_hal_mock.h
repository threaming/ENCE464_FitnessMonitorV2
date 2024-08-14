#include "fff.h"
#include <stdint.h>
#include "hal/i2c_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_I2C_HAL_FAKES_LIST(FUNC)    \
    FUNC(i2c_hal_register)              \
    FUNC(i2c_hal_transmit)        

VALUE_FUNC(bool, i2c_hal_register, I2c_Id_t);
VOID_FUNC(i2c_hal_transmit, I2c_Id_t, char*, uint32_t, bool, char);