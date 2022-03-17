#include "Pisco-LED-Code.h"

// The constructor
PiscoCode::PiscoCode(void) {
   currentDigit = 0;                        // Current digit to show. 
   lessSignificantDigit = 0;                // The less significant digit to be displayed.
   pwmSequence = 0;                         // PWM value of the most bright light the LED should blink. 
   pwmCounter = 0;                          // PWM counter from zero to pwmMax was used to set the PWM levels' timing.              
   sequenceTimes = 0;                       // Register the number of times we should repeat the PiscoCode. 
   currentPhase = 0;                        // The current phase we are working on now. 
   dimmedPWM = 0;                           // PWM value of the dimmed light the LED should stay on during the hole sequence. 
   startTimeLastPhase = 0;                  // Start time of the last phase. 
   currentPhaseDuration = 0;                // Register the total milliseconds this phase should last.
   isNegative = false;                      // It is true if the number to show is negative. 

}

void PiscoCode::setup(void (*LedOnOffFunc)(bool turnItON)) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             initialDimmedPWM;                                  // The initial value for dimmed LED if not chosen.
      pwmCounter =            0;                                                 // The counter's initial value is used to set the PWM levels' timing.
      LedOnOff =               LedOnOffFunc;                                     // Pointer to the LED activation function.
      LedOnOff(LED_OFF);                                                         // Turn the LED off
}

void PiscoCode::setup(void (*LedOnOffFunc)(bool turnItON), uint8_t dimPWM) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             dimPWM;                                            // The initial value for dimmed LED
      pwmCounter =            0;                                                 // The counter's initial value is used to set the PWM levels' timing.
      LedOnOff =               LedOnOffFunc;                                     // Pointer to the LED activation function.
      LedOnOff(LED_OFF);                                                         // Turn the LED off
}

// Function to check if there is a current sequence running. 
bool PiscoCode::isSequencing(void) {
   return( currentPhase != NOT_SEQUENCING );
}

