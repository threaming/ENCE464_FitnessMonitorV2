#include "unity.h"
#include "hal/i2c_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include <stdbool.h>
#include "driverlib/pin_map.h"
#include "tiva_mocks/i2c_mock.h"
#include "tiva_mocks/sysctl_mock.h"
#include "tiva_mocks/gpio_mock.h"

#define TEST_ADDRESS 0x1D
#define TEST_CMD_ADDRESS 0x31
#define FAKE_CLK 16000000

/* Helper functions */
void reset_fff(void) {
    FFF_I2C_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_GPIO_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void assert_callhistory (unsigned int history_len, uint32_t expected, uint32_t* history) {
    for (int i = 0; i <= history_len; i++) {
        if (history[i]==0) break;
        TEST_ASSERT_EQUAL(expected, history[i]);
    }
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
    I2CMasterBusBusy_fake.return_val = true;
    I2CMasterBusy_fake.return_val = false;
}

void tearDown(void) {
    
}

/* Test Cases ----------- i2c init ----------- */
void test_i2c_hal_register_ignores_wrong_id(void) {
    // Act
    bool success = i2c_hal_register(7);

    // Assert
    TEST_ASSERT_EQUAL(false, success);
}

void test_i2c_hal_register_ignores_right_id(void) {
    // Act
    bool success = i2c_hal_register(I2C_ID_1);

    // Assert
    TEST_ASSERT_EQUAL(true, success);
}

void test_i2c_hal_register_enable_peripherals(void) {
    // Act
    i2c_hal_register(I2C_ID_1);

    // Assert
    TEST_ASSERT_EQUAL(3, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_GPIOB, SysCtlPeripheralEnable_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_GPIOE, SysCtlPeripheralEnable_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralEnable_fake.arg0_history[2]);
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralReset_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_I2C0, SysCtlPeripheralReset_fake.arg0_val);
}

void test_i2c_hal_register_set_gpio_pins(void) {
    // Act
    i2c_hal_register(I2C_ID_1);

    // Assert
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2C_fake.call_count);
    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2C_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_3, GPIOPinTypeI2C_fake.arg1_val);
    TEST_ASSERT_EQUAL(1, GPIOPinTypeI2CSCL_fake.call_count);
    TEST_ASSERT_EQUAL(GPIO_PORTB_BASE, GPIOPinTypeI2CSCL_fake.arg0_val);
    TEST_ASSERT_EQUAL(GPIO_PIN_2, GPIOPinTypeI2CSCL_fake.arg1_val);
    TEST_ASSERT_EQUAL(2, GPIOPinConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(GPIO_PB2_I2C0SCL, GPIOPinConfigure_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(GPIO_PB3_I2C0SDA, GPIOPinConfigure_fake.arg0_history[1]);
}

void test_i2c_hal_register_setup_i2c(void) {
    // Arrange
    SysCtlClockGet_fake.return_val = FAKE_CLK;

    // Act
    i2c_hal_register(I2C_ID_1);

    // 
    TEST_ASSERT_EQUAL(1, SysCtlClockGet_fake.call_count);
    TEST_ASSERT_EQUAL(1, I2CMasterInitExpClk_fake.call_count);
    TEST_ASSERT_EQUAL(I2C0_BASE, I2CMasterInitExpClk_fake.arg0_val);
    TEST_ASSERT_EQUAL(FAKE_CLK, I2CMasterInitExpClk_fake.arg1_val);
    TEST_ASSERT_EQUAL(true, I2CMasterInitExpClk_fake.arg2_val);
}

/* Test Cases ----------- i2c transmit ----------- */
void test_i2c_hal_ignores_wrong_id(void) {
    // Arrange
    char data[] = {0, 0};

    // Act
    i2c_hal_transmit(7 ,data, 1, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(0, I2CMasterSlaveAddrSet_fake.call_count);
}

void test_i2c_hal_sets_correct_address(void) {
    // Arrange
    char data[] = {0, 0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 1, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(1, I2CMasterSlaveAddrSet_fake.call_count);
    TEST_ASSERT_EQUAL(TEST_ADDRESS, I2CMasterSlaveAddrSet_fake.arg1_val);
}

void test_i2c_hal_uses_correct_base_address(void) {
    // Arrange
    char data[] = {0, 0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 1, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(I2C0_BASE, I2CMasterSlaveAddrSet_fake.arg0_val);
    assert_callhistory(I2CMasterDataPut_fake.arg_history_len,
                       I2C0_BASE,
                       I2CMasterDataPut_fake.arg0_history);
    assert_callhistory(I2CMasterControl_fake.arg_history_len,
                       I2C0_BASE,
                       I2CMasterControl_fake.arg0_history);
    assert_callhistory(I2CMasterBusy_fake.arg_history_len,
                       I2C0_BASE,
                       I2CMasterBusy_fake.arg0_history);
}

void test_i2c_hal_initiates_read(void) {
    // Arrange
    char data[] = {0, 0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 1, I2C_READ, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(I2C_WRITE, I2CMasterSlaveAddrSet_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL(I2C_READ, I2CMasterSlaveAddrSet_fake.arg2_history[1]);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_SEND_START, I2CMasterControl_fake.arg1_history[0]);
}

void test_i2c_hal_initiates_write(void) {
    // Arrange
    char data[] = {0, 0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 1, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(I2C_WRITE, I2CMasterSlaveAddrSet_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL(I2C_WRITE, I2CMasterSlaveAddrSet_fake.arg2_history[1]);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_SEND_START, I2CMasterControl_fake.arg1_history[0]);
}

/* ------- reading ------- */
void test_i2c_hal_single_receive(void) {
    // Arrange
    char data[] = {TEST_CMD_ADDRESS, 
                   0};
    I2CMasterDataGet_fake.return_val = 0x55;

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 1, I2C_READ, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(TEST_CMD_ADDRESS, I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(2, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_SINGLE_RECEIVE, I2CMasterControl_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(0x55, data[1]);
    TEST_ASSERT_EQUAL(1, I2CMasterDataGet_fake.call_count);
}

void test_i2c_hall_burst_receive_initiated(void) {
    // Arrange
    char data[] = {TEST_CMD_ADDRESS, 
                   0,0,0,0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 4, I2C_READ, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(TEST_CMD_ADDRESS, I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(5, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_RECEIVE_START, I2CMasterControl_fake.arg1_history[1]);
}

void test_i2c_hall_burst_receive_continued(void) {
    // Arrange
    char data[] = {TEST_CMD_ADDRESS, 
                   0,0,0,0};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 4, I2C_READ, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(5, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_RECEIVE_CONT, I2CMasterControl_fake.arg1_history[2]);
}

void test_i2c_hall_burst_receive_stoped(void) {
    // Arrange
    char data[] = {TEST_CMD_ADDRESS, 
                   0,0,0,0};
    uint32_t testData[] = {0x55, 0x66, 0x77, 0x88};
    SET_RETURN_SEQ(I2CMasterDataGet, testData, 4);

    // Act
    i2c_hal_transmit(I2C_ID_1 ,data, 4, I2C_READ, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(5, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_RECEIVE_FINISH, I2CMasterControl_fake.arg1_history[4]);
    TEST_ASSERT_EQUAL((char)testData[0], data[1]);
    TEST_ASSERT_EQUAL((char)testData[2], data[3]);
    TEST_ASSERT_EQUAL(4, I2CMasterDataGet_fake.call_count);
}

/* ------ writing ------ */
void test_i2c_hal_single_send(void) {
    // Arrange
    char testData[] = {TEST_CMD_ADDRESS, 
                   0x55};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,testData, 1, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(TEST_CMD_ADDRESS, I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(2, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_SEND_FINISH, I2CMasterControl_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(testData[1], I2CMasterDataPut_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(2, I2CMasterDataPut_fake.call_count);
}

void test_i2c_hall_burst_send_continued(void) {
    // Arrange
    char testData[] = {TEST_CMD_ADDRESS, 
                   0x55,0x66,0x77,0x88};

    // Act
    i2c_hal_transmit(I2C_ID_1 ,testData, 4, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(TEST_CMD_ADDRESS, I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(5, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_SEND_CONT, I2CMasterControl_fake.arg1_history[2]);
}

void test_i2c_hall_burst_send_stoped(void) {
    // Arrange
    char testData[] = {TEST_CMD_ADDRESS, 
                   0x55, 0x66, 0x77, 0x88};
    SET_RETURN_SEQ(I2CMasterDataGet, (uint32_t*)testData, 4);

    // Act
    i2c_hal_transmit(I2C_ID_1 ,testData, 4, I2C_WRITE, TEST_ADDRESS);

    // Assert
    TEST_ASSERT_EQUAL(TEST_CMD_ADDRESS, I2CMasterDataPut_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(5, I2CMasterControl_fake.call_count);
    TEST_ASSERT_EQUAL(I2C_MASTER_CMD_BURST_SEND_FINISH, I2CMasterControl_fake.arg1_history[4]);
    TEST_ASSERT_EQUAL(testData[1], I2CMasterDataPut_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(testData[3], I2CMasterDataPut_fake.arg1_history[3]);
    TEST_ASSERT_EQUAL(5, I2CMasterDataPut_fake.call_count);
}