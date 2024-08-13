/*
 * display_manager.h
 *
 * Handles drawing to the device's screen, given its current state
 *
 *  Created on: 23/03/2022
 *      Author: Matthew Suter
 *
 *  FitnessThur9-1
 */

#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

#include "step_counter_main.h"

typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTRE,
    ALIGN_RIGHT,
} textAlignment_t;

typedef struct {
    uint32_t stepsTaken;
    uint32_t currentGoal;
    uint32_t newGoal;
    uint16_t secondsElapsed;
    displayMode_t displayMode;
} stepsInfo_t;


void displayInit(void); // Initalize the display
void displayUpdate(deviceStateInfo_t deviceState, uint16_t secondsElapsed); // Update the display, given the device's state and the length of the current workout
void displayLine(char* inStr, uint8_t row, textAlignment_t alignment); // For testing purposes (even though should never be called outside of this file)
void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting); // For testing purposes (even though should never be called outside of this file)
void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment); // For testing purposes (even though should never be called outside of this file)

#define DISPLAY_WIDTH 16

#endif /* DISPLAY_MANAGER_H_ */
