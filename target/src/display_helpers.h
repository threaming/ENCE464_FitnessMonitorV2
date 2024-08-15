#ifndef DISPLAY_HELPERS_H_
#define DISPLAY_HELPERS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTRE,
    ALIGN_RIGHT,
} textAlignment_t;

void displayLine(char* inStr, uint8_t row, textAlignment_t alignment);
void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting);
void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);

#define DISPLAY_WIDTH 16

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough
#define MS_TO_KMH 36/10
#define TIME_UNIT_SCALE 60

#endif /* DISPLAY_HELPERS_H_ */