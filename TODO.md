# TODO - Pisco Code Library

## High Priority
- [x] Split `signal_controller.hpp` into header/implementation files
- [x] Remove `TickCounter` abstraction, use `Timestamp` (uint32) everywhere
- [x] Change intensity levels to full 0-255 range (`IntensityLevel` type)
- [ ] Remove level setter/getters from `SignalEmitter` (delegate to controller)
- [x] Rename `tick_timestamp_` to `current_ts_`
- [x] Create `HardwarePwmLedController` class
- [x] Improve PWM LED turn-off behavior (disconnect PWM for true OFF)

## Medium Priority
- [x] Add input validation to `SignalController` setters 
- [x] Implement `getCurrentIntensityLevel()` in base class to avoid duplication
- [x] Refactor PWM logic in `update()` methods for cleaner code
- [x] Add `isAttached()` method to controllers
- [x] Move repeat logic to `SignalSequencer` class
- [x] Separate `setRepeatTimes()` from `showCode()` method
- [x] Add getter methods for peak and base levels
- [ ] Create comprehensive unit tests for new controller architecture

## Low Priority / Future
- [ ] Add sound signal controller
- [ ] Add vibration signal controller  
- [ ] Performance optimization for embedded systems
- [ ] Documentation improvements
- [ ] Create examples for different hardware platforms

## Architecture Decisions Made
- [x] Use Template Method Pattern for `SignalController`
- [x] Separate PWM-specific logic from base controller
- [x] Implement setter/getter methods directly in base class
- [x] Remove duplicate state between classes
- [x] Use consistent type system (`Counter`, `Index`, `IntensityLevel`)
- [p] Rename terminology: "dimmed" → "base", "pulse" → "peak"
- [x] Change `SignalLevel` to `SignalMode` for clarity

## Code Quality Improvements Made
- [x] Refactor `SignalElement` constructor parameters
- [p] Update constants with 'U' suffix for consistency
- [x] Rename `MIN_PULSE_BASE_GAP` to `MIN_INTENSITY_DIFFERENCE`
- [x] Improve type safety with `NumDigits`, `Index`, `Counter` types
- [x] Remove unused type aliases (`Index8`, `Index16`, etc.)

## Hardware Abstraction Layer
- [x] Add second GPIO port with hardware PWM support
- [x] Implement HAL for both onboard and PWM LEDs
- [x] Update examples to use both controller types for avr target
- [ ] Update examples to use both controller types for stm32 target

## Testing & Validation
- [x] Add tests for `SignalSequencer` class
- [x] Refactor tests to use proper type safety (`NumDigits`)
- [x] Clean up duplicate test code in `LedBlinkPattern`
- [x] Update `SignalPulseIterator` tests with improved logic
- [x] Add `CODE_NEG_103` test constant

## Documentation & Organization
- [p] API reference documentation
- [ ] Usage examples for different controller types
- [ ] Porting guide for new hardware platforms
- [ ] Performance characteristics documentation

## Safety-Critical Considerations
- [ ] Implement fail-safe behavior for invalid controller states
- [p] Add static analysis tool configuration
- [ ] Create safety validation test suite
- [ ] Add error handling for hardware failures

## Next Release Goals
- [ ] Complete removal of `SignalEmitter` level methods
- [ ] Add comprehensive documentation
- [ ] Create safety validation test suite
- [ ] Performance benchmarking on target hardware

---
[x] = Completed
[p] = Partially Completed

## Recent Major Refactors Completed ✅
- **Timing System**: Simplified from `TickCounter` to direct `Timestamp` usage
- **Type System**: Consistent use of semantic types (`Counter`, `Index`, `IntensityLevel`)
- **Controller Architecture**: Clean separation between interface and PWM implementation
- **Hardware Support**: Added hardware PWM controller alongside software PWM
- **Code Quality**: Major cleanup of unused code, improved naming, type safety