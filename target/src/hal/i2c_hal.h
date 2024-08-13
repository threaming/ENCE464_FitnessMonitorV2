#ifndef I2C_HAL_H_
#define I2C_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#define I2C_READ    1
#define I2C_WRITE   0

typedef enum {
    I2C_ID_1
    } I2c_Id_t;

bool i2c_hal_register(I2c_Id_t I2C_ID);
void i2c_hal_transmit(I2c_Id_t I2C_ID, char* pbData, uint32_t size, bool fRW, char addr);


#endif //I2C_HAL_H_