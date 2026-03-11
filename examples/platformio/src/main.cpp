// ----------------------------------------------------------------------------
// Pisco-Code PlatformIO Example
//
// Displays the value 42 in decimal via the onboard status LED using software
// PWM — no serial port, no display required.
//
// Digit 4 → four short blinks, digit 2 → two short blinks.
// A long-off framing period separates each complete sequence.
//
// Compatible with: Arduino Nano (AVR), ESP32, and any Arduino-framework board.
// ----------------------------------------------------------------------------

#include <Arduino.h>

#include "pisco_code.hpp"

using pisco_code::LedControllerSoftwarePwm;
using pisco_code::NumDigits;
using pisco_code::Radix;
using pisco_code::SignalCode;
using pisco_code::SignalEmitter;

// Callback that drives the onboard LED for software PWM (on/off toggle).
static void
ledWrite(uint8_t level)
{
    digitalWrite(LED_BUILTIN, level > 0 ? HIGH : LOW);
}

static LedControllerSoftwarePwm controller{ledWrite};
static SignalEmitter            emitter{controller};

static unsigned long lastMs = 0;

void
setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Display diagnostic code 42 in decimal (repeats automatically).
    // Replace 42 with any int value — negative values and hex/binary also work.
    emitter.showCode(SignalCode{42}, Radix::DEC, NumDigits{0});
}

void
loop()
{
    // Drive the Pisco-Code state machine at ~1 ms resolution.
    const unsigned long now = millis();
    if (now != lastMs)
    {
        lastMs = now;
        emitter.loop();
    }
}
