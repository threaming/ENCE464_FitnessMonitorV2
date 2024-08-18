//*****************************************************************************
//
// Temperature HAL implementation for the tiva board, TCN75A
//
// https://ww1.microchip.com/downloads/en/DeviceDoc/21935D.pdf
//
// Author:  A. Ming
// Created: 18.08.2024
//
//*****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include "temp_hal.h"
#include "i2c_hal.h"

#define TEMP_ADDR 0b1001111 // lsb A2,A1,A0 can be selected by jumpers
// TCN75A Register Pointers
#define TEMP_REG_TA 0x00
#define TEMP_REG_CONFIG 0x01
// Config values
#define TEMP_ONE_SHOT 0x80
#define TEMP_RES_9BIT 0x00  // 0.5°C
#define TEMP_RES_10BIT 0x20 // 0.25°C
#define TEMP_RES_11BIT 0x40 // 0.125°C
#define TEMP_RES_12BIT 0x60 // 0.0625°C


bool temp_hal_init(void){
    bool i2cInit = i2c_hal_register(I2C_ID_1);
    if(!i2cInit) return false; // error, i2c not initialised

    /*  Startup default is: (whole register 0)
        - Continuous Conversion
        - Comparator mode
        - Active-low Output
        - Fault Queue 1
        - 9-bit Resolution -> 0.5°C
    */
   char toTemp[] = {0, 0};
    /* Configure Resolution */
    toTemp[0] = TEMP_REG_CONFIG;
    toTemp[1] = TEMP_RES_12BIT;
    i2c_hal_transmit(I2C_ID_1, toTemp, 1, I2C_WRITE, TEMP_ADDR);
    return true;
}

void temp_hal_read(float* temp){
    char    fromTemp[] = {TEMP_REG_TA, 0, 0}; // starting address, placeholders
    uint8_t bytesToRead = 2;

    i2c_hal_transmit(I2C_ID_1, fromTemp, bytesToRead, I2C_READ, TEMP_ADDR);

    // Combine the two bytes into a single 16-bit value
    bool isNegative = (fromTemp[1] & 0x80) != 0;
    uint8_t integerPart = fromTemp[1] & 0x7F;   // 7 bits for integer part
    uint8_t fractionalBits = fromTemp[2];       // 4 bits for fractional part

    float fractionalPart = 0.0;
    if (fractionalBits & 0x80) fractionalPart += 0.5;    // 2^-1
    if (fractionalBits & 0x40) fractionalPart += 0.25;   // 2^-2
    if (fractionalBits & 0x20) fractionalPart += 0.125;  // 2^-3
    if (fractionalBits & 0x10) fractionalPart += 0.0625; // 2^-4
    *temp = (float)integerPart + fractionalPart;

    if (isNegative) {
        *temp = -(*temp);
    }
}