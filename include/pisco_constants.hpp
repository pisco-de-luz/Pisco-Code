#pragma once

#include "pisco_types.hpp"

namespace pisco_code
{

    enum class LedControlCode : LedCodeType
    {
        ON        = 0,
        OFF       = 1,
        FUNC_OK   = 100,
        FUNC_FAIL = 101 // optional, used in Mock logger
    };

    // Encoding bases
    enum class NumberBase : BaseType
    {
        BIN = 2,
        OCT = 8,
        DEC = 10,
        HEX = 16
    };

    constexpr DigitValue to_value(NumberBase base) noexcept
    {
        return static_cast<DigitValue>(base);
    }

    constexpr LedCodeType to_value(LedControlCode led_control_code) noexcept
    {
        return static_cast<LedCodeType>(led_control_code);
    }

    constexpr DigitValue to_digit(UInt32 value) noexcept
    {
        return static_cast<DigitValue>(value);
    }

    constexpr Counter to_count(UInt32 value) noexcept
    {
        return static_cast<Counter>(value);
    }

    constexpr UInt8 to_index(Index value) noexcept
    {
        return static_cast<UInt8>(value);
    }

    constexpr PhaseDuration to_phase_duration(UInt32 value) noexcept
    {
        return static_cast<PhaseDuration>(value);
    }

    // Lookup table indexed by base value (2 to 16), all other entries are zero
    constexpr NumDigits MAX_DIGITS_LUT[to_value(NumberBase::HEX) + 1] = {
        0,  0,
        24, // 2 = BIN
        0,  0, 0, 0, 0,
        9, // 8 = OCT
        0, // 9
        9, // 10 = DEC
        0,  0, 0, 0, 0,
        7 // 16 = HEX
    };

    constexpr NumDigits max_digits_for_base(NumberBase base)
    {
        return MAX_DIGITS_LUT[to_value(base)];
    }

    // Compile-time checks to catch omissions
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::BIN)] > 0,
                  "Missing BIN max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::OCT)] > 0,
                  "Missing OCT max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::DEC)] > 0,
                  "Missing DEC max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::HEX)] > 0,
                  "Missing HEX max digits");

    // Maximum number of digits that could be represented in the system
    constexpr NumDigits MAX_DIGITS_ABSOLUTE = 31; // for int32_t in binary base

    constexpr LedLevel DEFAULT_DIMMED_LEVEL = 3;
    constexpr LedLevel PWM_MAX              = 15;
    constexpr LedLevel MIN_PULSE_DIMMED_GAP = 2;
    constexpr LedLevel DEFAULT_PULSE_LEVEL  = PWM_MAX;

    // Loop timing constants
    constexpr Timestamp LOOP_INTERVAL_MS       = 64;
    constexpr Timestamp INIT_PHASE_MS          = 2000;
    constexpr Timestamp INIT_DIMMED_PHASE_MS   = 1300;
    constexpr Timestamp END_DIMMED_PHASE_MS    = 300;
    constexpr Timestamp NEGATIVE_BLINK_LONG_MS = 1100;
    constexpr Timestamp SHORT_BLINK_MS         = 350;
    constexpr Timestamp ZERO_DIGIT_BLINK_MS    = 440;
    constexpr Timestamp BETWEEN_BLINK_MS       = 350;
    constexpr Timestamp BETWEEN_DIGITS_MS      = 1200;
    constexpr Timestamp BETWEEN_CODES_MS       = 1500;
    constexpr Timestamp END_PHASE_MS           = 2000;

    // Conversion from ms to loop ticks
    constexpr LoopCounter to_loop_count(Timestamp milliseconds) noexcept
    {
        return static_cast<LoopCounter>(milliseconds / LOOP_INTERVAL_MS);
    }

} // namespace pisco_code
