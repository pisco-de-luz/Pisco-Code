#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

namespace pisco_code
{
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
        // Placeholder for logic to populate based on number
        void generateFromCode(BlinkCode code, NumberBase base, NumDigits min_digits = 0) noexcept;

      private:
        SignalStack signal_stack_;
        RepeatTimes repeat_times_;
        Counter     signal_size_{0};
        Index       signal_index_{0};
        Counter     signal_times_{0};
    };
} // namespace pisco_code