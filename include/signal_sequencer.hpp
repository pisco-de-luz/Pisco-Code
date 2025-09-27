#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_stack.hpp"

namespace pisco_code
{
    class SignalSequencer
    {
      public:
        SignalSequencer() noexcept : pulse_iterator_(signal_stack_)
        {
        }

        void clear() noexcept;
        void setRepeatTimes(RepeatTimes repeat_times) noexcept;
        void loadSignalCode(SignalCode code, NumberBase base,
                            NumDigits num_digits = 0) noexcept;
        [[nodiscard]] SignalPulseIterator createPulseIterator() const noexcept;
        [[nodiscard]] bool                shouldRepeat() const noexcept
        {
            return repeat_index_ < repeat_count_ - 1;
        }
        void incrementRepeatIndex() noexcept
        {
            if (shouldRepeat())
            {
                ++repeat_index_;
            }
        }
        [[nodiscard]] const SignalStack& stack() const noexcept
        {
            return signal_stack_;
        }

      private:
        SignalStack         signal_stack_;
        SignalPulseIterator pulse_iterator_;
        RepeatTimes         repeat_count_{1};
        Index               repeat_index_{0};
        Index               element_count_{0};
        Index               element_index_{0};
        Index               pulse_repeat_count_{0};
        Index               pulse_repeat_index_{0};
    };
} // namespace pisco_code