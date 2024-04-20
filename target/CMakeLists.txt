cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(FitnessTracker LANGUAGES C)

include(cmake/program.cmake)
add_subdirectory(libs/OrbitOLED EXCLUDE_FROM_ALL)
add_subdirectory(libs/tivaware EXCLUDE_FROM_ALL)
add_subdirectory(libs/freertos EXCLUDE_FROM_ALL)
add_subdirectory(src)

option(BUILD_DEMOS "Build demo applications" ON)
if(BUILD_DEMOS)
    add_subdirectory(demos)
endif()
