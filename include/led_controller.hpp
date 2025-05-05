// led_controller.hpp
#ifndef PISCO_LED_CONTROLLER_HPP
#define PISCO_LED_CONTROLLER_HPP

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace pisco
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
        virtual ~LedController()                   = default;
        virtual void setPeakLevel(uint8_t level)   = 0;
        virtual void setDimmedLevel(uint8_t level) = 0;
        virtual void turnOff()                     = 0;
        virtual void update(uint8_t pwm_counter) { (void) pwm_counter; }
        virtual void setBlinkMode(BlinkMode mode) = 0;
    };

    // class SoftwarePwmLedController : public LedController
    // {
    //   public:
    //     SoftwarePwmLedController();

    //     void update(uint8_t pwm_counter) override;

    //     void setPeakLevel(uint8_t level) override;
    //     void setDimmedLevel(uint8_t level) override;
    //     void turnOff() override;

    //     void attachLedControl(bool (*led_func)(uint8_t));

    //   private:
    //     uint8_t peak_level_           = 15;
    //     uint8_t dimmed_level_         = 0;
    //     bool (*led_control_)(uint8_t) = nullptr;
    //     BlinkMode mode_               = BlinkMode::None;
    // };

} // namespace pisco

#endif // PISCO_LED_CONTROLLER_HPP
