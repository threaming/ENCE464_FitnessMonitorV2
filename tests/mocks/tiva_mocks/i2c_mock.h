#include "fff.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_I2C_FAKES_LIST(FUNC)    \
    FUNC(I2CMasterSlaveAddrSet)     \
    FUNC(I2CMasterDataPut)          \
    FUNC(I2CMasterControl)          \
    FUNC(I2CMasterBusy)             \
    FUNC(I2CMasterDataGet)          \
    FUNC(I2CMasterBusBusy)          \
    FUNC(I2CMasterInitExpClk)

VOID_FUNC(I2CMasterSlaveAddrSet, uint32_t, uint8_t, bool);
VOID_FUNC(I2CMasterDataPut, uint32_t, uint8_t);
VOID_FUNC(I2CMasterControl, uint32_t, uint32_t);
VALUE_FUNC(bool, I2CMasterBusy, uint32_t);
VALUE_FUNC(uint32_t, I2CMasterDataGet, uint32_t);
VALUE_FUNC(bool, I2CMasterBusBusy, uint32_t);
VOID_FUNC(I2CMasterInitExpClk, uint32_t, uint32_t, bool);
