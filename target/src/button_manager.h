/*
 * button_manager.h
 *
 *  Created on: 1/04/2022
 *      Author: Daniel Rabbidge
 *
 *  FitnessThur9-1
 */

#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_
#include "step_counter_main.h"


// Init this library
void btnInit(void);

// Run at a fixed rate, modifies the device state according to the user's button and switch inputs
void btnUpdateState(void);

#endif /* BUTTON_MANAGER_H_ */
