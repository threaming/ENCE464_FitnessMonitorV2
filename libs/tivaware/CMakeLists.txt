set(TIVAWARE_MCU TM4C123GH6PM CACHE STRING "MCU part number")

add_library(tivaware STATIC ../../startup.c utils/ustdlib.c)
target_link_libraries(
    tivaware PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/driverlib/gcc/libdriver.a
)
target_include_directories(tivaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(tivaware PUBLIC "PART_${TIVAWARE_MCU}")
