# Changelog

All notable changes to Pisco Code are documented in this file.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Versioning follows [Semantic Versioning](https://semver.org/).

---

## [Unreleased]

### Added
- `isBusy()` method to `SignalEmitter` for improved state management
- `clampHighLevel()` and `clampLowLevel()` methods to `SignalController`
- `isValidCodeForBase()` input validation in `loadSignalCode`
- `levelsAreTooClose()` validation method in `SignalController`
- `shouldAdvancePulse()` helper method in `SignalEmitter`
- Architecture Overview documentation (`ArchitectureOverview.md`)
- Resource Footprint section in README with AVR / ARM size comparison

### Changed
- `SignalEmitter` constructor now accepts `SignalController&` (reference) instead of a pointer — eliminates null state by design
- `SignalElement` changed from `struct` to `class` with explicit access specifiers (MISRA C++ 2023 Rule 14.1.1)
- `noexcept` added to all non-throwing functions (C++ Core Guidelines F.6)
- `is_code_in_range` made `constexpr` and renamed for consistency (was `isCodeInRange`)
- Removed `static` from `static constexpr` members in headers (`constexpr` implies internal linkage)
- `using` declarations moved from global scope into `namespace pisco_code` (C++ Core Guidelines SF.7)
- Removed unused two-phase initialisation API (`attachLedControl()`, `isAttached()`, default constructors)
- Unused constructors deleted from LED controller classes
- Improved `SignalPulseIterator::next()` clarity and phase handling
- Simplified `phaseElapsed()` in `SignalEmitter`
- Simplified intensity level retrieval in both PWM controller `update()` methods
- Consolidated systematic test files into a single data-driven test file
- Refactored blinker tests to use a shared `BlinkerTestFixture`

### Fixed
- Removed unused member variables from `SignalSequencer`
- Removed unused `SignalElement` member

---

## [1.0.2] — 2025-01-xx

### Added
- Hardware PWM LED controller (`LedControllerHardwarePwm`) — true PWM duty-cycle output
- Support for a second GPIO/PWM port in AVR and STM32 examples
- Toolchain and linker script documentation improved
- CMake minimum required version bumped to 3.14

### Changed
- C++ standard updated to C++20 for hosted (native) targets
- README code examples and integration instructions updated
- AVR and STM32 examples refactored to demonstrate both controller types
- ASM language enablement moved to CMakeLists.txt for STM32 examples

---

## [1.0.1] — 2024-xx-xx

### Added
- CMake subproject mode: examples and tests are disabled automatically when included via `add_subdirectory()`
- `INTEGRATION.md` — detailed CMake integration guide (FetchContent, subproject, cross-compile)
- CMake integration test script (`scripts/test-cmake-integration.sh`)
- GitHub Actions release workflow: packages `include/` + `src/` as `.tar.gz` / `.zip` on version tags
- Release artifacts include `checksums.txt` (SHA-256)

### Changed
- C++ standard set to C++17 for hosted (native) targets
- `CMakeLists.txt` version and date fields updated; glob patterns replaced with explicit file lists
- Include directory variable names standardised across CMake files
- Forced `-gc-sections` linker option removed from static library targets

---

## [1.0.0] — 2024-xx-xx

### Added
- Core Pisco Code encoding: decimal, binary, and hexadecimal values via LED blink patterns
- Framing signal to unambiguously mark the start of a new digit sequence
- Support for zero digits in any position within the sequence
- Negative value representation via long-blink prefix
- Configurable minimum digit count (e.g. always show 3 digits for 0–5 V sensor readings)
- `SignalController` base class with Template Method pattern
- `LedControllerSoftwarePwm` — software PWM controller for standard digital GPIO
- `SignalEmitter`, `SignalSequencer`, `SignalStack`
- `SignalElement`, `SignalElementIterator`, `SignalPulseIterator`
- `library.properties` for Arduino Library Manager compatibility
- AVR (Arduino Nano) and STM32 (Nucleo-F410RB) example sketches
- CppUTest-based unit test suite (native build)
- MIT License

[Unreleased]: https://github.com/pisco-de-luz/Pisco-Code/compare/v1.0.2...HEAD
[1.0.2]: https://github.com/pisco-de-luz/Pisco-Code/compare/v1.0.1...v1.0.2
[1.0.1]: https://github.com/pisco-de-luz/Pisco-Code/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/pisco-de-luz/Pisco-Code/releases/tag/v1.0.0
