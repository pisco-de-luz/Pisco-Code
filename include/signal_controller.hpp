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

        void setPeakLevel(IntensityLevel level) noexcept;
        void setBaseLevel(IntensityLevel level) noexcept;
        void setCurrentSignalMode(SignalMode mode) noexcept;

        [[deprecated("Use setPeakLevel() instead — will be removed in "
                     "v2.0.0")]]
        void setHighLevel(IntensityLevel level) noexcept
        {
            setPeakLevel(level);
        }

        [[deprecated("Use setBaseLevel() instead — will be removed in "
                     "v2.0.0")]]
        void setLowLevel(IntensityLevel level) noexcept
        {
            setBaseLevel(level);
        }

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
        clamp_peak_level(IntensityLevel level) noexcept
        {
            return (level < MIN_PEAK_LEVEL) ? MIN_PEAK_LEVEL : level;
        }

        static constexpr IntensityLevel
        clamp_base_level(IntensityLevel level) noexcept
        {
            return (level > MAX_BASE_LEVEL) ? MAX_BASE_LEVEL : level;
        }

        IntensityLevel peak_level_ = DEFAULT_PEAK_LEVEL;
        IntensityLevel base_level_ = DEFAULT_BASE_LEVEL;
        SignalMode     mode_       = SignalMode::NOT_DEFINED;
    };

} // namespace pisco_code
