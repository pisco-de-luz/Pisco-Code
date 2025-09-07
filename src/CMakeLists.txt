# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for src directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

# Header path shared via target
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
# Path to freestanding shims - this CMakeLists.txt lives in src/
set(FREESTANDING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freestanding)

# Source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${INCLUDE_DIR}/*.hpp)


option(PISCO_FREESTANDING "Enable freestanding shims for bare-metal targets" ON)

# Core library
add_library(PiscoCodeCore ${SRC_FILES} ${HEADER_FILES})
target_include_directories(PiscoCodeCore PUBLIC ${INCLUDE_DIR})

# Warnings for library
target_compile_options(PiscoCodeCore PRIVATE -Wall -Wextra -Wpedantic)

if(PISCO_FREESTANDING)
  target_sources(PiscoCodeCore PRIVATE
    ${FREESTANDING_DIR}/pisco_runtime_shim.cpp
    ${FREESTANDING_DIR}/pisco_memory_shim.cpp
  )
  target_compile_definitions(PiscoCodeCore PRIVATE PISCO_FREESTANDING=1)
  target_compile_options(PiscoCodeCore PRIVATE
    -ffreestanding -fno-exceptions -fno-rtti
    -fno-unwind-tables -fno-asynchronous-unwind-tables
  )
endif()



