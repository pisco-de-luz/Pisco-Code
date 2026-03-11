// Pisco-Code PlatformIO Example — displays -103 via the onboard LED.
// Demonstrates negative sign and zero-digit handling with software PWM.
// Compatible with Arduino Nano (AVR), ESP32, and any Arduino-framework board.

#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#include "pisco_code.hpp"

using pisco_code::LedControlCode;
using pisco_code::LedControllerSoftwarePwm;
using pisco_code::NumDigits;
using pisco_code::Radix;
using pisco_code::SignalCode;
using pisco_code::SignalEmitter;

static bool
ledWrite(LedControlCode code)
{
    digitalWrite(LED_BUILTIN, code == LedControlCode::ON ? HIGH : LOW);
    return true;
}

static LedControllerSoftwarePwm controller{ledWrite};
static SignalEmitter            emitter{controller};

static unsigned long lastMs = 0;

void
setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    emitter.showCode(SignalCode{-103}, Radix::DEC, NumDigits{0});
}

void
loop()
{
    const unsigned long now = millis();
    if (now != lastMs)
    {
        lastMs = now;
        emitter.loop();
    }
}
