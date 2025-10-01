#include "signal_controller.hpp"

namespace pisco_code
{
    void SignalController::setPeakLevel(IntensityLevel level)
    {
        peak_level_ = level;
    }

    void SignalController::setBaseLevel(IntensityLevel level)
    {
        base_level_ = level;
    }

    void SignalController::setCurrentSignalMode(SignalMode mode)
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

} // namespace pisco_code