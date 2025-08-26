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
bool
turnLedOnOff(LedCodeType ctrlLED)
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

int
main()
{
    // Set LED pin as output
    LED_DDR |= (1 << LED_PIN);

    PiscoCode led1(turnLedOnOff);

    led1.setDimmedLevel(3);

    Timestamp fakeMillis = 0;

    led1.showCode(-102, NumberBase::DEC, 0, 2);
    while (led1.isRunning())
    {
        led1.loop(fakeMillis++ >> 6);
        _delay_ms(1);
    }
    for (;;)
    {
    }

    return 0;
}
