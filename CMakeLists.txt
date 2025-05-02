# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: Main CMake file for the PiscoCode project.
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.0
# //--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)
project(PiscoCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_SOURCE_DIR}/cmake/modules")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${SRC_DIR})
add_subdirectory(external/CppUTest)

include_directories(${INCLUDE_DIR})

# Pull in the AVR hex/bin logic only when cross-compiling for AVR
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "avr")
  include(avr_outputs)
endif()

enable_testing()
add_subdirectory(test)
