/* Pisco-LED-Code.ino
 * 
 * This sketch demonstrates how to use the Pisco-LED-Code
 * library to show decimal or hexadecimal values using just
 * a single LED. 
 * 
 * These values can be shown as positive or negative when needed.
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

PiscoCode      ledBuiltin;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  ledBuiltin.setup(ledBuiltinPort);  

  ledBuiltin.showDec(1024, 60, 2);  
}

void loop()
{
   ledBuiltin.loop(millis());
}
