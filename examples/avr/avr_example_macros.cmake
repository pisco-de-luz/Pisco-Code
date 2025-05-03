function(add_avr_example TARGET_NAME)
  set(oneValueArgs SOURCE MCU)
  cmake_parse_arguments(EX "" "${oneValueArgs}" "" ${ARGN})

  add_executable(${TARGET_NAME} ${EX_SOURCE})
  target_link_libraries(${TARGET_NAME} PiscoCodeCore)

  find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
  find_program(AVRDUDE     avrdude     REQUIRED)

  set(HEX_FILE  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex)
  set(ELF_FILE  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME})

  add_custom_command(
    OUTPUT ${HEX_FILE}
    COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom ${ELF_FILE} ${HEX_FILE}
    DEPENDS ${TARGET_NAME}
    COMMENT "→ Generating HEX for ${TARGET_NAME}"
  )

  add_custom_target(${TARGET_NAME}_hex ALL DEPENDS ${HEX_FILE})

  set(AVR_PORT "/dev/ttyACM0" CACHE STRING "AVR upload port")
  set(AVR_BAUD "19200"        CACHE STRING "AVR upload baud")

  add_custom_target(${TARGET_NAME}_upload
    DEPENDS ${TARGET_NAME}_hex
    COMMAND ${AVRDUDE}
      -v
      -p ${EX_MCU}
      -c stk500v1
      -P ${AVR_PORT}
      -b ${AVR_BAUD}
      -e
      -U flash:w:${HEX_FILE}:i
    COMMENT "→ Uploading ${TARGET_NAME}.hex to ${AVR_PORT}"
  )
endfunction()
