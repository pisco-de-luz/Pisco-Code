#ifndef MOCK_LED_CONTROLLER_ADAPTER_HPP
#define MOCK_LED_CONTROLLER_ADAPTER_HPP

#include <iostream>

#include "led_controller.hpp"
#include "mock_led_control_logger.hpp"
#include "pisco_constants.hpp"

using namespace pisco_code;
class MockLedControllerAdapter : public LedController
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

    void update(PwmTickPosition pwm_tick_position) override
    {
        if (logger_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case BlinkMode::PULSE:
                if (pwm_tick_position == 0)
                {
                    logger_->handle(
                        static_cast<LedCodeType>(LedControlCode::ON));
                }
                else if (pwm_tick_position == peak_level_)
                {
                    logger_->handle(
                        static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;

            case BlinkMode::DIMMED:
                if (pwm_tick_position == 0)
                {
                    logger_->handle(
                        static_cast<LedCodeType>(LedControlCode::ON));
                }
                else if (pwm_tick_position == dimmed_level_)
                {
                    logger_->handle(
                        static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;

            case BlinkMode::NONE:
            default:
                if (pwm_tick_position == 0)
                {
                    // Ensure LED is OFF during idle periods
                    logger_->handle(
                        static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;
        }
    }

  private:
    MockLedControlLogger* logger_       = nullptr;
    PwmTickPosition       peak_level_   = PWM_MAX;
    PwmTickPosition       dimmed_level_ = DEFAULT_DIMMED_LEVEL;
    BlinkMode             mode_         = BlinkMode::NONE;
};

#endif // MOCK_LED_CONTROLLER_ADAPTER_HPP
