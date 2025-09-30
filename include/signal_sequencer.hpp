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
        [[nodiscard]] RepeatTimes getRepeatTimes() const noexcept;
        void loadSignalCode(SignalCode code, NumberBase base,
                            NumDigits num_digits = 0) noexcept;
        [[nodiscard]] SignalPulseIterator createPulseIterator() const noexcept;
        void                              incrementRepeatIndex() noexcept;
        [[nodiscard]] bool                shouldRepeat() const noexcept;
        [[nodiscard]] const SignalStack&  stack() const noexcept;

      private:
        SignalStack         signal_stack_;
        SignalPulseIterator pulse_iterator_;
        RepeatTimes         repeat_count_{1};
        Index               repeat_index_{0};
        Counter             element_count_{0};
        Index               element_index_{0};
        Counter             pulse_repeat_count_{0};
        Index               pulse_repeat_index_{0};
    };
} // namespace pisco_code