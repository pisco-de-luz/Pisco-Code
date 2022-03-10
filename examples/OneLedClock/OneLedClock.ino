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
#include "RTClib.h"


/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode      ledBuiltin;         // declare an object of class PiscoCode
RTC_DS1307     rtc;                // declare an object of class RTC

const unsigned long  timeBetweenCounter = 30000UL;
unsigned long        lastMillis = timeBetweenCounter;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
//  Serial.begin(57600);
//  Serial.println("One LED Clock!");
  pinMode(LED_BUILTIN, OUTPUT);                    // initialize digital pin LED_BUILTIN as an output.                  
  ledBuiltin.setup(&turnLed1On, &turnLed1Off, 0);  // calling the PiscoCode class constructor.
  //ledBuiltin.showDec(2, 15, 2);                  // display the 1024 number on BUILTIN led.
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  } else {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }  
}

void turnLed1On(void) {
  digitalWrite(LED_BUILTIN, HIGH);
}

void turnLed1Off(void) {
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
   if ( (unsigned long)(millis() - lastMillis) >= timeBetweenCounter &&
         ! ledBuiltin.isSequencing() ) {
      DateTime now = rtc.now();
      
      unsigned long    bcd_now = now.twelveHour()*100+now.minute();
      
      ledBuiltin.showDec(bcd_now, 10, 1);
      lastMillis = millis();
   }  
   
   ledBuiltin.loop(millis());                  // We should call the LOOP function regularly.
}
