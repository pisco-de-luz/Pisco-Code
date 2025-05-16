#ifndef MOCK_LED_CONTROLLER_ADAPTER_HPP
#define MOCK_LED_CONTROLLER_ADAPTER_HPP

#include <iostream>

#include "MockLedControlLogger.hpp"

#include "led_controller.hpp"
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
            case BlinkMode::Pulse:
                if (loop_counter == 0)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::On));
                }
                else if (loop_counter == peak_level_)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;

            case BlinkMode::Dimmed:
                if (loop_counter == 0)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::On));
                }
                else if (loop_counter == dimmed_level_)
                {
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;

            case BlinkMode::None:
            default:
                if (loop_counter == 0)
                {
                    // Ensure LED is OFF during idle periods
                    logger_->handle(static_cast<LedCodeType>(LedControlCode::Off));
                }
                break;
        }
    }

  private:
    MockLedControlLogger* logger_       = nullptr;
    uint8_t               peak_level_   = PWM_MAX;
    uint8_t               dimmed_level_ = DEFAULT_DIMMED_LEVEL;
    BlinkMode             mode_         = BlinkMode::None;
};

#endif // MOCK_LED_CONTROLLER_ADAPTER_HPP
