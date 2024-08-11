//*****************************************************************************
//
// ADCdemo1.c - Simple interrupt driven program which samples with AIN0
//
// Author:  P.J. Bones	UCECE
// Last modified:	8.2.2018
//
//*****************************************************************************
// Based on the 'convert' series from 2016
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "circBufT.h"
#include "hal/adc_hal.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_BUF_SIZE 10

//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t ADC_inBuffer;		// Buffer of size BUF_SIZE integers (sample values)

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void pollADC(void)
{
    // Initiate a conversion
    adc_hal_start_conversion(ADC_ID_1);
}

//*****************************************************************************
//
// The callback for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void adc_callback(uint32_t adc_value) {
	writeCircBuf (&ADC_inBuffer, adc_value);
}

//*****************************************************************************
// Initialisation functions for the ADC
//*****************************************************************************
void initADC (void)
{
    initCircBuf (&ADC_inBuffer, ADC_BUF_SIZE);
    adc_hal_register(ADC_ID_1, &adc_callback);
}

//*****************************************************************************
// Averages all ADC values accuired in the buffer
//*****************************************************************************
uint32_t readADC() {
      uint32_t sum = 0;
      uint16_t i = 0;
      for (i = 0; i < ADC_BUF_SIZE; i++)
          sum = sum + readCircBuf (&ADC_inBuffer);

      return (sum+ADC_BUF_SIZE/2)/ADC_BUF_SIZE;
}

