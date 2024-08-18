#include "unity.h"
#include "hal/temp_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include <stdbool.h>
#include "i2c_hal_mock.h"
#include "tiva_mocks/sysctl_mock.h"
#include "tiva_mocks/gpio_mock.h"
#include "inc/hw_memmap.h"

#define CHIP_ADDR 0x4F

/* Helper functions */
void reset_fff(void) {
    FFF_I2C_HAL_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_GPIO_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Custom Fake Functions */
char transmitted[2];
void i2c_hal_transmit_fake_values(I2c_Id_t arg0, char* arg1,
                                  uint32_t arg2, bool arg3, char arg4)
{
    transmitted[0] = arg1[0];
    transmitted[1] = arg1[1];
}

char rawData[2];
void i2c_hal_transmit_fake_temp(I2c_Id_t arg0, char* arg1,
                                  uint32_t arg2, bool arg3, char arg4)
{
    arg1[1] = rawData[0];
    arg1[2] = rawData[1];
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
    i2c_hal_register_fake.return_val = true;
}

void tearDown(void) {
    
}

/* Test Function ----- init temp chip ------ */
void test_temp_init_register_i2c(void) {
    // Act
    bool succeed = temp_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_register_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_register_fake.arg0_val);
    TEST_ASSERT_EQUAL(true, succeed);
}

void test_temp_init_register_correct_chip(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    temp_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(CHIP_ADDR, i2c_hal_transmit_fake.arg4_val);
}

void test_temp_init_set_resolution(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    temp_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x01, transmitted[0]);
    TEST_ASSERT_EQUAL(0x60, transmitted[1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
}

/* Test Function ------ read temp chip */
void test_temp_read_from_correct_chip(void) {
    // Arrange
    float temp;

    // Act
    temp_hal_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(CHIP_ADDR, i2c_hal_transmit_fake.arg4_val);
}

void test_temp_read_from_correct_data(void) {
    // Arrange
    float temp;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    temp_hal_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(0x00, transmitted[0]);
}

void test_temp_read_correct_positive_data(void) {
    // Arrange
    float temp;
    rawData[0] = 0x0A;
    rawData[1] = 0x00;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_temp;

    // Act
    temp_hal_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(10, temp);
}

void test_temp_read_correct_negative_data(void) {
    // Arrange
    float temp;
    rawData[0] = 0x8A;
    rawData[1] = 0x00;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_temp;

    // Act
    temp_hal_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(-10, temp);
}

void test_temp_read_correct_fractal_data(void) {
    // Arrange
    float temp;
    rawData[0] = 0x0A;
    rawData[1] = 0xC0;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_temp;

    // Act
    temp_hal_read(&temp);

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL_FLOAT(10.75, temp);
}