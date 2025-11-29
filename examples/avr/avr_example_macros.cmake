# //--------------------------------------------------------------------------------------------
# // File:        avr_example_macros.cmake
# // Description: Reusable macro to define AVR examples
# // Author:      Andre Viegas
# //
# // NOTE: This example acts as a "consumer" of the Pisco Code library.
# // All compilation flags come from the toolchain (cmake/toolchains/avr-gcc.cmake).
# // This file only handles example-specific setup: sources, hex generation, upload target.
# //--------------------------------------------------------------------------------------------

function(add_avr_example EX_NAME BOARD BOARD_CANON)
    set(HAL_CPP "boards/${BOARD}/hal_led_${BOARD_CANON}.cpp")
    set(HEX_FILE "${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex")
    set(MAP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.map")

    find_program(AVR_OBJCOPY NAMES avr-objcopy objcopy REQUIRED)
    find_program(AVR_SIZE NAMES avr-size size REQUIRED)

    # Create executable
    add_executable(${EX_NAME}
        ${EX_NAME}.cpp
        $<TARGET_OBJECTS:avr_shared>
        boards/${BOARD}/board_config.hpp
        ${HAL_CPP}
    )

    # Include directories
    target_include_directories(${EX_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
        ${CMAKE_SOURCE_DIR}/include
    )

    # Link to Pisco Code library (just like any consumer would)
    target_link_libraries(${EX_NAME} PRIVATE pisco_code::bare)

    # Linker options (map file)
    target_link_options(${EX_NAME} PRIVATE
        -Wl,-Map=${MAP_FILE}
        -Wl,--cref
    )

    # Link required runtime libraries
    target_link_options(${EX_NAME} PRIVATE -Wl,--start-group)
    target_link_libraries(${EX_NAME} PRIVATE c gcc m)
    target_link_options(${EX_NAME} PRIVATE -Wl,--end-group)

    # Generate HEX file
    add_custom_command(
        OUTPUT ${HEX_FILE}
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${EX_NAME}> ${HEX_FILE}
        DEPENDS ${EX_NAME}
        COMMENT "Generating ${EX_NAME}.hex"
    )

    add_custom_target(${EX_NAME}_hex ALL DEPENDS ${HEX_FILE})

    # Show size after build
    add_custom_command(TARGET ${EX_NAME} POST_BUILD
        COMMAND ${AVR_SIZE} -C --mcu=${AVR_MCU} $<TARGET_FILE:${EX_NAME}>
        COMMENT "Size report"
    )

    # Upload configuration
    set(AVR_UPLOAD_PROGRAMMER "usbasp" CACHE STRING "Programmer used by avrdude")
    set(AVR_UPLOAD_PORT "" CACHE STRING "Serial port or USB device (if needed)")
    set(AVR_UPLOAD_BAUD "" CACHE STRING "Baud rate for serial-based upload")

    # Build upload command arguments
    unset(UPLOAD_PORT_ARG)
    unset(UPLOAD_BAUD_ARG)
    if(AVR_UPLOAD_PROGRAMMER STREQUAL "arduino" OR AVR_UPLOAD_PROGRAMMER STREQUAL "stk500v1")
        if(AVR_UPLOAD_PORT)
            list(APPEND UPLOAD_PORT_ARG -P ${AVR_UPLOAD_PORT})
        endif()
        if(AVR_UPLOAD_BAUD)
            list(APPEND UPLOAD_BAUD_ARG -b ${AVR_UPLOAD_BAUD})
        endif()
    endif()

    # Upload target using avrdude
    add_custom_target(${EX_NAME}_upload
        COMMAND ${CMAKE_COMMAND} -E echo "Flashing ${HEX_FILE} using ${AVR_UPLOAD_PROGRAMMER}..."
        COMMAND avrdude -v -p ${AVR_MCU} -c ${AVR_UPLOAD_PROGRAMMER}
                ${UPLOAD_PORT_ARG} ${UPLOAD_BAUD_ARG}
                -U flash:w:${HEX_FILE}:i
        DEPENDS ${EX_NAME}_hex
        USES_TERMINAL
        COMMENT "Uploading ${EX_NAME}.hex"
    )
endfunction()

