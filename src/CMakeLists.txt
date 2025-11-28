# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for src directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

# Public headers
# Support both main project and subproject modes
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
else()
    set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
endif()

# Sources listed explicitly
set(LIB_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/signal_emitter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signal_sequencer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signal_stack.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signal_controller.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/led_controller_software_pwm.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/led_controller_hardware_pwm.cpp
)

# Hosted objects for tests and hosted builds
add_library(PiscoCodeObjects OBJECT ${LIB_SOURCES})
target_include_directories(PiscoCodeObjects PUBLIC ${INCLUDE_DIR})
target_compile_features(PiscoCodeObjects PUBLIC cxx_std_11)
target_compile_options(PiscoCodeObjects PRIVATE
  -Wall -Wextra -Wpedantic
  -ffunction-sections -fdata-sections
)

# A tiny "flag bus" target that will carry MCU compile/link flags
add_library(pisco_mcu_flags INTERFACE)


# Bare objects for embedded builds
add_library(PiscoCodeObjectsBare OBJECT ${LIB_SOURCES})
target_include_directories(PiscoCodeObjectsBare PUBLIC ${INCLUDE_DIR})
target_compile_features(PiscoCodeObjectsBare PUBLIC cxx_std_11)
target_compile_definitions(PiscoCodeObjectsBare PUBLIC PISCO_FREESTANDING=1)
target_compile_options(PiscoCodeObjectsBare PRIVATE
-Wall -Wextra -Wpedantic
-ffunction-sections -fdata-sections
-ffreestanding -fno-builtin
$<$<COMPILE_LANGUAGE:CXX>:
-fno-exceptions -fno-rtti
-fno-unwind-tables -fno-asynchronous-unwind-tables
-fno-threadsafe-statics -fno-use-cxa-atexit
>
)

# Make bare objects consume whatever flags the bus provides
target_link_libraries(PiscoCodeObjectsBare PUBLIC pisco_mcu_flags)

# Build the freestanding shims with the bare objects so they see the define
target_sources(PiscoCodeObjectsBare PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_runtime_shim.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_memory_shim.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/freestanding/pisco_exit_shim.cpp
)

# Hosted static lib
add_library(PiscoCodeCore STATIC $<TARGET_OBJECTS:PiscoCodeObjects>)
target_include_directories(PiscoCodeCore PUBLIC ${INCLUDE_DIR})
# Note: --gc-sections is useful but shouldn't be forced on consumers
# Consumers can add it themselves if needed via CMAKE_EXE_LINKER_FLAGS

# Bare static lib
add_library(PiscoCodeCoreBare STATIC $<TARGET_OBJECTS:PiscoCodeObjectsBare>)
target_include_directories(PiscoCodeCoreBare PUBLIC ${INCLUDE_DIR})
# Note: --gc-sections is useful but shouldn't be forced on consumers
# Consumers can add it themselves if needed via CMAKE_EXE_LINKER_FLAGS

# Aliases for consumers
add_library(pisco_code::core      ALIAS PiscoCodeCore)
add_library(pisco_code::core_bare ALIAS PiscoCodeCoreBare)
