# Pisco Code Library — Architecture Overview

## 1. Purpose

Pisco Code encodes numeric values into blink patterns on a single LED (or any
binary/analog actuator). It targets embedded systems where no serial port, display,
or debugger is available in the field.

## 2. Data Flow

A call to `showCode(−103, DEC, 0)` triggers this pipeline:

```
 showCode(−103, DEC, 0)
       │
       ▼
 ┌─────────────────┐
 │ SignalSequencer │  Decomposes −103 into digits, pushes onto stack:
 │  loadSignalCode │    [3] [0] [1] [NEGATIVE]  (bottom → top)
 └────────┬────────┘
          │
          ▼
 ┌─────────────────┐
 │   SignalStack   │  Fixed-capacity LIFO (10 elements, 12 bytes).
 │                 │  Pop order: NEGATIVE, 1, 0, 3
 └────────┬────────┘
          │
          ▼
 ┌─────────────────┐
 │ PulseIterator   │  Wraps each symbol with framing and inter-symbol gaps.
 │  ┌─────────────┐│  Delegates per-element expansion to ElementIterator.
 │  │ElemIterator ││
 │  └─────────────┘│  Output for −103:
 └────────┬────────┘    FRAME · inter · NEG · inter · 1× · inter · gap
          │             · inter · 3×(pulse·gap·pulse·gap·pulse) · inter · FRAME
          ▼
 ┌─────────────────┐
 │  SignalEmitter  │  Drives the sequence one tick at a time via loop().
 │                 │  Each signal element has a mode and duration.
 └────────┬────────┘
          │  setCurrentSignalMode(mode)  →  update()
          ▼
 ┌─────────────────┐
 │ SignalController│  Maps mode → intensity level → hardware action.
 │ (abstract base) │
 ├────────┬────────┤
 │ SwPWM  │ HwPWM  │  Software PWM: ON/OFF toggling at tick resolution.
 └────────┴────────┘  Hardware PWM: direct intensity write (0–255).
          │
          ▼
     HAL callback       User-provided: bool f(LedControlCode) or
          │              void f(IntensityLevel)
          ▼
       Hardware
```

## 3. Signal Encoding

Each digit becomes a `SignalElement` — a 1-byte bitfield:

| Field      | Bits | Purpose                              |
|------------|------|--------------------------------------|
| `mode`     | 2    | GAP, BASE, PEAK, or NOT_DEFINED      |
| `times`    | 4    | Repetition count (0–15)              |
| `duration` | 2    | SHORT, MEDIUM, LONG, or EXTRA_LONG   |

The encoding rules:

| Symbol     | Mode   | Times | Duration | LED Behavior             |
|------------|--------|-------|----------|--------------------------|
| Digit N    | PEAK   | N     | SHORT    | N short blinks           |
| Digit 0    | GAP    | 1     | SHORT    | Brief darkness           |
| Negative   | PEAK   | 1     | LONG     | One long blink           |
| Framing    | GAP    | 1     | LONG     | Long darkness (start/end)|
| Inter-sym  | BASE   | 1     | LONG     | Dim glow between symbols |
| Intra-dig  | BASE   | 1     | SHORT    | Brief dim between pulses |

Intra-digit gaps are only inserted between repeated short PEAK pulses (digits > 1).
This makes individual pulses visually countable.

## 4. Component Responsibilities

### SignalEmitter (public API)

The only class application code interacts with. Owns the sequencer and pulse
iterator. The `loop()` method is called once per tick (typically 1 ms) and drives
the entire pipeline.

- `showCode()` — validates, encodes, and starts a new sequence
- `loop()` — advances the state machine by one tick
- `isRunning()` — true while a sequence is in progress
- Rejects `showCode()` while a sequence is already playing (`isBusy()` guard)

### SignalSequencer

Owns the `SignalStack` and repeat configuration. Converts a numeric code into a
stack of `SignalElement`s via `loadSignalCode()`.

- `repeat_count_` is user configuration (survives `clear()`)
- `repeat_index_` is progress state (reset by `clear()`)

### SignalStack

Fixed-capacity LIFO with rewind support. Push writes forward, pop reads backward,
`rewind()` resets the read pointer without clearing data. This allows the same
sequence to be replayed for repeat cycles.

Capacity is 10 elements (max 9 digits + 1 sign). The stack is trivially copyable
(12 bytes) and passed by value into `SignalPulseIterator`.

### SignalPulseIterator

Walks the full signal sequence: leading frame → (inter-symbol → symbol) × N →
trailing frame. For each symbol popped from the stack, it delegates pulse expansion
to an embedded `SignalElementIterator`.

### SignalElementIterator

Expands a single `SignalElement` into its constituent pulses. For a digit like 3
(mode=PEAK, times=3, duration=SHORT), it emits: pulse, gap, pulse, gap, pulse.
Gaps are only inserted between PEAK+SHORT repetitions — all other combinations
emit times × the element with no gaps.

### SignalController (abstract base)

Maps the current signal mode (PEAK, BASE, GAP) to an intensity level. Owns the
high/low intensity configuration with clamping logic that enforces a minimum
separation between levels (so blinks remain visually distinguishable).

Two concrete implementations:
- **SoftwarePwm** — toggles ON/OFF via a software PWM cycle (16 ticks per period)
- **HardwarePwm** — writes the intensity level directly to a timer PWM register

Both delegate actual hardware access to a user-provided callback.

## 5. Ownership and Lifecycle

```
SignalEmitter
 ├── owns SignalSequencer
 │    └── owns SignalStack
 ├── owns SignalPulseIterator (copy of stack)
 │    └── owns SignalElementIterator
 └── references SignalController (injected, not owned)
      └── holds function pointer to HAL callback
```

- `SignalEmitter` is constructed with a reference to a `SignalController`.
  The controller must outlive the emitter.
- `SignalPulseIterator` copies the stack by value (12 bytes) when created.
  This decouples iteration from the sequencer's state.
- All objects are stack-allocated. No heap, no exceptions, no RTTI.

## 6. Constraints and Invariants

| Constraint | Enforced by |
|---|---|
| Stack never exceeds 10 elements | `isFull()` guard in `push()` |
| `SignalElement` is exactly 1 byte | `static_assert` |
| `SignalStack` ≤ 16 bytes | `static_assert` (pass-by-value budget) |
| High − Low ≥ 32 intensity units | `levelsAreTooClose()` in setters |
| Peak level ≥ 32 | `clamp_peak_level()` |
| Base level ≤ 223 | `clamp_base_level()` |
| Enums fit their bitfields | `static_assert` on `SignalMode` and `SignalDuration` |
| Code within radix range | `is_valid_code_for_radix()` guard in `loadSignalCode()` |
| One sequence at a time | `isBusy()` guard in `showCode()` |

## 7. Timing Model

`loop()` is called once per tick (1 ms in typical usage). It maintains an internal
tick counter that resets on each `showCode()` call.

Signal durations map to tick counts:

| Duration       | Ticks (ms) |
|----------------|------------|
| SHORT          | 400        |
| MEDIUM         | 600        |
| LONG           | 1,200      |
| EXTRA_LONG     | 1,900      |

Phase transitions occur when the elapsed ticks exceed the current phase duration
**and** the controller reports `readyForPhaseChange()`. For software PWM, this
synchronizes transitions to PWM cycle boundaries (every 16 ticks) to avoid
mid-cycle glitches.

## 8. Design Notes

### 8.1 Three-Layer State Machine Instead of One Flat One

The design uses three nested FSMs, each responsible for exactly one level of the
signal structure:

```
SignalEmitter              (time-driven,      3 states)
  IDLE ──► STARTING ──► APPLYING_PULSE ──► IDLE
  │
  └─ SignalPulseIterator  (iterator-driven, 4 states + flag)
          LEADING_FRAME ──► IN_ELEMENTS ──► TRAILING_FRAME ──► DONE
                                │
                  need_inter_symbol_ flag injects INTER_SYMBOL
                  before each transition out of IN_ELEMENTS
       │
       └─ SignalElementIterator  (counter-driven, 2 states — Phase : bool)
                   PULSE ⇄ GAP
```

Each FSM only knows about its own level. `SignalEmitter` holds just three states:
`IDLE` when nothing is playing, `STARTING` to latch the first pulse, and
`APPLYING_PULSE` while ticking through each phase — it delegates all sequence logic
to `SignalPulseIterator`. `SignalPulseIterator` handles framing, inter-symbol gaps,
and symbol ordering — it
delegates pulse repetition and intra-digit gaps to `SignalElementIterator`. The
two-state counter at the bottom is the smallest meaningful machine for the job, and
using a `bool` rather than an enum for those two states is the right call on
memory-constrained MCUs.

The natural first idea is a single flat machine — one switch covering every state
from leading frame to trailing frame. It works, but "are we in the third pulse of a
digit?" and "are we between symbols?" live at different abstraction levels, and
mixing them makes each state harder to reason about independently.

Keeping the three layers separate pays off in:

1. **Testability.** `SignalElementIterator` can be tested in isolation — gap
   insertion rules, `times=0`, reset behavior — without any stack or sequencer
   setup. `SignalPulseIterator` tests verify sequence structure without touching
   timing. With a flat machine all of that becomes integration testing.
2. **No net simplification from merging.** A flat machine still needs the same
   state variables: phase within the full sequence, remaining pulses for the current
   element, and whether to insert an intra-digit gap. Nothing is removed, just
   folded into one place where the interactions are harder to see.

### 8.2 SignalSequencer as a Class

`loadSignalCode()` could be a free function returning a `SignalStack`, but the
sequencer also manages repeat state (`repeat_count_` vs `repeat_index_`), which
survives a `clear()` while progress state does not. Keeping that distinction inside
the sequencer avoids pushing two unrelated responsibilities into `SignalEmitter`.

### 8.3 Hand-Rolled Stack

The stack needs fixed capacity, no heap, LIFO order, rewind (replay without
clearing), and trivial copyability (passed by value into the iterator). No STL or
ETL container satisfies all five, so a small custom one made more sense than
wrapping something that almost fits.

## 9. Test Strategy

Tests run on the host (native) using CppUTest. The mock stack:

```
MockLedControllerAdapter (extends SignalController)
  └─ records ON/OFF events into MockLedControlLogger
      └─ reconstructs LedBlinkPattern from event stream
          └─ generates trace strings for assertion
```

Test levels:
- **Unit tests** — `SignalStack`, `SignalElementIterator`, `SignalSequencer`
  tested in isolation
- **Integration tests** — `SignalPulseIterator` tests verify the full iteration
  chain against expected element sequences
- **System tests** — `SignalEmitter` drives a mock controller through the
  complete pipeline; trace strings are compared against expected patterns
- **Systematic tests** — parametric tests cover all bases (BIN/OCT/DEC/HEX)
  with sequential, same-digit, and padded patterns up to max digits

## 10. Resource Footprint

Measured with `-Os`, no exceptions, no RTTI. These are `sizeof` values from actual
target compilers — not host (x86_64) builds, which inflate sizes due to 8-byte
pointers and alignment padding.

### RAM per Object

`SignalEmitter` contains the sequencer and pulse iterator, so its size includes
both. The total RAM per LED is: **emitter + controller**.

| Object                      | AVR (bytes) | ARM Cortex-M4 (bytes) |
|-----------------------------|------------:|----------------------:|
| SignalEmitter               |          68 |                    72 |
| — SignalSequencer           |          33 |                    33 |
| — SignalPulseIterator       |          19 |                    19 |
| — — SignalStack             |          12 |                    12 |
| — — SignalElement           |           1 |                     1 |
| LedControllerSoftwarePwm   |           8 |                    16 |
| LedControllerHardwarePwm   |           7 |                    12 |
| **Total (Emitter + SW PWM)** |      **76** |                **88** |
| **Total (Emitter + HW PWM)** |      **75** |                **84** |

ARM objects are larger due to 4-byte function pointers (vs 2-byte on AVR) and
alignment padding.

### Flash

| Target                       | Text  | Data | BSS |
|------------------------------|------:|-----:|----:|
| AVR ATmega328p               | 2,846 |   54 |   — |
| ARM Cortex-M4 (STM32-F410RB) | 2,648 |    4 |   4 |
