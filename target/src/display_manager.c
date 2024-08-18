/*
* display_manager.c
*
* Handles drawing to the device's screen, given its current state
*
*  Created on: 23/03/2022
*      Author: Matthew Suter
*
*  FitnessThur9-1
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils/ustdlib.h"

#include "serial_sender.h"
#include "display_manager.h"
#include "display_helpers.h"
#include "hal/display_hal.h"
#include "new_goal_reader.h"


/*******************************************
 *      Global functions
 *******************************************/
// Init the screen library
void displayInit(void)
{
    display_hal_init();
}



// Update the display, called on a loop
void displayUpdate(uint16_t secondsElapsed)
{
    deviceStateInfo_t* deviceState = get_modifiable_device_state();

    uint32_t newGoal = getNewGoal();

    // Check for flash message override
    if (deviceState->flashTicksLeft != 0) {
        char* emptyLine = "                ";
        display_hal_draw_string(emptyLine, 0, 0);
        displayLine(deviceState->flashMessage, 1, ALIGN_CENTRE);
        display_hal_draw_string (emptyLine, 0, 2);
        display_hal_draw_string (emptyLine, 0, 3);
        return;
    }


    uint32_t mTravelled = 0; // TODO: If I put this inside the case statement it won't compile. Work out why!

    switch (deviceState->displayMode) {
    case DISPLAY_STEPS:
        displayLine("", 0, ALIGN_CENTRE); // Clear the top line
        if (deviceState->displayUnits == UNITS_SI) {
            displayValue("", "steps", deviceState->stepsTaken, 1, ALIGN_CENTRE, false);
        } else {
            displayValue("", "% of goal", deviceState->stepsTaken * 100 / deviceState->currentGoal, 1, ALIGN_CENTRE, false);
        }
        displayTime("Time:", secondsElapsed, 2, ALIGN_CENTRE);
        break;
    case DISPLAY_DISTANCE:
        displayTime("Time:", secondsElapsed, 1, ALIGN_CENTRE);
        mTravelled = deviceState->stepsTaken * M_PER_STEP;

        // Protection against division by zero
        uint16_t speed;
        if (secondsElapsed != 0) {
            speed = (mTravelled / secondsElapsed) * MS_TO_KMH; // in km/h
        } else {
            speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
        }

        if (deviceState->displayUnits == UNITS_SI) {
            displayValue("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
            displayValue("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
        } else {
            displayValue("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
            displayValue("Speed", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
        }

        break;
    case DISPLAY_SET_GOAL:
        displayLine("Set goal:", 0, ALIGN_CENTRE);
        displayValue("Current:", "", deviceState->currentGoal, 2, ALIGN_CENTRE, false);

        // Display the step/distance preview
        char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
        uint16_t distance = newGoal * M_PER_STEP;
        if (deviceState->displayUnits != UNITS_SI) {
            distance = distance * KM_TO_MILES;
        }

        // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
        if (distance < 10*1000) {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", newGoal, distance / 1000, (distance % 1000)/100, deviceState->displayUnits == UNITS_SI ? "km" : "mi");
        } else {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d%s", newGoal, distance / 1000, deviceState->displayUnits == UNITS_SI ? "km" : "mi");
        }

        displayLine(toDraw, 1, ALIGN_CENTRE);

        break;

    
    default:
        break;


    }
}