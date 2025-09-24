#include "hal_led.hpp"
#include "pisco_code.hpp"
#include "stm32_systick.hpp"

using namespace pisco_code;

int
main()
{
    stm32_systick::init_1ms();
    hal_led::init();
    hal_led::Led1(LedControlCode::ON);
    stm32_systick::delay_ms(2000);
    hal_led::Led1(LedControlCode::OFF);

    pisco_code::SoftwarePwmLedController controller_led1(hal_led::Led1);
    pisco_code::SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setDimmedLevel(3);
    emitter_led1.showCode(SignalCode{-102}, NumberBase::DEC, NumDigits{0},
                          RepeatTimes{3});

    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        stm32_systick::delay_ms(1);
    }
    while (1)
    {
    }
}
