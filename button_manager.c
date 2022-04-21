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
#include "button_manager.h"

void btnUpdateState(deviceStateInfo_t* deviceStateInfo)
{
    updateButtons();

    displayMode_t currentDisplayMode = deviceStateInfo ->displayMode;

    // Changing units
    if (checkButton(UP) == PUSHED) {
        deviceStateInfo -> displayUnits = !(deviceStateInfo -> displayUnits);
    }

    // Changing screens
    if (checkButton(LEFT) == PUSHED) {
        displayClear();                             //Clears whole display to avoid overlapping previous display into empty lines but introduces
        deviceStateInfo -> displayMode = (deviceStateInfo -> displayMode + 1) % DISPLAY_NUM_STATES;      //flicker when pressing button

    } else if (checkButton(RIGHT) == PUSHED) {
        displayClear();
        // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
        if (deviceStateInfo -> displayMode > 0) {
            deviceStateInfo -> displayMode--;
        } else {
            deviceStateInfo -> displayMode = DISPLAY_NUM_STATES-1;
        }
//        deviceStateInfo -> displayMode = (deviceStateInfo -> displayMode - 1) % DISPLAY_NUM_STATES;
    }

       /*
    // Clearing the current steps, setting a new goal
    if ((checkButton(DOWN) == PUSHED) && (currentDisplayMode == DISPLAY_SET_GOAL)) {
        deviceStateInfo -> currentGoal = deviceStateInfo -> newGoal;

        // TODO: Require a long press here!
    } else if ((checkButton(DOWN) == PUSHED) && (currentDisplayMode != DISPLAY_SET_GOAL)) { //TODO: Figure out why only the first 'checkButton(DOWN)' works
        deviceStateInfo -> stepsTaken = 0;
    }
        */

    // Resetting steps and updating goal with long and short presses
    if ((isDown(DOWN) == true) && (currentDisplayMode != DISPLAY_SET_GOAL)) {
        longPressCount++;
        if (longPressCount >= LONG_PRESS_CYCLES) {
            deviceStateInfo -> stepsTaken = 0;
        }
    } else {
        if ((currentDisplayMode == DISPLAY_SET_GOAL) && checkButton(DOWN) == PUSHED) {      //Still updates goal if button is held. Check this is ok
            deviceStateInfo -> currentGoal = deviceStateInfo -> newGoal;
            deviceStateInfo -> displayMode = DISPLAY_STEPS;
        }
        longPressCount = 0;
    }
//    return stepInfo;
}

