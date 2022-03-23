/*
 * display_manager.c
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "serial_sender.h"
#include "display_manager.h"


//********************************************************
// Constants and static vars
//********************************************************

#define MS_TO_KMH 10/36



/*******************************************
 *      Local prototypes
 *******************************************/
static void displayLine(char* inStr, uint8_t row, textAlignment_t alignment);
static void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting);
static void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);


/*******************************************
 *      Global functions
 *******************************************/
// Init the screen library
void displayInit(void)
{
    OLEDInitialise();
}



// Update the display
void displayUpdate(displayMode_t displayMode, uint32_t steps_taken, uint16_t secondsElapsed)
{
    displayTime("Time:", secondsElapsed, 2, ALIGN_CENTRE);

    uint16_t mTravelled = 0; // TODO: If I put this inside the case statement it won't compile. Work out why!

    switch (displayMode) {
    case DISPLAY_STEPS:
        displayValue("", "steps", steps_taken, 1, ALIGN_CENTRE, false);
        break;
    case DISPLAY_DISTANCE:
        mTravelled = steps_taken * M_PER_STEP;
        displayValue("Dist:", "km", steps_taken, 1, ALIGN_CENTRE, true);
        uint16_t speed = (1000 * mTravelled / secondsElapsed) * MS_TO_KMH; // km/h
        displayValue("Speed", "kph", speed, 3, ALIGN_CENTRE, true);
        break;
    case DISPLAY_SET_GOAL:
        // TODO: Write goal setting UI
        break;
    }
}



void displayClear(void)
{
    char* toDraw = "                ";
    OLEDStringDraw (toDraw, 0, 0);
    OLEDStringDraw (toDraw, 0, 1);
    OLEDStringDraw (toDraw, 0, 2);
    OLEDStringDraw (toDraw, 0, 3);
}



/*******************************************
 *      Local Functions
 *******************************************/
// Draw a line to the OLED screen, with the specified alignment
static void displayLine(char* inStr, uint8_t row, textAlignment_t alignment)
{
    // Get the length of the string, but prevent it from being more than 16 chars long
    uint8_t inStrLength = 0;
    while (inStr[inStrLength] != '\0' && inStrLength < DISPLAY_WIDTH) {
        inStrLength++;
    }

    // Create a 16-char long array to write to
    uint8_t i = 0;
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        toDraw[i] = ' ';
    }
    toDraw[DISPLAY_WIDTH] = '\0'; // Set the last character to EOF

    // Set the starting position based on the alignment specified
    uint8_t startPos = 0;
    switch (alignment) {
    case ALIGN_LEFT:
        startPos = 0;
        break;
    case ALIGN_CENTRE:
        startPos = (DISPLAY_WIDTH - inStrLength) /  2;
        break;
    case ALIGN_RIGHT:
        startPos = (DISPLAY_WIDTH - inStrLength);
        break;
    }

    // Copy the string we were given onto the 16-char row
    for (i = 0; i < inStrLength; i++) {
        toDraw[i + startPos] = inStr[i];
    }

    OLEDStringDraw (toDraw, 0, row);
}



// Display a value, with a prefix and suffix
// Can optionally divide the value by 1000, to mimic floats without actually having to use them
static void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs

    if (thousandsFormatting) {
        // Print a number/1000 to 3dp, with decimal point and sign
        // Use a mega cool ternary operator to decide whether to use a minus sign
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %c%d.%03d %s", prefix, value<0? '-':' ', abs(value / 1000), abs(value) % 1000, suffix);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %d %s", prefix, value, suffix); // Can use %4d if we want uniform spacing
    }

    displayLine(toDraw, row, alignment);
}



static void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    uint16_t minutes = time / 60;
    uint16_t seconds = time % 60;

    usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d", prefix, minutes, seconds);

    displayLine(toDraw, row, alignment);
}

