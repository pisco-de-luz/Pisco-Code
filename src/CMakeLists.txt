# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for src directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.0
# //--------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS *.cpp)

add_executable(${PROJECT_NAME} 
    main.cpp
    # loop.cpp
    # Pisco-Code.cpp
    # show.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
)

add_library(PiscoCodeCore
    ${SRC_DIR}/Pisco-Code.cpp
    ${SRC_DIR}/loop.cpp
    ${SRC_DIR}/show.cpp
)

target_include_directories(PiscoCodeCore PUBLIC
    ${CMAKE_SOURCE_DIR}/include 
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

