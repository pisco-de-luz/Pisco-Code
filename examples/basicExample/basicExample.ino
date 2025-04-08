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
#include "Pisco-Code.h"

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode      ledBuiltin;
bool           ledBuiltinOK;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  if ( ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff) ) {
     ledBuiltin.showCode(1024,PiscoCode::DECIMAL);            // display the 1024 number on BUILTIN led.  
  }
}


void loop() {
   if ( ledBuiltinOK && ! ledBuiltin.isSequencing() ) {
       ledBuiltin.showCode(millis()/1000,PiscoCode::DECIMAL);
   }
   ledBuiltin.loop( (millis()>>6) & 0xff );                   // We should call the LOOP function regularly.
   // run other non-blocking function here
}


// Before using this function to turn the LED on and off, the caller will check if it is a valid
// function pointer, and it should respond to a LED_FUNC_OK call returning true. 
//
// This function will return true only if one of these three commands are received, LED_ON,
// LED_OFF, and LED_FUNC_OK. All other values will return false. 
bool turnLedOnOff(uint8_t ctrlLED) {
  bool funcOK = true;
  if ( ctrlLED == PiscoCode::LED_ON ) {              digitalWrite(LED_BUILTIN, HIGH);
  } else if ( ctrlLED == PiscoCode::LED_OFF ) {      digitalWrite(LED_BUILTIN, LOW);  
  } else if ( ctrlLED != PiscoCode::LED_FUNC_OK ) {  funcOK = false; }
  return( funcOK );
}
