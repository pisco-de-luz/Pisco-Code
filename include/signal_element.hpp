#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    // Single encoded unit (pulse)
    struct SignalElement
    {
      public:
        [[nodiscard]] constexpr SignalElement() noexcept : times_(0), level_(0), duration_(0) {}

        [[nodiscard]] constexpr SignalElement(SignalLevel lvl, Byte cnt,
                                              SignalDuration dur) noexcept
            : times_(cnt), level_(to_value(lvl)), duration_(to_value(dur))
        {
        }

        [[nodiscard]] constexpr SignalLevel get_level() const noexcept
        {
            return static_cast<SignalLevel>(level_);
        }
        [[nodiscard]] constexpr SignalDuration get_duration() const noexcept
        {
            return static_cast<SignalDuration>(duration_);
        }
        [[nodiscard]] constexpr SignalDuration get_times() const noexcept
        {
            return static_cast<SignalDuration>(times_);
        }

      private:
        Byte times_ : 4;    // How many times this unit repeats (0–15)
        Byte level_ : 2;    // Encoded SignalLevel
        Byte duration_ : 2; // Encoded SignalDuration
    };

    inline constexpr SignalElement SIGNAL_ELEMENT_NEGATIVE_PEAK{SignalLevel::PEAK, 1,
                                                                SignalDuration::LONG};
    inline constexpr SignalElement SIGNAL_ELEMENT_ZERO_GAP{SignalLevel::GAP, 1,
                                                           SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_NOT_DEFINED{SignalLevel::NOT_DEFINED, 0,
                                                              SignalDuration::SHORT};
    constexpr auto                 signal_element_digit_peak(DigitValue digit_value) noexcept
    {
        return SignalElement(SignalLevel::PEAK, digit_value, SignalDuration::SHORT);
    }

} // namespace pisco_code
