# Pisco Code Library Integration Guide

This guide shows how to integrate the Pisco Code library into your project using various methods.

## Quick Start

The Pisco Code library provides two library targets:
- **`PiscoCodeCore`** - Full C++ standard library support (for hosted environments)
- **`PiscoCodeCoreBare`** - Freestanding/bare-metal support (no standard library)

## Requirements

- CMake 3.19 or later
- C++11 compatible compiler
- For AVR: avr-gcc toolchain
- For STM32: arm-none-eabi-gcc toolchain

## Integration Method 1: Download Release Package

Best for: Offline builds, air-gapped systems, safety-critical projects requiring version pinning.

### Step 1: Download
```bash
# From GitHub releases page or:
wget https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gz

# Verify checksum
sha256sum pisco-code-v2.0.0.tar.gz
# Compare with checksums.txt from release

# Extract
tar -xzf pisco-code-v2.0.0.tar.gz -C libs/
```

### Step 2: Add to Your CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.19)
project(MyProject LANGUAGES CXX)

# Add Pisco Code library
add_subdirectory(libs/pisco-code-v2.0.0)

# Link to your executable
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

## Integration Method 2: CMake FetchContent

Best for: Modern CMake projects, automatic dependency management.

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.19)
project(MyProject LANGUAGES CXX)

include(FetchContent)

# Option 1: Fetch from specific release (recommended for reproducible builds)
FetchContent_Declare(
  pisco_code
  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gz
  URL_HASH SHA256=<hash_from_checksums.txt>  # Add for security
)

# Option 2: Fetch from git (always gets latest from branch)
# FetchContent_Declare(
#   pisco_code
#   GIT_REPOSITORY https://github.com/pisco-de-luz/Pisco-Code.git
#   GIT_TAG v2.0.0  # Or main for latest
# )

FetchContent_MakeAvailable(pisco_code)

# Link to your executable
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

## Integration Method 3: Git Submodule

Best for: Active development, contributing back to the library.

### Setup
```bash
# Add as submodule
git submodule add https://github.com/pisco-de-luz/Pisco-Code.git libs/pisco-code
git submodule update --init --recursive

# Clone a project with submodules
git clone --recurse-submodules https://your-repo.git
```

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.19)
project(MyProject LANGUAGES CXX)

# Add Pisco Code library
add_subdirectory(libs/pisco-code)

# Link to your executable
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

## Using PiscoCodeCoreBare (Bare-Metal/Freestanding)

For embedded systems without standard library:

```cmake
add_executable(firmware main.cpp)
target_link_libraries(firmware PRIVATE PiscoCodeCoreBare)

# The bare library includes:
# - Freestanding compilation (-ffreestanding)
# - No exceptions/RTTI
# - Custom memory management shims
# - Minimal runtime dependencies
```

## Configuration Options

Control what gets built when including Pisco Code:

```cmake
# Disable examples (automatically disabled in subproject mode)
set(PISCO_CODE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Disable tests (automatically disabled in subproject mode)
set(PISCO_CODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Then add the library
add_subdirectory(libs/pisco-code)
```

## Example Usage

### Simple Example
```cpp
#include "pisco_code.hpp"

int main() {
    // Create LED controller (implement your HAL functions)
    pisco_code::LedControllerSoftwarePwm led_controller(hal_led_callback);
    
    // Create signal emitter
    pisco_code::SignalEmitter emitter(&led_controller);
    
    // Show code 123 in decimal with 3 digits, repeat 2 times
    emitter.showCode(
        pisco_code::SignalCode{123},
        pisco_code::NumberBase::DEC,
        pisco_code::NumDigits{3},
        pisco_code::RepeatTimes{2}
    );
    
    // Main loop - call every 1ms
    while (emitter.isRunning()) {
        emitter.loop();
        delay_1ms();  // Your platform's 1ms delay
    }
    
    return 0;
}
```

### Cross-Platform Build Example

```cmake
cmake_minimum_required(VERSION 3.19)
project(MyEmbeddedProject)

# Add Pisco Code
add_subdirectory(libs/pisco-code)

if(CMAKE_CROSSCOMPILING)
    # Embedded/bare-metal build
    add_executable(firmware.elf 
        src/main.cpp
        src/hal_led.cpp
        src/systick.cpp
    )
    target_link_libraries(firmware.elf PRIVATE PiscoCodeCoreBare)
else()
    # Native build for testing
    add_executable(test_native 
        src/main.cpp
        tests/mock_hal.cpp
    )
    target_link_libraries(test_native PRIVATE PiscoCodeCore)
endif()
```

## Troubleshooting

### "Cannot find pisco_code.hpp"
- Ensure you linked against `PiscoCodeCore` or `PiscoCodeCoreBare`
- CMake automatically handles include directories via `target_link_libraries`

### Examples/Tests Building in Subproject Mode
- This should NOT happen (auto-disabled)
- If it does, explicitly disable:
  ```cmake
  set(PISCO_CODE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(PISCO_CODE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  ```

### C++ Standard Conflicts
- Pisco Code requires C++11 minimum
- Your project can use any standard >= C++11
- CMake will automatically use the higher standard

### Linker Errors with Bare Library
- Ensure you're using `PiscoCodeCoreBare` for freestanding builds
- Implement required freestanding functions if needed
- Check your linker script includes all sections

## Advanced: Using with Package Managers

### Conan (example)
```python
# conanfile.txt
[requires]
# Add when available on ConanCenter

[generators]
CMakeDeps
CMakeToolchain
```

### vcpkg (example)
```json
{
  "name": "my-project",
  "dependencies": [
    # Add when available in vcpkg registry
  ]
}
```

## Safety-Critical Considerations

For safety-critical applications:

1. **Pin specific versions** - Use release tarballs with checksums
2. **Audit source code** - All source is included in releases
3. **No dynamic allocation** - Library uses static allocation only
4. **No exceptions** - Bare library disables exceptions
5. **Deterministic** - No OS dependencies in bare-metal mode
6. **Minimal dependencies** - C++11 standard only

## Support

- Documentation: [README.md](README.md)
- Issues: https://github.com/pisco-de-luz/Pisco-Code/issues
- Examples: See `examples/` directory in full repository

## License

See [LICENSE](LICENSE) file for details.
