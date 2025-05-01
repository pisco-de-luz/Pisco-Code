# cmake/modules/avr_outputs.cmake
if (NOT TARGET avr_outputs)
  find_program(AVR_OBJCOPY avr-objcopy REQUIRED)

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${AVR_OBJCOPY} -O ihex
            $<TARGET_FILE:PiscoCode>
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    DEPENDS PiscoCode
    COMMENT "Generating .hex file"
  )

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMAND ${AVR_OBJCOPY} -O binary
            $<TARGET_FILE:PiscoCode>
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    DEPENDS PiscoCode
    COMMENT "Generating .bin file"
  )

  add_custom_target(
    avr_outputs ALL
    DEPENDS
      ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
      ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
  )
  add_dependencies(avr_outputs PiscoCode)
endif()
