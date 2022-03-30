/*
 * Step_Counter_Main.c
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */

// Comment this out to disable serial plotting
#define SERIAL_PLOTTING_ENABLED


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
// #include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils/ustdlib.h"
#include "buttons4.h"
#include "acc.h"
#include "math.h"
#include "circBufV.h"

#include "serial_sender.h"
#include "accl_manager.h"
#include "display_manager.h"

/**********************************************************
 * Constants and types
 **********************************************************/
#define RATE_SYSTICK_HZ 1000
#define RATE_BUTTONS_HZ 50
#define RATE_ACCL_HZ 200
#define RATE_DISPLAY_UPDATE_HZ 5
#ifdef SERIAL_PLOTTING_ENABLED
#define RATE_SERIAL_PLOT_HZ 100
#endif // SERIAL_PLOTTING_ENABLED
//#define SLOWTICK_RATE_HZ 100
//#define ACC_DATA_RATE 200
#define MAX_STR_LEN 16

#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

/*******************************************
 *      Local prototypes
 *******************************************/
void SysTickIntHandler (void);
void initClock (void);
void initSysTick (void);
void initDisplay (void);
void initAccl (void);
vector3_t getAcclData (void);


/*******************************************
 *      Globals
 *******************************************/
unsigned long ticksElapsed = 0; // Incremented once every system tick. Must be read with SysTickIntHandler(), or you can get garbled data!


/***********************************************************
 * Initialisation functions
 ***********************************************************/
void SysTickIntHandler (void)
{
    ticksElapsed++;
}



void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}



void initSysTick (void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet (SysCtlClockGet () / RATE_SYSTICK_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}



/***********************************************************
 * Helper functions
 ***********************************************************/
// Read the current systick value, without mangling the data
unsigned long readCurrentTick(void)
{
    unsigned long currentTick;
    SysTickIntDisable();
    currentTick = ticksElapsed;
    SysTickIntEnable();
    return currentTick;
}



/***********************************************************
 * Main Loop
 ***********************************************************/

int main(void)
{
    unsigned long last_button_process= 0;
    unsigned long last_accl_process = 0;
    unsigned long last_display_process = 0;

    unsigned long workoutStartTick = 0;

    #ifdef SERIAL_PLOTTING_ENABLED
    unsigned long last_serial_process = 0;
    #endif // SERIAL_PLOTTING_ENABLED

    uint8_t stepping = false;
    uint32_t steps = 0;
    vector3_t mean;
    displayMode_t displayMode = DISPLAY_DISTANCE; // TODO: Switch this back to DISPLAY_STEPS once done writing the distance display code

    initClock ();
    displayInit ();
    initButtons ();
    SerialInit ();
    initSysTick ();
    acclInit ();

    while(1)
    {
        unsigned long currentTick = readCurrentTick();

        if (last_button_process + RATE_SYSTICK_HZ/RATE_BUTTONS_HZ < currentTick) {
            // poll the buttons
            // TODO: Code here pls
            last_button_process = currentTick;
        }

        if (last_accl_process + RATE_SYSTICK_HZ/RATE_ACCL_HZ < currentTick) {
            // process the acceleration system
            last_accl_process = currentTick;

            acclProcess();

            mean = acclMean();

            uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

            if (combined >= STEP_THRESHOLD_HIGH && stepping == false) {
                stepping = true;
                steps++;
            } else if (combined <= STEP_THRESHOLD_LOW) {
                stepping = false;
            }
        }

        if (last_display_process + RATE_SYSTICK_HZ/RATE_DISPLAY_UPDATE_HZ < currentTick) {
            // Send message to booster display
            last_display_process = currentTick;

            uint16_t secondsElapsed = (currentTick - workoutStartTick)/RATE_SYSTICK_HZ;

            displayUpdate(displayMode, steps, secondsElapsed); // pass the current time in here if we also want to display the time since last reset
        }

        #ifdef SERIAL_PLOTTING_ENABLED
        if (last_serial_process + RATE_SYSTICK_HZ/RATE_SERIAL_PLOT_HZ < currentTick) {
            // plot the current data over serial
            last_serial_process = currentTick;

            SerialPlot(steps, mean.x, mean.y, mean.z);
        }
        #endif // SERIAL_PLOTTING_ENABLED


    }

}






