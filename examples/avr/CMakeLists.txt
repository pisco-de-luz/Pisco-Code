# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/avr/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     2.0
# //--------------------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/avr_example_macros.cmake)

set(BOARD "arduino-nano" CACHE STRING "AVR board under boards/")
set(EXAMPLES "basic_example" CACHE STRING "Examples to build")
separate_arguments(EXAMPLES)

# Build each example (each example acts as a consumer of the Pisco Code library)
foreach(EX ${EXAMPLES})
    add_avr_example(${EX})
endforeach()
