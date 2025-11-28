#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------------------
# Upload.sh — Unified uploader using CMake custom upload targets
# Usage: Upload.sh <config>/<example> [--dry-run]
# -----------------------------------------------------------------------------

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

# --- Logging ---
function say()  { echo -e "\033[1;36m$1\033[0m"; }
function good() { echo -e "\033[1;32m$1\033[0m"; }
function fail() { echo -e "\033[1;31m$1\033[0m"; }

# --- Args ---
if [[ $# -lt 1 || $# -gt 2 ]]; then
  fail "❌ Usage: ./scripts/Upload.sh <config>/<example> [--dry-run]"
  say  "Example: ./scripts/Upload.sh avr-arduino-nano/basic_example"
  exit 1
fi

INPUT="$1"
DRY_RUN="${2:-}"

if [[ "$INPUT" != */* ]]; then
  fail "❌ Invalid format. Use: <config>/<example>"
  exit 1
fi

CONFIG="${INPUT%%/*}"
TARGET="${INPUT##*/}"
UPLOAD_TARGET="${TARGET}_upload"
BUILD_DIR="build/$CONFIG"

say "🚀 Uploading '$TARGET' using config '$CONFIG'"
say "📁 Build folder: $BUILD_DIR"
say "🎯 CMake target: $UPLOAD_TARGET"

if [[ "$DRY_RUN" == "--dry-run" ]]; then
  say "🧪 Dry run mode — skipping actual upload"
  exit 0
fi

cmake --build "$BUILD_DIR" --target "$UPLOAD_TARGET" --parallel || {
  fail "❌ Upload failed."
  exit 1
}

good "✅ Upload completed successfully."
