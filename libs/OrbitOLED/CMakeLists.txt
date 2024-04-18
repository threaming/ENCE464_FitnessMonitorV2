file(GLOB lib_OrbitOled_SRC lib_OrbitOled/*.c)
add_library(OrbitOLED STATIC OrbitOLEDInterface.c ${lib_OrbitOled_SRC})
target_include_directories(OrbitOLED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(OrbitOLED PUBLIC tivaware)
