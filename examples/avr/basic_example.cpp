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

/**************************************************************************************
 * DEFINES
 **************************************************************************************/
#define LED_PORT   PORTD
#define LED_DDR    DDRD
#define LED_PIN    PD1    // TX‐LED on Nano

PiscoCode ledBuiltin;
bool ledBuiltinOK = false;

// Hardware abstraction for LED control
bool turnLedOnOff(uint8_t ctrlLED) {
    bool funcOK = true;
    switch (ctrlLED) {
        case PiscoCode::LED_ON:
            LED_PORT &= ~(1 << LED_PIN);
        break;
        case PiscoCode::LED_OFF:
            LED_PORT |=  (1 << LED_PIN);
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
    LED_DDR |= (1 << LED_PIN);

    // Setup PiscoCode instance
    ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff);
    ledBuiltin.setDimPWM(5);

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
