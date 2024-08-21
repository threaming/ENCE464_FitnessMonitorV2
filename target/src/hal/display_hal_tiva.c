#include "display_hal.h"
#include <stdint.h>
#include "../../target/libs/OrbitOLED/OrbitOLEDInterface.h"

// Define board types
#define TIVA_BOARD 1

// Set the current board type
#define CURRENT_BOARD TIVA_BOARD

void display_hal_init(void) {
    switch (CURRENT_BOARD) {
        case TIVA_BOARD:
            OLEDInitialise();
            break;
        default:
            break;
    }
}

void display_hal_draw_string(char* str, uint32_t col, uint32_t row) {
    switch (CURRENT_BOARD) {
        case TIVA_BOARD:
            OLEDStringDraw(str, col, row);
            break;
        default:
            break;
    }
}