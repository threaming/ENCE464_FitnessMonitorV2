/*
 * button_manager.h
 *
 *  Created on: 1/04/2022
 *      Author: djrab
 */

#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_
#include "step_counter_main.h"

#define LONG_PRESS_CYCLES 20
#define STEP_INCREMENT 100
#define STEP_DECREMENT 500

static uint16_t longPressCount = 0;

void btnUpdateState(deviceStateInfo_t* deviceStateInfo);


#endif /* BUTTON_MANAGER_H_ */
