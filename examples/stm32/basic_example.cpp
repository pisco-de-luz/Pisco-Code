#include "hal_led.hpp"
#include "pisco_code.hpp"
#include "stm32_systick.hpp"

int
main()
{
    stm32_systick::init_1ms();
    hal_led::init();

    pisco_code::SoftwarePwmLedController controller_led1(hal_led::led1);
    pisco_code::SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setDimmedLevel(3);
    emitter_led1.setRepeatTimes(2);
    emitter_led1.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});

    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        stm32_systick::delay_ms(1);
    }
    for (;;)
    {
    }
}
