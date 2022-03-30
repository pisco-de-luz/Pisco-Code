#include "Pisco-LED-Code.h"

// The constructor
PiscoCode::PiscoCode(void) {
   currentDigit =               0;       // Current digit to show. 
   lessSignificantDigit =       0;       // The less significant digit to be displayed.
   pwmSequence =           pwmMax;       // PWM value of the most bright light the LED should blink. 
   _pwmSequence =     pwmSequence;       // Always have the last value of pwmSequence just after show functions starts a new sequence.
   pwmCounter =                 0;       // PWM counter from zero to pwmMax was used to set the PWM levels' timing.              
   sequenceTimes =              0;       // Register the number of times we should repeat the PiscoCode. 1=repeat once (mean it will show the code twice)
   _sequenceTimes = sequenceTimes;       // Always have the last value of sequenceTimes just after show functions starts a new sequence.
   currentPhase =          PAUSED;       // The current phase we are working on now. 
   dimmedPWM =   initialDimmedPWM;       // PWM value of the dimmed light the LED should stay on during the hole sequence. 
   _dimmedPWM =         dimmedPWM;       // Always have the last value of dimmedPWM just after show functions starts a new sequence.
   minNumDigits =               0;       // Define the default value of the minimum number of digits to show.
   startTimeLastPhase =         0;       // Start time of the last phase. 
   currentPhaseDuration =       0;       // Register the total milliseconds this phase should last.
   isNegative =             false;       // It is true if the number to show is negative. 
   LedOnOff =             nullptr;       // Initialize this function pointer to null.
}

bool PiscoCode::setup(bool (*LedOnOffFunc)(uint8_t ctrlLED)) {
      bool setupOK =         false;
      currentPhase =        PAUSED;  
      pwmCounter =               0;                                              // The counter's initial value is used to set the PWM levels' timing.
      LedOnOff =      LedOnOffFunc;                                              // Pointer to the LED activation function.
      if ( _isExternalLedFuncOk() ) {                                             // If the external LED activation function is working correctly.
         setupOK = true;                                                         // Set the return variable as OK
         (void)_switchLED(TURN_LED_OFF);                                          // Turn the LED off
      }
      return( setupOK );
}

// Function to check if there is a current sequence running. 
bool PiscoCode::isSequencing(void) {
   return( currentPhase != PAUSED );
}

// Define the new value of default pwm.
void PiscoCode::setPWM(uint8_t pwm) {
   pwmSequence = pwm;
}

// Define the new value of default dimmed pwm.
void PiscoCode::setDimPWM(uint8_t dimPWM) {
   dimmedPWM = dimPWM;
}

// Define how many times the code should repeat.
void PiscoCode::setRepeat(uint8_t times) {
   sequenceTimes = times;
}

// Define the minimum number of digits to show.
void PiscoCode::setMinDigits(uint8_t minDigits) {
   minNumDigits = minDigits;
}


/* Encapsulate the hardware-dependent LED function inside a method. */
bool PiscoCode::_switchLED(bool turnItON) {
   /* The only way this function will return true is if it returns from LedOnOff external function */
   bool statusFuncOK = false;                               

   /* Check if the hardware dependent LED function is working correctly. 
    * If the function pointer is not null (are correctly set) */
   if ( LedOnOff != nullptr ) {
      if ( turnItON ) {                                         // If we want to turn it ON
         statusFuncOK = LedOnOff(LED_ON);                       // Turn the LED ON
      } else { 
         statusFuncOK = LedOnOff(LED_OFF);                      // Turn the LED OFF
      }
   } else {                                                     // Indicates that the external function pointer is null
      statusFuncOK = false;
   }
   return(statusFuncOK)
}

// As we depend on an external function to turn the LED on and off, it is prudent
// to check if the function pointer is valid and working as it should. 
bool PiscoCode::_isExternalLedFuncOk(void) {
   bool statusFuncOK = true;                                                        // Start the return variable as OK
   if ( LedOnOff != nullptr &&                                                      // If the pointer to the external LED function is not null (good sign) and
        LedOnOff(LED_FUNC_OK) == true) {                                            // calling the function with the LED_FUNC_OK code returned true as it should.
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