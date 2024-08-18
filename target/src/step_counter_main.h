/*
 * display_manager.h
 *
 *  Created on: 10/04/2022
 *      Author: Matthew Suter, Tim Preston-Marshall, Daniel Rabbidge
 *
 *  FitnessThur9-1
 */

#ifndef STEP_COUNTER_MAIN_H_
#define STEP_COUNTER_MAIN_H_

#include <stdbool.h>

#include "device_state.h"

void flashMessage(char* toShow, deviceStateInfo_t* deviceState);

#endif /* STEP_COUNTER_MAIN_H_ */
