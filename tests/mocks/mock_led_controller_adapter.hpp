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

        if (shouldTurnLedOn(sw_pwm_level))
        {
            logger_->handle(LedControlCode::ON);
        }
        else if (shouldTurnLedOff(sw_pwm_level))
        {
            logger_->handle(LedControlCode::OFF);
        }

        advancePwmTick();
    }
    [[nodiscard]] bool readyForPhaseChange() const noexcept
    {
        return (pwm_tick_position_ == 0);
    }

  private:
    [[nodiscard]] bool
    shouldTurnLedOn(IntensityLevel sw_pwm_level) const noexcept
    {
        return pwm_tick_position_ == 0 && sw_pwm_level > 0;
    }
    [[nodiscard]] bool
    shouldTurnLedOff(IntensityLevel sw_pwm_level) const noexcept
    {
        return pwm_tick_position_ == sw_pwm_level;
    }
    void advancePwmTick() noexcept
    {
        if (++pwm_tick_position_ > PWM_MAX)
        {
            pwm_tick_position_ = 0;
        }
    }

    MockLedControlLogger* logger_            = nullptr;
    PwmTickPosition       pwm_tick_position_ = 0;
};
