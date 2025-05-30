#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    // Single encoded unit (pulse)
    struct SignalElement
    {
        Byte times : 4;    // How many times this unit repeats (0–15)
        Byte level : 2;    // Encoded SignalLevel
        Byte duration : 2; // Encoded SignalDuration

        [[nodiscard]] constexpr SignalElement() noexcept : times(0), level(0), duration(0) {}

        [[nodiscard]] constexpr SignalElement(SignalLevel lvl, Byte cnt,
                                              SignalDuration dur) noexcept
            : times(cnt), level(to_value(lvl)), duration(to_value(dur))
        {
        }

        [[nodiscard]] constexpr SignalLevel get_level() const noexcept
        {
            return static_cast<SignalLevel>(level);
        }
        [[nodiscard]] constexpr SignalDuration get_duration() const noexcept
        {
            return static_cast<SignalDuration>(duration);
        }
    };

    constexpr SignalElement SIGNAL_ELEMENT_NEGATIVE_PEAK{SignalLevel::PEAK, 1,
                                                         SignalDuration::LONG};
    constexpr SignalElement SIGNAL_ELEMENT_ZERO_GAP{SignalLevel::GAP, 1, SignalDuration::SHORT};
    constexpr SignalElement SIGNAL_ELEMENT_NOT_DEFINED{SignalLevel::NOT_DEFINED, 0,
                                                       SignalDuration::SHORT};
    static constexpr auto   signal_element_digit_peak(DigitValue digit_value) noexcept
    {
        return SignalElement(SignalLevel::PEAK, digit_value, SignalDuration::SHORT);
    }

} // namespace pisco_code
