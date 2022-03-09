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
  pinMode(LED_BUILTIN, OUTPUT);    // initialize digital pin LED_BUILTIN as an output.                  
  ledBuiltin.setup(LED_BUILTIN);   // calling the PiscoCode class constructor.
  ledBuiltin.showDec(1024, 7, 1);  // display the 1024 number on BUILTIN led.
}

void loop() {
   if ( ! ledBuiltin.isSequencing() ) {        // if this PiscoCode object is not showing codes.
      ledBuiltin.showDec(millis()/3000, 7, 1); // display some number on BUILTIN led repeatedly.
   }   
   ledBuiltin.loop(millis());                  // We should call the LOOP function regularly.

   // run other non-blocking function here
}
