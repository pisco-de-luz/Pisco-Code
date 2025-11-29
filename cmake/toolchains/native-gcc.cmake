# //--------------------------------------------------------------------------------------------
# // Toolchain: native-gcc.cmake
# // Description: Native GCC toolchain for desktop/Linux builds (unit tests, development)
# // 
# // This toolchain is used for native builds on the development machine.
# // Primarily used for running unit tests with CppUTest.
# //--------------------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Toolchain binaries
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# =============================================================================
# Compiler Flags
# =============================================================================
# Warnings
set(WARN_FLAGS "-Wall -Wextra")

# Apply flags
set(CMAKE_C_FLAGS_INIT "${WARN_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${WARN_FLAGS}")

