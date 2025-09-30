// signal_controller.hpp
#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    // NOLINT(cppcoreguidelines-special-member-functions)
    class SignalController
    {
      public:
        virtual void               update()                             = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept = 0;

        void setPeakLevel(IntensityLevel level)
        {
            peak_level_ = level;
        }
        void setBaseLevel(IntensityLevel level)
        {
            base_level_ = level;
        }
        void setCurrentSignalMode(SignalMode mode)
        {
            mode_ = mode;
        }
        [[nodiscard]] IntensityLevel getPeakLevel() const noexcept
        {
            return peak_level_;
        }
        [[nodiscard]] IntensityLevel getBaseLevel() const noexcept
        {
            return base_level_;
        }
        [[nodiscard]] SignalMode getCurrentSignalMode() const noexcept
        {
            return mode_;
        }

        // Template method - implemented once in base class
        [[nodiscard]] IntensityLevel getCurrentIntensityLevel() const noexcept
        {
            switch (getCurrentSignalMode())
            {
                case SignalMode::PEAK:
                    return getPeakLevel();
                case SignalMode::BASE:
                    return getBaseLevel();
                case SignalMode::GAP:
                default:
                    return 0;
            }
        }

      private:
        IntensityLevel peak_level_ = PWM_MAX;
        IntensityLevel base_level_ = DEFAULT_BASE_LEVEL;
        SignalMode     mode_       = SignalMode::NOT_DEFINED;

      protected:
        SignalController()                                   = default;
        SignalController(const SignalController&)            = default;
        SignalController& operator=(const SignalController&) = default;
        SignalController(SignalController&&)                 = default;
        SignalController& operator=(SignalController&&)      = default;
        ~SignalController()                                  = default;
    };

} // namespace pisco_code
