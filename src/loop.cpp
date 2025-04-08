#include "loop.h"

/* loopCounter is a 8 bits counter that should increment every 64 milliseconds. */
void PiscoCode::loop(uint8_t loopCounter) {  
     if ( pwmCounter == 0 ) {                       
        if ( currentPhase == PAUSED ) {
           (void)_switchLED(TURN_LED_OFF);
        }
     }
     if ( currentPhase != PAUSED ) {                                       // There is a sequence to show.
        if ( pwmCounter == 0 &&                                            // If we are at the begining of the PWM phase and
             currentPhaseDuration != loopC_Blink4DigitZero &&              // it is not the digit zero and
             currentPhase != REPEAT_SEQUENCE &&                            // it is not the repeat phase and
             currentPhase != FINAL_PAUSE ) {                               // it is not the final phase
           if ( ! _switchLED(TURN_LED_ON) ) {
              currentPhase = PAUSED;
           }
        } 

        switch (currentPhase) {
            case START_SEQUENCE:
                 if ( startTimeLastPhase == 0 ) {
                     startTimeLastPhase = loopCounter;
                 }
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED, be turned off. 
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }
                 if ( _currentPhaseFinished(loopCounter) ) {
                    if ( isNegative ) {
                       currentPhase = NEGATIVE_SIGN_ON;
                       currentPhaseDuration = loopC_negativeLongBlink;
                    } else {
                       currentPhase = READ_NEXT_DIGIT;
                    }               
                    startTimeLastPhase = loopCounter;
                 }
                 break;
            case NEGATIVE_SIGN_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {
                     currentPhase = NEGATIVE_SIGN_OFF;
                     currentPhaseDuration = loopC_negativeLongBlink;
                     startTimeLastPhase = loopCounter;
                 }
                 break;
            case NEGATIVE_SIGN_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {
                     currentPhase = READ_NEXT_DIGIT;
                     currentPhaseDuration = loopC_betweenDigits;
                     startTimeLastPhase = loopCounter;
                 }
                 break;
            case READ_NEXT_DIGIT:
                 if ( currentDigit == MAX_DIGITS && _sequenceTimes-- ) {   // Indicates that there are no more digits to display at the end of the sequence.
                    currentPhase = END_SEQUENCE;
                    currentPhaseDuration = loopC_betweenDigits;
                    startTimeLastPhase = loopCounter;
                 } else {                                                  // Indicates that there are more digits to display.
                    currentPhaseDuration = loopC_shortBlink;
                    if ( blinksToShow[currentDigit] == 0 ) {
                       currentPhaseDuration = loopC_Blink4DigitZero;
                    }
                    currentPhase = SEQUENCING_ON;
                    startTimeLastPhase = loopCounter;
                 }
                 break;
            case SEQUENCING_ON:
                 if ( pwmCounter == _pwmSequence  ) {                      // If the PWM phase to turn the led off was reached.
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }
                 if ( _currentPhaseFinished(loopCounter) ) {
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                    currentPhase = SEQUENCING_OFF;
                    startTimeLastPhase = loopCounter;
                    blinksToShow[currentDigit]--;
                    if ( blinksToShow[currentDigit] > 0 ) {
                       currentPhaseDuration = loopC_betweenBlink;
                    } else {
                       currentPhaseDuration = loopC_betweenDigits;
                    }
                 }
                 break;
            case SEQUENCING_OFF:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {
                    if ( blinksToShow[currentDigit] > 0 ) {
                       currentPhase = SEQUENCING_ON;
                       startTimeLastPhase = loopCounter;
                          currentPhaseDuration = loopC_shortBlink;
                    } else {
                       if ( currentDigit < MAX_DIGITS) {
                          currentDigit++;
                       }
                       currentPhase = READ_NEXT_DIGIT;
                       startTimeLastPhase = loopCounter;
                    }
                 }
                 break;
            case END_SEQUENCE:
                 if ( pwmCounter == _dimmedPWM  ) {                        // If we reach the PWM value for the dimmed LED be turned off then
                    if ( ! _switchLED(TURN_LED_OFF) ) {
                       currentPhase = PAUSED;
                    }
                 }                
                 if ( _currentPhaseFinished(loopCounter) ) {
                     if ( _sequenceTimes == 0 ) {
                        currentPhase = FINAL_PAUSE;
                        currentPhaseDuration = loopC_betweenDigits;
                     } else {
                        currentPhase = REPEAT_SEQUENCE;
                        currentPhaseDuration = loopC_betweenCodes;
                     }
                     startTimeLastPhase = loopCounter;
                 }
                 break;
            case FINAL_PAUSE:
                 if ( _currentPhaseFinished(loopCounter) ) {
                    currentPhase = PAUSED;
                    startTimeLastPhase = loopCounter;
                 }
                 break;
             case REPEAT_SEQUENCE:
                 if ( _currentPhaseFinished(loopCounter) ) {
                    for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {     // For each digit
                        blinksToShow[dig] = digitToShow[dig];              // Refuel the variable blinksToShow with the original values.
                    }
                    currentDigit = leastSignificantDigit;                   // Set the currentDigit variable to the first digit to show
                    currentPhase = START_SEQUENCE;
                    startTimeLastPhase = loopCounter;
                    currentPhaseDuration = loopC_betweenDigits;
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
