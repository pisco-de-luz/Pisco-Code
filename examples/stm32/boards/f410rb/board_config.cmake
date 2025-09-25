# Board-specific configuration for STM32F410RB

# MCU Configuration
set(BOARD_CPU_FLAGS -mcpu=cortex-m4 -mthumb)
set(BOARD_FPU_FLAGS -mfpu=fpv4-sp-d16 -mfloat-abi=softfp)

# Compiler definitions
set(BOARD_MCU_DEFINES
    STM32F410Rx
    STM32F410xx
)

# File names
set(BOARD_HAL_LED_FILE "hal_led_nucleo_f410rb.cpp")
set(BOARD_STARTUP_FILE "startup_stm32f410rx.s")
set(BOARD_SYSTEM_FILE "system_stm32f4xx.c") 
set(BOARD_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/linker/stm32f410rb.ld")

# OpenOCD Configuration
set(BOARD_OPENOCD_INTERFACE "interface/stlink.cfg")
set(BOARD_OPENOCD_TARGET "target/stm32f4x.cfg")