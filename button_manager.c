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


displayMode_t updateState(displayMode_t currentState) {
    updateButtons();
    if (checkButton(LEFT) == PUSHED) {
        displayClear();
        currentState += 1;
    }
    else if (checkButton(RIGHT) == PUSHED) {
        displayClear();
        currentState -= 1;
    }
    return currentState;
}
