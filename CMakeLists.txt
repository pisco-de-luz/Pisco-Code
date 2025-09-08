# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: Main CMake file for the PiscoCode project.
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.19)
project(PiscoCode LANGUAGES C CXX)

# Build Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# Directory Variables
set(SRC_DIR     ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Expose to subdirs
include_directories(${INCLUDE_DIR})

# Subdirectories
add_subdirectory(${SRC_DIR})
add_subdirectory(examples)

# Unit tests (only for native builds)
if(NOT CMAKE_CROSSCOMPILING)
  message(STATUS "Native build detected — enabling unit tests.")
  set(CPPUTEST_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(CPPUTEST_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  add_subdirectory(external/CppUTest EXCLUDE_FROM_ALL)
  enable_testing()
  add_subdirectory(tests)
else()
  message(STATUS "Cross-compiling for ${CMAKE_SYSTEM_PROCESSOR} — skipping unit tests.")
endif()
