#ifndef MOCK_LED_CONTROLLER_ADAPTER_HPP
#define MOCK_LED_CONTROLLER_ADAPTER_HPP

#include "MockLedControlLogger.hpp"
#include "led_controller.hpp"
#include "pisco_constants.hpp"

class MockLedControllerAdapter : public pisco::LedController
{
  public:
    explicit MockLedControllerAdapter(MockLedControlLogger* logger) : logger_(logger) {}

    void setPeakLevel(uint8_t value) override { peak_level_ = value; }

    void setDimmedLevel(uint8_t value) override { dimmed_level_ = value; }

    void setBlinkMode(pisco::BlinkMode mode) override { mode_ = mode; }

    void turnOff() override
    {
        if (logger_)
            logger_->handle(pisco::LED_OFF);
    }

    void update(uint8_t pwm_counter) override
    {
        if (!logger_)
            return;

        if (pwm_counter == 0 && mode_ != pisco::BlinkMode::None)
        {
            logger_->handle(pisco::LED_ON);
        }
        else if ((mode_ == pisco::BlinkMode::Pulse && pwm_counter == peak_level_) ||
                 (mode_ == pisco::BlinkMode::Dimmed && pwm_counter == dimmed_level_))
        {
            logger_->handle(pisco::LED_OFF);
        }
    }

  private:
    MockLedControlLogger* logger_       = nullptr;
    uint8_t               peak_level_   = pisco::PWM_MAX;
    uint8_t               dimmed_level_ = pisco::INITIAL_DIMMED_PWM;
    pisco::BlinkMode      mode_         = pisco::BlinkMode::None;
};

#endif // MOCK_LED_CONTROLLER_ADAPTER_HPP
