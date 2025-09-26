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
#include "pisco_code.hpp"
using namespace pisco_code;

// LED glue for your board
bool board_led(LedCodeType c) noexcept {
    switch (static_cast<LedControlCode>(c)) {
        case LedControlCode::ON:      /* turn LED on  */ return true;
        case LedControlCode::OFF:     /* turn LED off */ return true;
        case LedControlCode::FUNC_OK:                    return true;
        default:                                         return false;
    }
}

int main() {
    SoftwarePwmLedController controller_led1(board_led);
    SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setDimmedLevel(3);

    const SignalCode  signal_code{-102};
    const NumDigits   num_digits{0};
    const RepeatTimes repeats{3};

    emitter_led1.showCode(signal_code, NumberBase::DEC, num_digits, repeats);
    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        avr_systick::delay_ms(1);
    }
}
```


# AVR Build and Upload Options (CMakePresets.json)

The AVR example system supports flexible configuration through `CMakePresets.json`.
You can override variables such as the target board, programmer type, upload port, and baud rate — either by modifying the preset directly or via the command line.

## Example preset (for Arduino as ISP)
```json
{
  "name": "avr-arduino-nano",
  "binaryDir": "build/avr-arduino-nano",
  "generator": "Unix Makefiles",
  "cacheVariables": {
    "CMAKE_TOOLCHAIN_FILE": "cmake/toolchains/avr-gcc.cmake",
    "BOARD": "arduino-nano",
    "EXAMPLES": "basic_example",
    "AVR_UPLOAD_PROGRAMMER": "stk500v1",
    "AVR_UPLOAD_PORT": "/dev/ttyACM0",
    "AVR_UPLOAD_BAUD": "19200"
  }
}
```

## Example preset (for USBasp)
```json
{
  "name": "avr-arduino-nano",
  "binaryDir": "build/avr-arduino-nano",
  "generator": "Unix Makefiles",
  "cacheVariables": {
    "CMAKE_TOOLCHAIN_FILE": "cmake/toolchains/avr-gcc.cmake",
    "BOARD": "arduino-nano",
    "EXAMPLES": "basic_example",
    "AVR_UPLOAD_PROGRAMMER": "usbasp"
  }
}
```

## Showing PiscoCodes, calling showCode() Function

The showCode() function needs two arguments, the "code-to-show" and the "base" to use. It will return three possible status codes.
 ```C++
    const SignalCode  signal_code{-102};
    const NumDigits   num_digits{0};
    const RepeatTimes repeats{3};
    emitter_led1.showCode(signal_code, NumberBase::DEC, num_digits, repeats);
 ```

### Arguments

| Argument               | Description                                            |
| ---:                   | :---                                                   |
| `signal_code`            | Signed integer value to be encoded as LED blinks. Valid range: `-999999999` to `999999999`.  |
| `base`               | Numeric base used to display the code. Use one of: `NumberBase::DEC`, `NumberBase::HEX`, `NumberBase::OCT` or `NumberBase::BIN` |
| `num_digits`   | Minimum number of digits to display. If set to `0`, the digit count is determined automatically. Use this to pad with leading zeros when needed. |
| `repeats`      | Number of times the full code should be repeated. Set to `1` for a single display, or any positive number. |

### Status Code Returned

| Status Code            | Description                                            |
| ---:                   | :---                                                   |
| TRUE                   | This function accepted the code-to-show and base passed as arguments and will start showing the code shortly. |
| FALSE       | This function is still working in the previous code and can not accept any new tasks. |


### Custom Settings
You can customize the brightness levels used during the blink sequence using the following methods:

| Method         | Desciption                                  |
| :---           | :---                                        |
| `setPeakLevel(LedLevel level)`   | Sets the LED brightness level for "peak" pulses (i.e., when a bit is active). Use this to adjust the maximum intensity of blinks. Valid range: `0` (off) to `PWM_MAX` (full brightness). |
| `setDimmedLevel(LedLevel level)` | Sets the LED brightness for "dimmed" intervals between pulses. Use this to reduce contrast or prevent full-off between blinks. Valid range: `0` to value lower than or equal to `setPeakLevel()`. |

#### Default values:

- PeakLevel = PWM_MAX
- DimmedLevel = DEFAULT_DIMMED_LEVEL (typically a low but visible level)

These settings are optional. If not configured, the library uses defaults suitable for most cases.

```C++
SoftwarePwmLedController controller_led1(hal_led::Led1);
SignalEmitter            emitter_led1(&controller_led1);

emitter_led1.setPeakLevel(10);
emitter_led1.setDimmedLevel(3);
```

## Runtime Control

These methods allow you to monitor and control the execution of LED blink sequences.

| Method         | Desciption                                  |
| :---           | :---                                        |
| `loop()`       | Must be called exactly once per millisecond. Drives the internal timing and state machine. If not called regularly, the LED signal timing will break. This method is non-blocking and fast. |
| `isRunning()`  | Returns `true` if a signal is currently being shown on the LED. Use this to check whether the current code has finished displaying or is still in progress. |

### Example

```C++
blinker.showCode(123, NumberBase::DEC, 0, 1);

while (blinker.isRunning()) {
    blinker.loop();     // must be called every 1 ms
    delay_1ms();        // implement this with a timer or busy wait
}
```

### Notes:

* `loop()` advances the LED pattern by 1 ms. You are responsible for calling it at a steady 1 kHz rate (e.g., using `_delay_ms(1)`, `SysTick`, or RTOS task).
* `isRunning()` is useful to wait for the current signal to complete before showing another code or putting the device to sleep.

### Build and Upload

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

These commands are backed by the `CMakePresets.json` file, which defines toolchains and build configurations for each supported target.
