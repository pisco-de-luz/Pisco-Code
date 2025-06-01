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
        [[nodiscard]] SignalSequencer() noexcept = default;

        void                  clear() noexcept;
        void                  popNextPulse() noexcept;
        void                  setRepeatTimes(RepeatTimes repeat_times) noexcept;
        [[nodiscard]] Counter getElementCount() const noexcept { return element_count_; }
        [[nodiscard]] bool    hasMoreSignalCodeToSequence() const noexcept;
        [[nodiscard]] bool    hasMoreSignalElements() const noexcept;
        [[nodiscard]] bool    hasMorePulse() const noexcept;
        SignalElement         popNextSignalElement() noexcept;
        // Placeholder for logic to populate based on number
        void loadSignalCode(SignalCode code, NumberBase base, NumDigits min_digits = 0) noexcept;

      private:
        SignalStack signal_stack_;
        RepeatTimes repeat_count_{1};
        Index       repeat_index_{0};
        Counter     element_count_{0};
        Index       element_index_{0};
        Counter     pulse_repeat_count_{0};
        Index       pulse_repeat_index_{0};
    };
} // namespace pisco_code