# //--------------------------------------------------------------------------------------------
# // File:        stm32_example_macros.cmake
# // Description: Reusable macro to define STM32 examples
# // Author:      Andre Viegas
# //
# // NOTE: This example acts as a "consumer" of the Pisco Code library.
# // All compilation flags come from the toolchain (cmake/toolchains/arm-none-eabi-gcc.cmake).
# // This file only handles example-specific setup: sources, linker script, upload target.
# //--------------------------------------------------------------------------------------------

function(add_stm32_example TARGET_NAME)
    set(BOARD_DIR  ${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD})
    set(VENDOR_DIR ${BOARD_DIR}/vendor)
    
    # Include board-specific configuration (linker script, defines, upload config)
    include(${BOARD_DIR}/board_config.cmake)
    
    # Source files for this example
    set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32_systick.cpp
        ${BOARD_DIR}/${BOARD_HAL_LED_FILE}
        ${BOARD_DIR}/hardfault.c
        ${BOARD_DIR}/libc_init_stub.c
        ${VENDOR_DIR}/${BOARD_STARTUP_FILE}
        ${VENDOR_DIR}/${BOARD_SYSTEM_FILE} 
    )

    # Create executable
    add_executable(${TARGET_NAME} ${SRC_FILES})

    # Include directories
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${BOARD_DIR}
    )
    target_include_directories(${TARGET_NAME} SYSTEM PRIVATE
        ${VENDOR_DIR}
    )

    # Board-specific definitions (e.g., STM32F410Rx)
    target_compile_definitions(${TARGET_NAME} PRIVATE
        ${BOARD_MCU_DEFINES}
    )

    # ASM files need special handling
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
    )

    # Linker configuration (board-specific linker script)
    target_link_options(${TARGET_NAME} PRIVATE
        -Wl,-Map=$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.map
        -T${BOARD_LINKER_SCRIPT}
    )

    # Link to Pisco Code library (just like any consumer would)
    target_link_libraries(${TARGET_NAME} PRIVATE pisco_code::bare)

    # Link required runtime libraries
    target_link_options(${TARGET_NAME} PRIVATE -Wl,--start-group)
    target_link_libraries(${TARGET_NAME} PRIVATE gcc)
    target_link_options(${TARGET_NAME} PRIVATE -Wl,--end-group)

    # Post-build: show size and generate hex/bin files
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin
        COMMENT "Size and artifacts"
    )

    # Upload target using OpenOCD
    add_custom_target(${TARGET_NAME}_upload
        COMMAND ${CMAKE_COMMAND} -E echo "Programming with OpenOCD..."
        COMMAND openocd -f ${BOARD_OPENOCD_INTERFACE} -f ${BOARD_OPENOCD_TARGET}
                -c "program $<TARGET_FILE:${TARGET_NAME}> verify reset exit"
        DEPENDS ${TARGET_NAME}
        USES_TERMINAL
        COMMENT "Uploading ${TARGET_NAME} via OpenOCD"
    )
endfunction()