#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalController
    {
      public:
        // Pure virtual interface
        virtual void               update()                             = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept = 0;

        // State management - declared but not implemented
        void setPeakLevel(IntensityLevel level);
        void setBaseLevel(IntensityLevel level);
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
        IntensityLevel peak_level_ = PWM_MAX;
        IntensityLevel base_level_ = DEFAULT_BASE_LEVEL;
        SignalMode     mode_       = SignalMode::NOT_DEFINED;
    };

} // namespace pisco_code
