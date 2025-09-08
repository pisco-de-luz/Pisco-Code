# tests/CMakeLists.txt — Build and run CppUTest-based unit tests
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB MOCK_SOURCES mocks/*.cpp)
file(GLOB HELPER_SOURCES helpers/*.cpp)

add_executable(PiscoCodeTests 
    ${TEST_SOURCES}
    ${MOCK_SOURCES}
    ${HELPER_SOURCES}
)

set_property(TARGET PiscoCodeTests PROPERTY CXX_STANDARD 20)
set_property(TARGET PiscoCodeTests PROPERTY CXX_STANDARD_REQUIRED YES)
set_property(TARGET PiscoCodeTests PROPERTY CXX_EXTENSIONS OFF)
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
