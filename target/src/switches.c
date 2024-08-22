/*
 * Module to read values from the Orbit Board toggle switches
 * Created 21/4/22
 * Author: Daniel Rabbidge
 * Based on the structure of buttons4.c by P.J Bones
 *
 * FitnessThur9-1
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "switches.h"


// *******************************************************
// Globals to module
// *******************************************************
static bool SW_state;	// Corresponds to the electrical state
static uint8_t SW_count;
static bool SW_flag;
//static bool SW_normal;   // Corresponds to the electrical state

// *******************************************************
// initSwitch: Initialise the variables associated with SW1
void initSwitch (void)
{
	// UP button (active HIGH)
    SysCtlPeripheralEnable (SW1_PERIPH);
    GPIOPinTypeGPIOInput (SW1_PORT_BASE, SW1_PIN);
    GPIOPadConfigSet (SW1_PORT_BASE, SW1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//    SW_normal = SW1_NORMAL;

    SW_state = SW1_NORMAL;
    SW_count = 0;
    SW_flag = false;
}


// *******************************************************
// updateSwitch: Function designed to be called regularly. It polls the switch and changes its state with debouncing
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_SW_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_SW_POLLS according to the polling rate.
void updateSwitch (void)
{
    bool SW_value;

    // Read the pins; true means HIGH, false means LOW
    SW_value = (GPIOPinRead (SW1_PORT_BASE, SW1_PIN) == SW1_PIN);


    if (SW_value != SW_state)
    {
        SW_count++;
        if (SW_count >= NUM_SW_POLLS)
        {
            SW_state = SW_value;
            SW_flag = true;    // Reset by call to checkButton()
            SW_count = 0;
        }
    }
    else
        SW_count = 0;

}




//A function that returns the switch state in order to abstract GPIO functions in other modules
bool isSwitchUp(void) {
    return SW_state;
}
