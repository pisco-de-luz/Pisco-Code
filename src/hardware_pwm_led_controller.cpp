#include "hardware_pwm_led_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    HardwarePwmLedController::HardwarePwmLedController()
    {
    }
    HardwarePwmLedController::HardwarePwmLedController(
        void (*led_func)(IntensityLevel)) : led_control_(led_func)
    {
    }

    void
    HardwarePwmLedController::attachLedControl(void (*led_func)(IntensityLevel))
    {
        led_control_ = led_func;
    }

    [[nodiscard]] bool HardwarePwmLedController::isAttached() const noexcept
    {
        return led_control_ != nullptr;
    }

    void HardwarePwmLedController::update()
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        const IntensityLevel target_level = getCurrentIntensityLevel();
        led_control_(target_level);
    }

    [[nodiscard]] bool
    HardwarePwmLedController::readyForPhaseChange() const noexcept
    {
        return (true);
    }

} // namespace pisco_code
