#include "board_config.hpp"
#include "hal_led.hpp"
#include <avr/io.h>

namespace
{

    inline void led_off() noexcept
    {
        NANO_LED_PORT &= ~(1 << NANO_LED_BIT);
    }
    inline void led_on() noexcept
    {
        NANO_LED_PORT |= (1 << NANO_LED_BIT);
    }

} // namespace

void
hal_led::init() noexcept
{
    NANO_LED_DDR |= (1 << NANO_LED_BIT);
    led_off();
}

bool
hal_led::Led1(LedControlCode code) noexcept
{
    switch (code)
    {
        case LedControlCode::ON:
            led_on();
            return true;
        case LedControlCode::OFF:
            led_off();
            return true;
        case LedControlCode::FUNC_OK:
            return true;
        default:
            return false;
    }
}
