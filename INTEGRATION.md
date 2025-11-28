# Pisco Code Library Integration Guide# Pisco Code Library Integration# Pisco Code Library Integration# Pisco Code Library Integration Guide



## Quick Start



Add this to your `CMakeLists.txt`:## Quick Start



```cmake

# Choose your version

set(PISCO_CODE_VERSION "v1.0.1" CACHE STRING "Pisco Code version to use")Add this to your `CMakeLists.txt`:## Quick Start (Recommended)This guide shows how to integrate the Pisco Code library into your project using various methods.



include(FetchContent)

FetchContent_Declare(pisco_code

  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz```cmake

  # URL_HASH SHA256=<hash>  # Add hash from release checksums.txt for verification

)# Choose your version

FetchContent_MakeAvailable(pisco_code)

set(PISCO_CODE_VERSION "v1.0.1" CACHE STRING "Pisco Code version to use")Add this to your `CMakeLists.txt`:## Quick Start

target_link_libraries(your_project PRIVATE PiscoCodeCore)

```



## Requirementsinclude(FetchContent)



- CMake 3.14 or laterFetchContent_Declare(pisco_code

- C++11 compatible compiler (library uses C++17 internally but only requires C++11 from consumers)

  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz```cmakeThe Pisco Code library provides two library targets:

## Available Targets

  # URL_HASH SHA256=<hash>  # Add hash from release checksums.txt for verification

| Target | Description | Use Case |

|--------|-------------|----------|)include(FetchContent)- **`PiscoCodeCore`** - Full C++ standard library support (for hosted environments)

| `PiscoCodeCore` | Full C++ standard library support | Desktop, Linux, hosted environments |

| `PiscoCodeCoreBare` | Freestanding mode (`-ffreestanding`) | Bare-metal, AVR, STM32, embedded |FetchContent_MakeAvailable(pisco_code)



## Version Pinning with Hash VerificationFetchContent_Declare(pisco_code- **`PiscoCodeCoreBare`** - Freestanding/bare-metal support (no standard library)



For production/safety-critical applications, pin to a specific version with hash verification:# Link to your target



```cmaketarget_link_libraries(your_target PRIVATE PiscoCodeCore)  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gz

set(PISCO_CODE_VERSION "v1.0.1" CACHE STRING "Pisco Code version")

# Or for bare-metal: target_link_libraries(your_target PRIVATE PiscoCodeCoreBare)

include(FetchContent)

FetchContent_Declare(pisco_code```  # URL_HASH SHA256=<hash>  # Uncomment and add hash from release checksums.txt for verification## Requirements

  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz

  URL_HASH SHA256=abc123...  # Get from release checksums.txt

)

FetchContent_MakeAvailable(pisco_code)That's it! CMake will automatically:)

```

- Download the release package (cached in `build/_deps/`)

## Bare-Metal / Embedded Example

- Build only the library (examples/tests are auto-disabled)FetchContent_MakeAvailable(pisco_code)- CMake 3.19 or later

```cmake

cmake_minimum_required(VERSION 3.14)- Make headers available to your project

project(MyEmbeddedProject LANGUAGES C CXX)

- C++11 compatible compiler

set(CMAKE_CXX_STANDARD 17)

## Available Targets

set(PISCO_CODE_VERSION "v1.0.1")

# Link to your target- For AVR: avr-gcc toolchain

include(FetchContent)

FetchContent_Declare(pisco_code| Target | Use Case |

  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz

)|--------|----------|target_link_libraries(your_target PRIVATE PiscoCodeCore)- For STM32: arm-none-eabi-gcc toolchain

FetchContent_MakeAvailable(pisco_code)

| `PiscoCodeCore` | Hosted environments (Linux, Windows, tests) |

add_executable(firmware main.cpp)

target_link_libraries(firmware PRIVATE PiscoCodeCoreBare)| `PiscoCodeCoreBare` | Bare-metal/freestanding (AVR, STM32, embedded) |# Or for bare-metal: target_link_libraries(your_target PRIVATE PiscoCodeCoreBare)

```



## What Gets Propagated to Your Project

## Requirements```## Integration Method 1: Download Release Package

The library is designed to be non-invasive:



| Property | Propagation | Notes |

|----------|-------------|-------|- CMake 3.14 or later

| Include directories | PUBLIC | Required for headers |

| C++11 minimum | PUBLIC | Minimum language requirement |- C++11 compatible compiler

| Compile warnings | PRIVATE | Not forced on consumers |

| Optimization flags | PRIVATE | Not forced on consumers |That's it! CMake will automatically:Best for: Offline builds, air-gapped systems, safety-critical projects requiring version pinning.

| Link flags | PRIVATE | Not forced on consumers |

## Complete Example

## Troubleshooting

- Download the release package (only once, cached in `build/_deps/`)

### "PiscoCodeCore target not found"

Ensure `FetchContent_MakeAvailable(pisco_code)` is called before `target_link_libraries()`.```cmake



### Compiler errors about C++ standardcmake_minimum_required(VERSION 3.14)- Verify checksum if provided### Step 1: Download

The library requires C++11 minimum. Set your project's standard:

```cmakeproject(MyBlinkProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # or 11, 14, 20, etc.

```- Build only the library (examples/tests are auto-disabled)```bash



### Hash verification fails# --- Pisco Code Library ---

Download the `checksums.txt` file from the release and use the correct SHA256 hash for your version.

set(PISCO_CODE_VERSION "v1.0.1")- Make headers available to your project# From GitHub releases page or:



include(FetchContent)wget https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gz

FetchContent_Declare(pisco_code

  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz## Available Targets

)

FetchContent_MakeAvailable(pisco_code)# Verify checksum



# --- Your Application ---| Target | Use Case |sha256sum pisco-code-v2.0.0.tar.gz

add_executable(my_firmware main.cpp hal_led.cpp)

target_link_libraries(my_firmware PRIVATE PiscoCodeCoreBare)|--------|----------|# Compare with checksums.txt from release

```

| `PiscoCodeCore` | Hosted environments (Linux, macOS, Windows, tests) |

### Example main.cpp

| `PiscoCodeCoreBare` | Bare-metal/freestanding (AVR, STM32, embedded) |# Extract

```cpp

#include "pisco_code.hpp"tar -xzf pisco-code-v2.0.0.tar.gz -C libs/



int main() {## Version Selection```

    pisco_code::LedControllerSoftwarePwm led(my_led_callback);

    pisco_code::SignalEmitter emitter(&led);

    

    emitter.showCode(SignalCode{42}, NumberBase::DEC, NumDigits{0});Change the URL to use a different version:### Step 2: Add to Your CMakeLists.txt

    

    while (emitter.isRunning()) {```cmake

        emitter.loop();

        delay_1ms();  // Your platform's delay```cmakecmake_minimum_required(VERSION 3.19)

    }

}# For version 2.1.0:project(MyProject LANGUAGES CXX)

```

URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.1.0/pisco-code-v2.1.0.tar.gz

## Offline/Air-Gapped Systems

```# Add Pisco Code library

For builds without internet access:

add_subdirectory(libs/pisco-code-v2.0.0)

1. Download the release tarball

2. Place in your project (e.g., `deps/`)## Requirements

3. Use local path:

# Link to your executable

```cmake

set(PISCO_CODE_VERSION "v1.0.1")- CMake 3.19 or lateradd_executable(my_app main.cpp)



FetchContent_Declare(pisco_code- C++11 compatible compilertarget_link_libraries(my_app PRIVATE PiscoCodeCore)

  URL ${CMAKE_CURRENT_SOURCE_DIR}/deps/pisco-code-${PISCO_CODE_VERSION}.tar.gz

  URL_HASH SHA256=<hash_from_checksums.txt>- For AVR: avr-gcc toolchain```

)

```- For ARM: arm-none-eabi-gcc toolchain



## Troubleshooting## Integration Method 2: CMake FetchContent



**"Cannot find pisco_code.hpp"**  ## Example Project

Ensure you linked against the library:

```cmakeBest for: Modern CMake projects, automatic dependency management.

target_link_libraries(your_target PRIVATE PiscoCodeCore)

``````cmake



**Bare-metal build errors**  cmake_minimum_required(VERSION 3.19)### CMakeLists.txt

Use `PiscoCodeCoreBare` for freestanding builds.

project(MyBlinkProject LANGUAGES CXX)```cmake

**C++ standard**  

Pisco Code requires C++11 minimum. Your project can use C++11, 14, 17, 20, etc.cmake_minimum_required(VERSION 3.19)



## Licenseinclude(FetchContent)project(MyProject LANGUAGES CXX)



MIT License - See [LICENSE](LICENSE)FetchContent_Declare(pisco_code


  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gzinclude(FetchContent)

)

FetchContent_MakeAvailable(pisco_code)# Option 1: Fetch from specific release (recommended for reproducible builds)

FetchContent_Declare(

add_executable(my_firmware main.cpp hal_led.cpp)  pisco_code

target_link_libraries(my_firmware PRIVATE PiscoCodeCoreBare)  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v2.0.0/pisco-code-v2.0.0.tar.gz

```  URL_HASH SHA256=<hash_from_checksums.txt>  # Add for security

)

```cpp

// main.cpp# Option 2: Fetch from git (always gets latest from branch)

#include "pisco_code.hpp"# FetchContent_Declare(

#   pisco_code

int main() {#   GIT_REPOSITORY https://github.com/pisco-de-luz/Pisco-Code.git

    pisco_code::LedControllerSoftwarePwm led(my_led_callback);#   GIT_TAG v2.0.0  # Or main for latest

    pisco_code::SignalEmitter emitter(&led);# )

    

    emitter.showCode(SignalCode{42}, NumberBase::DEC, NumDigits{0});FetchContent_MakeAvailable(pisco_code)

    

    while (emitter.isRunning()) {# Link to your executable

        emitter.loop();add_executable(my_app main.cpp)

        delay_1ms();target_link_libraries(my_app PRIVATE PiscoCodeCore)

    }```

}

```## Integration Method 3: Git Submodule



## Offline/Air-Gapped SystemsBest for: Active development, contributing back to the library.



For systems without internet access during build:### Setup

```bash

1. Download the release tarball manually# Add as submodule

2. Place it in your project (e.g., `deps/pisco-code-v2.0.0.tar.gz`)git submodule add https://github.com/pisco-de-luz/Pisco-Code.git libs/pisco-code

3. Use a local path:git submodule update --init --recursive



```cmake# Clone a project with submodules

FetchContent_Declare(pisco_codegit clone --recurse-submodules https://your-repo.git

  URL ${CMAKE_CURRENT_SOURCE_DIR}/deps/pisco-code-v2.0.0.tar.gz```

  URL_HASH SHA256=<hash_from_checksums.txt>

)### CMakeLists.txt

``````cmake

cmake_minimum_required(VERSION 3.19)

## Troubleshootingproject(MyProject LANGUAGES CXX)



### "Cannot find pisco_code.hpp"# Add Pisco Code library

Ensure you're linking against the library:add_subdirectory(libs/pisco-code)

```cmake

target_link_libraries(your_target PRIVATE PiscoCodeCore)# Link to your executable

```add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE PiscoCodeCore)

### Build errors with bare-metal```

Use `PiscoCodeCoreBare` instead of `PiscoCodeCore` for freestanding builds.

## Using PiscoCodeCoreBare (Bare-Metal/Freestanding)

### Want to use a specific C++ standard

Pisco Code requires C++11 minimum. Your project can use any standard >= C++11:For embedded systems without standard library:

```cmake

set(CMAKE_CXX_STANDARD 17)  # Your choice```cmake

```add_executable(firmware main.cpp)

target_link_libraries(firmware PRIVATE PiscoCodeCoreBare)

## License

# The bare library includes:

See [LICENSE](LICENSE) file.# - Freestanding compilation (-ffreestanding)

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
