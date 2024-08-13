/**********************************************************
 *
 * imu_hal_tiva.c
 *
 * Module to implement the tiva imu
 *
 * A. Ming
 * 19.08.2024
 *
 **********************************************************/
#include <stdbool.h>
#include "imu_hal.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "i2c_hal.h"

/* Accl Hardware Definitions */
#define ACCL_INT1Port       GPIO_PORTB_BASE
#define ACCL_INT2Port       GPIO_PORTE_BASE
#define ACCL_INT1           GPIO_PIN_4
#define ACCL_INT2           GPIO_PIN_4

#define ACCL                2
#define ACCL_ADDR           0x1D

#define ACCL_INT            0x2E
#define ACCL_OFFSET_X       0x1E
#define ACCL_OFFSET_Y       0x1F
#define ACCL_OFFSET_Z       0x20
#define ACCL_DATA_X0        0x32
#define ACCL_DATA_X1        0x33

#define ACCL_PWR_CTL        0x2D
// Parameters for ACCL_PWR_CTL:
#define ACCL_MEASURE        0x08

#define ACCL_DATA_FORMAT    0x31
// Parameters for ACCL_DATA_FORMAT:
#define ACCL_RANGE_2G       0x00
#define ACCL_RANGE_4G       0x01
#define ACCL_RANGE_8G       0x02
#define ACCL_RANGE_16G      0x03
#define ACCL_FULL_RES       0x08
#define ACCL_JUSTIFY        0x04

#define ACCL_BW_RATE        0x2C
// Parameters for ACCL_BW_RATE:
#define ACCL_RATE_3200HZ    0x0F
#define ACCL_RATE_1600HZ    0x0E
#define ACCL_RATE_800HZ     0x0D
#define ACCL_RATE_400HZ     0x0C
#define ACCL_RATE_200HZ     0x0B
#define ACCL_RATE_100HZ     0x0A
#define ACCL_RATE_50HZ      0x09
#define ACCL_RATE_25HZ      0x08
#define ACCL_RATE_12_5HZ    0x07
#define ACCL_RATE_6_25HZ    0x06
#define ACCL_RATE_3_13HZ    0x05
#define ACCL_RATE_1_56HZ    0x04
#define ACCL_RATE_0_78HZ    0x03
#define ACCL_RATE_0_39HZ    0x02
#define ACCL_RATE_0_20HZ    0x01
#define ACCL_RATE_0_10HZ    0x00

bool imu_hal_init(void) {
    bool i2cInit = i2c_hal_register(I2C_ID_1);
    if(!i2cInit) return false; // error, i2c not initialised
    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    /* Configure IMU */
    char toAccl1[] = {ACCL_DATA_FORMAT, ACCL_RANGE_16G | ACCL_FULL_RES};
    i2c_hal_transmit(I2C_ID_1, toAccl1, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl2[] = {ACCL_PWR_CTL, ACCL_MEASURE};
    i2c_hal_transmit(I2C_ID_1, toAccl2, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl3[] = {ACCL_BW_RATE, ACCL_RATE_100HZ};
    i2c_hal_transmit(I2C_ID_1, toAccl3, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl4[] = {ACCL_INT, 0x00}; // Disable interrupts from accelerometer.
    i2c_hal_transmit(I2C_ID_1, toAccl4, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl5[] = {ACCL_OFFSET_X, 0x00};
    i2c_hal_transmit(I2C_ID_1, toAccl5, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl6[] = {ACCL_OFFSET_Y, 0x00};
    i2c_hal_transmit(I2C_ID_1, toAccl6, 1, I2C_WRITE, ACCL_ADDR);

    char toAccl7[] = {ACCL_OFFSET_Z, 0x00};
    i2c_hal_transmit(I2C_ID_1, toAccl7, 1, I2C_WRITE, ACCL_ADDR);

    return true;
}

void imu_hal_get_data(int16_t* xAccel, int16_t* yAccel, int16_t* zAccel){
    char    fromAccl[] = {ACCL_DATA_X0, 0, 0, 0, 0, 0, 0}; // starting address, placeholders
    uint8_t bytesToRead = 6;

    i2c_hal_transmit(I2C_ID_1, fromAccl, bytesToRead, I2C_READ, ACCL_ADDR);

    *xAccel = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit acceleration readings.
    *yAccel = (fromAccl[4] << 8) | fromAccl[3];
    *zAccel = (fromAccl[6] << 8) | fromAccl[5];
}