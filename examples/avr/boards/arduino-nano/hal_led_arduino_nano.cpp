#include "board_config.hpp"
#include "hal_led.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include <avr/io.h>

using LedControlCode = pisco_code::LedControlCode;
using IntensityLevel = pisco_code::IntensityLevel;
namespace
{

    inline void ledOnboardOff() noexcept
    {
        LED_PORT &= ~(1 << LED_BIT);
    }
    inline void ledOnboardOn() noexcept
    {
        LED_PORT |= (1 << LED_BIT);
    }
    inline void ledHwPwmOff() noexcept
    {
        hal_led::ledHwPwmSetLevel(0);
    }
    inline void ledHwPwmOn() noexcept
    {
        hal_led::ledHwPwmSetLevel(255);
    }

} // namespace

void
hal_led::ledOnboardInit() noexcept
{
    // Configure LED1 (D13/PB5) as output
    LED_DDR |= (1 << LED_BIT);
    LED_PORT &= ~(1 << LED_BIT); // Start OFF
}

void
hal_led::ledHwPwmInit() noexcept
{
    // Configure LED2 (D3/PD3) as output
    PWM_LED_DDR |= (1 << PWM_LED_BIT);

    // Setup Timer2 for Fast PWM mode on OC2B (PD3)
    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM mode
    TCCR2B |= (1 << CS21);                 // Prescaler 8

    // Start with LED OFF
    OCR2B = 0; // 0% duty cycle = LED OFF
    PWM_LED_PORT &= ~(1 << PWM_LED_BIT);
}

void
hal_led::ledHwPwmSetLevel(IntensityLevel level) noexcept
{
    if (level == 0)
    {
        // Disconnect PWM from pin and force LOW
        TCCR2A &= ~(1 << COM2B1); // Disconnect OC2B from pin
        TCCR2A &= ~(1 << COM2B0);
        PWM_LED_PORT &= ~(1 << PWM_LED_BIT); // Force pin LOW
    }
    else
    {
        // Reconnect PWM to pin
        TCCR2A |= (1 << COM2B1);  // Clear OC2B on compare match
        TCCR2A &= ~(1 << COM2B0); // (COM2B1:0 = 10)
        OCR2B = level;            // Set PWM duty cycle
    }
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

bool
hal_led::ledHwPwm(LedControlCode code) noexcept
{
    switch (code)
    {
        case LedControlCode::ON:
            ledHwPwmOn();
            return true;
        case LedControlCode::OFF:
            ledHwPwmOff();
            return true;
        case LedControlCode::FUNC_OK:
            return true;
        default:
            return false;
    }
}
