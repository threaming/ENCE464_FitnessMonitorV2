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
#include "ADC_read.h"


#include "serial_sender.h"
#include "accl_manager.h"
#include "display_manager.h"
#include "button_manager.h"

#include "step_counter_main.h"

/**********************************************************
 * Constants and types
 **********************************************************/
#define RATE_SYSTICK_HZ 1000
#define RATE_IO_HZ 50
#define RATE_ACCL_HZ 200
#define RATE_DISPLAY_UPDATE_HZ 5
#ifdef SERIAL_PLOTTING_ENABLED
#define RATE_SERIAL_PLOT_HZ 100
#endif // SERIAL_PLOTTING_ENABLED
//#define SLOWTICK_RATE_HZ 100
//#define ACC_DATA_RATE 200
#define MAX_STR_LEN 16

#define STEP_GOAL_ROUNDING 100
#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

#define TARGET_DISTANCE_DEFAULT 500

#define POT_SCALE_COEFF 5000/4095 // in steps, adjusting to account for the potentiometer's maximum possible reading

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
//    ADCTickIntHandler();
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
    unsigned long lastIoProcess= 0;
    unsigned long lastAcclProcess = 0;
    unsigned long lastDisplayProcess = 0;

    #ifdef SERIAL_PLOTTING_ENABLED
    unsigned long lastSerialProcess = 0;
    #endif // SERIAL_PLOTTING_ENABLED

    uint8_t stepping = false;
    vector3_t mean;

    //TODO: Archive of previous code, remove if not needed
    //displayMode_t displayMode = DISPLAY_STEPS; // Assigns the initial state
    //uint32_t steps = 0;

    // Device state
    // Omnibus struct that holds loads of info about the device's current state, so it can be updated from any function
    deviceStateInfo_t deviceState;
    deviceState.displayMode = DISPLAY_STEPS;
    deviceState.stepsTaken = 0;
    deviceState.currentGoal = TARGET_DISTANCE_DEFAULT;
    deviceState.debugMode = false;
    deviceState.displayMode = DISPLAY_STEPS;
    deviceState.displayUnits= UNITS_SI;
    deviceState.workoutStartTick = 0;

//    //Initialize stepInfo characteristics
//    stepsInfo_t stepInfo;
//    stepInfo.displayMode = DISPLAY_STEPS;
//    stepInfo.stepsTaken = 0;
//    stepInfo.currentGoal = 9999;

    initClock ();
    displayInit ();
    initButtons ();
    SerialInit ();
    initSysTick ();
    acclInit ();
    initADC();

    while(1)
    {
        unsigned long currentTick = readCurrentTick();

        // Poll the buttons and potentiometer
        if (lastIoProcess + RATE_SYSTICK_HZ/RATE_IO_HZ < currentTick) {
            lastIoProcess = currentTick;

            btnUpdateState(&deviceState);
            pollADC();

            deviceState.newGoal = readADC() * POT_SCALE_COEFF; // Set the new goal value, scaling to give the desired range
            deviceState.newGoal = (deviceState.newGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING; // Round to the nearest 100 steps
        }

        // Read and process the accelerometer
        if (lastAcclProcess + RATE_SYSTICK_HZ/RATE_ACCL_HZ < currentTick) {
            lastAcclProcess = currentTick;

            acclProcess();

            mean = acclMean();

            uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

            if (combined >= STEP_THRESHOLD_HIGH && stepping == false) {
                stepping = true;
                deviceState.stepsTaken++;
            } else if (combined <= STEP_THRESHOLD_LOW) {
                stepping = false;
            }
        }

        // Write to the display
        if (lastDisplayProcess + RATE_SYSTICK_HZ/RATE_DISPLAY_UPDATE_HZ < currentTick) {
            lastDisplayProcess = currentTick;

            uint16_t secondsElapsed = (currentTick - deviceState.workoutStartTick)/RATE_SYSTICK_HZ;
            // uint16_t goalFromPotentiometer = 200; // TODO: When reading from the pot works, feed it through here!

            displayUpdate(deviceState, secondsElapsed);
//            displayUpdate(stepInfo); // pass the current time in here if we also want to display the time since last reset
        }

        // Send to USB via serial
        #ifdef SERIAL_PLOTTING_ENABLED
        if (lastSerialProcess + RATE_SYSTICK_HZ/RATE_SERIAL_PLOT_HZ < currentTick) {
            lastSerialProcess = currentTick;

            SerialPlot(deviceState.stepsTaken, mean.x, mean.y, mean.z);
        }
        #endif // SERIAL_PLOTTING_ENABLED



        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // Prevent the last process ticks from being 'in the future', which would prevent the update functions,
        // rendering the device inoperable.
        // This would take ~49 days, but maybe it could be left in a drawer without being turned off
        if (currentTick < lastIoProcess) {
            lastIoProcess = 0;
        }

        if (currentTick < lastAcclProcess) {
            lastAcclProcess = 0;
        }

        if (currentTick < lastDisplayProcess) {
            lastDisplayProcess = 0;
        }

        #ifdef SERIAL_PLOTTING_ENABLED
        if (currentTick < lastSerialProcess) {
            lastSerialProcess = 0;
        }
        #endif // SERIAL_PLOTTING_ENABLED
    }

}






