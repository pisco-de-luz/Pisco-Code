#include "software_pwm_led_controller.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    SoftwarePwmLedController::SoftwarePwmLedController() :
        peak_level_(PWM_MAX), base_level_(0)
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

    void SoftwarePwmLedController::setPeakLevel(IntensityLevel led_level)
    {
        peak_level_ = led_level;
    }

    void SoftwarePwmLedController::setCurrentSignalMode(SignalMode mode)
    {
        mode_ = mode;
    }

    void SoftwarePwmLedController::setBaseLevel(IntensityLevel led_level)
    {
        base_level_ = led_level;
    }

    void SoftwarePwmLedController::update()
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case SignalMode::PEAK:
                if (pwm_tick_position_ == 0)
                {
                    led_control_(LedControlCode::ON);
                }
                else if (pwm_tick_position_ == peak_level_)
                {
                    led_control_(LedControlCode::OFF);
                }
                break;

            case SignalMode::BASE:
                if (pwm_tick_position_ == 0)
                {
                    led_control_(LedControlCode::ON);
                }
                else if (pwm_tick_position_ == base_level_)
                {
                    led_control_(LedControlCode::OFF);
                }
                break;

            case SignalMode::GAP:
            default:
                if (pwm_tick_position_ == 0)
                {
                    // Ensure LED is OFF during idle periods
                    led_control_(LedControlCode::OFF);
                }
                break;
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
