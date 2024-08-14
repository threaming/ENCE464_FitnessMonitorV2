
#ifndef DEVICE_STATE_H_
#define DEVICE_STATE_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
} displayMode_t;


typedef enum {
    UNITS_SI = 0,       // Steps  /km
    UNITS_ALTERNATE,    // Percent/miles
    UNITS_NUM_TYPES,
} displayUnits_t;

typedef struct {
    uint32_t stepsTaken;
    uint32_t currentGoal;       // Goal the user is aiming for
    uint32_t newGoal;           // Value from the potentiometer, processed to give a new goal
    bool debugMode;             // Is enable/disable debug functionality
    displayMode_t displayMode;
    displayUnits_t displayUnits;
    unsigned long workoutStartTick;

    // For displaying temporary messages over the top of everything else
    char *flashMessage;
    unsigned long flashTicksLeft;
} deviceStateInfo_t;

deviceStateInfo_t* get_modifiable_device_state(void);

#endif /* DEVICE_STATE_H_ */
