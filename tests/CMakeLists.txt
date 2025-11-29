# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for tests/ directory - Unit tests using CppUTest
# // Author:      Andre Viegas
# // Date:        2025.11
# // Version:     1.4
# //--------------------------------------------------------------------------------------------

set(PISCO_CODE_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_encoding_blinker_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/binary_systematic_high_level_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code_blinker_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/decimal_systematic_high_level_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_pattern_of_digits_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hexadecimal_systematic_high_level_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/led_blink_pattern_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loop_behavior_blinker_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/octal_systematic_high_level_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/setter_behavior_blinker_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_element_iterator_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_element_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_pulse_iterator_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_sequencer_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_stack_test.cpp
)

set(PISCO_CODE_MOCK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_led_control_logger.cpp
)

set(PISCO_CODE_HELPER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/led_blink_pattern.cpp
)

add_executable(PiscoCodeTests 
    ${PISCO_CODE_TEST_SOURCES}
    ${PISCO_CODE_MOCK_SOURCES}
    ${PISCO_CODE_HELPER_SOURCES}
)

# C20 features for tests (designated initializers, etc.)
target_compile_features(PiscoCodeTests PRIVATE cxx_std_20)
target_compile_options(PiscoCodeTests PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(PiscoCodeTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers
)

target_include_directories(PiscoCodeTests SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include/Platforms
)

target_link_libraries(PiscoCodeTests PRIVATE
    pisco_code::core
    CppUTest
    CppUTestExt
)

add_test(NAME PiscoCodeTests COMMAND PiscoCodeTests)
