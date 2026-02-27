# TODO - Pisco Code Library

## High Priority
- [x] Split `signal_controller.hpp` into header/implementation files
- [x] Remove `TickCounter` abstraction, use `Timestamp` (uint32) everywhere
- [x] Change intensity levels to full 0-255 range (`IntensityLevel` type)
- [x] Remove level setter/getters from `SignalEmitter` (delegate to controller)
- [x] Rename `tick_timestamp_` to `current_ts_`
- [x] Create `LedControllerHardwarePwm` class
- [x] Improve PWM LED turn-off behavior (disconnect PWM for true OFF)

## Medium Priority
- [x] Add input validation to `SignalController` setters 
- [x] Implement `getCurrentIntensityLevel()` in base class to avoid duplication
- [x] Refactor PWM logic in `update()` methods for cleaner code
- [x] ~~Add `isAttached()` method to controllers~~ — Removed: two-phase init was never used; only the parameterised constructor remains
- [x] Move repeat logic to `SignalSequencer` class
- [x] Separate `setRepeatTimes()` from `showCode()` method
- [x] Add getter methods for peak and base levels
- [ ] Create comprehensive unit tests for new controller architecture

## Low Priority / Future
- [ ] Replace `limits_for(base)` `constexpr switch` with a policy template `BaseLimits<NumberBase::DEC>` for zero-overhead compile-time dispatch
- [ ] Add sound signal controller
- [ ] Add vibration signal controller  
- [ ] Performance optimization for embedded systems
- [ ] Documentation improvements

## Architecture Decisions Made
- [x] Use Template Method Pattern for `SignalController`
- [x] Separate PWM-specific logic from base controller
- [x] Implement setter/getter methods directly in base class
- [x] Remove duplicate state between classes
- [x] Use consistent type system (`Counter`, `Index`, `IntensityLevel`)
- [x] Rename terminology: "dimmed" → "base" → "low", "pulse" → "peak" → "high"
- [x] Change `SignalLevel` to `SignalMode` for clarity

## Code Quality Improvements Made
- [x] Refactor `SignalElement` constructor parameters
- [ ] Rename `bit_mask()` to `capacity_for_bits()` — current name is misleading; the function computes the maximum number of values that fit in N bits, not a bitmask
- [ ] Express `DEFAULT_HIGH_LEVEL`, `DEFAULT_LOW_LEVEL` and `MIN_INTENSITY_DIFFERENCE` as percentages of `MAX_BYTE_VALUE` instead of hardcoded absolute values
- [ ] Update constants with 'U' suffix for consistency
- [x] Rename `MIN_PULSE_BASE_GAP` to `MIN_INTENSITY_DIFFERENCE`
- [x] Improve type safety with `NumDigits`, `Index`, `Counter` types
- [x] Remove unused type aliases (`Index8`, `Index16`, etc.)

## Hardware Abstraction Layer
- [x] Add second GPIO port with hardware PWM support
- [x] Implement HAL for both onboard and PWM LEDs
- [x] Update examples to use both controller types for avr target
- [x] Update examples to use both controller types for stm32 target

## Testing & Validation
- [x] Add tests for `SignalSequencer` class
- [x] Refactor tests to use proper type safety (`NumDigits`)
- [x] Clean up duplicate test code in `LedBlinkPattern`
- [x] Update `SignalPulseIterator` tests with improved logic
- [x] Add `CODE_NEG_103` test constant

## Documentation & Organization
- [ ] API reference documentation
- [ ] Porting guide for new hardware platforms
- [ ] Performance characteristics documentation

## C++ Core Guidelines & MISRA Compliance
- [x] Move `using` declarations from global scope into `namespace pisco_code` in headers (SF.7)
- [x] Add `noexcept` to all non-throwing functions (C++ Core Guidelines F.6)
- [x] Remove unused two-phase initialisation API (`attachLedControl()`, `isAttached()`, default constructors) (C.41, C.45)
- [x] Change `SignalEmitter` constructor from `SignalController*` to `SignalController&` — eliminate null state by design (I.12)
- [x] Add `= delete` for `decltype(nullptr)` on LED controller constructors — prevent literal-null construction (I.12)
- [x] Remove `static` from `static constexpr` in headers — `constexpr` already implies internal linkage (DCL.S)
- [x] Change `struct SignalElement` to `class` with explicit access specifier (MISRA C++ 2023 Rule 14.1.1)
- [ ] Consider ALL_CAPS naming for enumerators and constants (naming consistency)

## Safety-Critical Considerations
- [ ] Implement fail-safe behavior for invalid controller states
- [ ] Add static analysis tool configuration
- [ ] Create safety validation test suite
- [ ] Add error handling for hardware failures

## Code Duplication Reduction
### High Priority
- [x] **#1** Collapse 4 systematic test files (`binary_`, `octal_`, `decimal_`, `hexadecimal_systematic_high_level_tests.cpp`) into 1 data-driven file — they are structurally identical, differing only in `NumberBase` (~240 redundant lines)
- ~~**#2**~~ Keep `generate_pattern_of_digits_test.cpp` verbose — each test has unique expected data; macro would hurt debuggability
- [x] **#3** Extract shared `BlinkerTestFixture` via `TEST_GROUP_BASE` for the `logger`/`controller`/`blinker` trio — reduces API-change touch-points from 7 to 1

### Medium Priority
- [ ] **#4** ~~Extract software PWM tick logic so `MockLedControllerAdapter` doesn't duplicate `LedControllerSoftwarePwm::update()` (~10 duplicated lines)~~ — Skipped: intentional mock pattern, not harmful duplication
- [x] **#5** ~~Make `MockLedControllerAdapter` call base class `setHighLevel`/`setLowLevel` instead of re-implementing clamping; remove duplicate in `LedBlinkPattern`~~ — Removed shadow members + dead setters (~50 lines removed)
- [ ] **#6** Extract shared example `app_main()` template so AVR/STM32 `basic_example.cpp` aren't 81% identical (~38 redundant lines)

### Low Priority
- [ ] **#7** Move identical `hal_led.hpp` to shared `examples/common/` — both platform copies are byte-for-byte identical (17 lines × 2)
- [ ] **#8** Extract `verifyCodeMinus103Iteration()` helper — assertion sequence is copy-pasted between `signal_pulse_iterator_test.cpp` and `signal_sequencer_test.cpp` (~50 duplicated lines)
- [ ] **#9** Unify `code_blinker_test.cpp` standalone mock with shared `MockLedControllerAdapter`; migrate from `new`/`delete` to stack allocation
- [x] **#10** Fix include path inconsistency in `loop_behavior_blinker_test.cpp` — now uses `helpers/`/`mocks/` prefixes via fixture

## Next Release Goals
- [x] Complete removal of `SignalEmitter` level methods
- [ ] Add comprehensive documentation
- [ ] Create safety validation test suite
- [ ] Performance benchmarking on target hardware

---

## Recent Major Refactors Completed ✅
- **Timing System**: Simplified from `TickCounter` to direct `Timestamp` usage
- **Type System**: Consistent use of semantic types (`Counter`, `Index`, `IntensityLevel`)
- **Controller Architecture**: Clean separation between interface and PWM implementation
- **Hardware Support**: Added hardware PWM controller alongside software PWM
- **Code Quality**: Major cleanup of unused code, improved naming, type safety
- **Null Safety**: `SignalEmitter` now takes a `SignalController&` reference (not pointer); LED controllers reject `nullptr` at compile time via `= delete`
- **noexcept**: All non-throwing public and private functions marked `noexcept`
- **Header Hygiene**: `using` declarations scoped inside `namespace pisco_code`, no leakage into user translation units