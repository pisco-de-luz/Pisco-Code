#ifndef SOFTWARE_PWM_LED_CONTROLLER_HPP
#define SOFTWARE_PWM_LED_CONTROLLER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
// #include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace pisco
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
        void turnOff() override;
        void update(uint8_t pwm_counter) override;

      private:
        bool (*led_control_)(uint8_t) = nullptr;
        uint8_t   peak_level_         = PWM_MAX;
        uint8_t   dimmed_level_       = INITIAL_DIMMED_PWM;
        BlinkMode mode_               = BlinkMode::None;
    };

} // namespace pisco

#endif // SOFTWARE_PWM_LED_CONTROLLER_HPP
