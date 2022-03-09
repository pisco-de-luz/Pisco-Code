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
PiscoCode      ledRX;

//int   ledPort1 = LED_BUILTIN;
int   ledBuiltinPort = LED_BUILTIN;
int   ledRXPort = 1;


unsigned long        lastMillis = 0;
const unsigned long  timeBetweenCounter = 9000UL;


/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.end();

  // initialize digital pin ledPort1 as an output.
  pinMode(ledBuiltinPort, OUTPUT);
  pinMode(ledRXPort, OUTPUT);
  
  ledBuiltin.setup(ledBuiltinPort);  
  ledRX.setup(ledRXPort);  

  digitalWrite(ledBuiltinPort,HIGH);
  delay(100);
  digitalWrite(ledBuiltinPort,LOW);
  delay(300);
  digitalWrite(ledBuiltinPort,HIGH);
  delay(100);
  digitalWrite(ledBuiltinPort,LOW);
  digitalWrite(ledRXPort,HIGH);
  delay(1800);
  ledBuiltin.showDec(-12, 60, 2);
  delay(2000);
//  ledRX.showDec(8, 60, 1);
}

void turnLed1On(void) {
  
}

//unsigned long        lastMillis = 0;
//const unsigned long  timeBetweenCounter = 9000UL;

void loop()
{
  if ( (unsigned long)(millis() - lastMillis) >= timeBetweenCounter &&
         ! ledBuiltin.isSequencing() ) {
      ledBuiltin.showDec(millis()/1000, 30, 1);
      lastMillis = millis();
   }

   ledBuiltin.loop(millis());
//   ledRX.loop(millis());
}
