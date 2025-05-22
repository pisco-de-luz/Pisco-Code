# tests/CMakeLists.txt — Build and run CppUTest-based unit tests
file(GLOB TEST_SOURCES "*.cpp")
file(GLOB MOCK_SOURCES mocks/*.cpp)
file(GLOB MOCK_HEADERS mocks/*.hpp)
file(GLOB HELPER_SOURCES helpers/*.cpp)
file(GLOB HELPER_HEADERS helpers/*.hpp)

add_executable(PiscoCodeTests 
    ${TEST_SOURCES}
    ${MOCK_SOURCES}
    ${HELPER_SOURCES}
    ${MOCK_HEADERS}
    ${HELPER_HEADERS}
)

target_include_directories(PiscoCodeTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include
    ${CMAKE_SOURCE_DIR}/external/CppUTest/include/Platforms
)

target_link_libraries(PiscoCodeTests
    PiscoCodeCore
    CppUTest
    CppUTestExt
)

add_test(NAME PiscoCodeTests COMMAND PiscoCodeTests)
