#include "avr_systick.hpp"
#include "hal_led.hpp"
#include "pisco_code.hpp"

int
main()
{
    avr_systick::init_1ms();
    hal_led::init();

    pisco_code::SoftwarePwmLedController controller_led1(hal_led::led1);
    pisco_code::SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setBaseLevel(3);
    emitter_led1.setRepeatTimes(2);
    emitter_led1.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});

    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        avr_systick::delay_ms(1);
    }
    for (;;)
    {
    }
}
