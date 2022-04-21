/*
 * Module to read values from the Orbit Board toggle switches
 * Created 21/4/22
 * Author: Daniel Rabbidge
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)
#include "switches.h"


// *******************************************************
// Globals to module
// *******************************************************
static bool SW_state;	// Corresponds to the electrical state
static uint8_t SW_count;
static bool SW_flag;
static bool SW_normal;   // Corresponds to the electrical state

// *******************************************************
// initButtons: Initialise the variables associated with the set of buttons
// defined by the constants in the buttons2.h header file.
void
initSwitch (void)
{
	int i;

	// UP button (active HIGH)
    SysCtlPeripheralEnable (SW1_PERIPH);
    GPIOPinTypeGPIOInput (SW1_PORT_BASE, SW1_PIN);
    GPIOPadConfigSet (SW1_PORT_BASE, SW1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    SW_normal = SW1_NORMAL;

    SW_state = SW1_NORMAL;
    SW_count = 0;
    SW_flag = false;
}

// *******************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.
void
updateSwitch (void)
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
            SW_flag = true;	   // Reset by call to checkButton()
            SW_count = 0;
        }
    }
    else
        SW_count = 0;

}

bool isSwitchDown(void) {
    return GPIOPinRead (SW1_PORT_BASE, SW1_PIN);
}

// *******************************************************
// checkButton: Function returns the new button logical state if the button
// logical state (PUSHED or RELEASED) has changed since the last call,
// otherwise returns NO_CHANGE.

//TODO: I don't think this function is necessary and can likely be deleted
/*
uint8_t
checkSwitch (void)
{
	if (SW_flag)
	{
		SW_flag = false;
		if (SW_state == SW_normal)
			return SW_DOWN;
		else
			return SW_UP;
	}
	return SW_NO_CHANGE;
}
*/

