/* Pisco-Code.ino
 * 
 * This sketch demonstrates how to use the Pisco-Code
 * library to show decimal or hexadecimal values using just
 * a single LED. 
 * 
 * These values can be shown as positive or negative when needed.
 * 
 * The Pisco-Code library is a nom blocking function that 
 * should be called frequently from the loop function. 
 * 
 * Andre Viegas
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "Pisco-Code.hpp"

// // LED pin (ATmega328P: PB5 = digital pin 13)
// #define LED_BUILTIN_PIN PB5

// Nano’s TX LED is on PD1 = digital pin 1
#define LED_BUILTIN_PIN PD1

PiscoCode ledBuiltin;
bool ledBuiltinOK = false;

// Hardware abstraction for LED control
bool turnLedOnOff(uint8_t ctrlLED) {
    bool funcOK = true;
    switch (ctrlLED) {
        case PiscoCode::LED_ON:
            PORTB |= (1 << LED_BUILTIN_PIN);  // Set pin high
            break;
        case PiscoCode::LED_OFF:
            PORTB &= ~(1 << LED_BUILTIN_PIN); // Set pin low
            break;
        case PiscoCode::LED_FUNC_OK:
            break;  // Nothing to do, just acknowledge
        default:
            funcOK = false;
            break;
    }
    return funcOK;
}

int main() {
    // Set LED pin as output
    DDRB |= (1 << LED_BUILTIN_PIN);

    // Setup PiscoCode instance
    ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff);

    uint32_t fakeMillis = 0;

    if (ledBuiltinOK) {
        ledBuiltin.showCode(1024, PiscoCode::DECIMAL);
    }

    while (1) {
        if (ledBuiltinOK && !ledBuiltin.isSequencing()) {
            ledBuiltin.showCode(fakeMillis / 1000, PiscoCode::DECIMAL);
        }

        // Call loop function with a counter based on 64ms steps
        ledBuiltin.loop((fakeMillis >> 6) & 0xFF);

        _delay_ms(1);  // delay ~1 ms
        fakeMillis++;
    }

    return 0;
}
