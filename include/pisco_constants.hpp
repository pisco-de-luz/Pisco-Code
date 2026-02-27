#pragma once

#include "pisco_types.hpp"
namespace pisco_code
{
    constexpr Timestamp SHORT_BLINK_MS      = 400U;
    constexpr Timestamp MEDIUM_BLINK_MS     = 600U;
    constexpr Timestamp LONG_BLINK_MS       = 1200U;
    constexpr Timestamp EXTRA_LONG_BLINK_MS = 1900U;

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

    constexpr Index to_index(UInt32 value) noexcept
    {
        return static_cast<Index>(value);
    }

    constexpr IntensityLevel SW_PWM_LEVEL_FACTOR = 16U;
    constexpr IntensityLevel to_sw_pwm_level(IntensityLevel level) noexcept
    {
        return ((level / SW_PWM_LEVEL_FACTOR) + 0U);
    }

    struct BaseLimits
    {
        NumDigits  max_digits;
        SignalCode max_value;
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
            default:
                return BaseLimits{0, 0};
        }
    }
    constexpr uint8_t max_digits_for_base(NumberBase base) noexcept
    {
        return limits_for(base).max_digits;
    }
    constexpr bool is_code_in_range(NumberBase base, SignalCode code) noexcept
    {
        const BaseLimits limits = limits_for(base);
        if (limits.max_digits == 0)
        {
            return false;
        }
        return (code >= -limits.max_value) && (code <= limits.max_value);
    }

    constexpr bool base_supported(NumberBase base) noexcept
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

    // Minimum intensity separation for BASE/PEAK levels to be
    // distinguishable
    constexpr NumBits        TIMES_BITS               = 4U;
    constexpr NumBits        MODE_BITS                = 2U;
    constexpr NumBits        DURATION_BITS            = 2U;
    constexpr UInt8          MAX_BYTE_VALUE           = 255;
    constexpr IntensityLevel PWM_MAX                  = 15U;
    constexpr IntensityLevel DEFAULT_LOW_LEVEL        = 50U;
    constexpr IntensityLevel DEFAULT_HIGH_LEVEL       = 200U;
    constexpr IntensityLevel MIN_INTENSITY_DIFFERENCE = 32U;
    constexpr IntensityLevel MIN_HIGH_LEVEL = MIN_INTENSITY_DIFFERENCE;
    constexpr IntensityLevel MAX_LOW_LEVEL =
        MAX_BYTE_VALUE - MIN_INTENSITY_DIFFERENCE;
    constexpr UInt32 bit_mask(NumBits bits) noexcept
    {
        return bits == 0 ? 0U : ((1U << bits));
    }
} // namespace pisco_code
