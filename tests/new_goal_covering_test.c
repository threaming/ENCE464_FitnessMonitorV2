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

