# tests/CMakeLists.txt — Build and run CppUTest-based unit tests
file(GLOB TEST_SOURCES "*.cpp")
set(MOCK_SOURCES
    mocks/MockLedControlLogger.cpp
    mocks/MockLedControllerAdapter.hpp 
)
set(HELPER_SOURCES
    helpers/LedBlinkPattern.cpp
)

add_executable(PiscoCodeTests 
    ${TEST_SOURCES}
    ${MOCK_SOURCES}
    ${HELPER_SOURCES}
)

target_include_directories(PiscoCodeTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include/Platforms
)

target_link_libraries(PiscoCodeTests
    PiscoCodeCore
    CppUTest
    CppUTestExt
)

add_test(NAME PiscoCodeTests COMMAND PiscoCodeTests)
