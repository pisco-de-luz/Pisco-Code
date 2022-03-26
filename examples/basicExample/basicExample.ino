/* Pisco-LED-Code.ino
 * 
 * This sketch demonstrates how to use the Pisco-LED-Code
 * library to show decimal or hexadecimal values using just
 * a single LED. 
 * 
 * These values can be shown as positive or negative when needed.
 * 
 * The Pisco-LED-Code library is a nom blocking function that 
 * should be called frequently from the loop function. 
 * 
 * Andre Viegas
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/
#include "Pisco-LED-Code.h"

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode      ledBuiltin;         // declare an object of class PiscoCode
bool           ledBuiltinOK;       // It is safe to show codes with ledBuiltin?

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);                               // initialize digital pin LED_BUILTIN as an output.                  
  if ( ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff) ) {     // calling the PiscoCode class constructor.
     ledBuiltin.showCode(12,PiscoCode::BINARY);                                // display the 1024 number on BUILTIN led.  
  }
}


void loop() {
   if ( ledBuiltinOK && ! ledBuiltin.isSequencing() ) {       // If ledBuiltin was set up and is not sequencing any code
       ledBuiltin.showCode(millis()/5000,PiscoCode::DECIMAL);                     // display some number on BUILTIN led repeatedly.    
   }
   ledBuiltin.loop(millis());                                 // We should call the LOOP function regularly.

   // run other non-blocking function here
}


// Before using this function to turn the LED on and off, the caller will check if it is a valid
// pointer to a correct function, and it should respond to a LED_FUNC_OK call returning true. 
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
