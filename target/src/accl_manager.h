/*
 * accl_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: Matt Suter
 *
 *  FitnessThur9-1
 */

#ifndef ACCL_MANAGER_H_
#define ACCL_MANAGER_H_

#include <stdint.h> // needed to init the vector type
#include "circBufV.h"


void acclInit(void);        // set up library
void acclProcess(void);     // Run periodically
vector3_t acclMean(void);   // Return mean acceleration, calculated using circular buffers


#endif /* ACCL_MANAGER_H_ */
