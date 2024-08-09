#include "fff.h"
#include <stdint.h>
#include "hal/adc_hal.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ADC_HAL_FAKES_LIST(FUNC)    \
    FUNC(adc_hal_register)              \
    FUNC(adc_hal_start_conversion)        

VOID_FUNC(adc_hal_register, Adc_Id_t, void *);
VOID_FUNC(adc_hal_start_conversion, Adc_Id_t);