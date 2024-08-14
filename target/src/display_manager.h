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

#include "device_state.h"

#define M_PER_STEP 9/10

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
void displayUpdate(uint16_t secondsElapsed); // Update the display, given the device's state and the length of the current workout

#define DISPLAY_WIDTH 16

#endif /* DISPLAY_MANAGER_H_ */
