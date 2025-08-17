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
        constexpr SignalElement() noexcept :
            level_(to_value(SignalLevel::NOT_DEFINED)), times_(0),
            duration_(to_value(SignalDuration::SHORT))
        {
        }

        constexpr SignalElement(SignalLevel lvl, Counter cnt,
                                SignalDuration dur) noexcept :
            level_(to_value(lvl)), times_(cnt), duration_(to_value(dur))
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
        [[nodiscard]] constexpr Counter get_times() const noexcept
        {
            return static_cast<Counter>(times_);
        }

      private:
        SignalLevelType level_ : LEVEL_BITS;
        SignalTimesType times_
            : TIMES_BITS; // Times this unit can repeats (0–15)
        SignalDurationType duration_ : DURATION_BITS;
    };

    inline constexpr SignalElement SIGNAL_ELEMENT_NEGATIVE_PEAK{
        SignalLevel::PEAK, 1, SignalDuration::LONG};
    inline constexpr SignalElement SIGNAL_ELEMENT_ZERO_GAP{
        SignalLevel::GAP, 1, SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_NOT_DEFINED{
        SignalLevel::NOT_DEFINED, 0, SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_BETWEEN_PEAK{
        SignalLevel::MIDDLE, 1, SignalDuration::SHORT};
    inline constexpr SignalElement SIGNAL_ELEMENT_DIGIT_PEAK{
        SignalLevel::PEAK, 1, SignalDuration::SHORT};
    constexpr auto
    signal_element_digit_peak(SignalTimesType digit_value) noexcept
    {
        return SignalElement(SignalLevel::PEAK, digit_value,
                             SignalDuration::SHORT);
    }

    static_assert(sizeof(SignalElement) == 1,
                  "SignalElement must be 1 byte in size");

} // namespace pisco_code
