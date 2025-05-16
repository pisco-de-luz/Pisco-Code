#include "software_pwm_led_controller.hpp"

#include "stdint.h" // NOLINT(modernize-deprecated-headers)

#include "pisco_constants.hpp"
namespace pisco_code
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

    void SoftwarePwmLedController::update(uint8_t loop_counter)
    {
        if (led_control_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case BlinkMode::Pulse:
                if (loop_counter == 0)
                {
                    led_control_(static_cast<LedCodeType>(LedControlCode::On));
                }
                else if (loop_counter == peak_level_)
                {
                    led_control_(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;

            case BlinkMode::Dimmed:
                if (loop_counter == 0)
                {
                    led_control_(static_cast<LedCodeType>(LedControlCode::On));
                }
                else if (loop_counter == dimmed_level_)
                {
                    led_control_(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;

            case BlinkMode::None:
            default:
                if (loop_counter == 0)
                {
                    // Ensure LED is OFF during idle periods
                    led_control_(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;
        }
    }

} // namespace pisco_code
