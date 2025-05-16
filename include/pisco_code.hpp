#pragma once

#include <stdint.h>

#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "software_pwm_led_controller.hpp"

namespace pisco_code
{

    class PiscoCode
    {
      public:
        explicit PiscoCode(bool (*led_func)(uint8_t))
            : controller_(led_func), blinker_(&controller_)
        {
        }

        void showCode(int32_t code, NumberBase base, uint8_t min_digits, uint8_t repeats)
        {
            blinker_.showCode(code, base, min_digits, repeats);
        }

        void loop(uint8_t loop_counter) { blinker_.loop(loop_counter); }

        void setDimmedLevel(uint8_t level) { blinker_.setDimmedLevel(level); }

        void setPeakLevel(uint8_t level) { blinker_.setPeakLevel(level); }

        bool isRunning() const { return blinker_.isRunning(); }

      private:
        SoftwarePwmLedController controller_;
        CodeBlinker              blinker_;
    };

} // namespace pisco_code
