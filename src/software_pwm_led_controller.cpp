#include "software_pwm_led_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    SoftwarePwmLedController::SoftwarePwmLedController()
    {
    }
    SoftwarePwmLedController::SoftwarePwmLedController(
        bool (*led_func)(LedControlCode)) : led_control_(led_func)
    {
    }

    void
    SoftwarePwmLedController::attachLedControl(bool (*led_func)(LedControlCode))
    {
        led_control_ = led_func;
    }
    [[nodiscard]] bool SoftwarePwmLedController::isAttached() const noexcept
    {
        return led_control_ != nullptr;
    }

    void SoftwarePwmLedController::update()
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        const IntensityLevel current_level = getCurrentIntensityLevel();
        const IntensityLevel sw_pwm_level  = to_sw_pwm_level(current_level);

        // PWM logic: ON at start, OFF when reaching target level
        if (pwm_tick_position_ == 0 && sw_pwm_level > 0)
        {
            led_control_(LedControlCode::ON);
        }
        else if (pwm_tick_position_ == sw_pwm_level)
        {
            led_control_(LedControlCode::OFF);
        }

        if (++pwm_tick_position_ > PWM_MAX)
        {
            pwm_tick_position_ = 0;
        }
    }

    [[nodiscard]] bool
    SoftwarePwmLedController::readyForPhaseChange() const noexcept
    {
        return (pwm_tick_position_ == 0);
    }

} // namespace pisco_code
