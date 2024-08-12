#include "unity.h"
#include "display_manager.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "display_manager_mock.h"
#include "tiva_mocks/sysctl_mock.h"

/* Helper functions */      
void reset_fff(void)
{
    FFF_DISPLAY_MANAGER_FAKES_LIST(RESET_FAKE);
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

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - displayInit */
void test_display_init_initializes_oled(void)
{   
    // Act
    displayInit();

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDInitialise_fake.call_count);
}

/* Test cases - displayUpdate */
void test_display_update_shows_flash_message_if_flash_ticks_left(void)
{
    // Arrange
    deviceStateInfo_t deviceState = {
        .flashTicksLeft = 5,
        .flashMessage = "FLASH MSG"
    };

    // Act
    displayUpdate(deviceState, 100);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("FLASH MSG", OLEDStringDraw_fake.arg1_val);
}

void test_display_update_shows_steps_when_display_mode_is_steps(void)
{
    // Arrange
    deviceStateInfo_t deviceState = {
        .displayMode = DISPLAY_STEPS,
        .displayUnits = UNITS_SI,
        .stepsTaken = 1234
    };

    // Act
    displayUpdate(deviceState, 100);

    // Assert
    TEST_ASSERT_EQUAL(3, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("1234 steps", OLEDStringDraw_fake.arg1_val);
}

void test_display_update_shows_distance_in_si_units(void)
{
    // Arrange
    deviceStateInfo_t deviceState = {
        .displayMode = DISPLAY_DISTANCE,
        .displayUnits = UNITS_SI,
        .stepsTaken = 1000
    };

    // Act
    displayUpdate(deviceState, 200);

    // Assert
    TEST_ASSERT_EQUAL(3, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Dist: 1.000 km", OLEDStringDraw_fake.arg1_val);
}

void test_display_update_handles_zero_seconds_elapsed(void)
{
    // Arrange
    deviceStateInfo_t deviceState = {
        .displayMode = DISPLAY_DISTANCE,
        .displayUnits = UNITS_SI,
        .stepsTaken = 1000
    };

    // Act
    displayUpdate(deviceState, 0);

    // Assert
    TEST_ASSERT_EQUAL(3, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Speed 36 kph", OLEDStringDraw_fake.arg1_val);
}

/* Test cases - displayLine */
void test_display_line_centres_text_correctly(void)
{
    // Act
    displayLine("Hello", 1, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("     Hello     ", OLEDStringDraw_fake.arg1_val);
}

/* Test cases - displayValue */
void test_display_value_formats_with_thousands_correctly(void)
{
    // Act
    displayValue("Speed", "kph", 123456, 1, ALIGN_CENTRE, true);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(" Speed 123.456 kph", OLEDStringDraw_fake.arg1_val);
}

void test_display_value_formats_without_thousands_correctly(void)
{
    // Act
    displayValue("Steps", "steps", 1234, 1, ALIGN_CENTRE, false);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(" Steps 1234 steps", OLEDStringDraw_fake.arg1_val);
}

/* Test cases - displayTime */
void test_display_time_formats_mm_ss_correctly(void)
{
    // Act
    displayTime("Time", 90, 1, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Time 01:30", OLEDStringDraw_fake.arg1_val);
}

void test_display_time_formats_hh_mm_ss_correctly(void)
{
    // Act
    displayTime("Time", 3661, 1, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Time 01:01:01", OLEDStringDraw_fake.arg1_val);
}
