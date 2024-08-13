#include "fff.h"
#include <stdint.h>
#include "driverlib/gpio.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_GPIO_FAKES_LIST(FUNC)   \
    FUNC(GPIOPinTypeI2C)            \
    FUNC(GPIOPinTypeI2CSCL)         \
    FUNC(GPIOPinConfigure)   

VOID_FUNC(GPIOPinTypeI2C, uint32_t, uint8_t);
VOID_FUNC(GPIOPinTypeI2CSCL, uint32_t, uint8_t);
VOID_FUNC(GPIOPinConfigure, uint32_t);