#ifndef ADC_READ_H_
#define ADC_READ_H_

#include <stdint.h>

#define STEP_GOAL_ROUNDING 100
#define POT_SCALE_COEFF 20000/4095 // in steps, adjusting to account for the potentiometer's maximum possible reading


//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void ADCIntHandler(void);

void initADC (void);
void pollADC(void);         // Called periodically at high frequency
uint32_t readADC(void);     // Call to read the averaged readings from the buffer
void setNewGoal(void);

#endif //ADC_READ_H_
