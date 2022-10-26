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
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
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
//  Serial.begin(115200);
//  Serial.println("One LED Clock!");

  // initialize digital pin ledPort1 as an output.
  pinMode(ledBuiltinPort, OUTPUT);
  pinMode(ledRXPort, OUTPUT);
  
  ledBuiltin.setup(&turnLed1On, &turnLed1Off, 1);  
  ledRX.setup(&turnLed2On, &turnLed2Off, 1);  
  //ledBuiltin.setup(ledBuiltinPort);  
  //ledRX.setup(ledRXPort);  

  digitalWrite(ledBuiltinPort,HIGH);
  delay(100);
  digitalWrite(ledBuiltinPort,LOW);
  delay(300);
  digitalWrite(ledBuiltinPort,HIGH);
  delay(100);
  digitalWrite(ledBuiltinPort,LOW);
  digitalWrite(ledRXPort,HIGH);
  delay(800);
  ledBuiltin.showDec(-20, 7, 2);
  ledRX.showDec(8, 15, 1);
}


//unsigned long        lastMillis = 0;
//const unsigned long  timeBetweenCounter = 9000UL;

void loop()
{
   if ( (unsigned long)(millis() - lastMillis) >= timeBetweenCounter &&
         ! ledBuiltin.isSequencing() ) {
      ledBuiltin.showDec(millis()/1000, 15, 1);
      lastMillis = millis();
   }
   if ( millis() > 12000 && millis() < 13000 ) {
      ledRX.showDec(18, 7, 1);
   }

   ledBuiltin.loop(millis());
   ledRX.loop(millis());
}


void turnLed1On(void) {
  digitalWrite(ledBuiltinPort,HIGH);
  //Serial.println("ON");
}

void turnLed1Off(void) {
  digitalWrite(ledBuiltinPort,LOW);
  //Serial.println("OFF");
}

void turnLed2On(void) {
  digitalWrite(ledRXPort,LOW);
}

void turnLed2Off(void) {
  digitalWrite(ledRXPort,HIGH);
}
