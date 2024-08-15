#include "unity.h"
#include "display_manager.h"
#include "hal/display_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "OLED_mock.h"

#define MAX_CALL_HISTORY 100
#define MAX_STRING_SIZE  200

// Global variables to store call history
static char arg0_history[MAX_CALL_HISTORY][MAX_STRING_SIZE];
static uint32_t arg1_history[MAX_CALL_HISTORY];
static uint32_t arg2_history[MAX_CALL_HISTORY];

// Helper functions      
void reset_fff(void)
{
    RESET_FAKE(OLEDInitialise);
    RESET_FAKE(OLEDStringDraw);
    RESET_FAKE(display_hal_init);
    RESET_FAKE(display_hal_draw_string);
    FFF_RESET_HISTORY();
    
    // Clear the history buffers
    memset(arg0_history, 0, sizeof(arg0_history));
    memset(arg1_history, 0, sizeof(arg1_history));
    memset(arg2_history, 0, sizeof(arg2_history));
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
void display_hal_draw_string_fake_values(const char* str, uint32_t row, uint32_t col) {
    size_t str_len = strlen(str);
    printf("display_hal_draw_string_fake_values called with str: '%s' (length: %zu), row: %u, col: %u\n", str, str_len, row, col);

    // Store the arguments for later assertions
    if (display_hal_draw_string_fake.call_count < MAX_CALL_HISTORY) {
        printf("Copying string to history...\n");
        if (str_len > 0) {
            strncpy(arg0_history[display_hal_draw_string_fake.call_count], str, MAX_STRING_SIZE - 1);
            arg0_history[display_hal_draw_string_fake.call_count][MAX_STRING_SIZE - 1] = '\0';  // Ensure null termination
        } else {
            strcpy(arg0_history[display_hal_draw_string_fake.call_count], "EMPTY_STRING");  // For debugging
        }
        
        printf("Copied string: '%s' to history\n", arg0_history[display_hal_draw_string_fake.call_count]);
        arg1_history[display_hal_draw_string_fake.call_count] = row;
        arg2_history[display_hal_draw_string_fake.call_count] = col;
    }

    // Increment the call count
    display_hal_draw_string_fake.call_count++;
}

void print_display_hal_draw_string_fake_details(void)
{
    for (int i = 0; i < display_hal_draw_string_fake.call_count; i++) {
        printf("Call %d: arg0_history[%d] = '%s'\n", i+1, i, arg0_history[i]);
        printf("Call %d: arg1_history[%d] = %u\n", i+1, i, arg1_history[i]);
        printf("Call %d: arg2_history[%d] = %u\n\n", i+1, i, arg2_history[i]);
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
    
    deviceStateInfo_t deviceState = { .flashTicksLeft = 10, .flashMessage = "FLASH!" };
    uint16_t secondsElapsed = 0;

    // Act
    displayUpdate(deviceState, secondsElapsed);

    print_display_hal_draw_string_fake_details();

    // Assert
    printf("String lengths: expected=%zu, actual=%zu\n", strlen("                "), strlen(arg0_history[0]));
    TEST_ASSERT_EQUAL(8, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("                ", arg0_history[1]);
    TEST_ASSERT_EQUAL_STRING("     FLASH!     ", arg0_history[3]);
    TEST_ASSERT_EQUAL_STRING("                ", arg0_history[5]);
    TEST_ASSERT_EQUAL_STRING("                ", arg0_history[7]);
    printf("test_display_update_should_display_flash_message_when_active completed\n");
}



void test_display_update_should_display_steps_in_si_units(void)
{
    // Arrange
    display_hal_draw_string_fake.custom_fake = display_hal_draw_string_fake_values;
    deviceStateInfo_t deviceState = { .displayMode = DISPLAY_STEPS, .displayUnits = UNITS_SI, .stepsTaken = 500 };
    uint16_t secondsElapsed = 100;

    // Act
    displayUpdate(deviceState, secondsElapsed);

    print_display_hal_draw_string_fake_details();

    // Assert
    TEST_ASSERT_EQUAL(6, display_hal_draw_string_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("                ", arg0_history[1]);
    TEST_ASSERT_EQUAL_STRING("    500 steps   ", arg0_history[3]);
    TEST_ASSERT_EQUAL_STRING("   Time: 1:40   ", arg0_history[5]);
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
    TEST_ASSERT_EQUAL_STRING("Test Line       ", arg0_history[1]);
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
    TEST_ASSERT_EQUAL_STRING("     Center     ", arg0_history[1]);
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
    TEST_ASSERT_EQUAL_STRING("           Right", arg0_history[1]);
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
    TEST_ASSERT_EQUAL_STRING("Val: 12345 units", arg0_history[1]);
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
    TEST_ASSERT_EQUAL_STRING("   Time: 2:05   ", arg0_history[1]);
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
    TEST_ASSERT_EQUAL_STRING(" Time: 1:01:05  ", arg0_history[1]);
}
