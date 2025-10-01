#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

using pisco_code::IntensityLevel;
using pisco_code::SignalMode;
namespace pisco_code
{
    class SignalController
    {
      public:
        // Pure virtual interface
        virtual void               update()                             = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept = 0;

        // State management - declared but not implemented
        void setHighLevel(IntensityLevel level);
        void setLowLevel(IntensityLevel level);
        void setCurrentSignalMode(SignalMode mode);

        [[nodiscard]] IntensityLevel getPeakLevel() const noexcept;
        [[nodiscard]] IntensityLevel getBaseLevel() const noexcept;
        [[nodiscard]] SignalMode     getCurrentSignalMode() const noexcept;
        [[nodiscard]] IntensityLevel getCurrentIntensityLevel() const noexcept;

      protected:
        SignalController()                                   = default;
        SignalController(const SignalController&)            = default;
        SignalController& operator=(const SignalController&) = default;
        SignalController(SignalController&&)                 = default;
        SignalController& operator=(SignalController&&)      = default;
        ~SignalController()                                  = default;

      private:
        IntensityLevel high_level_ = PWM_MAX;
        IntensityLevel low_level_  = DEFAULT_LOW_LEVEL;
        SignalMode     mode_       = SignalMode::NOT_DEFINED;
    };

} // namespace pisco_code
