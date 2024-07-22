#include <stdio.h>

#include "fff.h"
#include "unity.h"

DEFINE_FFF_GLOBALS;

/* Static variables */
uint32_t dummy_var = 3;
char dummy_char = 'f';

/* Mocks */
FAKE_VOID_FUNC(mocked_void_function);
FAKE_VOID_FUNC(mocked_arg_function, uint32_t*, uint32_t, char);
FAKE_VALUE_FUNC(uint32_t, mocked_value_function);

/* Unity setup/teardown*/
void setUp(void)
{
    RESET_FAKE(mocked_void_function);
    RESET_FAKE(mocked_arg_function);
    RESET_FAKE(mocked_value_function);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
}

/* Functions under test */
void foo(void)
{
    mocked_void_function();
    mocked_void_function();
}

void bar(void)
{
    mocked_arg_function(&dummy_var, dummy_var, dummy_char);
}

uint32_t baz(void)
{
    return mocked_value_function();
}

/* test cases */
void test_foo_calls_void_function_twice(void)
{
    foo();
    TEST_ASSERT_EQUAL(2, mocked_void_function_fake.call_count);
}

void test_bar_passes_dummy_var_as_pinter_and_value_args(void)
{
    bar();
    TEST_ASSERT_EQUAL_PTR(&dummy_var, mocked_arg_function_fake.arg0_val);
    TEST_ASSERT_EQUAL(dummy_var, mocked_arg_function_fake.arg1_val);
    TEST_ASSERT_EQUAL(dummy_char, mocked_arg_function_fake.arg2_val);
}

void test_baz_returns_the_return_value_of_mocked_value_function(void)
{
    mocked_value_function_fake.return_val = 1213;

    uint32_t result = baz();
    
    TEST_ASSERT_EQUAL(1213, result);
}
