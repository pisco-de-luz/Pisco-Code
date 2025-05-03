#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------------------
# test_build_system.sh — Integration test for CMake build logic and hex generation
# -----------------------------------------------------------------------------

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$ROOT_DIR"

BUILD_LOG="$(mktemp)"
trap 'rm -f "$BUILD_LOG"' EXIT

# Colored logging
function say()  { echo -e "\\033[1;36m$1\\033[0m"; }
function good() { echo -e "\\033[1;32m$1\\033[0m"; }
function fail() { echo -e "\\033[1;31m$1\\033[0m"; }

function assert_file_exists() {
  if [[ ! -f "$1" ]]; then
    fail "❌ ERROR: Expected file not found: $1"
    exit 1
  else
    good "✅ Found: $1"
  fi
}

say "📦 Testing build system in: $ROOT_DIR"

EXAMPLES=(
  "avr/basic_example"
#   "avr/one_led_clock"
)

say "🔁 Cleaning previous build output..."
rm -rf "$ROOT_DIR/build/avr" "$ROOT_DIR/build/native"

say "🔧 Configuring AVR preset..."
cmake --preset avr > "$BUILD_LOG" 2>&1 || { fail "❌ AVR configuration failed"; cat "$BUILD_LOG"; exit 1; }

for EXAMPLE in "${EXAMPLES[@]}"; do
  TARGET="${EXAMPLE##*/}"
  HEX="build/avr/examples/avr/${TARGET}.hex"
  say "🔨 Building example: $EXAMPLE"
  cmake --build build/avr --target "$TARGET" > "$BUILD_LOG" 2>&1 || { fail "❌ Build failed for $TARGET"; cat "$BUILD_LOG"; exit 1; }
  say "🧪 Verifying hex output for: $TARGET"
  assert_file_exists "$HEX"
done

say "🔧 Configuring native preset..."
cmake --preset native > "$BUILD_LOG" 2>&1 || { fail "❌ Native configuration failed"; cat "$BUILD_LOG"; exit 1; }

say "🔨 Building native (library + tests)..."
cmake --build build/native > "$BUILD_LOG" 2>&1 || { fail "❌ Native build failed"; cat "$BUILD_LOG"; exit 1; }

say "🧪 Verifying native core library was built..."
assert_file_exists build/native/src/libPiscoCodeCore.a

say "🧪 Verifying CppUTest libraries were built..."
assert_file_exists build/native/external/CppUTest/src/CppUTest/libCppUTest.a
assert_file_exists build/native/external/CppUTest/src/CppUTestExt/libCppUTestExt.a

say "🧪 Verifying at least one unit test executable exists..."
found_tests=$(find build/native/tests -type f -executable | wc -l)
if [[ "$found_tests" -lt 1 ]]; then
  fail "❌ ERROR: No unit test executables found in build/native/test"
  exit 1
else
  good "✅ Found $found_tests test executable(s)"
fi

say "🧪 Running unit tests..."
ctest --test-dir build/native -R PiscoCodeTests  --output-on-failure || { fail "❌ Unit tests failed"; exit 1; }

echo ""
good "🎉 Build system test PASSED — all checks completed successfully."
