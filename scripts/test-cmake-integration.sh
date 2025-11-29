#!/bin/bash
# //--------------------------------------------------------------------------------------------
# // File:        test-cmake-integration.sh
# // Description: Comprehensive CMake integration testing for all targets and modes
# //              Tests native, AVR, STM32 builds, and subproject integration
# // Usage:       ./scripts/test-cmake-integration.sh
# // Author:      Andre Viegas
# // Date:        2025
# // Note:        This script is READ-ONLY - it tests builds without modifying any files.
# //              Run this before committing CMake changes to verify everything works.
# //--------------------------------------------------------------------------------------------

set -e  # Exit on any error

echo "=========================================="
echo "Testing CMake Integration (Read-Only)"
echo "=========================================="
echo ""
echo "This script tests builds WITHOUT modifying any project files."
echo ""

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Test 1: Native build
echo "=========================================="
echo "Test 1: Native Build (with tests)"
echo "=========================================="
cd "$PROJECT_ROOT"
rm -rf build/native
cmake -S . -B build/native
cmake --build build/native
ctest --test-dir build/native --output-on-failure
echo "✅ Native build succeeded"
echo ""

# Test 2: AVR build
echo "=========================================="
echo "Test 2: AVR Build (Arduino Nano)"
echo "=========================================="
cd "$PROJECT_ROOT"
rm -rf build/avr-arduino-nano
cmake -S . -B build/avr-arduino-nano \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/avr-gcc.cmake \
    -DBOARD=arduino-nano \
    -DEXAMPLES=basic_example
cmake --build build/avr-arduino-nano
echo "✅ AVR build succeeded"
echo ""

# Test 3: STM32 build
echo "=========================================="
echo "Test 3: STM32 Build (Nucleo F410RB)"
echo "=========================================="
cd "$PROJECT_ROOT"
rm -rf build/stm32-f410rb
cmake -S . -B build/stm32-f410rb \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi-gcc.cmake \
    -DBOARD=f410rb \
    -DEXAMPLES=basic_example
cmake --build build/stm32-f410rb --target basic_example
echo "✅ STM32 build succeeded"
echo ""

# Test 4: Subproject mode simulation
echo "=========================================="
echo "Test 4: Subproject Mode"
echo "=========================================="
TEMP_PROJECT="/tmp/pisco-test-subproject-$$"
rm -rf "$TEMP_PROJECT"
mkdir -p "$TEMP_PROJECT"

# Cleanup temp directory on exit
cleanup_temp() {
    rm -rf "$TEMP_PROJECT"
}
trap cleanup_temp EXIT

cat > "$TEMP_PROJECT/CMakeLists.txt" << 'EOFTEST'
cmake_minimum_required(VERSION 3.14)
project(TestConsumer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(pisco-code)

# Test that library targets are available
if(NOT TARGET PiscoCodeCore)
    message(FATAL_ERROR "PiscoCodeCore target not found")
endif()

if(NOT TARGET PiscoCodeBare)
    message(FATAL_ERROR "PiscoCodeBare target not found")
endif()

# Test aliases work
if(NOT TARGET pisco_code::core)
    message(FATAL_ERROR "pisco_code::core alias not found")
endif()

if(NOT TARGET pisco_code::bare)
    message(FATAL_ERROR "pisco_code::bare alias not found")
endif()

# Create a simple test executable
add_executable(consumer_test main.cpp)
target_link_libraries(consumer_test PRIVATE pisco_code::core)
EOFTEST

cat > "$TEMP_PROJECT/main.cpp" << 'EOFCPP'
#include "pisco_code.hpp"
int main() {
    return 0;
}
EOFCPP

# Copy project files (excluding build directories)
mkdir -p "$TEMP_PROJECT/pisco-code"
rsync -a --exclude='build' --exclude='.git' "$PROJECT_ROOT/" "$TEMP_PROJECT/pisco-code/"

cd "$TEMP_PROJECT"
cmake -B build
cmake --build build

echo "✅ Subproject mode works correctly"
echo ""

echo "=========================================="
echo "🎉 All Tests Passed!"
echo "=========================================="
echo ""
echo "All CMake configurations verified successfully."
echo ""
