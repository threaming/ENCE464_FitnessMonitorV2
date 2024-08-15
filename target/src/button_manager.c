/*
 * Button_management.c
 *
 * Modifies the device's state according to the user's button and switch input
 *
 *  Created on: 31/03/2022
 *      Author: Daniel Rabbidge
 *
 *  FitnessThur9-1
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
#include "switches.h"
#include "step_counter_main.h"
#include "device_state.h"

#define DEBUG_STEP_INCREMENT 100
#define DEBUG_STEP_DECREMENT 500

//********************************************************
// Constants and static vars
//********************************************************
#define LONG_PRESS_CYCLES 20

static uint16_t longPressCount = 0;
static bool allowLongPress = true;


//********************************************************
// Init buttons and switch I/O handlers
//********************************************************
void btnInit(void)
{
    initButtons();
    initSwitch();
}


//********************************************************
// Run at a fixed rate, modifies the device's state depending on button presses
//********************************************************
void btnUpdateState()
{
    deviceStateInfo_t* deviceState = get_modifiable_device_state();

    updateButtons();
    updateSwitch();

    displayMode_t currentDisplayMode = deviceState ->displayMode;

    // Changing screens
    if (checkButton(LEFT) == PUSHED) {
        deviceState -> displayMode = (deviceState -> displayMode + 1) % DISPLAY_NUM_STATES;      //flicker when pressing button

    } else if (checkButton(RIGHT) == PUSHED) {
        // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
        if (deviceState -> displayMode > 0) {
            deviceState -> displayMode--;
        } else {
            deviceState -> displayMode = DISPLAY_NUM_STATES-1;
        }
    }

    // Enable/Disable test mode
    if (isSwitchUp()) {
        deviceState -> debugMode = true;
    } else {
        deviceState -> debugMode = false;
    }


    // Usage of UP and DOWN buttons
    if (deviceState -> debugMode) {
        // TEST MODE OPERATION
        if (checkButton(UP) == PUSHED) {
            deviceState -> stepsTaken = deviceState -> stepsTaken + DEBUG_STEP_INCREMENT;
        }

        if (checkButton(DOWN) == PUSHED) {
            if (deviceState -> stepsTaken >= DEBUG_STEP_DECREMENT) {
                deviceState -> stepsTaken = deviceState -> stepsTaken - DEBUG_STEP_DECREMENT;
            } else {
                deviceState -> stepsTaken = 0;
            }
        }


    } else {
        // NORMAL OPERATION

        // Changing units
        if (checkButton(UP) == PUSHED) {
            if (deviceState -> displayUnits == UNITS_SI) {
                deviceState -> displayUnits = UNITS_ALTERNATE;
            } else {
                deviceState -> displayUnits = UNITS_SI;
            }
        }

        // Resetting steps and updating goal with long and short presses
        if ((isDown(DOWN) == true) && (currentDisplayMode != DISPLAY_SET_GOAL) && (allowLongPress)) {
            longPressCount++;
            if (longPressCount >= LONG_PRESS_CYCLES) {
                deviceState -> stepsTaken = 0;
                flashMessage("Reset!", deviceState);
            }
        } else {
            if ((currentDisplayMode == DISPLAY_SET_GOAL) && checkButton(DOWN) == PUSHED) {
                deviceState -> currentGoal = deviceState -> newGoal;
                deviceState -> displayMode = DISPLAY_STEPS;

                allowLongPress = false; // Hacky solution: Protection against double-registering as a short press then a long press
            }
            longPressCount = 0;
        }

        if (checkButton(DOWN) == RELEASED) {
            allowLongPress = true;
        }


    }

}


