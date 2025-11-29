// //--------------------------------------------------------------------------------------------
// // HAL LED implementation for Arduino Nano (ATmega328P)
// //
// // LED1 (Onboard): PB5 (D13) - Software PWM (on/off toggle)
// // LED2 (PWM):     PD3 (D3)  - Hardware PWM via Timer2/OC2B
// //--------------------------------------------------------------------------------------------

#include "board_config.hpp"
#include "hal_led.hpp"
#include <avr/io.h>

using LedControlCode = pisco_code::LedControlCode;
using IntensityLevel = pisco_code::IntensityLevel;

namespace
{
    inline void ledOnboardOn() noexcept
    {
        LED_ONBOARD_PORT |= (1 << LED_ONBOARD_BIT);
    }

    inline void ledOnboardOff() noexcept
    {
        LED_ONBOARD_PORT &= ~(1 << LED_ONBOARD_BIT);
    }

    void initOnboardLed() noexcept
    {
        // Configure onboard LED (D13/PB5) as output, start OFF
        LED_ONBOARD_DDR |= (1 << LED_ONBOARD_BIT);
        ledOnboardOff();
    }

    void initPwmLed() noexcept
    {
        // Configure PWM LED (D3/PD3) as output
        LED_PWM_DDR |= (1 << LED_PWM_BIT);

        // Setup Timer2 for Fast PWM mode on OC2B (PD3)
        TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM mode
        TCCR2B |= (1 << CS21);                 // Prescaler 8

        // Start with LED OFF
        OCR2B = 0;
        LED_PWM_PORT &= ~(1 << LED_PWM_BIT);
    }

} // namespace

void
hal_led::init() noexcept
{
    initOnboardLed();
    initPwmLed();
}

bool
hal_led::ledOnboard(LedControlCode code) noexcept
{
    switch (code)
    {
        case LedControlCode::ON:
            ledOnboardOn();
            return true;
        case LedControlCode::OFF:
            ledOnboardOff();
            return true;
        case LedControlCode::FUNC_OK:
            return true;
        default:
            return false;
    }
}

void
hal_led::ledPwmSetLevel(IntensityLevel level) noexcept
{
    if (level == 0)
    {
        // Disconnect PWM from pin and force LOW
        TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0));
        LED_PWM_PORT &= ~(1 << LED_PWM_BIT);
    }
    else
    {
        // Connect PWM to pin (Clear OC2B on compare match)
        TCCR2A = (TCCR2A & ~(1 << COM2B0)) | (1 << COM2B1);
        OCR2B  = level;
    }
}
