# //--------------------------------------------------------------------------------------------
# // Toolchain: arm-none-eabi-gcc.cmake
# // Description: ARM Cortex-M cross-compilation toolchain
# // 
# // This toolchain sets ALL compilation flags needed for embedded ARM builds.
# // Libraries using this toolchain will automatically inherit these flags.
# //--------------------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain binaries
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# =============================================================================
# MCU Configuration (can be overridden via -DARM_MCU=cortex-m0 etc.)
# =============================================================================
set(ARM_MCU "cortex-m4" CACHE STRING "ARM Cortex core type")
set(ARM_FPU "fpv4-sp-d16" CACHE STRING "FPU type (leave empty for cores without FPU)")
set(ARM_FLOAT_ABI "softfp" CACHE STRING "Float ABI: soft, softfp, or hard")

# =============================================================================
# Compiler Flags - Applied to ALL targets
# =============================================================================
# MCU-specific flags
set(MCU_FLAGS "-mcpu=${ARM_MCU} -mthumb")
if(ARM_FPU)
    set(MCU_FLAGS "${MCU_FLAGS} -mfpu=${ARM_FPU} -mfloat-abi=${ARM_FLOAT_ABI}")
endif()

# Embedded C++ flags (no exceptions, no RTTI for size and determinism)
set(EMBEDDED_CXX_FLAGS "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit")

# Size optimization
set(SIZE_FLAGS "-Os -ffunction-sections -fdata-sections")

# Warnings
set(WARN_FLAGS "-Wall -Wextra")

# Combine all flags
set(CMAKE_C_FLAGS_INIT "${MCU_FLAGS} ${SIZE_FLAGS} ${WARN_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${MCU_FLAGS} ${SIZE_FLAGS} ${WARN_FLAGS} ${EMBEDDED_CXX_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS}")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS_INIT "${MCU_FLAGS} -Wl,--gc-sections -nostdlib --specs=nosys.specs")

# Prevent CMake from test-linking (no OS to run on)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
