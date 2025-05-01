#!/usr/bin/env bash
set -euo pipefail

#------------------------------------------------------------------------------
# Build.sh — Unified build script using CMake presets
# Usage: Build.sh <preset|all>
#------------------------------------------------------------------------------

# Determine script directory (project root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Define available CMake presets
readonly PRESETS=(
  "native"
  "avr"
)

# Helper: print usage
print_usage() {
  echo "Usage: $(basename "$0") [preset|all]"
  echo
  echo "Available presets:"
  for preset in "${PRESETS[@]}"; do
    echo "   $preset"
  done
  echo
  echo "Special:"
  echo "   all  Configure & build all presets in sequence"
}

# Helper: configure & build a preset
build_preset() {
  local preset="$1"
  echo "[${preset}] Configuring..."
  cmake --preset "$preset"
  echo "[${preset}] Building..."
  cmake --build --preset "$preset"
  echo "[${preset}] Done."
}

# Validate arguments
if [[ $# -ne 1 ]]; then
  print_usage
  exit 1
fi

case "$1" in
  all)
    for p in "${PRESETS[@]}"; do
      build_preset "$p"
    done
    ;;

  # match a single preset
  *)
    for p in "${PRESETS[@]}"; do
      if [[ "$1" == "$p" ]]; then
        build_preset "$1"
        exit 0
      fi
    done
    echo "Error: unknown preset '$1'"
    echo
    print_usage
    exit 1
    ;;
esac