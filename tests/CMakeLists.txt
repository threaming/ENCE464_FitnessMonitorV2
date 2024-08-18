cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)
set(TIVAWARE_MCU TM4C123GH6PM CACHE STRING "MCU part number")
enable_testing()
add_subdirectory(unity)

set(TARGET_DIR ../target)
set(SRC_DIR ../target/src)
set(HAL_DIR ../target/src/hal)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(RTOS_DIR ../target/libs/freertos/include)
set(TIVAWARE_SOURCES
    ${TIVAWARE_DIR}/utils/ustdlib.c
)
set(MOCK_DIR mocks)

# Adding Unity tests
add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circBufT.c)
add_unity_test(test_adc test_adc.c ${SRC_DIR}/ADC_read.c)
add_unity_test(test_adc_hal test_adc_hal.c ${HAL_DIR}/adc_hal_tiva.c)
add_unity_test(test_i2c_hal test_i2c_hal.c ${HAL_DIR}/i2c_hal_tiva.c)
add_unity_test(test_imu_hal test_imu_hal.c ${HAL_DIR}/imu_hal_tiva.c)
add_unity_test(test_display_manager test_display_manager.c ${SRC_DIR}/display_manager.c ${SRC_DIR}/display_helpers.c ${TIVAWARE_SOURCES})
add_unity_test(test_accl_manager test_accl_manager.c ${SRC_DIR}/accl_manager.c)
add_unity_test(test_display_hal test_display_hal.c ${HAL_DIR}/display_hal_tiva.c ${SRC_DIR}/display_helpers.c ${TIVAWARE_SOURCES})
add_unity_test(test_temp_hal test_temp_hal.c ${HAL_DIR}/temp_hal_tiva.c)
add_unity_test(test_temp_manager test_temp_manager.c ${SRC_DIR}/temp_manager.c)


# Include directories
target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})
target_include_directories(test_adc PRIVATE ${SRC_DIR})
target_include_directories(test_adc PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc PRIVATE ${MOCK_DIR})
target_include_directories(test_adc_hal PRIVATE ${SRC_DIR})
target_include_directories(test_adc_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc_hal PRIVATE ${MOCK_DIR})

target_include_directories(test_i2c_hal PRIVATE ${SRC_DIR})
target_include_directories(test_i2c_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_i2c_hal PRIVATE ${MOCK_DIR})
target_compile_definitions(test_i2c_hal PUBLIC "PART_${TIVAWARE_MCU}")

target_include_directories(test_imu_hal PRIVATE ${SRC_DIR})
target_include_directories(test_imu_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_imu_hal PRIVATE ${MOCK_DIR})
target_include_directories(test_imu_hal PRIVATE ${HAL_DIR})

target_include_directories(test_display_manager PRIVATE ${SRC_DIR})
target_include_directories(test_display_manager PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_display_manager PRIVATE ${HAL_DIR})
target_include_directories(test_display_manager PRIVATE ${MOCK_DIR})

target_include_directories(test_accl_manager PRIVATE ${SRC_DIR})
target_include_directories(test_accl_manager PRIVATE ${MOCK_DIR})
target_include_directories(test_accl_manager PRIVATE ${HAL_DIR})

target_include_directories(test_display_hal PRIVATE ${SRC_DIR})
target_include_directories(test_display_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_display_hal PRIVATE ${MOCK_DIR})
target_include_directories(test_display_hal PRIVATE ${HAL_DIR})

target_include_directories(test_temp_hal PRIVATE ${SRC_DIR})
target_include_directories(test_temp_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_temp_hal PRIVATE ${MOCK_DIR})
target_include_directories(test_temp_hal PRIVATE ${HAL_DIR})

target_compile_definitions(test_temp_manager PUBLIC "UNIT_TESTING")
target_include_directories(test_temp_manager PRIVATE ${SRC_DIR})
target_include_directories(test_temp_manager PRIVATE ${TARGET_DIR})
target_include_directories(test_temp_manager PRIVATE ${RTOS_DIR})
target_include_directories(test_temp_manager PRIVATE ../target/libs/freertos/portable/GCC/ARM_CM4F)
target_include_directories(test_temp_manager PRIVATE ${MOCK_DIR})
target_include_directories(test_temp_manager PRIVATE ${HAL_DIR})