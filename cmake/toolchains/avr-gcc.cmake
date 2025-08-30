# Toolchain file for AVR cross-compilation with ATmega328P

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Toolchain binaries
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# CPU frequency
set(F_CPU "16000000UL" CACHE STRING "CPU frequency")

# Common flags
set(MCU_FLAGS "-mmcu=atmega328p")
set(OPT_FLAGS "-Os")
set(WARN_FLAGS "-Wall -Wextra")

# Global flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -DF_CPU=${F_CPU} ${OPT_FLAGS} ${WARN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -DF_CPU=${F_CPU} ${OPT_FLAGS} ${WARN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -nodefaultlibs -Wl,--gc-sections" CACHE STRING "" FORCE)

# Disable standard C library linkage (optional if using bare metal)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

