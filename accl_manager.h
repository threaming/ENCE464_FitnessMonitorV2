/*
 * accl_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */

#ifndef ACCL_MANAGER_H_
#define ACCL_MANAGER_H_

#include <stdint.h> // needed to init the vector type

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;



void acclInit(void);       // set up library
void acclProcess(void);     // Run periodically
vector3_t acclMean(void);   // Return mean acceleration, calculated using circular buffers


#endif /* ACCL_MANAGER_H_ */
