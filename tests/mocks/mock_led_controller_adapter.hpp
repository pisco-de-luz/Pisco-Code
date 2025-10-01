#pragma once

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

    void update() override
    {
        if (logger_ == nullptr)
        {
            return;
        }

        IntensityLevel target_level = getCurrentIntensityLevel();

        // PWM logic: ON at start, OFF when reaching target level
        if (pwm_tick_position_ == 0 && target_level > 0)
        {
            logger_->handle(LedControlCode::ON);
        }
        else if (pwm_tick_position_ == target_level)
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
    IntensityLevel        peak_level_        = PWM_MAX;
    IntensityLevel        base_level_        = DEFAULT_BASE_LEVEL;
    SignalMode            mode_              = SignalMode::NOT_DEFINED;
    PwmTickPosition       pwm_tick_position_ = 0;
};
