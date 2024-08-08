#include "unity.h"
#include "hal/adc_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

/* Helper functions */
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
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

void dummy_callback(uint32_t aValue){
    
}

/* Custom Fakes */
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

/* Test Cases */
void test_adc_hal_enable_correct_adc(void) {
    
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, SysCtlPeripheralEnable_fake.call_count);
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_hal_initialises_adc_sequence(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceConfigure_fake.arg1_val);  
    TEST_ASSERT_EQUAL(ADC_TRIGGER_PROCESSOR, ADCSequenceConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.arg3_val);
}

void test_adc_hal_initialises_adc_sequence_step(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceStepConfigure_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceStepConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceStepConfigure_fake.arg1_val);  
    TEST_ASSERT_EQUAL(0, ADCSequenceStepConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL((ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END), ADCSequenceStepConfigure_fake.arg3_val);
}

void test_adc_hal_enables_adc_sequence(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCSequenceEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceEnable_fake.arg1_val);  
}

void test_adc_hal_registers_adc_interrupt(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntRegister_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntRegister_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntRegister_fake.arg1_val);  
    TEST_ASSERT_EQUAL(dummy_callback, ADCIntRegister_fake.arg2_val);
}

void test_adc_hal_enables_adc_interrupt(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;
    
    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);
    
    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntEnable_fake.call_count);
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCIntEnable_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCIntEnable_fake.arg1_val);  
}

void test_adc_hal_enables_adc_before_other_adc_operations(void)
{
    // Arrange
    SysCtlPeripheralReady_fake.custom_fake = SysCtlPeripheralReady_fake_is_ready;

    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);

    // Assert
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceConfigure);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceStepConfigure);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCSequenceEnable);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCIntRegister);
    assert_f1_called_before_f2((void*)SysCtlPeripheralEnable, (void*)ADCIntEnable);
}

