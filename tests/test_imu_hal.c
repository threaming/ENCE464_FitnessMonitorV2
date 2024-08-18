#include "unity.h"
#include "hal/imu_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include <stdbool.h>
#include "i2c_hal_mock.h"
#include "tiva_mocks/sysctl_mock.h"
#include "tiva_mocks/gpio_mock.h"
#include "inc/hw_memmap.h"

#define TEST_ADDRESS 0x1D
#define TEST_CMD_ADDRESS 0x31
#define FAKE_CLK 16000000

/* Helper functions */
void reset_fff(void) {
    FFF_I2C_HAL_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_GPIO_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
    i2c_hal_register_fake.return_val = true;
}

void tearDown(void) {
    
}

/* Fake Functions */
void i2c_hal_receive_fake_values(I2c_Id_t arg0, char* arg1,
                                  uint32_t arg2, bool arg3, char arg4)
{
    arg1[1] = 0x11;
    arg1[2] = 0x22;
    arg1[4] = 0x44;
    arg1[3] = 0x33;
    arg1[5] = 0x55;
    arg1[6] = 0x66;
}

int i = 0;
char transmitted[7][2];
void i2c_hal_transmit_fake_values(I2c_Id_t arg0, char* arg1,
                                  uint32_t arg2, bool arg3, char arg4)
{
    transmitted[i][0] = arg1[0];
    transmitted[i][1] = arg1[1];
    i++;
    if(i==7) i=0;
}

/* Test Functions ----- init imu chip ----- */
void test_imu_init_register_i2c(void) {
    // Act
    bool succeed = imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_register_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_register_fake.arg0_val);
    TEST_ASSERT_EQUAL(true, succeed);
}

void test_imu_init_set_gpio_int(void) {
    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, GPIOPinTypeGPIOInput_fake.call_count);
    TEST_ASSERT_EQUAL(GPIO_PORTE_BASE, GPIOPinTypeGPIOInput_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_4, GPIOPinTypeGPIOInput_fake.arg1_val);
}

void test_imu_init_set_format(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x31, transmitted[0][0]);
    TEST_ASSERT_EQUAL(0x03|0x08, transmitted[0][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_set_power(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x2D, transmitted[1][0]);
    TEST_ASSERT_EQUAL(0x08, transmitted[1][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_set_bw(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x2C, transmitted[2][0]);
    TEST_ASSERT_EQUAL(0x0A, transmitted[2][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_disable_interrupt(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x2E, transmitted[3][0]);
    TEST_ASSERT_EQUAL(0x00, transmitted[3][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_set_xoffset(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x1E, transmitted[4][0]);
    TEST_ASSERT_EQUAL(0x00, transmitted[4][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_set_yoffset(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x1F, transmitted[5][0]);
    TEST_ASSERT_EQUAL(0x00, transmitted[5][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_init_set_zoffset(void) {
    // Arrange
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(7, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x20, transmitted[6][0]);
    TEST_ASSERT_EQUAL(0x00, transmitted[6][1]);
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.arg2_val);
    TEST_ASSERT_EQUAL(I2C_WRITE, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

/* Test Functions ----- read imu chip ----- */
void test_imu_read_from_correct_chip(void) {
    // Arrange
    int16_t xAccel;
    int16_t yAccel;
    int16_t zAccel;

    // Act
    imu_hal_get_data(&xAccel, &yAccel, &zAccel);
    
    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_ID_1, i2c_hal_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL(0x1D, i2c_hal_transmit_fake.arg4_val);
}

void test_imu_read_from_correct_data(void) {
    // Arrange
    int16_t xAccel;
    int16_t yAccel;
    int16_t zAccel;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_transmit_fake_values;

    // Act
    imu_hal_get_data(&xAccel, &yAccel, &zAccel);
    
    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(0x32, transmitted[0][0]);
    TEST_ASSERT_EQUAL(6, i2c_hal_transmit_fake.arg2_val);
}

void test_imu_read_correct_data(void) {
    // Arrange
    int16_t xAccel;
    int16_t yAccel;
    int16_t zAccel;
    i2c_hal_transmit_fake.custom_fake = i2c_hal_receive_fake_values;

    // Act
    imu_hal_get_data(&xAccel, &yAccel, &zAccel);
    
    // Assert
    TEST_ASSERT_EQUAL(1, i2c_hal_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_READ, i2c_hal_transmit_fake.arg3_val);
    TEST_ASSERT_EQUAL(0x2211, xAccel);
    TEST_ASSERT_EQUAL(0x4433, yAccel);
    TEST_ASSERT_EQUAL(0x6655, zAccel);
}