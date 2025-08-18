#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalPulseIterator
    {
      public:
        explicit SignalPulseIterator(SignalStack signal_stack) noexcept :
            signal_stack_(signal_stack), remaining_(signal_stack.size())
        {
        }

        [[nodiscard]] static SignalElement next() noexcept
        {
            if (!hasNext())
            {
                return SIGNAL_ELEMENT_NOT_DEFINED;
            }
            return SIGNAL_ELEMENT_ZERO;
            // return {element_.get_level(), Byte{1}, element_.get_duration()};
        }

        [[nodiscard]] static bool hasNext() noexcept
        {
            return true;
            // return remaining_ > 0 || is_pulse_between_;
        }

        void reset() noexcept
        {
            // remaining_        = to_value(element_.get_times());
            // is_pulse_between_ = false;
        }

      private:
        void advance() noexcept
        {
            // if (isPeakShort() && remaining_ > 1)
            // {
            //     is_pulse_between_ = true;
            // }
            // --remaining_;
        }

        SignalStack signal_stack_;
        Counter     remaining_{0};
        bool        is_pulse_between_{false};
        // [[nodiscard]] bool isPeakShort() const noexcept
        // {
        //     return signal_stack_.get_level() == SignalLevel::PEAK &&
        //            element_.get_duration() == SignalDuration::SHORT;
        // }
    };
    constexpr auto MAX_BYTES_PASS_BY_VALUE = 16;

    static_assert(sizeof(SignalStack) <= MAX_BYTES_PASS_BY_VALUE,
                  "Stack grew, reconsider pass-by-value");

} // namespace pisco_code