set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(WIN32)
    set(
        ENCE_TOOLCHAIN_PATH "C:/ence464/tool-chain"
        CACHE STRING "Path to ENCE toolchain directory"
    )

    if(ENCE_TOOLCHAIN_PATH)
        if(NOT EXISTS "${ENCE_TOOLCHAIN_PATH}")
            message(FATAL_ERROR "Does not exist: ${ENCE_TOOLCHAIN_PATH}")
        endif()

        set(
            GCC_ARM_HINTS
            HINTS "${ENCE_TOOLCHAIN_PATH}/gcc-arm-none-eabi-13.2/bin"
        )
        set(
            OPENOCD_HINTS
            HINTS "${ENCE_TOOLCHAIN_PATH}/OpenOCD-0.10.0/bin"
        )
        set(
            CMAKE_SYSROOT ${ENCE_TOOLCHAIN_PATH}/gcc-arm-none-eabi-13.2
            CACHE PATH "Path to sysroot"
        )
    endif()
elseif(LINUX)
    set(CMAKE_SYSROOT /usr/lib/arm-none-eabi CACHE PATH "Path to sysroot")
endif()

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++ ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_AS arm-none-eabi-as ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_AR arm-none-eabi-ar ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_OBJDUMP arm-none-eabi-objdump ${GCC_ARM_HINTS} REQUIRED)
find_program(CMAKE_SIZE arm-none-eabi-size ${GCC_ARM_HINTS} REQUIRED)

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

set(SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/scripts)
set(
    CMAKE_EXE_LINKER_FLAGS
    "-T${SCRIPTS_DIR}/link.ld"
    CACHE INTERNAL "Linker flags"
)

find_program(OPENOCD openocd ${OPENOCD_HINTS} REQUIRED)
message(STATUS "Found OpenOCD: ${OPENOCD}")

find_program(
    GDB
    NAMES arm-none-eabi-gdb gdb-multiarch
    ${GCC_ARM_HINTS}
    REQUIRED
)
message(STATUS "Found GDB: ${GDB}")

function(add_tiva_executable TARGET_NAME)
    add_executable(
        ${TARGET_NAME} ${PROJECT_SOURCE_DIR}/startup.c ${ARGN}
    )
endfunction()
