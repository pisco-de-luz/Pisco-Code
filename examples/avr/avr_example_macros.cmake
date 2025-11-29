# //--------------------------------------------------------------------------------------------
# // File:        avr_example_macros.cmake
# // Description: Reusable macro to define AVR examples
# // Author:      Andre Viegas
# //
# // NOTE: This example acts as a "consumer" of the Pisco Code library.
# // All compilation flags come from the toolchain (cmake/toolchains/avr-gcc.cmake).
# // This file only handles example-specific setup: sources, hex generation, upload target.
# //--------------------------------------------------------------------------------------------

function(add_avr_example TARGET_NAME)
    set(BOARD_DIR  ${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD})
    
    # Include board-specific configuration (HAL file, upload config)
    include(${BOARD_DIR}/board_config.cmake)
    
    # Source files for this example
    set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/avr_systick.cpp
        ${BOARD_DIR}/${BOARD_HAL_LED_FILE}
    )

    # Create executable
    add_executable(${TARGET_NAME} ${SRC_FILES})

    # Include directories
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${BOARD_DIR}
        ${CMAKE_SOURCE_DIR}/include
    )

    # Link to Pisco Code library (just like any consumer would)
    target_link_libraries(${TARGET_NAME} PRIVATE pisco_code::bare)

    # Linker configuration (map file)
    target_link_options(${TARGET_NAME} PRIVATE
        -Wl,-Map=$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.map
        -Wl,--cref
    )

    # Link required runtime libraries
    target_link_options(${TARGET_NAME} PRIVATE -Wl,--start-group)
    target_link_libraries(${TARGET_NAME} PRIVATE c gcc m)
    target_link_options(${TARGET_NAME} PRIVATE -Wl,--end-group)

    # Post-build: show size and generate hex file
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND avr-size -C --mcu=${AVR_MCU} $<TARGET_FILE:${TARGET_NAME}>
        COMMAND avr-objcopy -O ihex -R .eeprom $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
        COMMENT "Size and artifacts"
    )

    # Convenience target for hex file
    add_custom_target(${TARGET_NAME}_hex ALL DEPENDS ${TARGET_NAME})

    # Build avrdude command based on programmer type
    # USBasp doesn't need port/baud, serial programmers (arduino, stk500v1) do
    set(AVRDUDE_CMD avrdude -v -p ${AVR_MCU} -c ${BOARD_UPLOAD_PROGRAMMER})
    if(DEFINED BOARD_UPLOAD_PORT AND NOT BOARD_UPLOAD_PROGRAMMER STREQUAL "usbasp")
        list(APPEND AVRDUDE_CMD -P ${BOARD_UPLOAD_PORT})
    endif()
    if(DEFINED BOARD_UPLOAD_BAUD AND NOT BOARD_UPLOAD_PROGRAMMER STREQUAL "usbasp")
        list(APPEND AVRDUDE_CMD -b ${BOARD_UPLOAD_BAUD})
    endif()
    list(APPEND AVRDUDE_CMD -U flash:w:${TARGET_NAME}.hex:i)

    # Upload target using avrdude
    add_custom_target(${TARGET_NAME}_upload
        COMMAND ${CMAKE_COMMAND} -E echo "Programming with avrdude..."
        COMMAND ${AVRDUDE_CMD}
        DEPENDS ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        USES_TERMINAL
        COMMENT "Uploading ${TARGET_NAME} via avrdude"
    )
endfunction()
