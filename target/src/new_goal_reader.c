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

#include "new_goal_reader.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_BUF_SIZE 10

//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t ADC_inBuffer;		// Buffer of size BUF_SIZE integers (sample values)
static uint32_t newGoal;

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void pollNewGoalReader(void)
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
void newGoal_callback(uint32_t adc_value) {
	writeCircBuf (&ADC_inBuffer, adc_value);
}

//*****************************************************************************
// Initialisation functions for the ADC
//*****************************************************************************
void initNewGoalReader (void)
{
    initCircBuf (&ADC_inBuffer, ADC_BUF_SIZE);
    adc_hal_register(ADC_ID_1, &newGoal_callback);
}

//*****************************************************************************
// Averages all ADC values accuired in the buffer
//*****************************************************************************
uint32_t readNewGoalValue() {
      uint32_t sum = 0;
      uint16_t i = 0;
      for (i = 0; i < ADC_BUF_SIZE; i++)
          sum = sum + readCircBuf (&ADC_inBuffer);

      return (sum+ADC_BUF_SIZE/2)/ADC_BUF_SIZE;
}

void setNewGoalValue(void)
{    
    pollNewGoalReader();

    newGoal = readNewGoalValue() * POT_SCALE_COEFF; // Set the new goal value, scaling to give the desired range
    newGoal = (newGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING; // Round to the nearest 100 steps
    if (newGoal == 0) { // Prevent a goal of zero, instead setting to the minimum goal (this also makes it easier to test the goal-reaching code on a small but non-zero target)
        newGoal = STEP_GOAL_ROUNDING;
    }
}

void setNewGoal(uint32_t goal)
{
    newGoal = goal;
}

uint32_t getNewGoal(void)
{
    return newGoal;
}
