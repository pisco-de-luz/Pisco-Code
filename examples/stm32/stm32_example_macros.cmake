# //--------------------------------------------------------------------------------------------
# // File:        stm32_example_macros.cmake
# // Description: Reusable macro to define STM32 examples in a uniform way
# // Author:      Andre Viegas
# //--------------------------------------------------------------------------------------------

enable_language(ASM)

function(add_stm32_example EX_NAME BOARD BOARD_CANON)
  set(HAL_CPP "boards/${BOARD}/hal_led_${BOARD_CANON}.cpp")
  set(STARTUP_ASM "boards/${BOARD}/vendor/startup_stm32f410rx.s")
  set(SYSTEM_C   "boards/${BOARD}/vendor/system_stm32f4xx.c")
  set(HARDFILE_C "boards/${BOARD}/hardfault.c")
  set(STUB_C     "boards/${BOARD}/libc_init_stub.c")

  # Optional linker script (set per board or globally)
  set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/linker/stm32f410rb.ld" CACHE FILEPATH "STM32 linker script")

  # Toolchain flags
  set(STM32_CPU_FLAGS  "-mcpu=cortex-m4" "-mthumb")
  set(STM32_FPU_FLAGS  "-mfpu=fpv4-sp-d16" "-mfloat-abi=softfp")

  # Output filenames
  set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.bin)
  set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex)
  set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.map)

  find_program(CMAKE_OBJCOPY NAMES arm-none-eabi-objcopy REQUIRED)
  find_program(CMAKE_SIZE    NAMES arm-none-eabi-size)

  add_executable(${EX_NAME}
    ${EX_NAME}.cpp
    ${HAL_CPP}
    ${SYSTEM_C}
    ${STARTUP_ASM}
    ${HARDFILE_C}
    ${STUB_C}
    $<TARGET_OBJECTS:stm32_shared>
  )

  target_include_directories(${EX_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/vendor
    ${CMAKE_SOURCE_DIR}/include
  )

  target_compile_definitions(${EX_NAME} PRIVATE
    STM32F410Rx STM32F410xx
  )

  target_compile_options(${EX_NAME} PRIVATE
    ${STM32_CPU_FLAGS}
    ${STM32_FPU_FLAGS}
    -fno-exceptions
    -fno-rtti
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
  )

  target_link_options(${EX_NAME} PRIVATE
    ${STM32_CPU_FLAGS}
    ${STM32_FPU_FLAGS}
    -T${LINKER_SCRIPT}
    -nostartfiles
    -nostdlib
    -Wl,--gc-sections
    -Wl,-Map=${MAP_FILE}
    -Wl,--cref
    -Wl,--start-group
  )

  target_link_libraries(${EX_NAME} PRIVATE
    gcc
    pisco_code::core_bare
  )

  target_link_options(${EX_NAME} PRIVATE -Wl,--end-group)

  # BIN + HEX + size
  add_custom_command(TARGET ${EX_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EX_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EX_NAME}> ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${EX_NAME}> ${HEX_FILE}
    COMMENT "Generating .bin/.hex and size report for ${EX_NAME}"
  )

  # Optional ST-Link upload via OpenOCD
  set(OPENOCD_INTERFACE "interface/stlink.cfg" CACHE STRING "OpenOCD interface cfg")
  set(OPENOCD_TARGET    "target/stm32f4x.cfg"  CACHE STRING "OpenOCD target cfg")

  add_custom_target(${EX_NAME}_upload
    COMMAND ${CMAKE_COMMAND} -E echo "Uploading ${EX_NAME} via OpenOCD..."
    COMMAND openocd -f ${OPENOCD_INTERFACE} -f ${OPENOCD_TARGET}
      -c "program $<TARGET_FILE:${EX_NAME}> verify reset exit"
    DEPENDS ${EX_NAME}
    USES_TERMINAL
    COMMENT "Flashing ${EX_NAME} via ST-Link (OpenOCD)"
  )
endfunction()
