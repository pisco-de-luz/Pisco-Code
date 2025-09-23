# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

message(STATUS "Target processor -> ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "avr")
  add_subdirectory(avr)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
  add_subdirectory(stm32)
endif()
