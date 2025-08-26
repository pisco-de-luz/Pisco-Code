#include <stdint.h>

#include "pisco_code.hpp"
#include <avr/io.h>
#include <util/delay.h>

using namespace pisco_code;

namespace
{
    constexpr uint8_t LED_PIN_PD1 = PD1; // Nano TX LED
    inline void       ledInit() noexcept
    {
        DDRD |= static_cast<uint8_t>(1U << LED_PIN_PD1);
    }
    inline void ledOn() noexcept
    {
        PORTD &= static_cast<uint8_t>(~(1U << LED_PIN_PD1));
    }
    inline void ledOff() noexcept
    {
        PORTD |= static_cast<uint8_t>(1U << LED_PIN_PD1);
    }

    // Adapter from library control codes to board I/O
    bool turnLedOnOff(LedCodeType code) noexcept
    {
        switch (code)
        {
            case pisco_code::to_value(LedControlCode::ON):
                ledOn();
                return true;
            case pisco_code::to_value(LedControlCode::OFF):
                ledOff();
                return true;
            case pisco_code::to_value(LedControlCode::FUNC_OK):
                return true;
            default:
                return false;
        }
    }
} // namespace

int
main()
{
    ledInit();

    PiscoCode led1(turnLedOnOff);

    led1.setDimmedLevel(3);

    Timestamp fake_millis = 0;

    led1.showCode(102, NumberBase::DEC, 0, static_cast<RepeatTimes>(2));
    while (led1.isRunning())
    {
        led1.loop(fake_millis++ >> 6);
        _delay_ms(1);
    }
    for (;;)
    {
    }

    return 0;
}
