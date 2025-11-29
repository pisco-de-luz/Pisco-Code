# Pisco Code Library Integration Guide

This guide shows how to integrate the Pisco Code library into your project.

## Quick Start (Hosted Environments)

Add this to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(PISCO_CODE_VERSION "v1.0.2" CACHE STRING "Pisco Code version")

include(FetchContent)
FetchContent_Declare(pisco_code
  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/${PISCO_CODE_VERSION}/pisco-code-${PISCO_CODE_VERSION}.tar.gz
  # URL_HASH SHA256=<hash>  # Add from release checksums.txt for verification
)
FetchContent_MakeAvailable(pisco_code)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

## Requirements

- CMake 3.14 or later
- C++17 compatible compiler

## Available Targets

| Target | Alias | Description | Use Case |
|--------|-------|-------------|----------|
| `PiscoCodeCore` | `pisco_code::core` | Full C++ standard library | Desktop, Linux, unit tests |
| `PiscoCodeBare` | `pisco_code::bare` | Freestanding mode | Bare-metal, AVR, STM32, embedded |

## Bare-Metal / Embedded Integration

For embedded targets (AVR, STM32, etc.), use `PiscoCodeBare` with a proper toolchain file.

**Important**: Your toolchain file must set MCU-specific flags in `CMAKE_CXX_FLAGS_INIT` so they apply to all targets including the library.

### Example: ARM Cortex-M Toolchain

```cmake
# toolchain-arm-cortex-m4.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# MCU-specific flags - applied to ALL targets
set(ARM_MCU "cortex-m4" CACHE STRING "ARM MCU type")
set(ARM_FPU "fpv4-sp-d16" CACHE STRING "ARM FPU type")
set(ARM_FLOAT_ABI "hard" CACHE STRING "ARM float ABI")

set(MCU_FLAGS "-mcpu=\${ARM_MCU} -mthumb -mfpu=\${ARM_FPU} -mfloat-abi=\${ARM_FLOAT_ABI}")

# Embedded C++ flags - no exceptions, no RTTI, size optimization
set(EMBEDDED_CXX_FLAGS "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit")
set(SIZE_FLAGS "-ffunction-sections -fdata-sections -fno-common")

set(CMAKE_C_FLAGS_INIT "\${MCU_FLAGS} \${SIZE_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "\${MCU_FLAGS} \${EMBEDDED_CXX_FLAGS} \${SIZE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,--gc-sections -specs=nano.specs -specs=nosys.specs")
```

### Example: AVR Toolchain

```cmake
# toolchain-avr.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# MCU-specific flags
set(AVR_MCU "atmega328p" CACHE STRING "AVR MCU type")
set(AVR_F_CPU "16000000UL" CACHE STRING "CPU frequency")

set(MCU_FLAGS "-mmcu=\${AVR_MCU}")
set(EMBEDDED_CXX_FLAGS "-fno-exceptions -fno-rtti -fno-threadsafe-statics")
set(SIZE_FLAGS "-ffunction-sections -fdata-sections")

set(CMAKE_C_FLAGS_INIT "\${MCU_FLAGS} \${SIZE_FLAGS} -DF_CPU=\${AVR_F_CPU}")
set(CMAKE_CXX_FLAGS_INIT "\${MCU_FLAGS} \${EMBEDDED_CXX_FLAGS} \${SIZE_FLAGS} -DF_CPU=\${AVR_F_CPU}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,--gc-sections")
```

### Consumer CMakeLists.txt for Embedded

```cmake
cmake_minimum_required(VERSION 3.14)
project(EmbeddedApp LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(pisco_code
  URL https://github.com/pisco-de-luz/Pisco-Code/releases/download/v1.0.2/pisco-code-v1.0.2.tar.gz
)
FetchContent_MakeAvailable(pisco_code)

add_executable(firmware main.cpp)
target_link_libraries(firmware PRIVATE PiscoCodeBare)

# Your linker script
target_link_options(firmware PRIVATE -T\${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
```

Build with:
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-cortex-m4.cmake
cmake --build build
```

## Architecture Notes

The Pisco Code library follows a **separation of concerns** design:

| Responsibility | Owner |
|----------------|-------|
| Source code, headers, C++ standard | Library |
| MCU architecture flags, embedded C++ ABI | Toolchain file |
| Linker scripts, board definitions | Consumer project |

This design ensures:
- **Consumer control**: You decide compilation flags for your safety requirements
- **No hidden flags**: Library doesn't inject unexpected compiler options
- **Reproducible builds**: Same source + same toolchain = same binary

## Integration Methods

### Method 1: FetchContent (Recommended)

See Quick Start above. Best for modern CMake projects with internet access.

### Method 2: add_subdirectory

```cmake
# Download and extract release package to libs/pisco-code/
add_subdirectory(libs/pisco-code)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

Best for: Offline builds, vendored dependencies, air-gapped systems.

### Method 3: Git Submodule

```bash
git submodule add https://github.com/pisco-de-luz/Pisco-Code.git external/pisco-code
```

```cmake
add_subdirectory(external/pisco-code)
target_link_libraries(my_app PRIVATE PiscoCodeCore)
```

## Troubleshooting

### "undefined reference to __cxa_pure_virtual"

Your toolchain needs `-fno-rtti` and possibly `-fno-exceptions`. Add to `CMAKE_CXX_FLAGS_INIT` in your toolchain file.

### Large binary size on embedded

Ensure your toolchain sets:
- `-ffunction-sections -fdata-sections` (compile flags)
- `-Wl,--gc-sections` (linker flags)

### Tests not building

Tests only build for native (x86_64) builds. Cross-compilation automatically disables tests.
