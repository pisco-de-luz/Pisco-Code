#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalController
    {
      public:
        virtual void               update() noexcept                    = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept = 0;

        void setHighLevel(IntensityLevel level) noexcept;
        void setLowLevel(IntensityLevel level) noexcept;
        void setCurrentSignalMode(SignalMode mode) noexcept;

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
        [[nodiscard]] bool levelsAreTooClose() const noexcept;

        static constexpr IntensityLevel
        clampHighLevel(IntensityLevel level) noexcept
        {
            return (level < MIN_HIGH_LEVEL) ? MIN_HIGH_LEVEL : level;
        }

        static constexpr IntensityLevel
        clampLowLevel(IntensityLevel level) noexcept
        {
            return (level > MAX_LOW_LEVEL) ? MAX_LOW_LEVEL : level;
        }

        IntensityLevel high_level_ = DEFAULT_HIGH_LEVEL;
        IntensityLevel low_level_  = DEFAULT_LOW_LEVEL;
        SignalMode     mode_       = SignalMode::NOT_DEFINED;
    };

} // namespace pisco_code
