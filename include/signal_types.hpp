#pragma once
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{
    constexpr Counter MAX_SIGNAL_ELEMENTS = 10U;

    enum class SignalMode : SignalModeType
    {
        NOT_DEFINED = 0,
        GAP,
        BASE,
        PEAK,
        MAX_VALUE,
    };

    // Signal duration (mapped to ms by higher layers)
    enum class SignalDuration : SignalDurationType
    {
        SHORT = 0,
        MEDIUM,
        LONG,
        EXTRA_LONG,
        MAX_VALUE,
    };

    static_assert(static_cast<unsigned>(SignalMode::MAX_VALUE) <=
                      capacity_for_bits(MODE_BITS),
                  "SignalMode fits MODE_BITS");
    static_assert(static_cast<unsigned>(SignalDuration::MAX_VALUE) <=
                      capacity_for_bits(DURATION_BITS),
                  "SignalDuration fits DURATION_BITS");
    static_assert(static_cast<unsigned>(MAX_DIGIT_VALUE) <
                      capacity_for_bits(TIMES_BITS),
                  "Max digit value fits TIMES_BITS");

    constexpr SignalModeType to_value(SignalMode mode) noexcept
    {
        return static_cast<SignalModeType>(mode);
    }

    constexpr SignalDurationType
    to_value(SignalDuration signal_duration) noexcept
    {
        return static_cast<SignalDurationType>(signal_duration);
    }

    constexpr SignalTimesType to_value(SignalTimesType signal_times) noexcept
    {
        return (signal_times);
    }
} // namespace pisco_code