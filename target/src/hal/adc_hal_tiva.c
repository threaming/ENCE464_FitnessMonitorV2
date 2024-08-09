//*****************************************************************************
//
// ADC HAL implementation for the tiva board
//
// Author:  A. Ming
// Created: 07.08.2024
//
//*****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include "adc_hal.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"

/* store callbacks */
void (*adc0_callback)(uint32_t) = NULL;

/* Private Function Prototypes*/
void adc_hal_adc0_isr_handler(void);

/* Main Functions*/
void adc_hal_register(Adc_Id_t ADC_ID, void (*callback)) {
    if (callback == NULL)
        return;

    switch (ADC_ID) {
        case ADC_ID_1:
            // ADC0
            SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
            while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
            ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
            ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                    ADC_CTL_END);  
            ADCSequenceEnable(ADC0_BASE, 3);
            ADCIntRegister (ADC0_BASE, 3, adc_hal_adc0_isr_handler);
            adc0_callback = callback;
            ADCIntEnable(ADC0_BASE, 3);
            break;

        default:
            break;
    }
}

void adc_hal_start_conversion(Adc_Id_t ADC_ID) {
    switch (ADC_ID) {
        case ADC_ID_1:
            // ADC0
            ADCProcessorTrigger(ADC0_BASE, 3);
            break;

        default:
            break;
    }
}

void adc_hal_adc0_isr_handler(void) {
    uint32_t adc_value;
	// Get the single sample from ADC0.  ADC_BASE is defined in
	ADCSequenceDataGet(ADC0_BASE, 3, &adc_value);
    ADCIntClear(ADC0_BASE, 3);
    adc0_callback(adc_value);
}