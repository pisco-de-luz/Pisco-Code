#include <stdint.h>

#include "avr_systick.hpp"
#include "pisco_code.hpp"
#include <avr/io.h>

using namespace pisco_code;

namespace
{
    constexpr uint8_t LED_PIN_PD1 = PD0; // Nano TX LED
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
    bool swPwmLed1(LedControlCode code) noexcept
    {
        switch (code)
        {
            case LedControlCode::ON:
                ledOn();
                return true;
            case LedControlCode::OFF:
                ledOff();
                return true;
            case LedControlCode::FUNC_OK:
                return true;
            default:
                return false;
        }
    }
} // namespace

int
main()
{
    avr_systick::systick_init_1ms();
    ledInit();
    SoftwarePwmLedController controller_led1(swPwmLed1);
    SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setDimmedLevel(3);

    const RepeatTimes repeats{3};
    const NumDigits   num_digits{0};
    const SignalCode  signal_code{-102};

    emitter_led1.showCode(signal_code, NumberBase::DEC, num_digits, repeats);
    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        avr_systick::delay_ms(1);
    }
    for (;;)
    {
    }

    return 0;
}
