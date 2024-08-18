#include "unity.h"
#include "temp_manager.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include <stdbool.h>
#include "rtos/queue_mock.h"
#include "rtos/task_mock.h"
#include "rtos/config_mock.h"
#include "temp_hal_mock.h"

#define QUEUE_FAKE_ADDRESS 0xFE
#define FAKE_TEMP 15.748

/* Helper functions */
void reset_fff(void) {
    FFF_QUEUE_FAKES_LIST(RESET_FAKE);
    FFF_TASK_FAKES_LIST(RESET_FAKE);
    FFF_TEMP_HAL_FAKES_LIST(RESET_FAKE);
    FFF_CONFIG_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void* getTask_from_temp_manager(void) {
    tempInit();
    return xTaskCreate_fake.arg0_val;
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
    temp_hal_init_fake.return_val = true;
    xQueueGenericCreate_fake.return_val = (QueueHandle_t) QUEUE_FAKE_ADDRESS;
    xTaskCreate_fake.return_val = true;
}

void tearDown(void) {
    
}

/* Custom Fake Functions */
BaseType_t xQueueReceive_fake_value(QueueHandle_t arg0, void * const arg1, TickType_t arg2) {
    *(float*)arg1 = FAKE_TEMP;
}

/* Test Cases ----- init temperature module ------ */
void test_temp_manager_init_temp_chip(void) {
    // Act
    tempInit();

    // Assert
    TEST_ASSERT_EQUAL(1, temp_hal_init_fake.call_count);
    TEST_ASSERT_EQUAL(0, vAssertCalled_fake.call_count);
}

void test_temp_manager_init_queue(void) {
    // Act
    tempInit();

    // Assert
    TEST_ASSERT_EQUAL(1, xQueueGenericCreate_fake.call_count);
    TEST_ASSERT_EQUAL(1, xQueueGenericCreate_fake.arg0_val);
    TEST_ASSERT_EQUAL(sizeof(float), xQueueGenericCreate_fake.arg1_val);
    TEST_ASSERT_EQUAL(0, vAssertCalled_fake.call_count);
}

void test_temp_manager_init_task(void) {
    // Act
    tempInit();

    // Assert
    TEST_ASSERT_EQUAL(1, xTaskCreate_fake.call_count);
    TEST_ASSERT_EQUAL(0, vAssertCalled_fake.call_count);
}

void test_temp_manager_task_reading(void) {
    // Arrange
    void (*task) (void*) = getTask_from_temp_manager();

    // Act
    task(NULL);

    // Assert
    TEST_ASSERT_EQUAL(1, temp_hal_read_fake.call_count);
    TEST_ASSERT_EQUAL(temp_hal_read_fake.arg0_val, xQueueGenericSend_fake.arg1_val);
}

void test_temp_manager_task_write_correct_queue(void) {
    // Arrange
    tempInit();
    void (*task) (void*) = getTask_from_temp_manager();

    // Act
    task(NULL);

    // Assert
    TEST_ASSERT_EQUAL(1, xQueueGenericSend_fake.call_count);
    TEST_ASSERT_EQUAL(QUEUE_FAKE_ADDRESS, xQueueGenericSend_fake.arg0_val);
    TEST_ASSERT_EQUAL(portMAX_DELAY, xQueueGenericSend_fake.arg2_val);
}

void test_temp_manager_read_correct_data(void) {
    // Arrange
    xQueueReceive_fake.custom_fake = xQueueReceive_fake_value;

    // Act
    float temp = tempGetTemp();

    // Assert
    TEST_ASSERT_EQUAL(1, xQueueReceive_fake.call_count);
    TEST_ASSERT_EQUAL(FAKE_TEMP, temp);
}

void test_temp_manager_read_correct_queue(void) {
    // Arrange
    tempInit();

    // Act
    tempGetTemp();

    // Assert
    TEST_ASSERT_EQUAL(1, xQueueReceive_fake.call_count);
    TEST_ASSERT_EQUAL(QUEUE_FAKE_ADDRESS, xQueueReceive_fake.arg0_val);
    TEST_ASSERT_EQUAL(portMAX_DELAY, xQueueReceive_fake.arg2_val);
}