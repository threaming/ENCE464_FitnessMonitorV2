#include "unity.h"
#include "display_manager.h"
#include "display_helpers.h"
#include "hal/display_hal.h"
#include "new_goal_reader.h"
#include "device_state.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "orbit_mocks/OLED_mock.h"
#include "display_hal_mock.h"
#include "adc_hal_mock.h"
#include "circBufT_mock.h"
#include "temp_manager_mock.h"

#define MAX_CALL_HISTORY 100
#define MAX_STRING_SIZE  200

// Global variables to store call history
static char custom_arg0_history_list[MAX_CALL_HISTORY][MAX_STRING_SIZE];
static uint32_t custom_arg1_history_list[MAX_CALL_HISTORY];
static uint32_t custom_arg2_history_list[MAX_CALL_HISTORY];

// Helper functions      
void reset_fff(void)
{
    FFF_DISPLAY_HAL_FAKES_LIST(RESET_FAKE);
    FFF_TEMP_MANAGER_FAKES_LIST(RESET_FAKE);
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_ADC_HAL_FAKES_LIST(RESET_FAKE);
    FFF_OLED_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    
    // Clear the history buffers
    memset(custom_arg0_history_list, 0, sizeof(custom_arg0_history_list));
    memset(custom_arg1_history_list, 0, sizeof(custom_arg1_history_list));
    memset(custom_arg2_history_list, 0, sizeof(custom_arg2_history_list));
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Fake Functions */
void display_hal_draw_string_fake_values(char* str, uint32_t col, uint32_t row) {
    size_t str_len = strlen(str);
    printf("display_hal_draw_string_fake_values called with str: '%s' (length: %zu), row: %u, col: %u\n", str, str_len, row, col);

    // Store the arguments for later assertions
    if (display_hal_draw_string_fake.call_count < MAX_CALL_HISTORY) {
        printf("Copying string to history...\n");
        if (str_len > 0) {
            strncpy(custom_arg0_history_list[display_hal_draw_string_fake.call_count], str, MAX_STRING_SIZE - 1);
            custom_arg0_history_list[display_hal_draw_string_fake.call_count][MAX_STRING_SIZE - 1] = '\0';  // Ensure null termination
        } else {
            strcpy(custom_arg0_history_list[display_hal_draw_string_fake.call_count], "EMPTY_STRING");  // For debugging
        }
        
        printf("Copied string: '%s' to history\n", custom_arg0_history_list[display_hal_draw_string_fake.call_count]);
        custom_arg1_history_list[display_hal_draw_string_fake.call_count] = row;
        custom_arg2_history_list[display_hal_draw_string_fake.call_count] = col;
    }

    // Increment the call count
    display_hal_draw_string_fake.call_count++;
}

void print_display_hal_draw_string_fake_details(void)
{
    for (int i = 0; i < display_hal_draw_string_fake.call_count; i++) {
        printf("Call %d: custom_arg0_history_list[%d] = '%s'\n", i+1, i, custom_arg0_history_list[i]);
        printf("Call %d: custom_arg1_history_list[%d] = %u\n", i+1, i, custom_arg1_history_list[i]);
        printf("Call %d: custom_arg2_history_list[%d] = %u\n\n", i+1, i, custom_arg2_history_list[i]);
    }
}

/* Test cases - displayInit */
void test_display_init_initialises_oled(void)
{   
    // Act
    displayInit();

    // Assert
    TEST_ASSERT_EQUAL(1, display_hal_init_fake.call_count);
}

/* Test cases - displayUpdate */
void test_display_update_should_display_flash_message_when_active(void)
{
    printf("test_display_update_should_display_flash_message_when_active started\n");
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    
    deviceStateInfo_t *deviceState = get_modifiable_device_state();
    
    deviceState -> flashTicksLeft = 10;
    deviceState -> flashMessage = "FLASH!";

    uint16_t secondsElapsed = 0;

    // Act
    displayUpdate(secondsElapsed);

    print_display_hal_draw_string_fake_details();

    // Assert
    printf("String lengths: expected=%zu, actual=%zu\n", strlen("                "), strlen(custom_arg0_history_list[0]));
    TEST_ASSERT_EQUAL(8, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("                ", custom_arg0_history_list[1]);
    TEST_ASSERT_EQUAL_STRING("     FLASH!     ", custom_arg0_history_list[3]);
    TEST_ASSERT_EQUAL_STRING("                ", custom_arg0_history_list[5]);
    TEST_ASSERT_EQUAL_STRING("                ", custom_arg0_history_list[7]);
    printf("test_display_update_should_display_flash_message_when_active completed\n");
}



void test_display_update_should_display_steps_in_si_units(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    
    deviceStateInfo_t *deviceState = get_modifiable_device_state();
    
    deviceState -> displayMode = DISPLAY_STEPS;
    deviceState -> displayUnits = UNITS_SI;
    deviceState -> stepsTaken = 500;

    uint16_t secondsElapsed = 100;

    // Act
    displayUpdate(secondsElapsed);

    print_display_hal_draw_string_fake_details();

    // Assert
    TEST_ASSERT_EQUAL(6, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("                ", custom_arg0_history_list[1]);
    TEST_ASSERT_EQUAL_STRING("    500 steps   ", custom_arg0_history_list[3]);
    TEST_ASSERT_EQUAL_STRING("   Time: 1:40   ", custom_arg0_history_list[5]);
}

/* Test cases - displayLine */
void test_display_line_should_display_line_left_aligned(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char line[] = "Test Line";

    // Act
    displayLine(line, 0, ALIGN_LEFT);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Test Line       ", custom_arg0_history_list[1]);
}

void test_display_line_should_display_line_center_aligned(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char line[] = "Center";

    // Act
    displayLine(line, 1, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("     Center     ", custom_arg0_history_list[1]);
}

void test_display_line_should_display_line_right_aligned(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char line[] = "Right";

    // Act
    displayLine(line, 2, ALIGN_RIGHT);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("           Right", custom_arg0_history_list[1]);
}

/* Test cases - displayValue */
void test_display_value_should_display_value_without_thousands_formatting(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char prefix[] = "Val:";
    char suffix[] = "units";
    int32_t value = 12345;

    // Act
    displayValue(prefix, suffix, value, 3, ALIGN_LEFT, false);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Val: 12345 units", custom_arg0_history_list[1]);
}

/* Test cases - displayTime */
void test_display_time_should_display_time_as_minutes_and_seconds(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char prefix[] = "Time:";
    uint16_t time = 125; // 2 minutes and 5 seconds

    // Act
    displayTime(prefix, time, 2, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("   Time: 2:05   ", custom_arg0_history_list[1]);
}

void test_display_time_should_display_time_as_hours_minutes_and_seconds(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    char prefix[] = "Time:";
    uint16_t time = 3665; // 1 hour, 1 minute, and 5 seconds

    // Act
    displayTime(prefix, time, 2, ALIGN_CENTRE);

    // Assert
    TEST_ASSERT_EQUAL(2, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(" Time: 1:01:05  ", custom_arg0_history_list[1]);
}
