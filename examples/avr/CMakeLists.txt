# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/avr/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/avr_example_macros.cmake)

set(BOARD "arduino-nano" CACHE STRING "AVR board under boards/")
set(EXAMPLES "basic_example" CACHE STRING "Examples to build")
string(REPLACE "-" "_" BOARD_CANON "${BOARD}")
separate_arguments(EXAMPLES)

add_library(avr_shared OBJECT
   avr_systick.cpp
)

target_include_directories(avr_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(avr_shared SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD} 
)

target_link_libraries(avr_shared PUBLIC PiscoCodeCore)

foreach(EX ${EXAMPLES})
  add_avr_example(${EX} ${BOARD} ${BOARD_CANON})
endforeach()
