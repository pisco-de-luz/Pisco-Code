# //--------------------------------------------------------------------------------------------
# // Module:      avr_outputs.cmake
# // Description: Generates .hex and .bin files from AVR executables
# // Usage:       avr_add_outputs(target_name)
# // Author:      Andre Viegas
# // Date:        2025.11
# // Version:     2.0
# //--------------------------------------------------------------------------------------------
# 
# This module provides a function to generate Intel HEX and binary files
# from an AVR executable target. Used by the AVR example build system.
# //--------------------------------------------------------------------------------------------

find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
find_program(AVR_SIZE avr-size REQUIRED)

# =============================================================================
# avr_add_outputs(TARGET)
# =============================================================================
# Adds custom targets to generate .hex file and display size information
# for the specified AVR executable target.
#
# Usage:
#   add_executable(my_avr_app main.cpp)
#   avr_add_outputs(my_avr_app)
#
function(avr_add_outputs TARGET)
    # Generate .hex file
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom
                $<TARGET_FILE:${TARGET}>
                $<TARGET_FILE_DIR:${TARGET}>/${TARGET}.hex
        COMMENT "Generating ${TARGET}.hex"
    )

    # Display size information
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${AVR_SIZE} --mcu=${AVR_MCU} -C $<TARGET_FILE:${TARGET}>
        COMMENT "Size and artifacts"
    )

    # Create a named target for the hex file (for explicit builds)
    add_custom_target(${TARGET}_hex
        DEPENDS ${TARGET}
        COMMENT "Building ${TARGET} hex file"
    )
endfunction()
