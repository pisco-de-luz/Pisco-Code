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

            if (phase_ == Phase::GAP)
            {
                phase_ = Phase::PULSE;
                return SIGNAL_ELEMENT_INTRA_DIGIT;
            }

            --remaining_;

            if (remaining_ > 0 && isPeakShort())
            {
                phase_ = Phase::GAP;
            }

            return {element_.get_mode(), Byte{1}, element_.get_duration()};
        }

        [[nodiscard]] bool hasNext() const noexcept
        {
            return remaining_ > 0 || phase_ == Phase::GAP;
        }

        void reset() noexcept
        {
            remaining_ = to_value(element_.get_times());
            phase_     = Phase::PULSE;
        }

      private:
        enum class Phase : bool
        {
            PULSE = false,
            GAP   = true,
        };

        [[nodiscard]] bool isPeakShort() const noexcept
        {
            return element_.get_mode() == SignalMode::PEAK &&
                   element_.get_duration() == SignalDuration::SHORT;
        }

        SignalElement   element_;
        SignalTimesType remaining_{0};
        Phase           phase_{Phase::PULSE};
    };
} // namespace pisco_code