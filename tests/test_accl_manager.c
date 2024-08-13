#include "unity.h"
#include "accl_manager.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include <stdbool.h>
#include "imu_hal_mock.h"
#include "circBufV_mock.h"

#define IMU_BUFFER_SIZE 20

/* Helper functions */
void reset_fff(void) {
    FFF_IMU_HAL_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFV_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
    
}

void tearDown(void) {
    
}

/* Fake Functions */
void imu_hal_get_fake_data(uint16_t* arg0, uint16_t* arg1, uint16_t* arg2) {
    *arg0 = 0x11;
    *arg1 = 0x22;
    *arg2 = 0x33;
}

static vector3_t acceleration;
void writeVecCircBuf_fake_function(circBufVec_t* arg0, vector3_t arg1) {
    acceleration = arg1;
}

/* Test Cases ------ initialize accelerator module ------ */
void test_accl_manager_init_imu(void) {
    // Act
    acclInit();

    // Assert
    TEST_ASSERT_EQUAL(1, imu_hal_init_fake.call_count);
}

void test_accl_manager_init_circ_buffer(void) {
    // Act
    acclInit();

    // Assert
    TEST_ASSERT_EQUAL(1, initVecCircBuf_fake.call_count);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(50, initVecCircBuf_fake.arg1_val);
}

/* Test Cases ------ imu reading process ------ */
void test_accl_manager_process_call_actions(void) {
    // Act
    acclProcess();

    // Assert
    TEST_ASSERT_EQUAL(1, imu_hal_get_data_fake.call_count);
    TEST_ASSERT_EQUAL(1, writeVecCircBuf_fake.call_count);
}

void test_accl_manager_process_store_to_correct_buffer(void) {
    // Arrange
    acclInit();
    circBufVec_t* buff = initVecCircBuf_fake.arg0_val;

    // Act
    acclProcess();

    // Assert
    TEST_ASSERT_EQUAL(1, imu_hal_get_data_fake.call_count);
    TEST_ASSERT_EQUAL(1, writeVecCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(buff, writeVecCircBuf_fake.arg0_val);
}

void test_accl_manager_process_store_data(void) {
    // Arrange
    imu_hal_get_data_fake.custom_fake = imu_hal_get_fake_data;
    writeVecCircBuf_fake.custom_fake = writeVecCircBuf_fake_function;

    // Act
    acclProcess();

    // Assert
    TEST_ASSERT_EQUAL(0x11, acceleration.x);
    TEST_ASSERT_EQUAL(0x22, acceleration.y);
    TEST_ASSERT_EQUAL(0x33, acceleration.z);
}

/* Test Cases ------ data averaging ------ */
void test_accl_manager_averaging_read_from_correct_buffer(void) {
    // Arrange
    acclInit();
    circBufVec_t* buff = initVecCircBuf_fake.arg0_val;

    // Act
    acclMean();

    // Assert
    TEST_ASSERT_EQUAL_PTR(buff, readVecCircBuf_fake.arg0_val);
}

void test_accl_manager_averaging_over_all_elements(void) {
    // Act
    acclMean();

    // Assert
    TEST_ASSERT_EQUAL(IMU_BUFFER_SIZE, readVecCircBuf_fake.call_count);
}

void test_accl_manager_averaging_calculate_correct_value(void) {
    // Arrange
    vector3_t buffReturnVals[IMU_BUFFER_SIZE] = {
    {4, 10, 4}, {10, 4, 10}, {4, 10, 4}, {10, 4, 10}, {4, 10, 4},
    {10, 4, 10}, {4, 10, 4}, {10, 4, 10}, {4, 10, 4}, {10, 4, 10},
    {4, 10, 4}, {10, 4, 10}, {4, 10, 4}, {10, 4, 10}, {4, 10, 4},
    {10, 4, 10}, {4, 10, 4}, {10, 4, 10}, {4, 10, 4}, {10, 4, 10}
    };
    SET_RETURN_SEQ(readVecCircBuf, buffReturnVals, IMU_BUFFER_SIZE);

    // Act
    vector3_t mean = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(7, mean.x);
    TEST_ASSERT_EQUAL(7, mean.y);
    TEST_ASSERT_EQUAL(7, mean.z);
}

void test_accl_manager_averaging_round_up(void) {
    // Arrange
    vector3_t buffReturnVals[20] = {
    {5, 10, 8}, {10, 8, 5}, {8, 5, 10}, {8, 10, 5}, {7, 8, 8},
    {8, 7, 8}, {8, 8, 7}, {6, 9, 8}, {9, 8, 6}, {8, 6, 9},
    {7, 9, 7}, {9, 7, 7}, {7, 7, 9}, {6, 8, 9}, {9, 9, 6},
    {8, 6, 9}, {7, 8, 8}, {8, 7, 8}, {8, 8, 7}, {9, 8, 6}
    };
    SET_RETURN_SEQ(readVecCircBuf, buffReturnVals, IMU_BUFFER_SIZE);

    // Act
    vector3_t mean = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(8, mean.x);
    TEST_ASSERT_EQUAL(8, mean.y);
    TEST_ASSERT_EQUAL(8, mean.z);
}

void test_accl_manager_averaging_round_down(void) {
    // Arrange
    vector3_t buffReturnVals[20] = {
    {7, 8, 7}, {8, 7, 8}, {7, 8, 7}, {8, 7, 8}, {8, 8, 7},
    {7, 7, 8}, {8, 7, 7}, {7, 8, 8}, {8, 8, 7}, {8, 7, 8},
    {7, 8, 7}, {8, 7, 8}, {7, 8, 7}, {8, 7, 8}, {8, 8, 7},
    {7, 7, 8}, {8, 7, 7}, {7, 8, 8}, {8, 8, 7}, {8, 7, 8}
    };
    SET_RETURN_SEQ(readVecCircBuf, buffReturnVals, IMU_BUFFER_SIZE);

    // Act
    vector3_t mean = acclMean();

    // Assert
    TEST_ASSERT_EQUAL(7, mean.x);
    TEST_ASSERT_EQUAL(7, mean.y);
    TEST_ASSERT_EQUAL(7, mean.z);
}