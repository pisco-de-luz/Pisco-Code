#include "signal_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    void SignalController::setHighLevel(IntensityLevel level)
    {
        high_level_ = (level < MIN_INTENSITY_DIFFERENCE)
                          ? MIN_INTENSITY_DIFFERENCE
                          : level;

        // Ensure low/high level maintains minimum difference
        if (low_level_ > high_level_ - MIN_INTENSITY_DIFFERENCE)
        {
            low_level_ = (high_level_ >= MIN_INTENSITY_DIFFERENCE)
                             ? high_level_ - MIN_INTENSITY_DIFFERENCE
                             : 0;
        }
    }

    void SignalController::setLowLevel(IntensityLevel level)
    {
        low_level_ = (level > MAX_LOW_LEVEL) ? MAX_LOW_LEVEL : level;

        // Ensure low/high level maintains minimum difference
        if (high_level_ < low_level_ + MIN_INTENSITY_DIFFERENCE)
        {
            high_level_ = low_level_ + MIN_INTENSITY_DIFFERENCE;
        }
    }

    void SignalController::setCurrentSignalMode(SignalMode mode)
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

} // namespace pisco_code