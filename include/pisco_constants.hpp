#pragma once

#include "pisco_types.hpp"
namespace pisco_code
{
    constexpr Timestamp LOOP_INTERVAL_MS    = 16U;
    constexpr Timestamp SHORT_BLINK_MS      = 400U;
    constexpr Timestamp MEDIUM_BLINK_MS     = 600U;
    constexpr Timestamp LONG_BLINK_MS       = 1200U;
    constexpr Timestamp EXTRA_LONG_BLINK_MS = 1900U;

    static_assert((SHORT_BLINK_MS / LOOP_INTERVAL_MS) <
                      sizeof(TickCounter) * (MAX_BYTE_VALUE + 1U),
                  "LOOP_INTERVAL_MS too small");
    static_assert((MEDIUM_BLINK_MS / LOOP_INTERVAL_MS) <
                      sizeof(TickCounter) * (MAX_BYTE_VALUE + 1U),
                  "LOOP_INTERVAL_MS too small");
    static_assert((LONG_BLINK_MS / LOOP_INTERVAL_MS) <
                      sizeof(TickCounter) * (MAX_BYTE_VALUE + 1U),
                  "LOOP_INTERVAL_MS too small");
    static_assert((EXTRA_LONG_BLINK_MS / LOOP_INTERVAL_MS) <
                      sizeof(TickCounter) * (MAX_BYTE_VALUE + 1U),
                  "LOOP_INTERVAL_MS too small");

    enum class LedControlCode : LedCodeType
    {
        ON      = 0U,
        OFF     = 1U,
        FUNC_OK = 100U,
    };

    enum class NumberBase : BaseType
    {
        BIN = 2U,
        OCT = 8U,
        DEC = 10U,
        HEX = 16U
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

    constexpr UInt8 to_index(Index value) noexcept
    {
        return static_cast<UInt8>(value);
    }

    constexpr TickCounter to_tick_counter(Timestamp timestamp_ms) noexcept
    {
        return static_cast<TickCounter>(timestamp_ms / LOOP_INTERVAL_MS);
    }

    struct BaseLimits
    {
        NumDigits  max_digits;
        SignalCode max_value; // maximum |code| allowed for this base
    };

    constexpr DigitValue MAX_DIGITS_BIN = 8U;
    constexpr DigitValue MAX_DIGITS_OCT = 9U;
    constexpr DigitValue MAX_DIGITS_DEC = 9U;
    constexpr DigitValue MAX_DIGITS_HEX = 7U;
    constexpr SignalCode MAX_VALUE_BIN  = 255U;       // 2^(8) - 1
    constexpr SignalCode MAX_VALUE_OCT  = 134217727U; // 8^(9) - 1
    constexpr SignalCode MAX_VALUE_DEC  = 999999999U; // 10^(9) - 1
    constexpr SignalCode MAX_VALUE_HEX  = 268435455U; // 16^(7) - 1

    constexpr BaseLimits limits_for(NumberBase base) noexcept
    {
        switch (base)
        {
            case NumberBase::BIN:
                return BaseLimits{MAX_DIGITS_BIN, MAX_VALUE_BIN};
            case NumberBase::OCT:
                return BaseLimits{MAX_DIGITS_OCT, MAX_VALUE_OCT};
            case NumberBase::DEC:
                return BaseLimits{MAX_DIGITS_DEC, MAX_VALUE_DEC};
            case NumberBase::HEX:
                return BaseLimits{MAX_DIGITS_HEX, MAX_VALUE_HEX};
        }
        return BaseLimits{0, 0};
    }
    constexpr uint8_t max_digits_for_base(NumberBase base) noexcept
    {
        return limits_for(base).max_digits;
    }
    inline bool isCodeInRange(NumberBase base, SignalCode code) noexcept
    {
        const BaseLimits limits = limits_for(base);
        if (limits.max_digits == 0)
        {
            return false;
        }
        return (code >= -limits.max_value) && (code <= limits.max_value);
    }

    constexpr bool base_supported(NumberBase base)
    {
        return max_digits_for_base(base) != 0;
    }

    static_assert(base_supported(NumberBase::BIN), "BIN not supported");
    static_assert(base_supported(NumberBase::OCT), "OCT not supported");
    static_assert(base_supported(NumberBase::DEC), "DEC not supported");
    static_assert(base_supported(NumberBase::HEX), "HEX not supported");
    static_assert(limits_for(NumberBase::BIN).max_value == MAX_VALUE_BIN,
                  "BIN max value mismatch");
    static_assert(limits_for(NumberBase::OCT).max_value == MAX_VALUE_OCT,
                  "OCT max value mismatch");
    static_assert(limits_for(NumberBase::DEC).max_value == MAX_VALUE_DEC,
                  "DEC max value mismatch");
    static_assert(limits_for(NumberBase::HEX).max_value == MAX_VALUE_HEX,
                  "HEX max value mismatch");

    constexpr IntensityLevel DEFAULT_BASE_LEVEL = 3U;
    constexpr IntensityLevel PWM_MAX            = 15U;

    // Minimum intensity separation for BASE/PEAK levels to be
    // distinguishable
    constexpr IntensityLevel MIN_INTENSITY_DIFFERENCE = 2U;
    constexpr IntensityLevel DEFAULT_PULSE_LEVEL      = PWM_MAX;
    constexpr NumBits        TIMES_BITS               = 4U;
    constexpr NumBits        MODE_BITS                = 2U;
    constexpr NumBits        DURATION_BITS            = 2U;

    constexpr UInt32 bit_mask(NumBits bits) noexcept
    {
        return bits == 0 ? 0U : ((1U << bits));
    }
} // namespace pisco_code
