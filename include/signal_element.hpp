#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{

    constexpr Counter MAX_SIGNAL_ELEMENTS = 32; // UPPER_CASE for constexpr variable

    // Signal strength or symbolic meaning
    enum class SignalLevel : Byte
    {
        NOT_DEFINED = 0,
        GAP, // Represents silent or idle pulse (e.g., digit 0)
        PEAK // Represents active/visible/strong pulse
    };

    // Signal duration (mapped to ms by higher layers)
    enum class SignalDuration : Byte
    {
        SHORT = 0,
        MEDIUM,
        LONG,
        EXTRA_LONG
    };

    static constexpr Byte to_value(SignalLevel level) noexcept
    {
        return static_cast<Byte>(level);
    }

    static constexpr Byte to_value(SignalDuration signal_duration) noexcept
    {
        return static_cast<Byte>(signal_duration);
    }

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

    class SignalStack
    {
      public:
        [[nodiscard]] constexpr SignalStack() noexcept = default;

        void                        pushNewSignalUnit(SignalElement unit) noexcept;
        [[nodiscard]] bool          hasNextSignalUnit() const noexcept;
        [[nodiscard]] SignalElement popNextSignalUnit() noexcept;
        [[nodiscard]] Counter       size() const noexcept;
        void                        clear() noexcept;
        void                        rewind() noexcept;

        // Placeholder for logic to populate based on number
        void generateFromCode(BlinkCode code, NumberBase base, NumDigits min_digits = 0) noexcept;

      private:
        SignalElement signal_elements_[MAX_SIGNAL_ELEMENTS]{};
        Counter       count_{0};
        Index         read_index_{0};
    };

    class SignalSequencer
    {
      public:
        [[nodiscard]] SignalSequencer(SignalStack signal_stack, RepeatTimes repeat_times)
            : signal_stack_(signal_stack), repeat_times_(repeat_times)
        {
        }
        [[nodiscard]] bool hasSignalCodeToSequence() const noexcept;
        [[nodiscard]] bool hasMoreSignalElements() const noexcept;
        [[nodiscard]] bool hasMorePulse() noexcept;

      private:
        SignalStack signal_stack_;
        RepeatTimes repeat_times_;
        Counter     signal_size_{0};
        Index       signal_index_{0};
        Counter     signal_times_{0};
    };

} // namespace pisco_code
