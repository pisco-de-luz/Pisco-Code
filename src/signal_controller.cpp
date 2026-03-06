#include "signal_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    void SignalController::setPeakLevel(IntensityLevel level) noexcept
    {
        peak_level_ = clamp_peak_level(level);

        if (levelsAreTooClose())
        {
            base_level_ = peak_level_ - MIN_INTENSITY_DIFFERENCE;
        }
    }

    void SignalController::setBaseLevel(IntensityLevel level) noexcept
    {
        base_level_ = clamp_base_level(level);

        if (levelsAreTooClose())
        {
            peak_level_ = base_level_ + MIN_INTENSITY_DIFFERENCE;
        }
    }

    void SignalController::setCurrentSignalMode(SignalMode mode) noexcept
    {
        mode_ = mode;
    }

    IntensityLevel SignalController::getPeakLevel() const noexcept
    {
        return peak_level_;
    }

    IntensityLevel SignalController::getBaseLevel() const noexcept
    {
        return base_level_;
    }

    SignalMode SignalController::getCurrentSignalMode() const noexcept
    {
        return mode_;
    }

    IntensityLevel SignalController::getCurrentIntensityLevel() const noexcept
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

    bool SignalController::levelsAreTooClose() const noexcept
    {
        return peak_level_ - base_level_ < MIN_INTENSITY_DIFFERENCE;
    }

} // namespace pisco_code