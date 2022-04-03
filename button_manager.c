/*
 * Button_management.c
 *
 *  Created on: 31/03/2022
 *      Author: Daniel Rabbidge
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"
#include "buttons4.h"
#include "display_manager.h"


stepsInfo_t updateState(stepsInfo_t stepInfo) {
    updateButtons();
    if ((checkButton(LEFT) == PUSHED) && (stepInfo.displayMode < DISPLAY_SET_GOAL)) {
        displayClear();                                     //Clears whole display to avoid overlapping previous display into empty lines but introduces
        stepInfo.displayMode += 1;                          //flicker when pressing button

    } else if ((checkButton(RIGHT) == PUSHED) && (stepInfo.displayMode > DISPLAY_STEPS)) {
        displayClear();
        stepInfo.displayMode -= 1;

    } else if ((checkButton(DOWN) == PUSHED) && (stepInfo.displayMode == DISPLAY_SET_GOAL)) {
        stepInfo.currentGoal = stepInfo.newGoal;

    } else if ((checkButton(DOWN) == PUSHED) && (stepInfo.displayMode != DISPLAY_SET_GOAL)) { //TODO: Figure out why only the first 'checkButton(DOWN)' works
        stepInfo.stepsTaken = 0;
    }
    return stepInfo;
}
