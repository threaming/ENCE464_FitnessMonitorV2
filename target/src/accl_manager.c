/*
 * Acceleration Manager
 * Matt Suter, Tim Preston-Marshall, Daniel Rabbidge
 * ENCE361 2022
 *
 * FitnessThur9-1
 */

#include <stdint.h>
#include <stdbool.h>
#include "hal/imu_hal.h"
#include "circBufV.h"
#include "accl_manager.h"


//********************************************************
// Constants and static vars
//********************************************************
#define BUF_SIZE 20 // WARNING: If this is set too high, we run out of heap space and the z-buffer gets garbled data
static circBufVec_t acclBuffer;


/*******************************************
 *      Global functions
 *******************************************/
// Init the library
void acclInit(void)
{
    imu_hal_init(); // Init the chip over I2C
    initVecCircBuf(&acclBuffer, BUF_SIZE);
}

// Run periodically to store acceleration to the circular buffer
void acclProcess(void)
{
    vector3_t acceleration;
    imu_hal_get_data(&acceleration.x, &acceleration.y, &acceleration.z);
    writeVecCircBuf(&acclBuffer, acceleration);
}

// Return the mean acceleration stored within the circular buffers
vector3_t acclMean(void)
{
    // Sum with 32-bit ints to prevent overflow, then dividing the total sum for better accuracy
    int32_t result_x = 0;
    int32_t result_y = 0;
    int32_t result_z = 0;

    uint8_t i = 0;
    for (i = 0; i < BUF_SIZE; i++) {
        vector3_t nextVector = readVecCircBuf(&acclBuffer);
        result_x = result_x + nextVector.x;
        result_y = result_y + nextVector.y;
        result_z = result_z + nextVector.z;
    }

    vector3_t result = {0};
    result.x = (result_x+BUF_SIZE/2) / BUF_SIZE;
    result.y = (result_y+BUF_SIZE/2) / BUF_SIZE;
    result.z = (result_z+BUF_SIZE/2) / BUF_SIZE;

    return result;
}