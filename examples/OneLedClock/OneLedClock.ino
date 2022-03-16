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

//#define DEBUG
/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode      ledBuiltin;         // declare an object of class PiscoCode
RTC_DS3231     rtc;                // declare an object of class RTC

const unsigned long  timeBetweenCounter = 30000UL;
unsigned long        lastMillis = timeBetweenCounter;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  #ifdef DEBUG
    Serial.begin(57600);
    Serial.println("One LED Clock!");
  #endif
  pinMode(LED_BUILTIN, OUTPUT);                    // initialize digital pin LED_BUILTIN as an output.                  
  ledBuiltin.setup(&turnLedOnOff, 1);              // calling the PiscoCode class constructor.

  if (! rtc.begin()) {
    #ifdef DEBUG
       Serial.println("Couldn't find RTC");
       Serial.flush();
       while (1) delay(10);
    #endif
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

// This function is used to turn the LED on and off. 
void turnLedOnOff(bool turnItON) {
  if ( turnItON ) {
     digitalWrite(LED_BUILTIN, HIGH);
  } else {
     digitalWrite(LED_BUILTIN, LOW);  
  }
}


void loop() {   
   if ( (unsigned long)(millis() - lastMillis) >= timeBetweenCounter &&     // If has passed X seconds and 
         ! ledBuiltin.isSequencing() ) {                                    // it is not sequencing.
      DateTime now = rtc.now();                                             // Get the correct time again 
      
      unsigned long    bcd_now = now.twelveHour()*100+now.minute();         // Convert it to BCD to be shown 

      ledBuiltin.showDec(bcd_now, 3, 1);                                    // Start sequencing the current time. 
      lastMillis = millis();                                                // Save the current time on lastMillis
   }  
   
   ledBuiltin.loop(millis());                                               // We should call the LOOP function regularly.
}
