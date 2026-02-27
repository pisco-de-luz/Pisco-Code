#pragma once

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

    void update() noexcept override
    {
        if (logger_ == nullptr)
        {
            return;
        }

        IntensityLevel current_level = getCurrentIntensityLevel();
        IntensityLevel sw_pwm_level  = to_sw_pwm_level(current_level);

        // PWM logic: ON at start, OFF when reaching target level
        if (pwm_tick_position_ == 0 && sw_pwm_level > 0)
        {
            logger_->handle(LedControlCode::ON);
        }
        else if (pwm_tick_position_ == sw_pwm_level)
        {
            logger_->handle(LedControlCode::OFF);
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
    PwmTickPosition       pwm_tick_position_ = 0;
};
