#include "signal_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    void SignalController::setHighLevel(IntensityLevel level) noexcept
    {
        high_level_ = clampHighLevel(level);

        if (levelsAreTooClose())
        {
            low_level_ = high_level_ - MIN_INTENSITY_DIFFERENCE;
        }
    }

    void SignalController::setLowLevel(IntensityLevel level) noexcept
    {
        low_level_ = clampLowLevel(level);

        if (levelsAreTooClose())
        {
            high_level_ = low_level_ + MIN_INTENSITY_DIFFERENCE;
        }
    }

    void SignalController::setCurrentSignalMode(SignalMode mode) noexcept
    {
        mode_ = mode;
    }

    IntensityLevel SignalController::getPeakLevel() const noexcept
    {
        return high_level_;
    }

    IntensityLevel SignalController::getBaseLevel() const noexcept
    {
        return low_level_;
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
        return high_level_ - low_level_ < MIN_INTENSITY_DIFFERENCE;
    }

} // namespace pisco_code