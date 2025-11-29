// //--------------------------------------------------------------------------------------------
// // Basic Example - Demonstrates Pisco Code library with two LEDs
// //
// // LED1 (Onboard): Software PWM controller - displays code 123 in decimal
// // LED2 (PWM):     Hardware PWM controller - displays code -102 in decimal
// //--------------------------------------------------------------------------------------------

#include "hal_led.hpp"
#include "pisco_code.hpp"
#include "stm32_systick.hpp"

int
main()
{
    stm32_systick::init_1ms();
    hal_led::init();

    // LED1: Software PWM controller (on/off toggle for onboard LED)
    pisco_code::LedControllerSoftwarePwm controller_led1(hal_led::ledOnboard);
    pisco_code::SignalEmitter            emitter_led1(&controller_led1);

    // LED2: Hardware PWM controller (smooth dimming for external LED)
    pisco_code::LedControllerHardwarePwm controller_led2(
        hal_led::ledPwmSetLevel);
    pisco_code::SignalEmitter emitter_led2(&controller_led2);

    // Start displaying codes
    emitter_led1.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});
    emitter_led2.showCode(SignalCode{-102}, NumberBase::DEC, NumDigits{0});

    // Main loop - call every 1ms
    while (emitter_led1.isRunning() || emitter_led2.isRunning())
    {
        emitter_led1.loop();
        emitter_led2.loop();
        stm32_systick::delay_ms(1);
    }

    // Halt
    for (;;)
    {
    }
}
