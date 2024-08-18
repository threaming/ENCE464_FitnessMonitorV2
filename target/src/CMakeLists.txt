add_tiva_executable(fitness-tracker
    accl_manager.c
    new_goal_reader.c
    button_manager.c
    buttons4.c
    circBufT.c
    circBufV.c
    display_manager.c
    display_helpers.c
    serial_sender.c
    step_counter_main.c
    switches.c
    hal/adc_hal_tiva.c
    hal/i2c_hal_tiva.c
    hal/imu_hal_tiva.c
    hal/display_hal_tiva.c
    device_state.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
