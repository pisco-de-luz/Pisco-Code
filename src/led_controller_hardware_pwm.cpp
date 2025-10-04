#include "led_controller_hardware_pwm.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    LedControllerHardwarePwm::LedControllerHardwarePwm()
    {
    }
    LedControllerHardwarePwm::LedControllerHardwarePwm(
        void (*led_func)(IntensityLevel)) : led_control_(led_func)
    {
    }

    void
    LedControllerHardwarePwm::attachLedControl(void (*led_func)(IntensityLevel))
    {
        led_control_ = led_func;
    }

    [[nodiscard]] bool LedControllerHardwarePwm::isAttached() const noexcept
    {
        return led_control_ != nullptr;
    }

    void LedControllerHardwarePwm::update()
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        const IntensityLevel target_level = getCurrentIntensityLevel();
        led_control_(target_level);
    }

    [[nodiscard]] bool
    LedControllerHardwarePwm::readyForPhaseChange() const noexcept
    {
        return (true);
    }

} // namespace pisco_code
