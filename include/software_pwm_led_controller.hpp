#ifndef SOFTWARE_PWM_LED_CONTROLLER_HPP
#define SOFTWARE_PWM_LED_CONTROLLER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
// #include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace pisco_code
{
    class SoftwarePwmLedController : public LedController
    {
      public:
        explicit SoftwarePwmLedController(bool (*led_func)(LedCodeType));
        explicit SoftwarePwmLedController();
        ~SoftwarePwmLedController();
        void attachLedControl(bool (*led_func)(LedCodeType));

        void setPeakLevel(LedLevel level) override;
        void setDimmedLevel(LedLevel level) override;
        void setBlinkMode(BlinkMode mode) override;
        void update(LoopCounter loop_counter) override;

      private:
        LedLevel peak_level_              = PWM_MAX;
        LedLevel dimmed_level_            = DEFAULT_DIMMED_LEVEL;
        bool (*led_control_)(LedCodeType) = nullptr;
        BlinkMode mode_                   = BlinkMode::NONE;
    };

} // namespace pisco_code

#endif // SOFTWARE_PWM_LED_CONTROLLER_HPP
