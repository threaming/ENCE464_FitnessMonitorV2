#include "unity.h"
#include "display_hal.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL

#include "orbit_mocks/OLED_mock.h"

/* Helper functions */
void reset_fff(void) {
    RESET_FAKE(OLEDInitialise);
    RESET_FAKE(OLEDStringDraw);
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void) {
    reset_fff();
}

void tearDown(void) {
    
}

// Very small list of tests - as test_display_manager covers the functionality, this is just to prove the Hardware Abstraction Layer is working!

void test_display_hal_init_initializes_oled(void) {
    // Act
    display_hal_init();

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDInitialise_fake.call_count);
}

void test_display_hal_draw_string_calls_oled_string_draw(void) {
    // Arrange
    char* test_str = "Testing";
    uint32_t test_col = 0;
    uint32_t test_row = 0;

    // Act
    display_hal_draw_string(test_str, test_col, test_row);

    // Assert
    TEST_ASSERT_EQUAL(1, OLEDStringDraw_fake.call_count);
    TEST_ASSERT_EQUAL_STRING(test_str, OLEDStringDraw_fake.arg0_val);
    TEST_ASSERT_EQUAL(test_col, OLEDStringDraw_fake.arg1_val);
    TEST_ASSERT_EQUAL(test_row, OLEDStringDraw_fake.arg2_val);
}