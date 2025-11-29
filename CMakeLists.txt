# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: Main CMake file for the PiscoCode project.
# // Author:      Andre Viegas
# // Date:        2025.11
# // Version:     1.3
# //--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)
project(PiscoCode LANGUAGES C CXX)

option(PISCO_CODE_BUILD_EXAMPLES "Build PiscoCode examples" ON)
option(PISCO_CODE_BUILD_TESTS "Build PiscoCode tests" ON)

# Detect if we're being included as a subproject
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PISCO_CODE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(PISCO_CODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

if(PISCO_CODE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PISCO_CODE_BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Native build detected — enabling unit tests.")
    set(CPPUTEST_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(CPPUTEST_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/CppUTest EXCLUDE_FROM_ALL)
    enable_testing()
    add_subdirectory(tests)
elseif(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for ${CMAKE_SYSTEM_PROCESSOR} — skipping unit tests.")
endif()
