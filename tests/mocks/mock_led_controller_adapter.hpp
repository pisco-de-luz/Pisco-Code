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
    explicit MockLedControllerAdapter(MockLedControlLogger* logger) : logger_(logger) {}

    void setPeakLevel(uint8_t value) override { peak_level_ = value; }

    void setDimmedLevel(uint8_t value) override { dimmed_level_ = value; }

    void setBlinkMode(BlinkMode mode) override { mode_ = mode; }

    void update(uint8_t loop_counter) override
    {
        if (logger_ == nullptr)
        {
            return;
        }

        switch (mode_)
        {
            case BlinkMode::PULSE:
                if (loop_counter == 0)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::ON));
                }
                else if (loop_counter == peak_level_)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;

            case BlinkMode::DIMMED:
                if (loop_counter == 0)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::ON));
                }
                else if (loop_counter == dimmed_level_)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;

            case BlinkMode::NONE:
            default:
                if (loop_counter == 0)
                {
                    // Ensure LED is OFF during idle periods
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::OFF));
                }
                break;
        }
    }

  private:
    MockLedControlLogger* logger_       = nullptr;
    uint8_t               peak_level_   = PWM_MAX;
    uint8_t               dimmed_level_ = DEFAULT_DIMMED_LEVEL;
    BlinkMode             mode_         = BlinkMode::NONE;
};

#endif // MOCK_LED_CONTROLLER_ADAPTER_HPP
