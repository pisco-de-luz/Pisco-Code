#include "software_pwm_led_controller.hpp"

#include "stdint.h" // NOLINT(modernize-deprecated-headers)

#include "pisco_constants.hpp"
namespace pisco
{

    SoftwarePwmLedController::SoftwarePwmLedController()
        : peak_level_(PWM_MAX), dimmed_level_(0), led_control_(nullptr)
    {
    }
    SoftwarePwmLedController::SoftwarePwmLedController(bool (*led_func)(uint8_t))
        : led_control_(led_func)
    {
    }

    SoftwarePwmLedController::~SoftwarePwmLedController()
    {
        led_control_ = nullptr;
    }

    void SoftwarePwmLedController::attachLedControl(bool (*led_func)(uint8_t))
    {
        led_control_ = led_func;
    }

    void SoftwarePwmLedController::setPeakLevel(uint8_t level)
    {
        peak_level_ = level;
    }

    void SoftwarePwmLedController::setBlinkMode(BlinkMode mode)
    {
        mode_ = mode;
    }

    void SoftwarePwmLedController::setDimmedLevel(uint8_t level)
    {
        dimmed_level_ = level;
    }

    void SoftwarePwmLedController::turnOff()
    {
        if (led_control_ != nullptr)
        {
            (void) led_control_(static_cast<pisco::LedCodeType>(LedControlCode::Off));
        }
    }

    void SoftwarePwmLedController::update(uint8_t pwm_counter)
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case pisco::BlinkMode::Pulse:
                if (pwm_counter == 0)
                {
                    led_control_(static_cast<pisco::LedCodeType>(LedControlCode::On));
                }
                else if (pwm_counter == peak_level_)
                {
                    led_control_(static_cast<pisco::LedCodeType>(LedControlCode::Off));
                }
                break;

            case pisco::BlinkMode::Dimmed:
                if (pwm_counter == 0)
                {
                    led_control_(static_cast<pisco::LedCodeType>(LedControlCode::On));
                }
                else if (pwm_counter == dimmed_level_)
                {
                    led_control_(static_cast<pisco::LedCodeType>(LedControlCode::Off));
                }
                break;

            case pisco::BlinkMode::None:
            default:
                if (pwm_counter == 0)
                {
                    // Ensure LED is OFF during idle periods
                    led_control_(static_cast<pisco::LedCodeType>(LedControlCode::Off));
                }
                break;
        }
    }

} // namespace pisco
