# Pisco Code

Pisco Code is a lightweight mechanism for representing decimal or hexadecimal values using a single status LED or an actuator such as a vibration motor. The method supports both positive and negative values and is designed for use in embedded systems with minimal hardware resources.

# Why

In many embedded projects, especially during early prototyping or field maintenance, there is no convenient way to observe internal variables. Serial interfaces, debugging tools, or displays are not always available.

Pisco Code provides a simple and intuitive method for conveying values through blink patterns or vibrations, allowing engineers and technicians to interpret internal states or diagnostic codes without additional hardware.

# Conventional Approach

A common practice is to use simple blink patterns on a status LED to indicate conditions or values. For example, blinking the LED _n_ times to represent the value _n_.

For larger values, some implementations concatenate digit sequences. For example, the code `312` may be shown as three blinks, a pause, one blink, a pause, and two blinks.

Status LED showing the code `312` using a sequence of blinks.
![code312.png](https://github.com/pisco-de-luz/Pisco-Code/blob/6c91251ed2d3033bc74d414339b4a64d40685235/graphics/code312.png)

This method is workable but limited: it does not naturally support zero digits and may be ambiguous when interpreting sequences such as `302`.

# Proposed Solution

Pisco Code extends the conventional approach by introducing a framing signal. Before transmitting a numeric sequence, the LED is held at a low brightness (or partial duty cycle) to indicate the start of a new sequence. Once the sequence completes, the LED is turned off.

Status LED showing the code `121` using Pisco Code.

![Video-Code-121-132x132.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/d0bb059eef8c726219ba4b1a40f07471f648127f/graphics/Video-Code-121-132x132.gif) 
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/2d7ab53852a28b9fdf24be6b03f43a6450f9fef0/graphics/pisco-code-121.png" height="132">

This framing mechanism improves readability, ensuring that observers can reliably distinguish between digit groups, including zeros, and recognize the start of a new value.

# Representation of Zero

The introduction of a framing signal enables explicit representation of the digit zero, which is often ambiguous in conventional blink-based systems.

For example, in the sequence below representing the value `120`, digits `1` and `2` are shown with one and two bright blinks, separated by pauses. The digit `0` is represented not by a blink, but by an intentional gap in the sequence. This makes it possible to represent zero clearly in any position within the code.

![Pisco-Code-120-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/ba3e80ebc5cc06cf77d8f8e30f36b71e3b5d880f/graphics/Pisco-Code-120-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-120.png" height="144">

In some cases, it is also necessary to define a minimum number of digits to convey information accurately. For example, when displaying a voltage measurement between 0–5 V with two decimal places (e.g., 0.02 V), at least three digits must always be shown. With this convention, the observer can infer the decimal point location.

![Pisco-Code-002-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/b1c607510b4095d7174fb170666a0196a63d295a/graphics/Pisco-Code-002-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-002.png" height="144">

# Binary and Hexadecimal Support

Pisco Code can be extended to represent binary (0–1) or hexadecimal (0–15) values.

* Binary values are represented as sequences containing only two possible digits, which may result in longer sequences for larger numbers.

* Hexadecimal values allow compact representation but may require multiple blinks for a single digit, making interpretation slower.

For decimal use cases, Pisco Code has been successfully applied in the [Pisco de Luz](https://www.piscodeluz.org/?lang=en) project since 2020. The system is used in the field to read operational data such as lighting usage hours, battery or solar voltage, and temperature, without requiring additional display hardware.

# Representation of Negative Values

Some applications require distinguishing between positive and negative values. To address this, Pisco Code introduces a **negative sign indicator**: a long blink at the beginning of the sequence.

After this initial blink, the digit-by-digit representation proceeds as usual. For example, the sequence shown below represents the value `-12`, with the initial long blink indicating the negative sign.  

![Pisco-Code-Negative12-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/347a2f6999becbef7c22bfab5b0d4cb3d843b71c/graphics/Pisco-Code-Negative12-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/d46fea2847a2d3f49e9fccbcebee1c75f28df785/graphics/pisco-code-12-negative.png" height="144">


# Usage

PiscoCode is a target-independent library for LED blink-coding. You provide:

- A 1 ms periodic call to loop()
- A small function to turn your LED on or off when requested

No dynamic memory, no exceptions. Ideal for safety-critical systems.

```C++
#include "avr_systick.hpp"
#include "hal_led.hpp"
#include "pisco_code.hpp"

int main()
{
    avr_systick::init_1ms();
    hal_led::init();

    // LED1: Software PWM controller (on/off toggle for onboard LED)
    pisco_code::LedControllerSoftwarePwm controller_led1(hal_led::ledOnboard);
    pisco_code::SignalEmitter            emitter_led1(&controller_led1);

    // LED2: Hardware PWM controller (smooth dimming for external LED)
    pisco_code::LedControllerHardwarePwm controller_led2(hal_led::ledPwmSetLevel);
    pisco_code::SignalEmitter            emitter_led2(&controller_led2);

    // Start displaying codes
    emitter_led1.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});
    emitter_led2.showCode(SignalCode{-102}, NumberBase::DEC, NumDigits{0});

    // Main loop - call every 1ms
    while (emitter_led1.isRunning() || emitter_led2.isRunning())
    {
        emitter_led1.loop();
        emitter_led2.loop();
        avr_systick::delay_ms(1);
    }

    // Halt
    for (;;) {}
}
```

For detailed CMake integration instructions, see [INTEGRATION.md](INTEGRATION.md).

# AVR Build and Upload Options

The AVR example system supports flexible configuration via CMake variables.
You can override variables such as the target board, programmer type, upload port, and baud rate via command-line arguments.

## Build Configuration Variables

Configure builds by passing variables to CMake:

```bash
# Example: Build with Arduino bootloader programmer
cmake -S . -B build/avr-arduino-nano \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/avr-gcc.cmake \
    -DBOARD=arduino-nano \
    -DEXAMPLES=basic_example \
    -DBOARD_UPLOAD_PROGRAMMER=arduino \
    -DBOARD_UPLOAD_PORT=/dev/ttyUSB0 \
    -DBOARD_UPLOAD_BAUD=57600
```

```bash
# Example: Build with USBasp programmer (default)
cmake -S . -B build/avr-arduino-nano \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/avr-gcc.cmake \
    -DBOARD=arduino-nano \
    -DEXAMPLES=basic_example
```

Or use the convenience script which handles default configuration:

```bash
./scripts/Build.sh avr-arduino-nano
```

## Showing PiscoCodes - The showCode() Method

The `showCode()` method starts displaying a numeric code on the LED. It takes three arguments.

```C++
emitter.showCode(SignalCode{-102}, NumberBase::DEC, NumDigits{0});
```

### Arguments

| Argument       | Type         | Description                                            |
| :---           | :---         | :---                                                   |
| `code`         | `SignalCode` | Signed integer value to be encoded as LED blinks. Valid range depends on base (e.g., `-999999999` to `999999999` for decimal). |
| `base`         | `NumberBase` | Numeric base: `NumberBase::DEC`, `NumberBase::HEX`, `NumberBase::OCT`, or `NumberBase::BIN` |
| `num_digits`   | `NumDigits`  | Minimum number of digits to display. Set to `0` for automatic, or use a value to pad with leading zeros. |

### Return Value

| Return Value | Description                                            |
| :---         | :---                                                   |
| `true`       | Code accepted and will start displaying shortly. |
| `false`      | Emitter is busy with a previous code. Wait for `isRunning()` to return `false`. |

### Repeat Control

To repeat the code multiple times, use `setRepeatTimes()` before calling `showCode()`:

```C++
emitter.setRepeatTimes(RepeatTimes{3});  // Repeat 3 times
emitter.showCode(SignalCode{42}, NumberBase::DEC, NumDigits{0});
```

| Method                              | Description                                  |
| :---                                | :---                                         |
| `setRepeatTimes(RepeatTimes times)` | Sets how many times to repeat the full code sequence. Default is `1`. |
| `getRepeatTimes()`                  | Returns the current repeat count setting. |


### Custom Settings

Customize the brightness levels used during the blink sequence. These methods are called on the **controller**, not the emitter.

| Method                              | Description                                  |
| :---                                | :---                                         |
| `setHighLevel(IntensityLevel level)` | Sets brightness for "on" pulses (blinks). Valid range: `0` to `255`. |
| `setLowLevel(IntensityLevel level)`  | Sets brightness for "base" state between pulses. Valid range: `0` to `255`. |
| `getPeakLevel()`                     | Returns the current high level setting. |
| `getBaseLevel()`                     | Returns the current low level setting. |

#### Default values:

| Constant             | Value | Description |
| :---                 | :---  | :---        |
| `DEFAULT_HIGH_LEVEL` | 200   | Default brightness for blinks |
| `DEFAULT_LOW_LEVEL`  | 50    | Default brightness for base state |

These settings are optional. If not configured, the library uses defaults suitable for most cases.

```C++
pisco_code::LedControllerSoftwarePwm controller(hal_led::ledOnboard);
pisco_code::SignalEmitter            emitter(&controller);

// Customize brightness levels on the CONTROLLER
controller.setHighLevel(180);
controller.setLowLevel(30);

// Set repeat count on the EMITTER  
emitter.setRepeatTimes(RepeatTimes{2});

// Start displaying
emitter.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});
```

## Controller Types

PiscoCode provides two controller types for different LED hardware configurations:

### LedControllerSoftwarePwm

For LEDs controlled via GPIO on/off (software-simulated PWM). The callback receives `LedControlCode::ON` or `LedControlCode::OFF`.

```C++
// Callback signature: bool function(LedControlCode)
bool ledCallback(LedControlCode code) {
    if (code == LedControlCode::ON) {
        // Turn LED on
    } else {
        // Turn LED off
    }
    return true;
}

pisco_code::LedControllerSoftwarePwm controller(ledCallback);
```

### LedControllerHardwarePwm

For LEDs with hardware PWM support. The callback receives an intensity level (0-255).

```C++
// Callback signature: void function(IntensityLevel)
void pwmCallback(IntensityLevel level) {
    // Set PWM duty cycle (0-255)
    TIM2->CCR1 = level;
}

pisco_code::LedControllerHardwarePwm controller(pwmCallback);
```

### Deferred Attachment

Both controllers support deferred attachment for cases where the callback isn't available at construction time:

```C++
pisco_code::LedControllerSoftwarePwm controller;  // Default constructor

// Later, when ready:
controller.attachLedControl(ledCallback);

if (controller.isAttached()) {
    // Ready to use
}
```

## Runtime Control

These methods allow you to monitor and control the execution of LED blink sequences.

| Method         | Description                                  |
| :---           | :---                                         |
| `loop()`       | Must be called exactly once per millisecond. Drives the internal timing and state machine. Non-blocking and fast. |
| `isRunning()`  | Returns `true` if a signal is currently being displayed. |

### Example

```C++
pisco_code::LedControllerSoftwarePwm controller(hal_led::ledOnboard);
pisco_code::SignalEmitter            emitter(&controller);

emitter.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});

while (emitter.isRunning()) {
    emitter.loop();     // Must be called every 1 ms
    delay_1ms();        // Your platform's delay function
}
```

### Notes

* `loop()` advances the LED pattern by 1 ms. You are responsible for calling it at a steady 1 kHz rate (e.g., using `_delay_ms(1)`, `SysTick`, or RTOS task).
* `isRunning()` is useful to wait for the current signal to complete before showing another code or putting the device to sleep.

## Value Limits by Base

| Base  | Max Digits | Value Range |
| :---  | :---       | :---        |
| BIN   | 8          | ±0b11111111 |
| OCT   | 9          | ±0777777777 |
| DEC   | 9          | ±999999999 |
| HEX   | 7          | ±0xFFFFFFF |

## Build and Upload

To build and flash the examples on different targets, use the provided scripts. 

#### For the STM32F410RB board:
```bash
./scripts/Build.sh stm32-f410rb/basic_example
./scripts/Upload.sh stm32-f410rb/basic_example
```

#### For the Arduino Nano:
```bash
./scripts/Build.sh avr-arduino-nano/basic_example
./scripts/Upload.sh avr-arduino-nano/basic_example
```

#### To run all unit tests on the host:
```bash
./scripts/Build.sh native
```

#### To test CMake integration (for developers):
```bash
./scripts/test-cmake-integration.sh
```
This script verifies that the library works correctly in all modes: native builds, cross-compilation (AVR/STM32), and as a subproject.

## Installation

### Download Release Package (Recommended)

1. Go to [Releases](https://github.com/pisco-de-luz/Pisco-Code/releases)
2. Download the latest `pisco-code-vX.X.X.tar.gz` 
3. Extract to your project:

```bash
tar -xzf pisco-code-v1.0.0.tar.gz -C libs/
```

4. Add to your CMakeLists.txt:
```cmake
add_subdirectory(libs/pisco-code-v1.0.0)
target_link_libraries(your_project PRIVATE pisco_code::core)  # For desktop/tests
# OR
target_link_libraries(your_project PRIVATE pisco_code::bare)  # For embedded (AVR, STM32)
```

For detailed CMake integration examples including cross-compilation toolchains, FetchContent usage, and subproject integration, see [INTEGRATION.md](INTEGRATION.md).

### Verify Download
```bash
sha256sum pisco-code-v1.0.0.tar.gz
# Compare with checksums.txt from release
```