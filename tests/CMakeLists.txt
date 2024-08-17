cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

set(SRC_DIR ../target/src)
set(HAL_DIR ../target/src/hal)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)
#add_unity_test(test_example test_example.c)
#add_unity_test(test_fff test_fff.c)
add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circBufT.c)
add_unity_test(test_new_goal_reader test_new_goal_reader.c ${SRC_DIR}/new_goal_reader.c ${SRC_DIR}/device_state.c)
add_unity_test(new_goal_covering_test new_goal_covering_test.c ${SRC_DIR}/new_goal_reader.c ${SRC_DIR}/circBufT.c ${HAL_DIR}/adc_hal_tiva.c ${SRC_DIR}/button_manager.c ${SRC_DIR}/display_manager.c ${SRC_DIR}/device_state.c)
add_unity_test(test_adc_hal test_adc_hal.c ${HAL_DIR}/adc_hal_tiva.c)
target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})

target_include_directories(test_new_goal_reader PRIVATE ${SRC_DIR})
target_include_directories(test_new_goal_reader PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_new_goal_reader PRIVATE ${MOCK_DIR})

target_include_directories(new_goal_covering_test PRIVATE ${SRC_DIR})
target_include_directories(new_goal_covering_test PRIVATE ${TIVAWARE_DIR})
target_include_directories(new_goal_covering_test PRIVATE ${MOCK_DIR})

target_include_directories(test_adc_hal PRIVATE ${SRC_DIR})
target_include_directories(test_adc_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc_hal PRIVATE ${MOCK_DIR})