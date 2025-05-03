#!/usr/bin/env bash
set -euo pipefail

#------------------------------------------------------------------------------
# Build.sh — Unified build script using CMake presets + example support
# Usage:
#   Build.sh <preset>                → build core + tests if native
#   Build.sh <preset>/<example>     → build only specific example
#   Build.sh all                    → build everything
#------------------------------------------------------------------------------

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."  # <— this is the fix

readonly PRESETS=("native" "avr")
readonly ROOT_DIR="$(pwd)"


print_usage() {
  echo "Usage:"
  echo "  $(basename "$0") <preset>              → build preset (e.g. native, avr)"
  echo "  $(basename "$0") <preset>/<example>    → build specific example"
  echo "  $(basename "$0") all                   → build all presets"
  echo
  echo "Available presets:"
  for p in "${PRESETS[@]}"; do echo "  - $p"; done
}

# Configure and build a preset
build_preset() {
  local preset="$1"
  local build_dir="$ROOT_DIR/build/$preset"

  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    echo "[${preset}] Configuring..."
    cmake --preset "$preset"
  else
    echo "[${preset}] Already configured. Skipping cmake --preset."
  fi

  echo "[${preset}] Building..."
  cmake --build --preset "$preset" --parallel

  if [[ "$preset" == "native" ]]; then
    echo "[${preset}] Testing..."
    ctest --test-dir "$build_dir" --output-on-failure
  fi

  echo "[${preset}] Done."
}

# Build single example target
build_example() {
  local preset="$1"
  local target="$2"
  local build_dir="$ROOT_DIR/build/$preset"

  if [[ ! -f "$build_dir/CMakeCache.txt" ]]; then
    echo "[${preset}] Configuring..."
    cmake --preset "$preset"
  fi

  echo "[${preset}] Building target: $target"
  cmake --build "$build_dir" --target "$target"
  echo "[${preset}] Target '$target' done."
}

# Parse argument
if [[ $# -ne 1 ]]; then
  print_usage
  exit 1
fi

INPUT="$1"

if [[ "$INPUT" == "all" ]]; then
  for p in "${PRESETS[@]}"; do
    build_preset "$p"
  done
elif [[ "$INPUT" == */* ]]; then
  PRESET="${INPUT%%/*}"
  TARGET="${INPUT##*/}"
  if [[ ! " ${PRESETS[*]} " =~ " ${PRESET} " ]]; then
    echo "❌ Error: unknown preset '${PRESET}'"
    print_usage
    exit 1
  fi
  build_example "$PRESET" "$TARGET"
elif [[ " ${PRESETS[*]} " =~ " ${INPUT} " ]]; then
  build_preset "$INPUT"
else
  echo "❌ Error: unknown input '$INPUT'"
  print_usage
  exit 1
fi
