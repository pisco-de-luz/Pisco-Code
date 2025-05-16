// led_controller.hpp
#ifndef PISCO_LED_CONTROLLER_HPP
#define PISCO_LED_CONTROLLER_HPP

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace pisco_code
{

    enum class BlinkMode
    {
        None,
        Pulse,
        Dimmed
    };

    class LedController
    {
      public:
        virtual void setPeakLevel(uint8_t level)   = 0;
        virtual void setDimmedLevel(uint8_t level) = 0;
        virtual void update(uint8_t pwm_counter) { (void) pwm_counter; }
        virtual void setBlinkMode(BlinkMode mode) = 0;
    };

} // namespace pisco_code

#endif // PISCO_LED_CONTROLLER_HPP
