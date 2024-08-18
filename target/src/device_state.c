#include "device_state.h"

// Static global variable to hold the device state
static deviceStateInfo_t device_state = {0};

// Public getter function
deviceStateInfo_t* get_modifiable_device_state(void) {
    return &device_state;
}