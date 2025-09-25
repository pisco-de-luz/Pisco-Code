# //--------------------------------------------------------------------------------------------
# // File:        stm32_example_macros.cmake
# // Description: Reusable macro to define STM32 examples in a uniform way
# // Author:      Andre Viegas
# //--------------------------------------------------------------------------------------------

enable_language(ASM)

function(add_stm32_example TARGET_NAME)
    set(BOARD_DIR  ${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD})
    set(VENDOR_DIR ${BOARD_DIR}/vendor)
    
    # Include board-specific configuration
    include(${BOARD_DIR}/board_config.cmake)
    
    # Generic source files (all STM32 boards need these)
    set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32_systick.cpp
        ${BOARD_DIR}/${BOARD_HAL_LED_FILE}
        ${BOARD_DIR}/hardfault.c
        ${BOARD_DIR}/libc_init_stub.c
        ${VENDOR_DIR}/${BOARD_STARTUP_FILE}
        ${VENDOR_DIR}/${BOARD_SYSTEM_FILE} 
    )

    add_executable(${TARGET_NAME} ${SRC_FILES})

    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${BOARD_DIR}
    )
    target_include_directories(${TARGET_NAME} SYSTEM PRIVATE
        ${VENDOR_DIR}
    )

    # Use board-specific definitions
    target_compile_definitions(${TARGET_NAME} PRIVATE
        ${BOARD_MCU_DEFINES}
    )

    # Generic STM32 embedded flags
    set(STM32_EMBEDDED_COMMON_FLAGS
        -fno-exceptions
    )
    set(STM32_EMBEDDED_CXX_FLAGS
        -fno-rtti
        -fno-use-cxa-atexit
        -fno-threadsafe-statics
    )
    
    set(STM32_LINK_FLAGS
        -nostdlib
        -Wl,--gc-sections
        -Wl,-Map=$<TARGET_FILE_DIR:${TARGET_NAME}>/${TARGET_NAME}.map
        -T${BOARD_LINKER_SCRIPT}
        -Wl,--start-group
        -Wl,--end-group
    )

    # Feed board-specific flags to the library
    target_compile_options(pisco_mcu_flags INTERFACE ${BOARD_CPU_FLAGS} ${BOARD_FPU_FLAGS})
    target_link_options(pisco_mcu_flags    INTERFACE ${BOARD_CPU_FLAGS} ${BOARD_FPU_FLAGS})

    target_compile_options(${TARGET_NAME} PRIVATE
        ${BOARD_CPU_FLAGS}
        ${BOARD_FPU_FLAGS}
        ${STM32_EMBEDDED_COMMON_FLAGS}
        $<$<COMPILE_LANGUAGE:CXX>:${STM32_EMBEDDED_CXX_FLAGS}>
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
    )

    target_link_options(${TARGET_NAME} PRIVATE
        ${BOARD_CPU_FLAGS}
        ${BOARD_FPU_FLAGS}
        ${STM32_LINK_FLAGS}
        ${STM32_EMBEDDED_COMMON_FLAGS}
        $<$<COMPILE_LANGUAGE:CXX>:${STM32_EMBEDDED_CXX_FLAGS}>
    )

    target_link_options(${TARGET_NAME} PRIVATE -Wl,--start-group)
    target_link_libraries(${TARGET_NAME} PRIVATE pisco_code::core_bare gcc)
    target_link_options(${TARGET_NAME} PRIVATE -Wl,--end-group)

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin
        COMMENT "Size and artifacts"
    )

    add_custom_target(${TARGET_NAME}_upload
        COMMAND ${CMAKE_COMMAND} -E echo "Programming with OpenOCD..."
        COMMAND openocd -f ${BOARD_OPENOCD_INTERFACE} -f ${BOARD_OPENOCD_TARGET}
                -c "program $<TARGET_FILE:${TARGET_NAME}> verify reset exit"
        DEPENDS ${TARGET_NAME}
        USES_TERMINAL
        COMMENT "Uploading ${TARGET_NAME} via OpenOCD"
    )
endfunction()