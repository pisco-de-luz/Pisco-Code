set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# MCU-specific flags (can be overridden via -DARM_MCU_FLAGS="...")
# Default to Cortex-M4 with FPU (common for STM32F4)
if(NOT DEFINED ARM_MCU_FLAGS)
    set(ARM_MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp")
endif()

# Apply MCU flags globally so all targets (including libraries) get them
set(CMAKE_C_FLAGS_INIT "${ARM_MCU_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${ARM_MCU_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${ARM_MCU_FLAGS}")

# Tell CMake not to link during try_compile
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
