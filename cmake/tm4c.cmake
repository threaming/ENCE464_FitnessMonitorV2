set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(AS arm-none-eabi-as)
set(AR arm-none-eabi-ar)
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

set(
    CMAKE_C_FLAGS
        "-mcpu=cortex-m4 \
        -mthumb \
        -specs=nano.specs \
        -specs=nosys.specs \
        -mfloat-abi=hard \
        -mfpu=fpv4-sp-d16"
    CACHE INTERNAL "C compiler flags"
)

set(SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/../scripts)
set(
    CMAKE_EXE_LINKER_FLAGS
    "-T${SCRIPTS_DIR}/link.ld"
    CACHE INTERNAL "Linker flags"
)

find_program(OPENOCD openocd REQUIRED)
message(STATUS "Found OpenOCD: ${OPENOCD}")

find_program(GDB arm-none-eabi-gdb)
if(NOT GDB)
    find_program(GDB gdb-multiarch REQUIRED)
endif()
message(STATUS "Found GDB: ${GDB}")
