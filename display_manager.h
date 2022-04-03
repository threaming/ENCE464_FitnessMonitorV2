/*
 * display_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */

#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
} displayMode_t;

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
void displayUpdate(stepsInfo_t stepInfo);
void displayClear(void);

#define M_PER_STEP 9/10 // 0.9m per step. Defined here so we can access it in the main programme and also when displaying
#define DISPLAY_WIDTH 16

#endif /* DISPLAY_MANAGER_H_ */
