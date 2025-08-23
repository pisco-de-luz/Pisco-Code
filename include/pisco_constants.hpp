#pragma once

#include "pisco_types.hpp"

namespace pisco_code
{
    // Loop timing constants
    constexpr Timestamp LOOP_INTERVAL_MS       = 64;
    constexpr Timestamp INIT_PHASE_MS          = 1900;
    constexpr Timestamp INIT_DIMMED_PHASE_MS   = 1300;
    constexpr Timestamp END_DIMMED_PHASE_MS    = 300;
    constexpr Timestamp NEGATIVE_BLINK_LONG_MS = 1100;
    constexpr Timestamp SHORT_BLINK_MS         = 350;
    constexpr Timestamp MEDIUM_BLINK_MS        = 500;
    constexpr Timestamp LONG_BLINK_MS          = 1100;
    constexpr Timestamp EXTRA_LONG_BLINK_MS    = 1300;
    constexpr Timestamp ZERO_DIGIT_BLINK_MS    = 440;
    constexpr Timestamp BETWEEN_BLINK_MS       = 350;
    constexpr Timestamp BETWEEN_DIGITS_MS      = 1200;
    constexpr Timestamp BETWEEN_CODES_MS       = 1500;
    constexpr Timestamp END_PHASE_MS           = 2000;

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

    constexpr PhaseDuration to_phase_duration(Timestamp milliseconds) noexcept
    {
        return static_cast<PhaseDuration>(milliseconds / LOOP_INTERVAL_MS);
    }

    struct BaseLimits
    {
        NumDigits  max_digits;
        SignalCode max_value; // maximum |code| allowed for this base
    };

    // Index 0..16, fill only supported bases
    constexpr BaseLimits LIMITS_LUT[17] = {
        {0,         0},
        {0,         0},
        {8,       255}, // BIN  2^8 - 1
        {0,         0},
        {0,         0},
        {0,         0},
        {0,         0},
        {0,         0},
        {9, 134217727}, // OCT  8^9 - 1
        {0,         0},
        {9, 999999999}, // DEC
        {0,         0},
        {0,         0},
        {0,         0},
        {0,         0},
        {0,         0},
        {7, 268435455}  // HEX 16^7 - 1
    };

    // Accessors
    constexpr BaseLimits limits_for(NumberBase base)
    {
        return LIMITS_LUT[to_value(base)];
    }

    constexpr NumDigits max_digits_for_base(NumberBase base)
    {
        return limits_for(base).max_digits;
    }

    constexpr bool base_supported(NumberBase base)
    {
        return max_digits_for_base(base) != 0;
    }

    // Range check using LUT only
    inline bool isCodeInRange(NumberBase base, SignalCode code) noexcept
    {
        const BaseLimits lim = limits_for(base);
        if (lim.max_digits == 0)
        {
            return false;
        }
        return (code >= -lim.max_value) && (code <= lim.max_value);
    }

    // Compile‑time sanity checks
    static_assert(LIMITS_LUT[to_value(NumberBase::BIN)].max_value <= INT32_MAX,
                  "BIN range too large");
    static_assert(LIMITS_LUT[to_value(NumberBase::OCT)].max_value <= INT32_MAX,
                  "OCT range too large");
    static_assert(LIMITS_LUT[to_value(NumberBase::DEC)].max_value <= INT32_MAX,
                  "DEC range too large");
    static_assert(LIMITS_LUT[to_value(NumberBase::HEX)].max_value <= INT32_MAX,
                  "HEX range too large");

    // Compile-time checks to catch omissions
    static_assert(LIMITS_LUT[to_value(NumberBase::BIN)].max_digits > 0,
                  "Missing BIN max digits");
    static_assert(LIMITS_LUT[to_value(NumberBase::OCT)].max_digits > 0,
                  "Missing OCT max digits");
    static_assert(LIMITS_LUT[to_value(NumberBase::DEC)].max_digits > 0,
                  "Missing DEC max digits");
    static_assert(LIMITS_LUT[to_value(NumberBase::HEX)].max_digits > 0,
                  "Missing HEX max digits");

    // Maximum number of digits that could be represented in the system
    constexpr NumDigits MAX_DIGITS_ABSOLUTE = 31; // for int32_t in binary base

    constexpr LedLevel DEFAULT_DIMMED_LEVEL = 3;
    constexpr LedLevel PWM_MAX              = 15;
    constexpr LedLevel MIN_PULSE_DIMMED_GAP = 2;
    constexpr LedLevel DEFAULT_PULSE_LEVEL  = PWM_MAX;
    constexpr NumBits  TIMES_BITS           = 4;
    constexpr NumBits  LEVEL_BITS           = 2;
    constexpr NumBits  DURATION_BITS        = 2;

    constexpr UInt32 bit_mask(NumBits bits) noexcept
    {
        return bits == 0 ? 0U : ((1U << bits));
    }

    // Conversion from ms to loop ticks
    constexpr TickCounter to_loop_count(Timestamp milliseconds) noexcept
    {
        return static_cast<TickCounter>(milliseconds / LOOP_INTERVAL_MS);
    }

} // namespace pisco_code
