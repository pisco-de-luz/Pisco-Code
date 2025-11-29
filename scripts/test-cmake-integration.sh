#!/bin/bash
# //--------------------------------------------------------------------------------------------
# // File:        test-cmake-integration.sh
# // Description: Comprehensive CMake integration testing for all targets and modes
# //              Tests native, AVR, STM32 builds, and subproject integration
# // Usage:       ./scripts/test-cmake-integration.sh
# // Author:      Andre Viegas
# // Date:        2025
# // Note:        This script is used during development to verify CMake changes.
# //              It creates temporary CMakeLists.txt files for testing and restores
# //              originals on failure. Run this before committing CMake changes.
# //--------------------------------------------------------------------------------------------

set -e  # Exit on any error

echo "=========================================="
echo "Testing CMakeLists.txt Changes"
echo "=========================================="
echo ""

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BACKUP_FILE="${PROJECT_ROOT}/CMakeLists.txt.backup"
CMAKE_FILE="${PROJECT_ROOT}/CMakeLists.txt"

# Cleanup function
cleanup() {
    if [ $? -ne 0 ]; then
        echo ""
        echo "❌ Test failed! Restoring original files..."
        if [ -f "$BACKUP_FILE" ]; then
            mv "$BACKUP_FILE" "$CMAKE_FILE"
            echo "✅ Original CMakeLists.txt restored"
        fi
        if [ -f "$SRC_BACKUP_FILE" ]; then
            mv "$SRC_BACKUP_FILE" "$SRC_CMAKE_FILE"
            echo "✅ Original src/CMakeLists.txt restored"
        fi
    else
        echo ""
        echo "✅ All tests passed!"
        if [ -f "$BACKUP_FILE" ]; then
            rm "$BACKUP_FILE"
            echo "🗑️  Root CMakeLists.txt backup removed"
        fi
        if [ -f "$SRC_BACKUP_FILE" ]; then
            rm "$SRC_BACKUP_FILE"
            echo "🗑️  src/CMakeLists.txt backup removed"
        fi
    fi
}

trap cleanup EXIT

# Backup current CMakeLists.txt
echo "📦 Backing up current CMakeLists.txt and src/CMakeLists.txt..."
cp "$CMAKE_FILE" "$BACKUP_FILE"
echo ""

# Backup src/CMakeLists.txt too
SRC_CMAKE_FILE="${PROJECT_ROOT}/src/CMakeLists.txt"
SRC_BACKUP_FILE="${PROJECT_ROOT}/src/CMakeLists.txt.backup"
cp "$SRC_CMAKE_FILE" "$SRC_BACKUP_FILE"

# Create new CMakeLists.txt
echo "📝 Creating new consumer-friendly CMakeLists.txt..."
cat > "$CMAKE_FILE" << 'EOF'
# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: Main CMake file for the PiscoCode project.
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     2.0
# //--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.19)
project(PiscoCode LANGUAGES C CXX)

# Consumer-friendly options
option(PISCO_CODE_BUILD_EXAMPLES "Build PiscoCode examples" ON)
option(PISCO_CODE_BUILD_TESTS "Build PiscoCode tests" ON)

# Detect if we're being included as a subproject
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # We're a subproject - default to not building examples/tests
    set(PISCO_CODE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(PISCO_CODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
endif()

# Build Settings (only if not set by parent project)
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Always build the library
add_subdirectory(src)

# Conditionally build examples
if(PISCO_CODE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Unit tests (only for native builds and if enabled)
if(PISCO_CODE_BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Native build detected — enabling unit tests.")
    set(CPPUTEST_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(CPPUTEST_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/CppUTest EXCLUDE_FROM_ALL)
    enable_testing()
    add_subdirectory(tests)
elseif(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for ${CMAKE_SYSTEM_PROCESSOR} — skipping unit tests.")
endif()
EOF

# Update src/CMakeLists.txt to be subproject-aware
echo "📝 Updating src/CMakeLists.txt for subproject compatibility..."
sed -i 's|set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)|# Support both main project and subproject modes\nif(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)\n    set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)\nelse()\n    set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)\nendif()|' "$SRC_CMAKE_FILE"
echo ""

# Test 1: Native build
echo "=========================================="
echo "Test 1: Native Build (with tests)"
echo "=========================================="
cd "$PROJECT_ROOT"
rm -rf build/native
cmake -S . -B build/native
cmake --build build/native
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
TEMP_PROJECT="/tmp/pisco-test-subproject"
rm -rf "$TEMP_PROJECT"
mkdir -p "$TEMP_PROJECT"

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

# Create a simple test executable
add_executable(consumer_test main.cpp)
target_link_libraries(consumer_test PRIVATE PiscoCodeCore)
EOFTEST

cat > "$TEMP_PROJECT/main.cpp" << 'EOFCPP'
#include "pisco_code.hpp"
int main() {
    return 0;
}
EOFCPP

cp -r "$PROJECT_ROOT" "$TEMP_PROJECT/pisco-code"

cd "$TEMP_PROJECT"
cmake -B build
cmake --build build

# Verify examples/tests were NOT built
if [ -d "build/pisco-code/examples" ]; then
    echo "❌ Examples were built in subproject mode (should be disabled)"
    exit 1
fi

if [ -d "build/pisco-code/tests" ]; then
    echo "❌ Tests were built in subproject mode (should be disabled)"
    exit 1
fi

echo "✅ Subproject mode works correctly (examples/tests disabled)"
echo ""

echo "=========================================="
echo "🎉 All Tests Passed!"
echo "=========================================="
echo ""
echo "New CMakeLists.txt is ready for commit."
echo ""
