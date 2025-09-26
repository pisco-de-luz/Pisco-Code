#include <stm32f4xx.h>

#include "board_config.hpp"
#include "hal_led.hpp"

namespace
{
    void led_on() noexcept
    {
        LED_GPIO->BSRR = (1 << LED_PIN);
    }
    void led_off() noexcept
    {
        LED_GPIO->BSRR = (1 << (LED_PIN + 16));
    }
} // namespace

void
hal_led::init() noexcept
{
    RCC->AHB1ENR |= (1 << LED_RCC_BIT);
    LED_GPIO->MODER &= ~(0b11 << (2 * LED_PIN));
    LED_GPIO->MODER |= (0b01 << (2 * LED_PIN));
    led_off();
}

bool
hal_led::led1(LedControlCode code) noexcept
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
