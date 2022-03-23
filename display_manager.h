/*
 * display_manager.h
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */

#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
} displayMode_t;

void displayInit(void);
void displayUpdate(displayMode_t displayMode);


#endif /* DISPLAY_MANAGER_H_ */
