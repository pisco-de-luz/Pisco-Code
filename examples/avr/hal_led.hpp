#pragma once

#include "pisco_code.hpp"
#include "pisco_types.hpp"

namespace hal_led
{
    // Initialize all LEDs (onboard + PWM)
    void init() noexcept;

    // Software PWM callback for onboard LED (directly toggled on/off)
    bool ledOnboard(pisco_code::LedControlCode code) noexcept;

    // Hardware PWM level setter for external LED (0-255 intensity)
    void ledPwmSetLevel(pisco_code::IntensityLevel level) noexcept;

} // namespace hal_led
