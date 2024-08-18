#ifndef TEMP_HAL_H_
#define TEMP_HAL_H_

#include <stdbool.h>

bool temp_hal_init(void);
void temp_hal_read(float* temp);


#endif