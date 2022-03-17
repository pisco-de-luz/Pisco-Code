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

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);                    // initialize digital pin LED_BUILTIN as an output.                  
  ledBuiltin.setup(&turnLedOnOff, 2);  // calling the PiscoCode class constructor.
  //ledBuiltin.setup(&turnLed1On, &turnLed1Off, 2);  // calling the PiscoCode class constructor.  

  ledBuiltin.showDec(490, 15, 2);                  // display the 1024 number on BUILTIN led.
}


void loop() {
   if ( millis() >15000 && ! ledBuiltin.isSequencing() ) {
       ledBuiltin.showDec(30, 8, 1); // display some number on BUILTIN led repeatedly.    
   }
   ledBuiltin.loop(millis());                  // We should call the LOOP function regularly.

   // run other non-blocking function here
}


// Before using this function to turn the LED on and off, the caller will check if it is a valid
// pointer to a correct function, and it should respond to a LED_FUNC_OK call returning true. 
//
// This function will return true only if one of these three commands are received, LED_ON,
// LED_OFF, and LED_FUNC_OK. All other values will return false. 
bool turnLedOnOff(uint8_t ctrlLED) {
  bool funcOK = true;
  if ( ctrlLED == LED_ON ) {
     digitalWrite(LED_BUILTIN, HIGH);
  } else if ( ctrlLED == LED_OFF ) {
     digitalWrite(LED_BUILTIN, LOW);  
  } else if ( ctrlLED != LED_FUNC_OK ) {
     funcOK = false;
  }
  return( funcOK );
}
