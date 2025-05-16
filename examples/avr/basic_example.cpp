#include <stdint.h>

#include "pisco_code.hpp"
#include <avr/io.h>
#include <util/delay.h>

using namespace pisco_code;

/**************************************************************************************
 * DEFINES
 **************************************************************************************/
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_PIN PD1 // TX‐LED on Nano

// Hardware abstraction for LED control
bool turnLedOnOff(LedCodeType ctrlLED)
{
    switch (ctrlLED)
    {
        case static_cast<LedCodeType>(LedControlCode::On):
            LED_PORT &= ~(1 << LED_PIN);
            return true;
        case static_cast<LedCodeType>(LedControlCode::Off):
            LED_PORT |= (1 << LED_PIN);
            return true;
        case static_cast<LedCodeType>(LedControlCode::FuncOk):
            return true;
        default:
            return false;
    }
}

// Converts milliseconds to a 5-digit HHHMM representation
int32_t millisToBCDTime(uint32_t fakeMillis)
{
    constexpr uint32_t MILLIS_PER_MINUTE = 60000UL;
    constexpr uint32_t MINUTES_PER_HOUR  = 60;

    const uint32_t totalMinutes = fakeMillis / MILLIS_PER_MINUTE;
    const uint32_t hours        = totalMinutes / MINUTES_PER_HOUR;
    const uint32_t minutes      = totalMinutes % MINUTES_PER_HOUR;
    const uint32_t cappedHours  = (hours > 999) ? 999 : hours;

    return static_cast<int32_t>(cappedHours * 100 + minutes);
}

int main()
{
    // Set LED pin as output
    LED_DDR |= (1 << LED_PIN);

    pisco_code::PiscoCode led1(turnLedOnOff);

    // Configure blinking behavior
    led1.setDimmedLevel(3);

    uint32_t fakeMillis = 0; // 1 hour, 1 minute, 0 seconds

    led1.showCode(-102, NumberBase::DECIMAL, 0, 3);
    while (led1.isRunning())
    {
        led1.loop(fakeMillis++ >> 6);
        _delay_ms(1);
    }
    while (true)
    {
    }
    while (true)
    {
        if (!led1.isRunning() && fakeMillis <= static_cast<uint32_t>(INT32_MAX))
        {
            int32_t bcdTime = millisToBCDTime(fakeMillis);
            led1.setDimmedLevel(7);
            led1.showCode(bcdTime, NumberBase::DECIMAL, 3, 1);
        }

        uint8_t loopCounter = static_cast<uint8_t>(fakeMillis >> 6);
        led1.loop(loopCounter);

        _delay_ms(1);
        fakeMillis++;
    }

    return 0;
}
