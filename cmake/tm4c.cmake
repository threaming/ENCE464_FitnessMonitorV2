set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(WIN32)
    set(
        ENCE_TOOLCHAIN_PATH "C:/ence461/tool-chain"
        CACHE STRING "Path to ENCE toolchain directory"
    )

    if(ENCE_TOOLCHAIN_PATH)
        if(NOT EXISTS "${ENCE_TOOLCHAIN_PATH}")
            message(FATAL_ERROR "Does not exist: ${ENCE_TOOLCHAIN_PATH}")
        endif()

        set(
            GCC_ARM_HINTS
            HINTS "${ENCE_TOOLCHAIN_PATH}/gcc-arm-none-eabi-9-2019-q4/bin"
        )
        set(
            OPENOCD_HINTS
            HINTS "${ENCE_TOOLCHAIN_PATH}/OpenOCD-0.10.0/bin"
        )
        find_program(
            CMAKE_MAKE_PROGRAM make
            HINTS "${ENCE_TOOLCHAIN_PATH}/msys64/usr/bin"
            REQUIRED
        )
    endif()
endif()

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++ ${GCC_ARM_HINTS} REQUIRED)
find_program(AS arm-none-eabi-as ${GCC_ARM_HINTS} REQUIRED)
find_program(AR arm-none-eabi-ar ${GCC_ARM_HINTS} REQUIRED)
find_program(OBJCOPY arm-none-eabi-objcopy ${GCC_ARM_HINTS} REQUIRED)
find_program(OBJDUMP arm-none-eabi-objdump ${GCC_ARM_HINTS} REQUIRED)
find_program(SIZE arm-none-eabi-size ${GCC_ARM_HINTS} REQUIRED)

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

find_program(OPENOCD openocd ${OPENOCD_HINTS} REQUIRED)
message(STATUS "Found OpenOCD: ${OPENOCD}")

find_program(GDB arm-none-eabi-gdb ${GCC_ARM_HINTS})
if(NOT GDB)
    find_program(GDB gdb-multiarch ${GCC_ARM_HINTS} REQUIRED)
endif()
message(STATUS "Found GDB: ${GDB}")
