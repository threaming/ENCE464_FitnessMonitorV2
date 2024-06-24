cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

add_unity_test(test_example test_example.c)
add_unity_test(test_fff test_fff.c)