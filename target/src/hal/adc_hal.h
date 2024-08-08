#ifndef ADC_READ_HAL_H_
#define ADC_READ_HAL_H_

#include <stdint.h>

typedef enum {
    ADC_ID_1
    } Adc_Id_t;

void adc_hal_register(Adc_Id_t ADC_ID, void *callback);

#endif //ADC_READ_HAL_H_