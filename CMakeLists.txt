# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: Main CMake file for the PiscoCode project.
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.19)
project(PiscoCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug) # Enable debug symbols

list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_SOURCE_DIR}/cmake/modules")

set(SRC_DIR     ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Core library
add_subdirectory(${SRC_DIR})
include_directories(${INCLUDE_DIR})

# Target-specific examples
add_subdirectory(examples)

# Unit tests — only for host/native builds
if(NOT CMAKE_CROSSCOMPILING)
  message(STATUS "Native build detected — enabling unit tests.")
  add_subdirectory(external/CppUTest)
  enable_testing()
  add_subdirectory(tests)
else()
  message(STATUS "Cross-compiling for ${CMAKE_SYSTEM_PROCESSOR} — skipping unit tests.")
endif()
