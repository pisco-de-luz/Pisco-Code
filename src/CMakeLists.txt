# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for src directory - Library targets only
# // Author:      Andre Viegas
# // Date:        2025.11
# // Version:     1.3
# //--------------------------------------------------------------------------------------------

# Include directory
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(PISCO_CODE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
else()
    set(PISCO_CODE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
endif()

set(PISCO_CODE_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_emitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_sequencer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_stack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/led_controller_software_pwm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/led_controller_hardware_pwm.cpp
)

set(PISCO_CODE_FREESTANDING_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_runtime_shim.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_memory_shim.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_exit_shim.cpp
)

# =============================================================================
# PiscoCodeCore - Hosted library (desktop, Linux, unit tests)
# =============================================================================
# Uses standard C++ library. For desktop development and testing.
#
add_library(PiscoCodeCore STATIC ${PISCO_CODE_LIB_SOURCES})
target_include_directories(PiscoCodeCore PUBLIC ${PISCO_CODE_INCLUDE_DIR})
target_compile_features(PiscoCodeCore PUBLIC cxx_std_17)

# =============================================================================
# PiscoCodeBare - Bare-metal library (AVR, STM32, embedded)
# =============================================================================
# For freestanding/embedded environments. Consumer's toolchain must provide
# appropriate flags (-fno-exceptions, -fno-rtti, etc.)
#
add_library(PiscoCodeBare STATIC 
    ${PISCO_CODE_LIB_SOURCES}
    ${PISCO_CODE_FREESTANDING_SOURCES}
)
target_include_directories(PiscoCodeBare PUBLIC ${PISCO_CODE_INCLUDE_DIR})
target_compile_features(PiscoCodeBare PUBLIC cxx_std_17)
target_compile_definitions(PiscoCodeBare PUBLIC PISCO_FREESTANDING=1)

add_library(pisco_code::core ALIAS PiscoCodeCore)
add_library(pisco_code::bare ALIAS PiscoCodeBare)
