# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/stm32/ directory
# // Author:      Andre Viegas
# // Date:        2025.09
# // Version:     2.0
# //--------------------------------------------------------------------------------------------

# Enable ASM language for startup files
enable_language(ASM)

include(${CMAKE_CURRENT_LIST_DIR}/stm32_example_macros.cmake)

set(BOARD "f410rb" CACHE STRING "STM32 board under boards/")
set(EXAMPLES "basic_example" CACHE STRING "Examples to build")
string(REPLACE "-" "_" BOARD_CANON "${BOARD}")
separate_arguments(EXAMPLES)

# Build each example (each example acts as a consumer of the Pisco Code library)
foreach(EX ${EXAMPLES})
    add_stm32_example(${EX} ${BOARD} ${BOARD_CANON})
endforeach()
