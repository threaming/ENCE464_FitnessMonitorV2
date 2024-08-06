#include "unity.h"
#include "ADC_read.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

#define ADC_BUF_SIZE 10
#define FAKE_ADC_VALUE 0xFACCEADC

/* Helper functions */      
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

circBuf_t* get_circBuf_ptr_and_reset_fff(void)
{
    initADC();
    circBuf_t* buffer_ptr = initCircBuf_fake.arg0_val;
    reset_fff();
    return buffer_ptr;
}

void assert_f1_called_before_f2(void* f1, void* f2)
{
    int8_t i_f1 = -1;
    int8_t i_f2 = -1;

    for (uint8_t i = 0; i < FFF_CALL_HISTORY_LEN; i++)
    {
        if(fff.call_history[i] == NULL)
            break;

        if (i_f1 == -1 && fff.call_history[i] == f1) i_f1 = i;
        if (i_f2 == -1 && fff.call_history[i] == f2) i_f2 = i;        
    }
    
    TEST_ASSERT(i_f1 < i_f2);
}

/* Custom fakes */
int32_t ADCSequenceDataGet_fake_adc_value(uint32_t arg0, uint32_t arg1,
                                  uint32_t *arg2)
{
    (void)arg0;
    (void)arg1;
    *arg2 = FAKE_ADC_VALUE;
    return 0;
}

bool SysCtlPeripheralReady_fake_is_ready(uint32_t arg0){
    return 1;
}


/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - initADC */
void test_adc_init_initialises_buffer(void)
{   
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, initCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, initCircBuf_fake.arg1_val);
}

void test_adc_init_enables_adc(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_init_initialises_adc_sequence(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceConfigure_fake.arg1_val);  
    TEST_ASSERT_EQUAL(ADC_TRIGGER_PROCESSOR, ADCSequenceConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.arg3_val);
}

void test_adc_init_initialises_adc_sequence_step(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceStepConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceStepConfigure_fake.arg1_val);  
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL((ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END), ADCSequenceStepConfigure_fake.arg3_val);
}

void test_adc_init_enables_adc_sequence(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceEnable_fake.arg1_val);  
}

void test_adc_init_registers_adc_interrupt(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntRegister_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntRegister_fake.arg1_val);  
    TEST_ASSERT_EQUAL(ADCIntHandler, ADCIntRegister_fake.arg2_val);
}

void test_adc_init_enables_adc_before_other_adc_operations(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    initADC();

    // Assert
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceConfigure);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceStepConfigure);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceEnable);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCIntRegister);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCIntEnable);
}

void test_adc_init_enables_adc_interrupt(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;
    
    // Act
    initADC();
    
    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntEnable_fake.arg1_val);  
}

/* Test cases - pollADC */
void test_adc_poll_triggers_adc(void)
{
    // Act
    pollADC();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCProcessorTrigger_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCProcessorTrigger_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCProcessorTrigger_fake.arg1_val);
}

/* Test cases - ADCIntHandler */
void test_isr_read_correct_adc_channel(void)
{
    // Act
    ADCIntHandler();

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceDataGet_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceDataGet_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceDataGet_fake.arg1_val);
}

void test_isr_writes_to_correct_buffer(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;
    circBuf_t* buff = get_circBuf_ptr_and_reset_fff();

    // Act
    ADCIntHandler();

    // Assert
    TEST_ASSERT_EQUAL(1, writeCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(buff, writeCircBuf_fake.arg0_val);
}

void test_isr_writes_correct_value_to_buffer(void)
{
    // Arrange
    ADCSequenceDataGet_fake.custom_fake = ADCSequenceDataGet_fake_adc_value;

    // Act
    ADCIntHandler();

    // Assert
    TEST_ASSERT_EQUAL(1, writeCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL_UINT32(FAKE_ADC_VALUE, writeCircBuf_fake.arg1_val);
}

void test_isr_clears_interrupt_signal(void)
{
    // Act
    ADCIntHandler();    
    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntClear_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntClear_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntClear_fake.arg1_val);
}

/* Test cases - readADC */
void test_adc_read_from_right_buffer(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;
    circBuf_t* buff = get_circBuf_ptr_and_reset_fff();

    // Act
    readADC();

    // Assert
    TEST_ASSERT_EQUAL_PTR(buff, readCircBuf_fake.arg0_val);
}

void test_adc_read_average_over_all_elements(void)
{
    // Act
    readADC();

    // Assert
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, readCircBuf_fake.call_count);

}

void test_adc_read_calculate_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {4,10,4,10,4,10,4,10,4,10};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readADC();

    // Assert
    TEST_ASSERT_EQUAL(7, adcAvg);
}

void test_adc_read_round_up_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {5,10,5,10,5,10,5,10,5,10};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readADC();

    // Assert
    TEST_ASSERT_EQUAL(8, adcAvg);
}

void test_adc_read_round_down_average_correctly(void)
{
    // Arrange
    int32_t buffReturnVals[ADC_BUF_SIZE] = {1,2,3,4,5,6,7,8,9,9};
    SET_RETURN_SEQ(readCircBuf, buffReturnVals, ADC_BUF_SIZE);

    // Act
    uint32_t adcAvg= readADC();

    // Assert
    TEST_ASSERT_EQUAL(5, adcAvg);
}