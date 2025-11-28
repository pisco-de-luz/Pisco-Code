#!/usr/bin/env bash
set -euo pipefail

#------------------------------------------------------------------------------
# Build.sh — Unified build script for PiscoCode
# Usage:
#   Build.sh <config>                → build core + tests if native
#   Build.sh <config>/<example>      → build only specific example
#   Build.sh all                     → build everything
#------------------------------------------------------------------------------

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

readonly CONFIGS=("native" "avr-arduino-nano" "stm32-f410rb")
readonly ROOT_DIR="$(pwd)"

print_usage() {
  echo "Usage:"
  echo "  $(basename "$0") <config>              → build config (e.g. native, avr-arduino-nano)"
  echo "  $(basename "$0") <config>/<example>    → build specific example"
  echo "  $(basename "$0") all                   → build all configs"
  echo
  echo "Available configs:"
  for c in "${CONFIGS[@]}"; do echo "  - $c"; done
}

# Get CMake arguments for a configuration
get_cmake_args() {
  local config="$1"
  local args=""
  
  case "$config" in
    native)
      # No extra args needed for native
      ;;
    avr-arduino-nano)
      args="-DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/avr-gcc.cmake"
      args+=" -DBOARD=arduino-nano"
      args+=" -DEXAMPLES=basic_example"
      ;;
    stm32-f410rb)
      args="-DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi-gcc.cmake"
      args+=" -DBOARD=f410rb"
      args+=" -DEXAMPLES=basic_example"
      ;;
    *)
      echo "❌ Unknown configuration: $config"
      exit 1
      ;;
  esac
  
  echo "$args"
}

# Configure and build a configuration
build_config() {
  local config="$1"
  local build_dir="$ROOT_DIR/build/$config"
  local cmake_args
  cmake_args=$(get_cmake_args "$config")

  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    echo "[$config] Configuring..."
    # shellcheck disable=SC2086
    cmake -S . -B "$build_dir" $cmake_args
  else
    echo "[$config] Already configured. Skipping configure step."
  fi

  echo "[$config] Building..."
  cmake --build "$build_dir" --parallel

  if [[ "$config" == "native" ]]; then
    echo "[$config] Testing..."
    ctest --test-dir "$build_dir" -R PiscoCodeTests --verbose --output-on-failure
  fi

  echo "[$config] Done."
}

# Build single example target
build_example() {
  local config="$1"
  local target="$2"
  local build_dir="$ROOT_DIR/build/$config"
  local cmake_args
  cmake_args=$(get_cmake_args "$config")

  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    echo "[$config] Configuring..."
    # shellcheck disable=SC2086
    cmake -S . -B "$build_dir" $cmake_args
  fi

  echo "[$config] Building target: $target"
  cmake --build "$build_dir" --target "$target"
  echo "[$config] Target '$target' done."
  
  # If it's an AVR config, show memory usage using avr-size
  if [[ "$config" == avr-* ]]; then
    ELF_PATH="build/${config}/examples/avr/${target}"
    if [[ -f "${ELF_PATH}" ]]; then
      echo
      echo "Memory usage for ${target} (Device: atmega328p)"
      avr-size -C --mcu=atmega328p "${ELF_PATH}"
      echo
    else
      echo "Warning: ELF file not found at ${ELF_PATH}, skipping memory report"
    fi
  fi
}

# Parse argument
if [[ $# -ne 1 ]]; then
  print_usage
  exit 1
fi

INPUT="$1"

if [[ "$INPUT" == "all" ]]; then
  for c in "${CONFIGS[@]}"; do
    build_config "$c"
  done
elif [[ "$INPUT" == */* ]]; then
  CONFIG="${INPUT%%/*}"
  TARGET="${INPUT##*/}"
  if [[ ! " ${CONFIGS[*]} " =~ " ${CONFIG} " ]]; then
    echo "❌ Error: unknown config '${CONFIG}'"
    print_usage
    exit 1
  fi
  build_example "$CONFIG" "$TARGET"
elif [[ " ${CONFIGS[*]} " =~ " ${INPUT} " ]]; then
  build_config "$INPUT"
else
  echo "❌ Error: unknown input '$INPUT'"
  print_usage
  exit 1
fi
