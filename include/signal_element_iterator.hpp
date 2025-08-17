#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalElementIterator
    {
      public:
        explicit SignalElementIterator(SignalElement element) noexcept :
            element_(element), remaining_(element.get_times())
        {
        }

        [[nodiscard]] SignalElement current() const noexcept
        {
            if (is_pulse_between_)
            {
                return SIGNAL_ELEMENT_BETWEEN_PEAK;
            }
            if (remaining_ > 0)
            {
                return {element_.get_level(), 1, element_.get_duration()};
            }
            if (remaining_ == 0)
            {
                return SIGNAL_ELEMENT_NOT_DEFINED;
            }
            return element_;
        }

        [[nodiscard]] bool hasNext() const noexcept
        {
            return remaining_ > 0 || is_pulse_between_;
        }

        void reset() noexcept
        {
            remaining_        = element_.get_times();
            is_pulse_between_ = false;
        }

        void advance() noexcept
        {
            if (!hasNext())
            {
                return;
            }
            if (is_pulse_between_)
            {
                is_pulse_between_ = false;
                return;
            }
            if (isPeakShort() && remaining_ > 1)
            {
                is_pulse_between_ = true;
            }
            --remaining_;
        }

      private:
        SignalElement      element_;
        Counter            remaining_{0};
        bool               is_pulse_between_{false};
        [[nodiscard]] bool isPeakShort() const noexcept
        {
            return element_.get_level() == SignalLevel::PEAK &&
                   element_.get_duration() == SignalDuration::SHORT;
        }
    };
} // namespace pisco_code