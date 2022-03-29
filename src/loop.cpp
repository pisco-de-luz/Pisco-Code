#include "loop.h"

// Millis is a 32 bits counter that should increment every one millisecond.
void PiscoCode::loop(uint32_t Millis) {  
     if ( pwmCounter == 0 ) {                       
        if ( currentPhase == PAUSED ) {
           (void)_switchLED(TURN_LED_OFF);                                 // Turns the LED off at the begining.
        }
     }
     if ( currentPhase != PAUSED ) {                                       // There is a sequence to show.
        if ( pwmCounter == 0 &&                                            // If we are at the begining of the PWM phase and
             currentPhaseDuration != mSec_Blink4DigitZero &&               // it is not the digit zero and
             currentPhase != REPEAT_SEQUENCE &&                            // it is not the repeat phase and
             currentPhase != FINAL_PAUSE ) {                               // it is not the final phase
           if ( ! _switchLED(TURN_LED_ON) ) {                              // Turn the LED on and if we could not turn the LED on, then
              currentPhase = PAUSED;                                       // Stop sequencing
           }
        } 

        switch (currentPhase) {
            case START_SEQUENCE:                                           // The first phase that all sequence starts. 
                 if ( startTimeLastPhase == 0 ) {                          // If the start time of the last phase was not defined
                     startTimeLastPhase = Millis;                          // Set it to Millis
                 }
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED, be turned off. 
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                    if ( isNegative ) {                                    // If the code is negative
                       currentPhase = NEGATIVE_SIGN_ON;                    // Change the current phase to NEGATIVE_SIGN_ON
                       currentPhaseDuration = mSec_negativeLongBlink;      // Set the duration of this new phase      
                    } else {
                       currentPhase = READ_NEXT_DIGIT;                     // Change the current phase to READ_NEXT_DIGIT
                    }               
                    startTimeLastPhase = Millis;                           // Set it to Millis
                 }
                 break;
            case NEGATIVE_SIGN_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                     currentPhase = NEGATIVE_SIGN_OFF;                     // Change the current phase to NEGATIVE_SIGN_OFF
                     currentPhaseDuration = mSec_negativeLongBlink;        // Set the duration of this new phase
                     startTimeLastPhase = Millis;                          // Set it to Millis
                 }
                 break;
            case NEGATIVE_SIGN_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                     currentPhase = READ_NEXT_DIGIT;                       // Change the current phase to READ_NEXT_DIGIT
                     currentPhaseDuration = mSec_betweenDigits;            // Set the duration of this new phase
                     startTimeLastPhase = Millis;                          // Set it to Millis
                 }
                 break;
            case READ_NEXT_DIGIT:
                 if ( currentDigit == MAX_DIGITS && _sequenceTimes-- ) {   // Indicates that there are no more digits to display at the end of the sequence.
                    currentPhase = END_SEQUENCE;                           // Change the current phase to END_SEQUENCE
                    currentPhaseDuration = mSec_betweenDigits;             // Set the duration of this new phase
                    startTimeLastPhase = Millis;                           // Set it to Millis
                 } else {                                                  // Indicates that there are more digits to display.
                    currentPhaseDuration = mSec_shortBlink;                // Set the duration of this new phase
                    if ( blinksToShow[currentDigit] == 0 ) {               // If the current digit is zero then
                       currentPhaseDuration = mSec_Blink4DigitZero;        // Set the duration of this new phase
                    }
                    currentPhase = SEQUENCING_ON;                          // Change the current phase to SEQUENCING_ON
                    startTimeLastPhase = Millis;                           // Set it to Millis
                 }
                 break;
            case SEQUENCING_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                    currentPhase = SEQUENCING_OFF;                         // Change the current phase to SEQUENCING_OFF
                    startTimeLastPhase = Millis;                           // Set it to Millis
                    blinksToShow[currentDigit]--;                          // Decrement the number of blinks is left for the current digit finish.
                    if ( blinksToShow[currentDigit] > 0 ) {                // If we did not finish blinking this digit, then
                       currentPhaseDuration = mSec_betweenBlink;           // Set the duration of this new phase
                    } else {                                               // If we have already finished blinking this digit. 
                       currentPhaseDuration = mSec_betweenDigits;          // Set the duration of this new phase
                    }
                 }
                 break;
            case SEQUENCING_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                    if ( blinksToShow[currentDigit] > 0 ) {                // If we did not finish blinking this digit, then
                       currentPhase = SEQUENCING_ON;                       // Change the current phase to SEQUENCING_ON
                       startTimeLastPhase = Millis;                        // Set it to Millis
                          currentPhaseDuration = mSec_shortBlink;          // Set the duration of this new phase
                    } else {                                               // If we have already finished blinking this digit. 
                       if ( currentDigit < MAX_DIGITS) {                   // If currentDigit variable is still less than the maximum number of digits allowed.
                          currentDigit++;                                  // Increments the currentDigit variable.
                       }
                       currentPhase = READ_NEXT_DIGIT;                     // Change the current phase to READ_NEXT_DIGIT
                       startTimeLastPhase = Millis;                        // Set it to Millis
                    }
                 }
                 break;
            case END_SEQUENCE:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {                    // If we could not turn the LED on or off, then
                       currentPhase = PAUSED;                              // Stop sequencing
                    }
                 }                
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                     if ( _sequenceTimes == 0 ) {                          // If we have already repeated the number of times chosen.
                        currentPhase = FINAL_PAUSE;                        // Change the current phase to FINAL_PAUSE
                        currentPhaseDuration = mSec_betweenDigits;         // Set the duration of this new phase
                     } else {                                              // If we still need to repeat once again.
                        currentPhase = REPEAT_SEQUENCE;                    // Change the current phase to REPEAT_SEQUENCE
                        currentPhaseDuration = mSec_betweenCodes;          // Set the duration of this new phase
                     }
                     startTimeLastPhase = Millis;                          // Set it to Millis
                 }
                 break;
            case FINAL_PAUSE:
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                    currentPhase = PAUSED;                                 // Stop sequencing
                    startTimeLastPhase = Millis;                           // Set it to Millis
                 }
                 break;
             case REPEAT_SEQUENCE:
                 if ( _currentPhaseFinished(Millis) ) {                    // If the current phase has just finished then 
                    for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {     // For each digit
                        blinksToShow[dig] = digitToShow[dig];              // Refuel the variable blinksToShow with the original values.
                    }
                    currentDigit = lessSignificantDigit;                   // Set the currentDigit variable to the first digit to show
                    currentPhase = START_SEQUENCE;                         // Change the current phase to START_SEQUENCE to start a new repeated cycle.
                    startTimeLastPhase = Millis;                           // Set it to Millis
                    currentPhaseDuration = mSec_betweenDigits;             // Set the duration of this new phase
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
bool PiscoCode::_currentPhaseFinished(uint32_t Millis) {
   return( ((uint32_t)(Millis - startTimeLastPhase) > currentPhaseDuration) &&  
            pwmCounter == _pwmSequence );
}
