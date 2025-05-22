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
        case to_value(LedControlCode::ON):
            LED_PORT &= ~(1 << LED_PIN);
            return true;
        case to_value(LedControlCode::OFF):
            LED_PORT |= (1 << LED_PIN);
            return true;
        case to_value(LedControlCode::FUNC_OK):
            return true;
        default:
            return false;
    }
}

// Converts milliseconds to a 5-digit HHHMM representation
BlinkCode millisToBCDTime(Timestamp fakeMillis)
{
    constexpr Timestamp MILLIS_PER_MINUTE = 60000UL;
    constexpr Timestamp MINUTES_PER_HOUR  = 60;

    const Timestamp totalMinutes = fakeMillis / MILLIS_PER_MINUTE;
    const Timestamp hours        = totalMinutes / MINUTES_PER_HOUR;
    const Timestamp minutes      = totalMinutes % MINUTES_PER_HOUR;
    const Timestamp cappedHours  = (hours > 999) ? 999 : hours;

    return static_cast<BlinkCode>(cappedHours * 100 + minutes);
}

int main()
{
    // Set LED pin as output
    LED_DDR |= (1 << LED_PIN);

    PiscoCode led1(turnLedOnOff);

    // Configure blinking behavior
    led1.setDimmedLevel(3);

    Timestamp fakeMillis = 0; // 1 hour, 1 minute, 0 seconds

    led1.showCode(-102, NumberBase::DEC, 0, 3);
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
        if (!led1.isRunning() && fakeMillis <= static_cast<Timestamp>(INT32_MAX))
        {
            BlinkCode bcdTime = millisToBCDTime(fakeMillis);
            led1.setDimmedLevel(7);
            led1.showCode(bcdTime, NumberBase::DEC, 3, 1);
        }

        Counter loopCounter = static_cast<Counter>(fakeMillis >> 6);
        led1.loop(loopCounter);

        _delay_ms(1);
        fakeMillis++;
    }

    return 0;
}
