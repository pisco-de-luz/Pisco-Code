#include "code_blinker.hpp"
#include "software_pwm_led_controller.hpp"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

using namespace pisco;

/**************************************************************************************
 * DEFINES
 **************************************************************************************/
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_PIN PD1 // TX‐LED on Nano

// Hardware abstraction for LED control
bool turnLedOnOff(uint8_t ctrlLED)
{
    switch (ctrlLED)
    {
        case LED_ON:
            LED_PORT &= ~(1 << LED_PIN);
            return true;
        case LED_OFF:
            LED_PORT |= (1 << LED_PIN);
            return true;
        case LED_FUNC_OK:
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

    // Create controller and blinker
    SoftwarePwmLedController ledController(turnLedOnOff);
    CodeBlinker              blinker(&ledController);

    // Configure blinking behavior
    blinker.setDimmedLevel(5);

    uint32_t fakeMillis = 0;

    blinker.showCode(14, base_t::DECIMAL, 3, 1);

    while (true)
    {
        if (!blinker.isRunning() && fakeMillis <= static_cast<uint32_t>(INT32_MAX))
        {
            int32_t bcdTime = millisToBCDTime(fakeMillis);
            blinker.showCode(bcdTime, base_t::DECIMAL, 3, 1);
        }

        uint8_t loopCounter = static_cast<uint8_t>(fakeMillis >> 6);
        blinker.loop(loopCounter);

        _delay_ms(1);
        fakeMillis++;
    }

    return 0;
}
