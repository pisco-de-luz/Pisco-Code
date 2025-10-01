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
    void setHighLevel(IntensityLevel level)
    {
        high_level_ = (level > PWM_MAX) ? PWM_MAX : level;
        if (high_level_ < MIN_INTENSITY_DIFFERENCE)
        {
            high_level_ = MIN_INTENSITY_DIFFERENCE;
        }
        if (low_level_ >= high_level_)
        {
            low_level_ = high_level_ - MIN_INTENSITY_DIFFERENCE;
        }
    }
    void setLowLevel(IntensityLevel level)
    {
        constexpr auto MAX_LOW_LEVEL = PWM_MAX - MIN_INTENSITY_DIFFERENCE;
        low_level_ = (level > MAX_LOW_LEVEL) ? MAX_LOW_LEVEL : level;
        if (low_level_ >= high_level_)
        {
            low_level_ = high_level_ - MIN_INTENSITY_DIFFERENCE;
        }
    }

  private:
    MockLedControlLogger* logger_            = nullptr;
    IntensityLevel        high_level_        = PWM_MAX;
    IntensityLevel        low_level_         = DEFAULT_LOW_LEVEL;
    SignalMode            mode_              = SignalMode::NOT_DEFINED;
    PwmTickPosition       pwm_tick_position_ = 0;
};
