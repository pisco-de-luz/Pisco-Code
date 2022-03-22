#include "Pisco-LED-Code.h"

// The constructor
PiscoCode::PiscoCode(void) {
   currentDigit =               0;       // Current digit to show. 
   lessSignificantDigit =       0;       // The less significant digit to be displayed.
   pwmSequence =                0;       // PWM value of the most bright light the LED should blink. 
   pwmCounter =                 0;       // PWM counter from zero to pwmMax was used to set the PWM levels' timing.              
   sequenceTimes =              0;       // Register the number of times we should repeat the PiscoCode. 
   currentPhase =               0;       // The current phase we are working on now. 
   dimmedPWM =                  0;       // PWM value of the dimmed light the LED should stay on during the hole sequence. 
   startTimeLastPhase =         0;       // Start time of the last phase. 
   currentPhaseDuration =       0;       // Register the total milliseconds this phase should last.
   isNegative =             false;       // It is true if the number to show is negative. 
   LedOnOff =             nullptr;       // Initialize this function pointer to null.
   setupOK =                false;
}

bool PiscoCode::setup(bool (*LedOnOffFunc)(uint8_t ctrlLED)) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =              initialDimmedPWM;                                 // The initial value for dimmed LED if not chosen.
      pwmCounter =             0;                                                // The counter's initial value is used to set the PWM levels' timing.
      LedOnOff =               LedOnOffFunc;                                     // Pointer to the LED activation function.
      if ( isExternalLedFuncOk() ) {                                             // If the external LED activation function is working correctly.
         setupOK = true;                                                         // Set the return variable as OK
         (void)switchLED(TURN_LED_OFF);                                          // Turn the LED off
      }
      return( setupOK );
}

bool PiscoCode::setup(bool (*LedOnOffFunc)(uint8_t ctrlLED), uint8_t dimPWM) {
      setupOK =                false;
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =              dimPWM;                                           // The initial value for dimmed LED
      pwmCounter =             0;                                                // The counter's initial value is used to set the PWM levels' timing.
      LedOnOff =               LedOnOffFunc;                                     // Pointer to the LED activation function.
      if ( isExternalLedFuncOk() ) {                                             // If the external LED activation function is working correctly.
         setupOK = true;                                                         // Set the return variable as OK
         (void)switchLED(TURN_LED_OFF);                                          // Turn the LED off
      }
      return( setupOK );
}

// Function to check if there is a current sequence running. 
bool PiscoCode::isSequencing(void) {
   return( currentPhase != NOT_SEQUENCING );
}

// Encapsulate the hardware-dependent LED function inside a method. 
bool PiscoCode::switchLED(bool turnItON) {
   // Check if the hardware dependent LED function is working correctly. 
   if ( LedOnOff != nullptr &&                                  // If the function pointer are set and
        LedOnOff(LED_FUNC_OK) ) {                               // it seems to be working correctly.
      if ( turnItON ) {                                         // If we want to turn it ON
         LedOnOff(LED_ON);                                      // Turn the LED ON
      } else { 
         LedOnOff(LED_OFF);                                     // Turn the LED OFF
      }
   }
}

// As we depend on an external function to turn the LED on and off, it is prudent
// to check if the function pointer is valid and working as it should. 
bool PiscoCode::isExternalLedFuncOk(void) {
   bool statusFuncOK = true;                                                        // Start the return variable as OK
   if ( LedOnOff != nullptr ) {                                                     // If the pointer to the external LED function is not null (good sign)
      for(uint8_t ctrlLED=0;ctrlLED < 255;ctrlLED++) {                              // Call this function with all possible values to check if it is working correctly
         if ( ctrlLED != LED_ON && ctrlLED != LED_OFF && ctrlLED != LED_FUNC_OK &&  // If the ctrlLED codes are out of the valid options and ​ 
             LedOnOff(ctrlLED) == true ) {                                          // calling the function with an invalid code return true. 
            statusFuncOK = false;                                                   // If we reach here, it indicates that something goes wrong.  
         }
      }
   } else {                                                                         // If the pointer to the external LED function is null
      statusFuncOK = false;                                                         // Set the return variable as not OK
   }
   return(statusFuncOK);
}