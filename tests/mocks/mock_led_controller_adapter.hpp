#ifndef MOCK_LED_CONTROLLER_ADAPTER_HPP
#define MOCK_LED_CONTROLLER_ADAPTER_HPP

#include <iostream>

#include "mock_led_control_logger.hpp"
#include "pisco_constants.hpp"
#include "signal_controller.hpp"

using namespace pisco_code;
class MockLedControllerAdapter : public SignalController
{
  public:
    explicit MockLedControllerAdapter(MockLedControlLogger* logger) :
        logger_(logger)
    {
    }

    void setPeakLevel(uint8_t value) override
    {
        peak_level_ = value;
    }

    void setDimmedLevel(uint8_t value) override
    {
        dimmed_level_ = value;
    }

    void setBlinkMode(BlinkMode mode) override
    {
        mode_ = mode;
    }

    void update() override
    {
        if (logger_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case BlinkMode::PULSE:
                if (pwm_tick_position_ == 0)
                {
                    logger_->handle(LedControlCode::ON);
                }
                else if (pwm_tick_position_ == peak_level_)
                {
                    logger_->handle(LedControlCode::OFF);
                }
                break;

            case BlinkMode::DIMMED:
                if (pwm_tick_position_ == 0)
                {
                    logger_->handle(LedControlCode::ON);
                }
                else if (pwm_tick_position_ == dimmed_level_)
                {
                    logger_->handle(LedControlCode::OFF);
                }
                break;

            case BlinkMode::NONE:
            default:
                if (pwm_tick_position_ == 0)
                {
                    // Ensure LED is OFF during idle periods
                    logger_->handle(LedControlCode::OFF);
                }
                break;
        }
        if (++pwm_tick_position_ > PWM_MAX)
        {
            pwm_tick_position_ = 0;
        }
    }
    [[nodiscard]] bool readyForPhaseChange() const noexcept
    {
        return (pwm_tick_position_ == 0);
    }

  private:
    MockLedControlLogger* logger_            = nullptr;
    PwmTickPosition       peak_level_        = PWM_MAX;
    PwmTickPosition       dimmed_level_      = DEFAULT_DIMMED_LEVEL;
    BlinkMode             mode_              = BlinkMode::NONE;
    PwmTickPosition       pwm_tick_position_ = 0;
};

#endif // MOCK_LED_CONTROLLER_ADAPTER_HPP
