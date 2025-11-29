# //--------------------------------------------------------------------------------------------
# // Toolchain: avr-gcc.cmake
# // Description: AVR cross-compilation toolchain for ATmega microcontrollers
# // 
# // This toolchain sets ALL compilation flags needed for embedded AVR builds.
# // Libraries using this toolchain will automatically inherit these flags.
# //--------------------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Toolchain binaries
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# =============================================================================
# MCU Configuration (can be overridden via -DAVR_MCU=atmega168 etc.)
# =============================================================================
set(AVR_MCU "atmega328p" CACHE STRING "AVR MCU type")
set(AVR_F_CPU "16000000UL" CACHE STRING "CPU frequency in Hz")

# =============================================================================
# Compiler Flags - Applied to ALL targets
# =============================================================================
# MCU-specific
set(MCU_FLAGS "-mmcu=${AVR_MCU} -DF_CPU=${AVR_F_CPU}")

# Embedded C++ flags (no exceptions, no RTTI for size)
set(EMBEDDED_CXX_FLAGS "-fno-exceptions -fno-rtti -fno-threadsafe-statics")

# Size optimization
set(SIZE_FLAGS "-Os -ffunction-sections -fdata-sections")

# Warnings
set(WARN_FLAGS "-Wall -Wextra")

# Combine all flags
set(CMAKE_C_FLAGS_INIT "${MCU_FLAGS} ${SIZE_FLAGS} ${WARN_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${MCU_FLAGS} ${SIZE_FLAGS} ${WARN_FLAGS} ${EMBEDDED_CXX_FLAGS}")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS_INIT "${MCU_FLAGS} -Wl,--gc-sections")

# Prevent CMake from test-linking (no OS to run on)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

