#ifndef IMU_HAL_H_
#define IMU_HAL_H_

#include <stdint.h>
#include <stdbool.h>

bool imu_hal_init(void);
void imu_hal_get_data(int16_t* xAccel, int16_t* yAccel, int16_t* zAccel);

#endif //IMU_HAL_H_