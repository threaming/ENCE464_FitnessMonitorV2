

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
#include "i2c_driver.h"
#include "circBufT.h"


#include "accl_manager.h"



//********************************************************
// Constants and static vars
//********************************************************
#define BUF_SIZE 25


static circBuf_t xBuffer;
static circBuf_t yBuffer;
static circBuf_t zBuffer;


/*******************************************
 *      Local prototypes
 *******************************************/
void initAcclChip(void);
vector3_t getAcclData(void);

/*******************************************
 *      Global functions
 *******************************************/
// Init the library
void acclInit(void)
{
    initAcclChip(); // Init the chip over I2C

    initCircBuf(&xBuffer, BUF_SIZE);
    initCircBuf(&yBuffer, BUF_SIZE);
    initCircBuf(&zBuffer, BUF_SIZE);
}



// Run periodically to store acceleration to the circular buffer
void acclProcess(void)
{
    vector3_t acceleration = getAcclData();
    writeCircBuf(&xBuffer, acceleration.x);
    writeCircBuf(&yBuffer, acceleration.y);
    writeCircBuf(&zBuffer, acceleration.z);
}



// Return the mean acceleration stored within the circular buffers
vector3_t acclMean(void)
{
//    return getAcclData();



    vector3_t result = {0};

    // For some reason this for loop throws an error
    // TODO: Work out why and solve it!
    /*for (uint8_t i = 0; i < BUF_SIZE; i++) {
        result.x = result.x + readCircBuf(&xBuffer);
        result.y = result.y + readCircBuf(&yBuffer);
        result.z = result.z + readCircBuf(&zBuffer);
    }*/

    uint8_t i = 0;
    while (i < BUF_SIZE) {
        result.x = result.x + readCircBuf(&xBuffer) / BUF_SIZE;
        result.y = result.y + readCircBuf(&yBuffer) / BUF_SIZE;
        result.z = result.z + readCircBuf(&zBuffer) / BUF_SIZE;
        i++;
    }

    return result;
}



/*******************************************
 *      Local Functions
 *******************************************/
// Init the accl chip via I2C
void initAcclChip(void)
{
    char    toAccl[] = {0, 0};  // parameter, value

    /*
    * Enable I2C Peripheral
    */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    /*
    * Set I2C GPIO pins
    */
    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    /*
    * Setup I2C
    */
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    //Initialize ADXL345 Acceleromter

    // set +-16g, 13 bit resolution, active low interrupts
    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_16G | ACCL_FULL_RES);
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_PWR_CTL;
    toAccl[1] = ACCL_MEASURE;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);


    toAccl[0] = ACCL_BW_RATE;
    toAccl[1] = ACCL_RATE_100HZ;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_INT;
    toAccl[1] = 0x00;       // Disable interrupts from accelerometer.
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_X;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Y;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Z;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);
}



// Read the accl chip
vector3_t getAcclData(void)
{
    char    fromAccl[] = {0, 0, 0, 0, 0, 0, 0}; // starting address, placeholders for data to be read.
    vector3_t acceleration;
    uint8_t bytesToRead = 6;

    fromAccl[0] = ACCL_DATA_X0;
    I2CGenTransmit(fromAccl, bytesToRead, READ, ACCL_ADDR);

    acceleration.x = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit acceleration readings.
    acceleration.y = (fromAccl[4] << 8) | fromAccl[3];
    acceleration.z = (fromAccl[6] << 8) | fromAccl[5];

    return acceleration;
}


