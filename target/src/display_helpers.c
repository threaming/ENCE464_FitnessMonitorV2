#include <stdint.h>
#include <stdlib.h>
#include "utils/ustdlib.h"

#include "display_helpers.h"
#include "hal/display_hal.h"



// Draw a line to the OLED screen, with the specified alignment
void displayLine(char* inStr, uint8_t row, textAlignment_t alignment)
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
    display_hal_draw_string(toDraw, 0, row);
}



// Can optionally divide the value by 1000, to mimic floats without actually having to use them
void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs

    if (thousandsFormatting) {
        // Print a number/1000 to 3dp, with decimal point and sign
        // Use a mega cool ternary operator to decide whether to use a minus sign
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s%c%d.%03d %s", prefix, value<0? '-':' ', abs(value / 1000), abs(value) % 1000, suffix);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %d %s", prefix, value, suffix); // Can use %4d if we want uniform spacing
    }

    displayLine(toDraw, row, alignment);
}



// Display a given number of seconds, formatted as mm:ss or hh:mm:ss
void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    uint16_t minutes = (time / TIME_UNIT_SCALE) % TIME_UNIT_SCALE;
    uint16_t seconds = time % TIME_UNIT_SCALE;
    uint16_t hours =   time / (TIME_UNIT_SCALE * TIME_UNIT_SCALE);

    if (hours == 0) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d", prefix, minutes, seconds);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d:%02d", prefix, hours, minutes, seconds);
    }

    displayLine(toDraw, row, alignment);
}

