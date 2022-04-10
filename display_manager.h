/*
 * display_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: mattr
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

void displayInit(void);
void displayUpdate(deviceStateInfo_t deviceState, uint32_t newGoal, uint16_t secondsElapsed);
void displayClear(void);

#define M_PER_STEP 9/10 // 0.9m per step. Defined here so we can access it in the main programme and also when displaying
#define DISPLAY_WIDTH 16

#endif /* DISPLAY_MANAGER_H_ */
