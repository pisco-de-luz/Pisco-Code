// //--------------------------------------------------------------------------------------------
// // BasicBlink — Display code 42 on the built-in LED using Pisco Code
// //
// // Call emitter.loop() once per millisecond.
// //--------------------------------------------------------------------------------------------

#include <Pisco-Code.h>

// HAL callback: controls the onboard LED via digital pin
bool
ledOnboard(pisco_code::LedControlCode code)
{
    switch (code)
    {
        case pisco_code::LedControlCode::ON:
            digitalWrite(LED_BUILTIN, HIGH);
            return true;
        case pisco_code::LedControlCode::OFF:
            digitalWrite(LED_BUILTIN, LOW);
            return true;
        default:
            return false;
    }
}

pisco_code::LedControllerSoftwarePwm controller(ledOnboard);
pisco_code::SignalEmitter            emitter(controller);

void
setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Display code 103 in decimal, auto-detect number of digits
    emitter.showCode(pisco_code::SignalCode{103}, pisco_code::NumberBase::DEC,
                     pisco_code::NumDigits{0});
}

void
loop()
{
    emitter.loop();
    delay(1);
}
