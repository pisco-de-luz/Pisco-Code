#include "avr_systick.hpp"
#include "hal_led.hpp"
#include "pisco_code.hpp"

using namespace pisco_code;

int
main()
{
    avr_systick::init_1ms();
    hal_led::init();
    SoftwarePwmLedController controller_led1(hal_led::Led1);
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
