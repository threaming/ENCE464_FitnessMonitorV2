#include "unity.h"
#include "new_goal_reader.h"
#include "button_manager.h"
// #include "display_manager.h"
#include "device_state.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "adc_hal_mock.h"
#include "buttons4_mock.h"
#include "switches_mock.h"
#include "OrbitOLEDInterface_mock.h"
#include "step_counter_main_mock.h"
#include "ustdlib_mock.h"


/* Helper functions */      
void reset_fff(void)
{
    FFF_ADC_HAL_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_BUTTONS4_FAKES_LIST(RESET_FAKE);
    FFF_SWITCHES_FAKES_LIST(RESET_FAKE);
    FFF_ORBITOLED_FAKES_LIST(RESET_FAKE);
    FFF_STEP_COUNTER_MAIN_FAKES_LIST(RESET_FAKE);
    FFF_USTDLIB_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}


/* Custom Fakes */
uint8_t checkButton_down_is_pushed_fake(uint8_t arg0) {
    if (arg0 == DOWN){
        return PUSHED;
    } else {
        return RELEASED;
    }
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - set currentGoal  with newGoal value using btnUpdateState */
void test_set_current_goal_to_new_goal(void)
{   
    // Arrange

    uint32_t value = 1;

    deviceStateInfo_t* deviceState = get_modifiable_device_state();
    deviceState -> displayMode = DISPLAY_SET_GOAL;
    uint32_t newGoal = value;

    checkButton_fake.custom_fake = checkButton_down_is_pushed_fake;
    isSwitchUp_fake.return_val = false;
    isDown_fake.return_val = false;

    setNewGoal(newGoal);

    // Act

    btnUpdateState();

    // Assert
    TEST_ASSERT_EQUAL(deviceState->currentGoal, value);
    
}
