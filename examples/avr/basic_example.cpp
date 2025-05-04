/* Pisco-Code.ino
 *
 * This sketch demonstrates how to use the Pisco-Code
 * library to show decimal or hexadecimal values using just
 * a single LED.
 *
 * These values can be shown as positive or negative when needed.
 *
 * The Pisco-Code library is a nom blocking function that
 * should be called frequently from the loop function.
 *
 * Andre Viegas
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/
#include "Pisco-Code.hpp"
#include <avr/io.h>
#include <stdint.h> // NOLINT(modernize-deprecated-headers)
#include <util/delay.h>

/**************************************************************************************
 * DEFINES
 **************************************************************************************/
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_PIN PD1 // TX‐LED on Nano

// Hardware abstraction for LED control
bool turnLedOnOff(uint8_t ctrlLED)
{
    bool funcOK = true;
    switch (ctrlLED)
    {
        case pisco::LED_ON:
            LED_PORT &= ~(1 << LED_PIN);
            break;
        case pisco::LED_OFF:
            LED_PORT |= (1 << LED_PIN);
            break;
        case pisco::LED_FUNC_OK:
            break; // Nothing to do, just acknowledge
        default:
            funcOK = false;
            break;
    }
    return funcOK;
}

// Converts milliseconds to a 5-digit HHHMM representation
// Example: 2 hours 34 minutes → 234
// Ensures result fits safely in a signed 32-bit int
int32_t millisToBCDTime(uint32_t fakeMillis)
{
    constexpr uint32_t MILLIS_PER_MINUTE = 60000UL;
    constexpr uint32_t MINUTES_PER_HOUR  = 60;

    const uint32_t totalMinutes = fakeMillis / MILLIS_PER_MINUTE;
    const uint32_t hours        = totalMinutes / MINUTES_PER_HOUR;
    const uint32_t minutes      = totalMinutes % MINUTES_PER_HOUR;

    // Cap at 999 hours to fit HHHMM (max = 99959)
    const uint32_t cappedHours = (hours > 999) ? 999 : hours;

    return static_cast<int32_t>(cappedHours * 100 + minutes);
}

int main()
{
    PiscoCode ledBuiltin;
    bool      ledBuiltinOK = false;

    // Set LED pin as output
    LED_DDR |= (1 << LED_PIN);

    // Setup PiscoCode instance
    ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff);
    ledBuiltin.setDimPwm(5);

    uint32_t fakeMillis = 0;

    if (ledBuiltinOK)
    {
        ledBuiltin.showCode(14, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    }

    while (true)
    {
        if (ledBuiltinOK && !ledBuiltin.isSequencing() &&
            fakeMillis <= static_cast<uint32_t>(INT32_MAX))
        {
            // Convert fakeMillis to a 5-digit HHHMM representation
            ledBuiltin.setMinDigits(3); // Guarantee 3 digits to represent H:MM
            ledBuiltin.showCode(millisToBCDTime(fakeMillis),
                                static_cast<uint8_t>(pisco::base_t::DECIMAL));
        }

        // Call loop function with a counter based on 64ms steps
        const auto loopCounter = static_cast<uint8_t>(fakeMillis >> 6);
        ledBuiltin.loop(loopCounter);

        _delay_ms(1); // delay ~1 ms
        fakeMillis++;
    }

    return 0;
}
