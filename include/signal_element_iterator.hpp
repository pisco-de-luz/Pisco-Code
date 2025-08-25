#pragma once

#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalElementIterator
    {
      public:
        explicit SignalElementIterator(SignalElement element) noexcept :
            element_(element), remaining_(to_value(element.get_times()))
        {
        }

        [[nodiscard]] SignalElement next() noexcept
        {
            if (!hasNext())
            {
                return SIGNAL_ELEMENT_NOT_DEFINED;
            }
            if (is_pulse_between_)
            {
                is_pulse_between_ = false;
                return SIGNAL_ELEMENT_INTRA_DIGIT;
            }
            advance();
            return {element_.get_level(), Byte{1}, element_.get_duration()};
        }

        [[nodiscard]] bool hasNext() const noexcept
        {
            return remaining_ > 0 || is_pulse_between_;
        }

        void reset() noexcept
        {
            remaining_        = to_value(element_.get_times());
            is_pulse_between_ = false;
        }

      private:
        void advance() noexcept
        {
            if (isPeakShort() && remaining_ > 1)
            {
                is_pulse_between_ = true;
            }
            --remaining_;
        }

        SignalElement      element_;
        SignalTimesType    remaining_{0};
        bool               is_pulse_between_{false};
        [[nodiscard]] bool isPeakShort() const noexcept
        {
            return element_.get_level() == SignalLevel::PEAK &&
                   element_.get_duration() == SignalDuration::SHORT;
        }
    };
} // namespace pisco_code