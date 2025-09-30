#pragma once

#include "pisco_code.hpp"
#include "pisco_types.hpp"

using IntensityLevel = pisco_code::IntensityLevel;
namespace hal_led
{
    void ledOnboardInit() noexcept;
    bool ledOnboard(LedControlCode code) noexcept;
    void ledHwPwmInit() noexcept;
    void ledHwPwmSetLevel(IntensityLevel level) noexcept;
    bool ledHwPwm(LedControlCode code) noexcept;
} // namespace hal_led
