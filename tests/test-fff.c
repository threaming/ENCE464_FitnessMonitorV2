#include <stdio.h>

#include "fff.h"
#include "unity.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(mocked_function);
FAKE_VOID_FUNC(display_string, const char *);

void setUp(void)
{
}

void tearDown(void)
{
}

void call_mocked_function(void)
{
    mocked_function();
    mocked_function();
}

void test_call_mocked_function(void)
{
    call_mocked_function();
    TEST_ASSERT_EQUAL(2, mocked_function_fake.call_count);
}

void display_number(int number)
{
    char str[255];
    snprintf(str, sizeof(str), "Number = %05d", number);
    display_string(str);
}

/**
 * We do the assertion here rather than checking test_display_number via
 * display_string_fake.arg0_val because the value that str points to is no
 * longer valid after display_number returns. It may work fine but is
 * technically undefined behaviour and a memory error.
 *
 * This could be avoided by using something like gtest/gmock.
 */
static void display_string_fake_custom_fake(const char * str)
{
    TEST_ASSERT_EQUAL_STRING("Number = 00010", str);
}

void test_display_number(void)
{
    display_string_fake.custom_fake = display_string_fake_custom_fake;
    display_number(10);
}
