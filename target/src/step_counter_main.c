/*
 * Step_Counter_Main.c
 *
 *  Created on: 23/03/2022
 *      Authors: Matthew Suter, Daniel Rabbidge, Timothy Preston-Marshall
 *
 *  Main code for the ENCE361 step counter project
 *
 *  FitnessThur9-1
 */

// Comment this out to disable serial plotting
// #define SERIAL_PLOTTING_ENABLED


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "math.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef SERIAL_PLOTTING_ENABLED
#include "serial_sender.h"
#endif //SERIAL_PLOTTING_ENABLED

#include "new_goal_reader.h"
#include "temp_manager.h"
#include "prompt_to_move.h"
#include "accl_manager.h"
#include "display_manager.h"
#include "button_manager.h"
#include "step_counter_main.h"
#include "device_state.h"

/**********************************************************
 * Constants and types
 **********************************************************/
#define RATE_SYSTICK_HZ 1000
#define RATE_IO_HZ 75
#define RATE_ACCL_HZ 200
#define RATE_DISPLAY_UPDATE_HZ 5
#define FLASH_MESSAGE_TIME 3/2 // seconds

#define MAX_STR_LEN 16

#ifdef SERIAL_PLOTTING_ENABLED
#define RATE_SERIAL_PLOT_HZ 100
#endif // SERIAL_PLOTTING_ENABLED

#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

#define TARGET_DISTANCE_DEFAULT 1000


/*******************************************
 *      Local prototypes
 *******************************************/
void initClock (void);
void vAssertCalled( const char * pcFile, unsigned long ulLine );


/*******************************************
 *      Globals
 *******************************************/




/***********************************************************
 * Initialisation functions
 ***********************************************************/
void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}



/***********************************************************
 * Helper functions
 ***********************************************************/
// Read the current systick value, without mangling the data
unsigned long readCurrentTick(void)
{
    return xTaskGetTickCount();
}

// Flash a message onto the screen, overriding everything else
void flashMessage(char* toShow, deviceStateInfo_t* deviceState, int8_t flashMessageTime)
{
    deviceState->flashTicksLeft = RATE_DISPLAY_UPDATE_HZ * flashMessageTime;

    uint8_t i = 0;
    while (toShow[i] != '\0' && i < MAX_STR_LEN) {
        (deviceState->flashMessage)[i] = toShow[i];

        i++;
    }

    deviceState->flashMessage[i] = '\0';
}

void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
 (void)pcFile; // unused
 (void)ulLine; // unused
 while (true);
}

void superloop(void* args) {
    
    deviceStateInfo_t* deviceState = get_modifiable_device_state();

    // Variable declarations
    unsigned long lastIoProcess= 0;
    unsigned long lastAcclProcess = 0;
    unsigned long lastDisplayProcess = 0;

    uint8_t stepHigh = false;
    vector3_t mean;

    while(1) {
        unsigned long currentTick = readCurrentTick();

        // Poll the buttons and potentiometer
        if (lastIoProcess + RATE_SYSTICK_HZ/RATE_IO_HZ < currentTick) {
            lastIoProcess = currentTick;

        // updateSwitch();
            btnUpdateState();
            setNewGoalValue();
        }

        // Read and process the accelerometer
        if (lastAcclProcess + RATE_SYSTICK_HZ/RATE_ACCL_HZ < currentTick) {
            lastAcclProcess = currentTick;

            acclProcess();

            mean = acclMean();

            uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

            if (combined >= STEP_THRESHOLD_HIGH && stepHigh == false) {
                stepHigh = true;
                deviceState->stepsTaken++;

                // flash a message if the user has reached their goal
                if (deviceState->stepsTaken == deviceState->currentGoal && deviceState->flashTicksLeft == 0) {
                    flashMessage("Goal reached!", deviceState, FLASH_MESSAGE_TIME);
                }

            } else if (combined <= STEP_THRESHOLD_LOW) {
                stepHigh = false;
            }

            // Don't start the workout until the user begins walking
            if (deviceState->stepsTaken == 0) {
                deviceState->workoutStartTick = currentTick;
            }
        }

        // Write to the display
        if (lastDisplayProcess + RATE_SYSTICK_HZ/RATE_DISPLAY_UPDATE_HZ < currentTick) {
            lastDisplayProcess = currentTick;

            if (deviceState->flashTicksLeft > 0) {
                deviceState->flashTicksLeft--;
            }

            uint16_t secondsElapsed = (currentTick - deviceState->workoutStartTick)/RATE_SYSTICK_HZ;
            displayUpdate(secondsElapsed);
        }

        // Send to USB via serial
        #ifdef SERIAL_PLOTTING_ENABLED
        if (lastSerialProcess + RATE_SYSTICK_HZ/RATE_SERIAL_PLOT_HZ < currentTick) {
            lastSerialProcess = currentTick;

            SerialPlot(deviceState.stepsTaken, mean.x, mean.y, mean.z);
        }
        #endif // SERIAL_PLOTTING_ENABLED



        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming th battery lasts that long)
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


/***********************************************************
 * Main Loop
 ***********************************************************/

int main(void)
{
    
    deviceStateInfo_t* deviceState = get_modifiable_device_state();

    #ifdef SERIAL_PLOTTING_ENABLED
    unsigned long lastSerialProcess = 0;
    #endif // SERIAL_PLOTTING_ENABLED

    // Device state
    // Omnibus struct that holds loads of info about the device's current state, so it can be updated from any function
    deviceState->displayMode = DISPLAY_STEPS;
    deviceState->stepsTaken = 0;
    deviceState->currentGoal = TARGET_DISTANCE_DEFAULT;
    deviceState->debugMode = false;
    deviceState->displayUnits= UNITS_SI;
    deviceState->workoutStartTick = 0;
    deviceState->flashTicksLeft = 0;
    deviceState->flashMessage = calloc(MAX_STR_LEN + 1, sizeof(char));

    // Init libs
    initClock();
    displayInit();
    btnInit();
    acclInit();
    initNewGoalReader();
    tempInit();
    init_prompt_to_move();

    #ifdef SERIAL_PLOTTING_ENABLED
    SerialInit ();
    #endif // SERIAL_PLOTTING_ENABLED

    xTaskCreate(&superloop, "superloop", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0; // Should never reach here

}






