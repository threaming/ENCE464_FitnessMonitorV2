#ifndef ADC_READ_H_
#define ADC_READ_H_

#include <stdint.h>

void pollADC(void);

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void ADCIntHandler(void);

void initADC (void);

uint32_t readADC(void);

#endif //ADC_READ_H_
