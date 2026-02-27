#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalElement
    {
      public:
        constexpr SignalElement() noexcept :
            mode_(to_value(SignalMode::NOT_DEFINED)), times_(0),
            duration_(to_value(SignalDuration::SHORT))
        {
        }

        constexpr SignalElement(SignalMode mode, SignalTimesType times,
                                SignalDuration duration) noexcept :
            mode_(to_value(mode)), times_(times), duration_(to_value(duration))
        {
        }

        [[nodiscard]] constexpr SignalMode get_mode() const noexcept
        {
            return static_cast<SignalMode>(mode_);
        }
        [[nodiscard]] constexpr SignalDuration get_duration() const noexcept
        {
            return static_cast<SignalDuration>(duration_);
        }
        [[nodiscard]] constexpr SignalTimesType get_times() const noexcept
        {
            return static_cast<SignalTimesType>(times_);
        }

      private:
        SignalModeType  mode_ : MODE_BITS;
        SignalTimesType times_
            : TIMES_BITS; // Times this unit can repeats (0–15)
        SignalDurationType duration_ : DURATION_BITS;
    };

    inline constexpr SignalElement SIGNAL_ELEMENT_FRAMING{SignalMode::GAP, 1,
                                                          SignalDuration::LONG};
    inline constexpr SignalElement SIGNAL_ELEMENT_INTER_SYMBOL{
        SignalMode::BASE, 1, SignalDuration::LONG};
    inline constexpr SignalElement SIGNAL_ELEMENT_NEGATIVE{
        SignalMode::PEAK, 1, SignalDuration::LONG};
    inline constexpr SignalElement SIGNAL_ELEMENT_ZERO{SignalMode::GAP, 1,
                                                       SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_NOT_DEFINED{
        SignalMode::NOT_DEFINED, 0, SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_INTRA_DIGIT{
        SignalMode::BASE, 1, SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_DIGIT{SignalMode::PEAK, 1,
                                                        SignalDuration::SHORT};

    constexpr auto signal_element_digit(DigitValue digit) noexcept
    {
        return SignalElement(SignalMode::PEAK, digit, SignalDuration::SHORT);
    }
    [[nodiscard]] constexpr SignalElement
    signal_element_from_digit(DigitValue digit) noexcept
    {
        return (digit == 0)
                   ? SIGNAL_ELEMENT_ZERO
                   : signal_element_digit(static_cast<SignalTimesType>(digit));
    }

    static_assert(sizeof(SignalElement) == 1,
                  "SignalElement must be 1 byte in size");

} // namespace pisco_code
