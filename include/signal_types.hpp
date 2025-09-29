#pragma once
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{
    constexpr Counter MAX_SIGNAL_ELEMENTS = 10;

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
                      bit_mask(LEVEL_BITS),
                  "SignalMode fits LEVEL_BITS");
    static_assert(static_cast<unsigned>(SignalDuration::MAX_VALUE) <=
                      bit_mask(DURATION_BITS),
                  "SignalDuration fits DURATION_BITS");

    static constexpr SignalModeType to_value(SignalMode level) noexcept
    {
        return static_cast<SignalModeType>(level);
    }

    static constexpr SignalDurationType
    to_value(SignalDuration signal_duration) noexcept
    {
        return static_cast<SignalDurationType>(signal_duration);
    }

    static constexpr SignalTimesType
    to_value(SignalTimesType signal_times) noexcept
    {
        return (signal_times);
    }
} // namespace pisco_code