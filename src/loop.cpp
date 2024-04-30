#include "loop.h"

/* loopCounter is a 8 bits counter that should increment every 64 milliseconds. */
void PiscoCode::loop(uint8_t loopCounter) {  
     if ( pwmCounter == 0 ) {                       
        if ( currentPhase == PAUSED ) {
           (void)_switchLED(TURN_LED_OFF);                                 // Turns the LED off at the begining.
        }
     }
     if ( currentPhase != PAUSED ) {                                       // There is a sequence to show.
        if ( pwmCounter == 0 &&                                            // If we are at the begining of the PWM phase and
             currentPhaseDuration != loopC_Blink4DigitZero &&              // it is not the digit zero and
             currentPhase != REPEAT_SEQUENCE &&                            // it is not the repeat phase and
             currentPhase != FINAL_PAUSE ) {                               // it is not the final phase
           if ( ! _switchLED(TURN_LED_ON) ) {                              // Turn the LED on and if we could not turn the LED on, then
              currentPhase = PAUSED;                                       // Stop sequencing
           }
        } 

        switch (currentPhase) {
            case START_SEQUENCE:                                           // The first phase that all sequence starts. 
                 if ( startTimeLastPhase == 0 ) {                          // If the start time of the last phase was not defined
                     startTimeLastPhase = loopCounter;                     // Set it to loopCounter
                 }
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED, be turned off. 
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }
                 if ( _currentPhaseFinished(loopCounter) ) {               // If the current phase has just finished then 
                    if ( isNegative ) {                                    // If the code is negative
                       currentPhase = NEGATIVE_SIGN_ON;                    // Change the current phase to NEGATIVE_SIGN_ON
                       currentPhaseDuration = loopC_negativeLongBlink;      // Set the duration of this new phase      
                    } else {
                       currentPhase = READ_NEXT_DIGIT;                     // Change the current phase to READ_NEXT_DIGIT
                    }               
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                 }
                 break;
            case NEGATIVE_SIGN_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                     currentPhase = NEGATIVE_SIGN_OFF;                     // Change the current phase to NEGATIVE_SIGN_OFF
                     currentPhaseDuration = loopC_negativeLongBlink;        // Set the duration of this new phase
                     startTimeLastPhase = loopCounter;                          // Set it to loopCounter
                 }
                 break;
            case NEGATIVE_SIGN_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                     currentPhase = READ_NEXT_DIGIT;                       // Change the current phase to READ_NEXT_DIGIT
                     currentPhaseDuration = loopC_betweenDigits;            // Set the duration of this new phase
                     startTimeLastPhase = loopCounter;                          // Set it to loopCounter
                 }
                 break;
            case READ_NEXT_DIGIT:
                 if ( currentDigit == MAX_DIGITS && _sequenceTimes-- ) {   // Indicates that there are no more digits to display at the end of the sequence.
                    currentPhase = END_SEQUENCE;                           // Change the current phase to END_SEQUENCE
                    currentPhaseDuration = loopC_betweenDigits;             // Set the duration of this new phase
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                 } else {                                                  // Indicates that there are more digits to display.
                    currentPhaseDuration = loopC_shortBlink;                // Set the duration of this new phase
                    if ( blinksToShow[currentDigit] == 0 ) {               // If the current digit is zero then
                       currentPhaseDuration = loopC_Blink4DigitZero;        // Set the duration of this new phase
                    }
                    currentPhase = SEQUENCING_ON;                          // Change the current phase to SEQUENCING_ON
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                 }
                 break;
            case SEQUENCING_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                    currentPhase = SEQUENCING_OFF;                         // Change the current phase to SEQUENCING_OFF
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                    blinksToShow[currentDigit]--;                          // Decrement the number of blinks is left for the current digit finish.
                    if ( blinksToShow[currentDigit] > 0 ) {                // If we did not finish blinking this digit, then
                       currentPhaseDuration = loopC_betweenBlink;           // Set the duration of this new phase
                    } else {                                               // If we have already finished blinking this digit. 
                       currentPhaseDuration = loopC_betweenDigits;          // Set the duration of this new phase
                    }
                 }
                 break;
            case SEQUENCING_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                    if ( blinksToShow[currentDigit] > 0 ) {                // If we did not finish blinking this digit, then
                       currentPhase = SEQUENCING_ON;                       // Change the current phase to SEQUENCING_ON
                       startTimeLastPhase = loopCounter;                        // Set it to loopCounter
                          currentPhaseDuration = loopC_shortBlink;          // Set the duration of this new phase
                    } else {                                               // If we have already finished blinking this digit. 
                       if ( currentDigit < MAX_DIGITS) {                   // If currentDigit variable is still less than the maximum number of digits allowed.
                          currentDigit++;                                  // Increments the currentDigit variable.
                       }
                       currentPhase = READ_NEXT_DIGIT;                     // Change the current phase to READ_NEXT_DIGIT
                       startTimeLastPhase = loopCounter;                        // Set it to loopCounter
                    }
                 }
                 break;
            case END_SEQUENCE:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                     if ( _sequenceTimes == 0 ) {                          // If we have already repeated the number of times chosen.
                        currentPhase = FINAL_PAUSE;                        // Change the current phase to FINAL_PAUSE
                        currentPhaseDuration = loopC_betweenDigits;         // Set the duration of this new phase
                     } else {                                              // If we still need to repeat once again.
                        currentPhase = REPEAT_SEQUENCE;                    // Change the current phase to REPEAT_SEQUENCE
                        currentPhaseDuration = loopC_betweenCodes;          // Set the duration of this new phase
                     }
                     startTimeLastPhase = loopCounter;                          // Set it to loopCounter
                 }
                 break;
            case FINAL_PAUSE:
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                    currentPhase = PAUSED;                                 // Stop sequencing
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                 }
                 break;
             case REPEAT_SEQUENCE:
                 if ( _currentPhaseFinished(loopCounter) ) {                    // If the current phase has just finished then 
                    for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {     // For each digit
                        blinksToShow[dig] = digitToShow[dig];              // Refuel the variable blinksToShow with the original values.
                    }
                    currentDigit = leastSignificantDigit;                   // Set the currentDigit variable to the first digit to show
                    currentPhase = START_SEQUENCE;                         // Change the current phase to START_SEQUENCE to start a new repeated cycle.
                    startTimeLastPhase = loopCounter;                           // Set it to loopCounter
                    currentPhaseDuration = loopC_betweenDigits;             // Set the duration of this new phase
                 }
                 break;
                
        }
     }
     // Increment the pwmCounter variable on every loop until it reaches pwmMax when it is restart it to zero. 
     if ( pwmCounter++ > pwmMax ) {                                         
         pwmCounter = 0;
     }
}

// If true, it indicates that it has already passed the time duration of the 
// current phase, and the PWM phase to turn the led off was reached.
bool PiscoCode::_currentPhaseFinished(uint8_t loopCounter) {
   return( ((uint8_t)(loopCounter - startTimeLastPhase) > currentPhaseDuration) &&  
            pwmCounter == _pwmSequence );
}
