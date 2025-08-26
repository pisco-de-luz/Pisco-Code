# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for src directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

# Header path shared via target
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${INCLUDE_DIR}/*.hpp)

# Core library
add_library(PiscoCodeCore ${SRC_FILES} ${HEADER_FILES})
target_include_directories(PiscoCodeCore PUBLIC ${INCLUDE_DIR})

# Warnings for library
target_compile_options(PiscoCodeCore PRIVATE -Wall -Wextra -Wpedantic)
