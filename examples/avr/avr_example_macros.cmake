function(add_avr_example EX_NAME BOARD BOARD_CANON)
  set(HAL_CPP "boards/${BOARD}/hal_led_${BOARD_CANON}.cpp")
  set(HEX_FILE "${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex")
  set(MAP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.map")

  find_program(CMAKE_OBJCOPY NAMES avr-objcopy objcopy REQUIRED)

  add_executable(${EX_NAME}
    ${EX_NAME}.cpp
    $<TARGET_OBJECTS:avr_shared>
    boards/${BOARD}/board_config.hpp
    ${HAL_CPP}
  )

  target_include_directories(${EX_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_SOURCE_DIR}/include
  )

  target_link_libraries(${EX_NAME} PRIVATE PiscoCodeCore)

  target_link_options(${EX_NAME} PRIVATE
    -Wl,-Map=${MAP_FILE}
    -Wl,--gc-sections
    -Wl,--cref
    -Wl,--start-group
  )
  target_link_libraries(${EX_NAME} PRIVATE c gcc m)
  target_link_options(${EX_NAME} PRIVATE -Wl,--end-group)

  add_custom_command(
    OUTPUT ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
            $<TARGET_FILE:${EX_NAME}>
            ${HEX_FILE}
    DEPENDS ${EX_NAME}
    COMMENT "Generating ${EX_NAME}.hex"
  )

  add_custom_target(${EX_NAME}_hex
    DEPENDS ${HEX_FILE}
  )

  # Upload configuration
  set(AVR_UPLOAD_PROGRAMMER "usbasp"      CACHE STRING "Programmer used by avrdude")
  set(AVR_UPLOAD_MCU        "atmega328p"  CACHE STRING "MCU used by avrdude")
  set(AVR_UPLOAD_PORT       ""            CACHE STRING "Serial port or USB device (if needed)")
  set(AVR_UPLOAD_BAUD       ""            CACHE STRING "Baud rate for serial-based upload")

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


  add_custom_target(${EX_NAME}_upload
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing ${HEX_FILE} using ${AVR_UPLOAD_PROGRAMMER}..."
    COMMAND avrdude
      -v
      -p ${AVR_UPLOAD_MCU}
      -c ${AVR_UPLOAD_PROGRAMMER}
      ${UPLOAD_PORT_ARG}
      ${UPLOAD_BAUD_ARG}
      -U flash:w:${HEX_FILE}:i
    DEPENDS ${EX_NAME}_hex
    USES_TERMINAL
    COMMENT "→ Uploading ${EX_NAME}.hex"
  )
endfunction()

