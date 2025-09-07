# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/avr/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/avr_example_macros.cmake)

add_avr_example(basic_example
  SOURCE basic_example.cpp
  MCU atmega328p
)

# Put the map next to the ELF
target_link_options(basic_example PRIVATE
  -Wl,-Map=$<TARGET_FILE_DIR:basic_example>/basic_example.map
  -Wl,--gc-sections
  -Wl,--cref
)

target_sources(basic_example PRIVATE
  avr_systick.cpp
)


