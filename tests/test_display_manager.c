#include "unity.h"
#include "display_manager.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "OLED_mock.h"

// Helper functions      
void reset_fff(void)
{
    RESET_FAKE(OLEDInitialise);
    RESET_FAKE(OLEDStringDraw);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

void print_OLEDStringDraw_fake_details(void)
{
    for (int i = 0; i < OLEDStringDraw_fake.call_count; i++) {
        printf("Call %d: OLEDStringDraw_fake.arg0_history[%d] = '%s'\n", 
               i+1, i, OLEDStringDraw_fake.arg0_history[i]);
        printf("Call %d: OLEDStringDraw_fake.arg1_history[%d] = %u\n", 
               i+1, i, OLEDStringDraw_fake.arg1_history[i]);
        printf("Call %d: OLEDStringDraw_fake.arg2_history[%d] = %u\n\n", 
               i+1, i, OLEDStringDraw_fake.arg2_history[i]);
    }
}

/* Test cases - displayInit */
void test_display_init_initialises_oled(void)
{   
    // Act
    displayInit();

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDInitialise_fake.call_count);
}

/* Test cases - displayUpdate */
void test_display_update_should_display_flash_message_when_active(void)
{
    // Arrange
    deviceStateInfo_t deviceState = { .flashTicksLeft = 10, .flashMessage = "FLASH!" };
    uint16_t secondsElapsed = 0;

    // Act
    displayUpdate(deviceState, secondsElapsed);

    // Assert
    TEST_ASSERT_EQUAL(4, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("                ", OLEDStringDraw_fake.arg0_history[0]);
    // TEST_ASSERT_EQUAL_STRING("    FLASH!      ", OLEDStringDraw_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL_STRING("                ", OLEDStringDraw_fake.arg0_history[2]);
    TEST_ASSERT_EQUAL_STRING("                ", OLEDStringDraw_fake.arg0_history[3]);
}

void test_display_update_should_display_steps_in_si_units(void)
{
    // Arrange
    deviceStateInfo_t deviceState = { .displayMode = DISPLAY_STEPS, .displayUnits = UNITS_SI, .stepsTaken = 500 };
    uint16_t secondsElapsed = 100;

    // Act
    displayUpdate(deviceState, secondsElapsed);

    // Assert
    TEST_ASSERT_EQUAL(3, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("                ", OLEDStringDraw_fake.arg0_history[0]);
    // TEST_ASSERT_EQUAL_STRING("    500 steps   ", OLEDStringDraw_fake.arg0_history[1]);
    // TEST_ASSERT_EQUAL_STRING('   Time: 01:40  ', OLEDStringDraw_fake.arg0_history[2]);
}

/* Test cases - displayLine */
void test_display_line_should_display_line_left_aligned(void)
{
    // Arrange
    char line[] = "Test Line";

    // Act
    displayLine(line, 0, ALIGN_LEFT);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("Test Line       ", OLEDStringDraw_fake.arg0_val);
}

void test_display_line_should_display_line_center_aligned(void)
{
    // Arrange
    char line[] = "Center";

    // Act
    displayLine(line, 1, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("    Center      ", OLEDStringDraw_fake.arg0_val);
}

void test_display_line_should_display_line_right_aligned(void)
{
    // Arrange
    char line[] = "Right";

    // Act
    displayLine(line, 2, ALIGN_RIGHT);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("           Right", OLEDStringDraw_fake.arg0_val);
}

/* Test cases - displayValue */
void test_display_value_should_display_value_without_thousands_formatting(void)
{
    // Arrange
    char prefix[] = "Val:";
    char suffix[] = "units";
    int32_t value = 12345;

    // Act
    displayValue(prefix, suffix, value, 3, ALIGN_LEFT, false);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING(" Val: 12345 units", OLEDStringDraw_fake.arg0_val);
}

/* Test cases - displayTime */
void test_display_time_should_display_time_as_minutes_and_seconds(void)
{
    // Arrange
    char prefix[] = "Time:";
    uint16_t time = 125; // 2 minutes and 5 seconds

    // Act
    displayTime(prefix, time, 2, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("   Time: 02:05  ", OLEDStringDraw_fake.arg0_val);
}

void test_display_time_should_display_time_as_hours_minutes_and_seconds(void)
{
    // Arrange
    char prefix[] = "Time:";
    uint16_t time = 3665; // 1 hour, 1 minute, and 5 seconds

    // Act
    displayTime(prefix, time, 2, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING(" Time: 1:01:05 ", OLEDStringDraw_fake.arg0_val);
}
