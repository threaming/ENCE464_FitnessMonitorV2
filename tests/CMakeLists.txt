cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

set(SRC_DIR ../target/src)
set(HAL_DIR ../target/src/hal)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(TIVAWARE_SOURCES
    ${TIVAWARE_DIR}/utils/ustdlib.c
)
set(MOCK_DIR mocks)

# Adding Unity tests
add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circBufT.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/ADC_read.c)
add_unity_test(test_adc_hal test_adc_hal.c ${HAL_DIR}/adc_hal_tiva.c)
add_unity_test(test_display_manager test_display_manager.c ${SRC_DIR}/display_manager.c ${TIVAWARE_SOURCES})

# Include directories
target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})
target_include_directories(test_adc PRIVATE ${SRC_DIR})
target_include_directories(test_adc PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc PRIVATE ${MOCK_DIR})
target_include_directories(test_adc_hal PRIVATE ${SRC_DIR})
target_include_directories(test_adc_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc_hal PRIVATE ${MOCK_DIR})
target_include_directories(test_display_manager PRIVATE ${SRC_DIR})
target_include_directories(test_display_manager PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_display_manager PRIVATE ${HAL_DIR})
target_include_directories(test_display_manager PRIVATE ${MOCK_DIR})
