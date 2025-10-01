#include "avr_systick.hpp"
#include "hal_led.hpp"
#include "pisco_code.hpp"

int
main()
{
    avr_systick::init_1ms();
    hal_led::ledOnboardInit();
    hal_led::ledHwPwmInit();

    // pisco_code::SoftwarePwmLedController controller_led1(hal_led::ledHwPwm);
    pisco_code::SoftwarePwmLedController controller_led1(hal_led::ledOnboard);
    pisco_code::HardwarePwmLedController controller_led2(
        hal_led::ledHwPwmSetLevel);
    pisco_code::SignalEmitter emitter_led1(&controller_led1);
    pisco_code::SignalEmitter emitter_led2(&controller_led2);

    controller_led1.setLowLevel(20);
    emitter_led1.setRepeatTimes(2);
    emitter_led1.showCode(SignalCode{123}, NumberBase::DEC, NumDigits{0});

    controller_led2.setLowLevel(20);
    emitter_led2.setRepeatTimes(2);
    emitter_led2.showCode(SignalCode{-103}, NumberBase::DEC, NumDigits{0});

    while (emitter_led1.isRunning() || emitter_led2.isRunning())
    {
        emitter_led1.loop();
        emitter_led2.loop();
        avr_systick::delay_ms(1);
    }
    for (;;)
    {
    }
}
