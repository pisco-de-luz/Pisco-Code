#include "led_controller_software_pwm.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    LedControllerSoftwarePwm::LedControllerSoftwarePwm(
        bool (*led_func)(LedControlCode)) noexcept : led_control_(led_func)
    {
    }

    void LedControllerSoftwarePwm::update() noexcept
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        const IntensityLevel sw_pwm_level = to_sw_pwm_level(getCurrentIntensityLevel());

        if (shouldTurnLedOn(sw_pwm_level))
        {
            led_control_(LedControlCode::ON);
        }
        else if (shouldTurnLedOff(sw_pwm_level))
        {
            led_control_(LedControlCode::OFF);
        }

        advancePwmTick();
    }

    bool LedControllerSoftwarePwm::shouldTurnLedOn(
        IntensityLevel sw_pwm_level) const noexcept
    {
        return pwm_tick_position_ == 0 && sw_pwm_level > 0;
    }

    bool LedControllerSoftwarePwm::shouldTurnLedOff(
        IntensityLevel sw_pwm_level) const noexcept
    {
        return pwm_tick_position_ == sw_pwm_level;
    }

    void LedControllerSoftwarePwm::advancePwmTick() noexcept
    {
        if (++pwm_tick_position_ > PWM_MAX)
        {
            pwm_tick_position_ = 0;
        }
    }

    [[nodiscard]] bool
    LedControllerSoftwarePwm::readyForPhaseChange() const noexcept
    {
        return (pwm_tick_position_ == 0);
    }

} // namespace pisco_code
