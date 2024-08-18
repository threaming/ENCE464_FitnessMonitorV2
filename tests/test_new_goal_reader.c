#include "unity.h"
#include "new_goal_reader.h"


#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "adc_hal_mock.h"

#define ADC_BUF_SIZE 10
#define FAKE_ADC_VALUE 0xFACCEADC

/* Helper functions */      
void reset_fff(void)
{
    FFF_ADC_HAL_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

circBuf_t* get_circBuf_ptr_and_reset_fff(void)
{
    initNewGoalReader();
    circBuf_t* buffer_ptr = initCircBuf_fake.arg0_val;
    reset_fff();
    return buffer_ptr;
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - initNewGoalReader */
void test_adc_init_initialises_buffer(void)
{   
    // Act
    initNewGoalReader();

    // Assert
    TEST_ASSERT_EQUAL(1, initCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, initCircBuf_fake.arg1_val);
}

void test_adc_init_registers_adc(void)
{
    // Act
    initNewGoalReader();

    // Assert
    TEST_ASSERT_EQUAL(1, adc_hal_register_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_ID_1, adc_hal_register_fake.arg0_val);
}

/* Test cases - pollNewGoalReader */
void test_adc_poll_triggers_adc(void)
{
    // Act
    pollNewGoalReader();

    // Assert
    TEST_ASSERT_EQUAL(1, adc_hal_start_conversion_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_ID_1, adc_hal_start_conversion_fake.arg0_val);
}

/* Test cases - newGoal_callback */
void test_callback_writes_to_correct_buffer(void)
{
    // Arrange
    initNewGoalReader();
    void (*isr) (void) = adc_hal_register_fake.arg1_val;
    circBuf_t* buff = get_circBuf_ptr_and_reset_fff();

    // Act
    isr();

    // Assert
    TEST_ASSERT_EQUAL(1, writeCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(buff, writeCircBuf_fake.arg0_val);
}

void test_callback_writes_correct_value_to_buffer(void)
{
    // Arrange
    initNewGoalReader();
    void (*isr) (uint32_t) = adc_hal_register_fake.arg1_val;

    // Act
    isr(FAKE_ADC_VALUE);

    // Assert
    TEST_ASSERT_EQUAL(1, writeCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL_UINT32(FAKE_ADC_VALUE, writeCircBuf_fake.arg1_val);
}

/* Test cases - readNewGoalValue */
void test_adc_read_from_right_buffer(void)
{
    // Arrange
    circBuf_t* buff = get_circBuf_ptr_and_reset_fff();

    // Act
    readNewGoalValue();

    // Assert
    TEST_ASSERT_EQUAL_PTR(buff, readCircBuf_fake.arg0_val);
}

void test_adc_read_average_over_all_elements(void)
{
    // Act
    readNewGoalValue();

    // Assert
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, readCircBuf_fake.call_count);
}

void test_adc_read_calculate_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {4,10,4,10,4,10,4,10,4,10};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readNewGoalValue();

    // Assert
    TEST_ASSERT_EQUAL(7, adcAvg);
}

void test_adc_read_round_up_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {5,10,5,10,5,10,5,10,5,10};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readNewGoalValue();

    // Assert
    TEST_ASSERT_EQUAL(8, adcAvg);
}

void test_adc_read_round_down_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {1,2,3,4,5,6,7,8,9,9};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readNewGoalValue();

    // Assert
    TEST_ASSERT_EQUAL(5, adcAvg);
}