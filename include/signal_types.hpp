#pragma once
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"

namespace pisco_code
{
    constexpr Counter MAX_SIGNAL_ELEMENTS = 10;

    // Signal strength or symbolic meaning
    enum class SignalLevel : SignalLevelType
    {
        NOT_DEFINED = 0,
        GAP,    // Represents silent or idle pulse (e.g., digit 0)
        PEAK,   // Represents active/visible/strong pulse
        MIDDLE, // Represents a middle level, not used in current logic
    };

    // Signal duration (mapped to ms by higher layers)
    enum class SignalDuration : SignalDurationType
    {
        SHORT = 0,
        MEDIUM,
        LONG,
        EXTRA_LONG
    };

    static constexpr SignalLevelType to_value(SignalLevel level) noexcept
    {
        return static_cast<SignalLevelType>(level);
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