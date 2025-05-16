#ifndef SOFTWARE_PWM_LED_CONTROLLER_HPP
#define SOFTWARE_PWM_LED_CONTROLLER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
// #include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace pisco_code
{
    class SoftwarePwmLedController : public LedController
    {
      public:
        explicit SoftwarePwmLedController(bool (*led_func)(uint8_t));
        explicit SoftwarePwmLedController();
        ~SoftwarePwmLedController();
        void attachLedControl(bool (*led_func)(uint8_t));

        void setPeakLevel(uint8_t level) override;
        void setDimmedLevel(uint8_t level) override;
        void setBlinkMode(BlinkMode mode) override;
        void update(uint8_t pwm_counter) override;

      private:
        uint8_t peak_level_           = PWM_MAX;
        uint8_t dimmed_level_         = DEFAULT_DIMMED_LEVEL;
        bool (*led_control_)(uint8_t) = nullptr;
        BlinkMode mode_               = BlinkMode::None;
    };

} // namespace pisco_code

#endif // SOFTWARE_PWM_LED_CONTROLLER_HPP
