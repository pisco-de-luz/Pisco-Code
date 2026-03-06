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
        ON  = 0U,
        OFF = 1U,
    };

    enum class Radix : RadixType
    {
        BIN = 2U,
        OCT = 8U,
        DEC = 10U,
        HEX = 16U
    };

    using NumberBase
        [[deprecated("Use Radix instead — will be removed in v2.0.0")]] = Radix;

    constexpr DigitValue to_value(Radix radix) noexcept
    {
        return static_cast<DigitValue>(radix);
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

    constexpr UnsignedCode to_unsigned(SignalCode code) noexcept
    {
        return static_cast<UnsignedCode>((code < 0) ? -code : code);
    }

    constexpr IntensityLevel SW_PWM_LEVEL_FACTOR = 16U;
    constexpr IntensityLevel to_sw_pwm_level(IntensityLevel level) noexcept
    {
        return ((level / SW_PWM_LEVEL_FACTOR) + 0U);
    }

    struct RadixLimits
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

    constexpr RadixLimits limits_for(Radix radix) noexcept
    {
        switch (radix)
        {
            case Radix::BIN:
                return RadixLimits{MAX_DIGITS_BIN, MAX_VALUE_BIN};
            case Radix::OCT:
                return RadixLimits{MAX_DIGITS_OCT, MAX_VALUE_OCT};
            case Radix::DEC:
                return RadixLimits{MAX_DIGITS_DEC, MAX_VALUE_DEC};
            case Radix::HEX:
                return RadixLimits{MAX_DIGITS_HEX, MAX_VALUE_HEX};
            default:
                return RadixLimits{0, 0};
        }
    }
    constexpr uint8_t max_digits_for_radix(Radix radix) noexcept
    {
        return limits_for(radix).max_digits;
    }
    constexpr bool is_code_in_range(Radix radix, SignalCode code) noexcept
    {
        const RadixLimits limits = limits_for(radix);
        if (limits.max_digits == 0)
        {
            return false;
        }
        return (code >= -limits.max_value) && (code <= limits.max_value);
    }

    constexpr bool radix_supported(Radix radix) noexcept
    {
        return max_digits_for_radix(radix) != 0;
    }

    constexpr bool is_valid_code_for_radix(Radix radix, SignalCode code) noexcept
    {
        return radix_supported(radix) && is_code_in_range(radix, code);
    }

    static_assert(radix_supported(Radix::BIN), "BIN not supported");
    static_assert(radix_supported(Radix::OCT), "OCT not supported");
    static_assert(radix_supported(Radix::DEC), "DEC not supported");
    static_assert(radix_supported(Radix::HEX), "HEX not supported");
    static_assert(limits_for(Radix::BIN).max_value == MAX_VALUE_BIN,
                  "BIN max value mismatch");
    static_assert(limits_for(Radix::OCT).max_value == MAX_VALUE_OCT,
                  "OCT max value mismatch");
    static_assert(limits_for(Radix::DEC).max_value == MAX_VALUE_DEC,
                  "DEC max value mismatch");
    static_assert(limits_for(Radix::HEX).max_value == MAX_VALUE_HEX,
                  "HEX max value mismatch");

    // Minimum intensity separation for BASE/PEAK levels to be
    // distinguishable
    constexpr NumBits        TIMES_BITS               = 4U;
    constexpr NumBits        MODE_BITS                = 2U;
    constexpr NumBits        DURATION_BITS            = 2U;
    constexpr UInt8          MAX_BYTE_VALUE           = 255U;
    constexpr IntensityLevel PWM_MAX                  = 15U;
    constexpr IntensityLevel DEFAULT_BASE_LEVEL       = 50U;
    constexpr IntensityLevel DEFAULT_PEAK_LEVEL       = 200U;
    constexpr IntensityLevel MIN_INTENSITY_DIFFERENCE = 32U;
    constexpr IntensityLevel MIN_PEAK_LEVEL = MIN_INTENSITY_DIFFERENCE;
    constexpr IntensityLevel MAX_BASE_LEVEL =
        MAX_BYTE_VALUE - MIN_INTENSITY_DIFFERENCE;
    constexpr UInt32 capacity_for_bits(NumBits bits) noexcept
    {
        return bits == 0 ? 0U : ((1U << bits));
    }
} // namespace pisco_code
