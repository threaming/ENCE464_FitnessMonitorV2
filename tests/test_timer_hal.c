#include "unity.h"
#include "timer_hal.h"
#include "inc/hw_memmap.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

// Include Mocks
#include "tiva_mocks/timer_mock.h"
#include "tiva_mocks/sysctl_mock.h"
#include "mocks/prompt_to_move_mock.h"
#include "mocks/rtos_mocks/rtos_mock.h"
#include "step_counter_main_mock.h"
#include "device_state_mock.h"

/* Helper functions */
void reset_fff(void)
{
    printf("Resetting FFF\n");
    FFF_TIMER_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_PROMPT_TO_MOVE_FAKES_LIST(RESET_FAKE);
    FFF_RTOS_WRAPPER_FAKES_LIST(RESET_FAKE);
    FFF_STEP_COUNTER_MAIN_FAKES_LIST(RESET_FAKE);
    FFF_DEVICE_STATE_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    printf("Setting up\n");
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - timer_hal_init */
void test_timer_hal_init_initializes_timer(void)
{
    // Arrange
    // uint8_t timer_period_seconds = 5;

    // // Act
    // timer_hal_init(timer_period_seconds);

    // //Assert
    // TEST_ASSERT_EQUAL(1, TimerConfigure_fake.call_count);
    // TEST_ASSERT_EQUAL(TIMER0_BASE, TimerConfigure_fake.arg0_val);
    // TEST_ASSERT_EQUAL(TIMER_CFG_PERIODIC, TimerConfigure_fake.arg1_val);
    // TEST_ASSERT_EQUAL(1, TimerLoadSet_fake.call_count);
    // TEST_ASSERT_EQUAL(TIMER0_BASE, TimerLoadSet_fake.arg0_val);
    // TEST_ASSERT_EQUAL(TIMER_A, TimerLoadSet_fake.arg1_val);
}

/* Test cases - timer_hal_reset */
void test_timer_hal_reset_resets_timer(void)
{
    // Arrange
    uint8_t timer_period_seconds = 5;

    // Act
    timer_hal_reset(timer_period_seconds);

    // Assert
    TEST_ASSERT_EQUAL(1, TimerDisable_fake.call_count);
    TEST_ASSERT_EQUAL(TIMER0_BASE, TimerDisable_fake.arg0_val);
    TEST_ASSERT_EQUAL(TIMER_A, TimerDisable_fake.arg1_val);
    TEST_ASSERT_EQUAL(1, TimerIntClear_fake.call_count);
    TEST_ASSERT_EQUAL(TIMER0_BASE, TimerIntClear_fake.arg0_val);
    TEST_ASSERT_EQUAL(TIMER_TIMA_TIMEOUT, TimerIntClear_fake.arg1_val);
    TEST_ASSERT_EQUAL(1, TimerLoadSet_fake.call_count);
    TEST_ASSERT_EQUAL(TIMER0_BASE, TimerLoadSet_fake.arg0_val);
    TEST_ASSERT_EQUAL(TIMER_A, TimerLoadSet_fake.arg1_val);
    TEST_ASSERT_EQUAL(SysCtlClockGet() * timer_period_seconds, TimerLoadSet_fake.arg2_val);
    TEST_ASSERT_EQUAL(1, TimerEnable_fake.call_count);
    TEST_ASSERT_EQUAL(TIMER0_BASE, TimerEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(TIMER_A, TimerEnable_fake.arg1_val);
}