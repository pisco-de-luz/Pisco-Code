# Pisco Code Library — Architecture Overview

---

## 1. Problem Statement

- Embedded systems often have **no serial port, no display, no debugger** in the field
- The classic limitation: a single status LED is **binary** — on or off
- Pisco Code solves this: encode any numeric value into a readable blink pattern

---

## 2. Concept

- Pisco Code encodes decimal / hex / octal / binary values into **blink patterns**
- A **framing signal** (low-brightness prefix) solves the *"where does the sequence start?"* problem
- Works on LEDs, vibration motors, buzzers — any binary or analog actuator
- Example: `showCode(121, DEC, 0)` blinks out the value **121** on a single LED

---

## 3. Architecture

```
┌─────────────────────────────────┐
│         Application Code        │  showCode(123, DEC, 0)
├─────────────────────────────────┤
│         SignalEmitter           │  Orchestrator / Public API
├─────────────────────────────────┤
│        SignalSequencer          │  Sequence & repeat logic
├─────────────────────────────────┤
│         SignalStack             │  Digit decomposition
├─────────────────────────────────┤
│       SignalController          │  Abstract base (Template Method)
├──────────────┬──────────────────┤
│  SoftwarePwm │  HardwarePwm     │  Concrete controllers
├──────────────┴──────────────────┤
│        HAL Callback             │  User-provided function
├─────────────────────────────────┤
│         Hardware (LED)          │
└─────────────────────────────────┘
```

Key source files:

| Component | File |
|---|---|
| Public API | `src/signal_emitter.cpp` |
| Timing & repeat | `src/signal_sequencer.cpp` |
| Digit decomposition | `src/signal_stack.cpp` |
| Base controller | `src/signal_controller.cpp` |
| Software PWM | `src/led_controller_software_pwm.cpp` |
| Hardware PWM | `src/led_controller_hardware_pwm.cpp` |

---

## 4. Controller Types

| Controller | Callback Signature | Use Case |
|---|---|---|
| `LedControllerSoftwarePwm` | `bool f(LedControlCode)` | GPIO on/off, simple LEDs |
| `LedControllerHardwarePwm` | `void f(IntensityLevel)` | Timer PWM, 0–255 intensity |

- HAL is fully **user-provided** — the library never touches hardware directly
- Same library binary, different platform examples:
  - `examples/avr/hal_led.hpp`
  - `examples/stm32/hal_led.hpp`

---

## 5. Type System & Constants

- Semantic types: `IntensityLevel`, `NumDigits`, `SignalCode`, `Timestamp`
- `NumberBase` enum: `BIN`, `OCT`, `DEC`, `HEX`
- Compile-time validation via `static_assert`
- `constexpr` dispatch — `limits_for(base)` → zero runtime cost
- `BaseLimits` struct carries max digits and max value per base

```cpp
constexpr BaseLimits limits_for(NumberBase base) noexcept; // fully constexpr

static_assert(base_supported(NumberBase::DEC), "DEC not supported");
static_assert(limits_for(NumberBase::HEX).max_value == MAX_VALUE_HEX, "mismatch");
```

---

## 6. Timing Model

```cpp
while (emitter.isRunning()) {
    emitter.loop();   // Called every 1 ms
    delay_1ms();
}
```

- **No dynamic memory**, **no exceptions**, **no threads**
- `loop()` is a pure time-driven state machine — deterministic and predictable
- Signal durations defined as `constexpr Timestamp`:

| Constant | Value |
|---|---|
| `SHORT_BLINK_MS` | 400 ms |
| `MEDIUM_BLINK_MS` | 600 ms |
| `LONG_BLINK_MS` | 1200 ms |
| `EXTRA_LONG_BLINK_MS` | 1900 ms |

---

## 7. Supported Bases & Encoding

| Base | Max Digits | Max Value |
|---|---|---|
| `BIN` | 8 | 255 |
| `OCT` | 9 | 134,217,727 |
| `DEC` | 9 | 999,999,999 |
| `HEX` | 7 | 268,435,455 |

- Supports **negative values** (sign is encoded in the signal)
- `NumDigits{0}` → auto-detect digit count from the value

---

## 8. Test Architecture

- Framework: **CppUTest** (git submodule in `external/`)
- Systematic coverage per base: decimal, hex, octal, binary
- Mock-based: `MockLedControllerAdapter` + `MockLedControlLogger`
- Pattern validation: `LedBlinkPattern` — trace-based verification
- Tests run on **host (native)** — no hardware required

```
tests/
  decimal_systematic_high_level_tests.cpp
  hexadecimal_systematic_high_level_tests.cpp
  binary_systematic_high_level_tests.cpp
  octal_systematic_high_level_tests.cpp
  signal_stack_test.cpp
  signal_sequencer_test.cpp
  ...
```

---

## 9. CMake Integration

| Target | Use Case |
|---|---|
| `pisco_code::core` | Desktop, Linux, unit tests |
| `pisco_code::bare` | AVR, STM32, bare-metal |

- Works as **FetchContent**, **subproject**, or **installed package** (see `INTEGRATION.md`)
- Consumer controls all MCU flags — no hidden flags from the library
- Cross-compile tested: **AVR** (ATmega328P) and **STM32** (F410RB)

---

## 10. Usage Example

```cpp
// Declare controller with a user HAL callback
pisco_code::LedControllerSoftwarePwm controller(hal_led::ledOnboard);
pisco_code::SignalEmitter             emitter(controller);

// Show value 123 in decimal, auto digit count
emitter.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});

// Drive from a 1 ms ISR or main loop
while (emitter.isRunning()) {
    emitter.loop();
    delay_1ms();
}
```

- Run tests locally: `./scripts/Build.sh native`
