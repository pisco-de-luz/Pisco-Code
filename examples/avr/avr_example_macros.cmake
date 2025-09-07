function(add_avr_example TARGET_NAME)
  set(oneValueArgs SOURCE MCU)
  cmake_parse_arguments(EX "" "${oneValueArgs}" "" ${ARGN})

  add_executable(${TARGET_NAME} ${EX_SOURCE})
  target_link_libraries(${TARGET_NAME} 
     PRIVATE 
        PiscoCodeCore
        gcc
  )

  find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
  find_program(AVRDUDE     avrdude     REQUIRED)

  set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex)
  set(ELF_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME})

  # POST-BUILD: generate .hex whenever executable is built
  add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom ${ELF_FILE} ${HEX_FILE}
    COMMENT "→ Generating HEX: ${HEX_FILE}"
    VERBATIM
  )

  set(AVR_PORT "/dev/ttyACM0" CACHE STRING "AVR upload port")
  set(AVR_BAUD "19200"        CACHE STRING "AVR upload baud")

  add_custom_target(${TARGET_NAME}_upload
    DEPENDS ${TARGET_NAME}
    COMMAND ${AVRDUDE}
      -v
      -p ${EX_MCU}
      -c stk500v1
      -P ${AVR_PORT}
      -b ${AVR_BAUD}
      -F
      -e
      -U flash:w:${HEX_FILE}:i
    COMMENT "→ Uploading ${TARGET_NAME}.hex to ${AVR_PORT}"
  )
endfunction()
