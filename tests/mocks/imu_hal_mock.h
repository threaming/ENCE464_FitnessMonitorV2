#include "fff.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_IMU_HAL_FAKES_LIST(FUNC)    \
    FUNC(imu_hal_init)                  \
    FUNC(imu_hal_get_data) 

VALUE_FUNC(bool, imu_hal_init);
VOID_FUNC(imu_hal_get_data, uint16_t*, uint16_t*, uint16_t*);