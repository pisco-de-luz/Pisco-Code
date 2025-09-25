# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/stm32/ directory
# // Author:      Andre Viegas
# // Date:        2025.09
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/stm32_example_macros.cmake)

set(BOARD "f410rb" CACHE STRING "STM32 board under boards/")
set(EXAMPLES "basic_example" CACHE STRING "Examples to build")
string(REPLACE "-" "_" BOARD_CANON "${BOARD}")
separate_arguments(EXAMPLES)

add_library(stm32_shared OBJECT
   stm32_systick.cpp
)

target_include_directories(stm32_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}   
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(stm32_shared SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/vendor 
)

target_link_libraries(stm32_shared PUBLIC PiscoCodeCore)

foreach(EX ${EXAMPLES})
  add_stm32_example(${EX} ${BOARD} ${BOARD_CANON})
endforeach()
