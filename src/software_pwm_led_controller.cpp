#include "software_pwm_led_controller.hpp"
#include "pisco_constants.hpp"

namespace pisco
{

    SoftwarePwmLedController::SoftwarePwmLedController()
        : peak_level_(15), dimmed_level_(0), led_control_(nullptr)
    {
    }
    SoftwarePwmLedController::SoftwarePwmLedController(bool (*led_func)(uint8_t))
        : led_control_(led_func), peak_level_(PWM_MAX), dimmed_level_(INITIAL_DIMMED_PWM),
          mode_(BlinkMode::None)
    {
    }

    void SoftwarePwmLedController::attachLedControl(bool (*led_func)(uint8_t))
    {
        led_control_ = led_func;
    }

    void SoftwarePwmLedController::setPeakLevel(uint8_t level)
    {
        peak_level_ = level;
    }

    void SoftwarePwmLedController::setDimmedLevel(uint8_t level)
    {
        dimmed_level_ = level;
    }

    void SoftwarePwmLedController::turnOff()
    {
        if (led_control_)
        {
            (void) led_control_(pisco::LED_OFF);
        }
    }

    void SoftwarePwmLedController::update(uint8_t pwm_counter)
    {
        if (!led_control_)
            return;

        if (pwm_counter == 0)
        {
            led_control_(LED_ON);
        }
        else if ((mode_ == pisco::BlinkMode::Pulse && pwm_counter == peak_level_) ||
                 (mode_ == pisco::BlinkMode::Dimmed && pwm_counter == dimmed_level_))
        {
            led_control_(LED_OFF);
        }
    }

} // namespace pisco
