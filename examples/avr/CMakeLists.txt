# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/avr/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

# Shared objects for all AVR examples
add_library(avr_shared OBJECT
    avr_systick.cpp
)
target_include_directories(avr_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(avr_shared PUBLIC PiscoCodeCore)

# Board name from preset
set(BOARD "arduino-nano" CACHE STRING "Board folder under boards/")
string(REPLACE "-" "_" BOARD_CANON "${BOARD}")

# Examples to build
set(EXAMPLES "basic_example" CACHE STRING "Examples to build")
separate_arguments(EXAMPLES)

# Required tool
find_program(CMAKE_OBJCOPY NAMES avr-objcopy objcopy REQUIRED)

# Helper for building 1 example
function(add_avr_example_for_board EX_NAME)
  set(HAL_CPP "boards/${BOARD}/hal_led_${BOARD_CANON}.cpp")

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
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.map
    -Wl,--gc-sections
    -Wl,--cref
    -Wl,--start-group
  )
  target_link_libraries(${EX_NAME} PRIVATE c gcc m)
  target_link_options(${EX_NAME} PRIVATE -Wl,--end-group)

  add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
            $<TARGET_FILE:${EX_NAME}>
            ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex
    DEPENDS ${EX_NAME}
    COMMENT "Generating ${EX_NAME}.hex"
  )

  add_custom_target(${EX_NAME}_hex
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EX_NAME}.hex
  )
endfunction()

# Call for each example
foreach(EX ${EXAMPLES})
  add_avr_example_for_board(${EX})
endforeach()

