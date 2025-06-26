#include "software_pwm_led_controller.hpp"

#include "pisco_constants.hpp"
namespace pisco_code
{

    SoftwarePwmLedController::SoftwarePwmLedController() :
        peak_level_(PWM_MAX), dimmed_level_(0), led_control_(nullptr)
    {
    }
    SoftwarePwmLedController::SoftwarePwmLedController(
        bool (*led_func)(LedCodeType)) : led_control_(led_func)
    {
    }

    SoftwarePwmLedController::~SoftwarePwmLedController()
    {
        led_control_ = nullptr;
    }

    void
    SoftwarePwmLedController::attachLedControl(bool (*led_func)(LedCodeType))
    {
        led_control_ = led_func;
    }

    void SoftwarePwmLedController::setPeakLevel(LedLevel led_level)
    {
        peak_level_ = led_level;
    }

    void SoftwarePwmLedController::setBlinkMode(BlinkMode mode)
    {
        mode_ = mode;
    }

    void SoftwarePwmLedController::setDimmedLevel(LedLevel led_level)
    {
        dimmed_level_ = led_level;
    }

    void SoftwarePwmLedController::update(PwmTickPosition pwm_tick_position)
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case BlinkMode::PULSE:
                if (pwm_tick_position == 0)
                {
                    led_control_(to_value(LedControlCode::ON));
                }
                else if (pwm_tick_position == peak_level_)
                {
                    led_control_(to_value(LedControlCode::OFF));
                }
                break;

            case BlinkMode::DIMMED:
                if (pwm_tick_position == 0)
                {
                    led_control_(to_value(LedControlCode::ON));
                }
                else if (pwm_tick_position == dimmed_level_)
                {
                    led_control_(to_value(LedControlCode::OFF));
                }
                break;

            case BlinkMode::NONE:
            default:
                if (pwm_tick_position == 0)
                {
                    // Ensure LED is OFF during idle periods
                    led_control_(to_value(LedControlCode::OFF));
                }
                break;
        }
    }

} // namespace pisco_code
