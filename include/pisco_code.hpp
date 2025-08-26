#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "software_pwm_led_controller.hpp"

using pisco_code::LedControlCode;
using pisco_code::NumberBase;
using pisco_code::RepeatTimes;
using pisco_code::Timestamp;
namespace pisco_code
{

    class PiscoCode
    {
      public:
        explicit PiscoCode(bool (*led_func)(LedControlCode)) :
            controller_(led_func), blinker_(&controller_)
        {
        }

        void showCode(SignalCode code, NumberBase base, NumDigits min_digits,
                      RepeatTimes repeats)
        {
            blinker_.showCode(code, base, min_digits, repeats);
        }

        void loop(TickCounter tick_counter)
        {
            blinker_.loop(tick_counter);
        }

        void setDimmedLevel(LedLevel level)
        {
            blinker_.setDimmedLevel(level);
        }

        void setPeakLevel(LedLevel level)
        {
            blinker_.setPeakLevel(level);
        }

        bool isRunning() const
        {
            return blinker_.isRunning();
        }

      private:
        SoftwarePwmLedController controller_;
        SignalEmitter            blinker_;
    };

} // namespace pisco_code
