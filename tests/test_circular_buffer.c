#include "unity.h"
#include "circBufT.h"

const uint8_t STANDARD_TEST_CAPACITY = 5;
static circBuf_t buff;

void setUp(void)
{
    initCircBuf(&buff, STANDARD_TEST_CAPACITY);
}

void tearDown(void)
{
    freeCircBuf(&buff);
}

/* Helper functions */
void writeConsecutiveSequenceToBuffer(uint16_t start, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      writeCircBuf(&buff, start + i);
    }
} 

void readConsecutiveSequenceFromBuffer(uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      readCircBuf(&buff);
    }
} 

void assertReadingSequence(uint16_t start, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      TEST_ASSERT_EQUAL(start + i, readCircBuf(&buff));
    }
} 

uint32_t* reconstructBufferWithSize(uint32_t size)
{
  freeCircBuf(&buff);
  return initCircBuf(&buff, size);
}

/* Test cases */

void test_new_buffer_is_empty(void)
{
    // Arrange: given buffer is empty

    // Act: when buffer is read
    int32_t data = readCircBuf(&buff);

    // Assert: then 0 is returned
    TEST_ASSERT_EQUAL(NULL, data);
}

void test_single_element_in_single_element_out(void)
{
    // Arrange: given buffer has a single element
    writeCircBuf(&buff, 11);

    // Act: when buffer is read
    int32_t value = readCircBuf(&buff);

    // Assert: then the same value is returned
    TEST_ASSERT_EQUAL(11, value);
}

void test_first_element_in_first_element_out(void)
{
    // Arrange
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);

    // Act/Assert
    assertReadingSequence(20, STANDARD_TEST_CAPACITY);
}

void test_write_and_read_indices_are_independent(void)
{
    for (uint8_t i = 0; i < STANDARD_TEST_CAPACITY; i++)
    {
      // Arrange: given one element is written
      writeCircBuf(&buff, 20 + i);

      // Act: when buffer is read
      int32_t value = readCircBuf(&buff);

      // Assert: the last written element is returned
      TEST_ASSERT_EQUAL(20 + i, value);
    }
}

void test_buffer_is_clean_after_full_buffer_cycle_completed(void)
{
    // Arange: given buffer is fully written to and and then fully read from
    writeConsecutiveSequenceToBuffer(5, STANDARD_TEST_CAPACITY);
    readConsecutiveSequenceFromBuffer(STANDARD_TEST_CAPACITY);

    // Act: when buffer is read
    int32_t value = readCircBuf(&buff);

    // Assert: same behaviour as when buffer was empty
    TEST_ASSERT_EQUAL(0, value);
}

void test_buffer_is_circular(void)
{
    // Arrange: given buffer is fully written to and then fully read from
    writeConsecutiveSequenceToBuffer(5, STANDARD_TEST_CAPACITY);
    readConsecutiveSequenceFromBuffer(STANDARD_TEST_CAPACITY);

    // Arrange: given a new value is written
    writeCircBuf(&buff, 7);

    // Act: when buffer is read
    int32_t value = readCircBuf(&buff);

    // Assert: the last written element is returned
    TEST_ASSERT_EQUAL(7, value);
}

void test_no_values_overwritten_after_full(void)
{
    // Arrange: given buffer is filled to capacity
    writeConsecutiveSequenceToBuffer(5, STANDARD_TEST_CAPACITY);

    // Given: when one more element is written to buffer
    writeCircBuf(&buff, 100);

    // Assert: first element in, first element out, no overflow
    int32_t value = readCircBuf(&buff);
    TEST_ASSERT_EQUAL(5, value);
}

void test_min_capacity_when_buffer_is_created_then_buffer_empty(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act/Assert
    TEST_ASSERT_EQUAL(0, readCircBuf(&buff));
}

void test_min_capacity_when_single_element_written_to_buffer_then_same_value_is_read(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act
    writeCircBuf(&buff, 87);

    // Act/Assert
    TEST_ASSERT_EQUAL(87, readCircBuf(&buff));
}

void test_capacity_0_invalid(void)
{
    // Arrange/Act
    int32_t *abuff = reconstructBufferWithSize(0);

    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(NULL, abuff);
}

void test_capacity_higher_than_max_invalid(void)
{
    // Arrange/Act
    int32_t *abuff = reconstructBufferWithSize(MAX_BUFFER_CAPACITY+1);

    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(NULL, abuff);
}

void test_capacity_at_limit(void)
{
    // Arrange
    reconstructBufferWithSize(MAX_BUFFER_CAPACITY);

    // Act
    writeConsecutiveSequenceToBuffer(20, MAX_BUFFER_CAPACITY);

    // Assert: the return value of initCircBuf is not NULL
    assertReadingSequence(20, MAX_BUFFER_CAPACITY);
}