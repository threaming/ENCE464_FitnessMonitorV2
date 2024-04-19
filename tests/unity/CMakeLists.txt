# CMake support for Unity test framework
# Harry Mander 2024
# Unity is copyright of Mike Karlesky, Mark VanderVoord, Greg Williams

find_program(RUBY NAMES ruby ruby3.0 ruby2.0 REQUIRED)

set(UNITY_AUTO_DIR ${CMAKE_CURRENT_LIST_DIR}/auto PARENT_SCOPE)

add_library(unity STATIC unity.c)
target_compile_definitions(unity PUBLIC UNITY_USE_COMMAND_LINE_ARGS=1)
target_include_directories(unity PUBLIC .)

function(add_unity_test TARGET MAIN_TEST_FILE)
    set(runner_src ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.runner.c)
    set(UNITY_AUTO
        ${UNITY_AUTO_DIR}/generate_test_runner.rb
        ${UNITY_AUTO_DIR}/run_test.erb
        ${UNITY_AUTO_DIR}/type_sanitizer.rb
        ${UNITY_AUTO_DIR}/yaml_helper.rb
    )
    add_custom_command(
        OUTPUT ${runner_src}
        COMMENT "Generating test runner for ${MAIN_TEST_FILE}"
        COMMAND
            ${RUBY} ${UNITY_AUTO_DIR}/generate_test_runner.rb
            --cmdline_args=1
            ${MAIN_TEST_FILE} ${runner_src}
        DEPENDS ${UNITY_AUTO} ${MAIN_TEST_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    add_executable(${TARGET} ${runner_src} ${MAIN_TEST_FILE} ${ARGN})
    target_link_libraries(${TARGET} PRIVATE unity)
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME ${TARGET} COMMAND ${TARGET})
endfunction()
