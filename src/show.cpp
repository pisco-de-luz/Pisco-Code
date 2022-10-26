#include "show.h"

// Show decimal codes <codeToShow> using the <pwm> bright to blink repeating <times> times. 
uint8_t PiscoCode::showCode(int32_t codeToShow, uint8_t base) {
   uint8_t   status = OK;                                 // Set the initial status of this function as OK

   _sequenceTimes =      sequenceTimes+1;                 // Always have the last value of sequenceTimes just after show functions starts a new sequence.
   _pwmSequence =            pwmSequence;                 // Always have the last value of pwmSequence just after show functions starts a new sequence.
   _dimmedPWM =                dimmedPWM;                 // Always have the last value of dimmedPWM just after show functions starts a new sequence.
   if ( currentPhase == PAUSED && _pwmSequence > 0 ) {    // If it is not sequencing and pwmSequencewm and sequenceTimes are set
      isNegative = false;
      if ( codeToShow < 0 ) {                             // Check if the code is negative
         isNegative = true;                               // Set the isNegative variable to true
         codeToShow = -codeToShow;                        // and invert the code signal.
      }
       
      if ( _pwmSequence > pwmMax ) {                      // Ensure that the pwm variable has a valid value.
         _pwmSequence = pwmMax;                           // Setting to the maximum limit. 
      }

      currentDigit = (MAX_DIGITS - 1);                    // Set the currentDigit to the least significative one.      

      // We will separate each digit to show in digitToShow[] array.
      // Likewise, we will define how many times the LED should blink
      // in blinksToShow[] array.
      // Example: if codeToShow = 768
      //          digitToShow[MAX_DIGITS - 1] = 8 and blinksToShow[MAX_DIGITS - 1] = 8
      //          digitToShow[MAX_DIGITS - 2] = 6 and blinksToShow[MAX_DIGITS - 2] = 6
      //          digitToShow[MAX_DIGITS - 3] = 7 and blinksToShow[MAX_DIGITS - 3] = 7
      //          currentDigit = lessSignificantDigit = (MAX_DIGITS - 3);
      for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {                       // For each possible digit
         digitToShow[dig] =  (codeToShow % base);                                // Get the least significant digit of the code. 
         blinksToShow[dig] = digitToShow[dig];                                 // Set how many times the LED should blink to represent this digit. 
         if ( digitToShow[dig] > 0 ) { currentDigit = dig; }                   // Update the currentDigit variable if the current digit is greater than zero.
         codeToShow /= base;                                                     // Removes the least significant digit from the code to display.
      }
      if ( minNumDigits > 0 && minNumDigits < MAX_DIGITS ) {                 // If the variable minNumDigits is set
         currentDigit = (MAX_DIGITS - minNumDigits);                          // define the minimum number of digits to show in this sequence.
      }
      lessSignificantDigit = currentDigit;                                     // Set the less significant digit to be displayed.
      currentPhase = START_SEQUENCE;                                           // Defines the currentPhase to start sequence. 
      startTimeLastPhase = 0;                                                  // As we are starting a new sequence, the start time of the last phase is zero. 
      currentPhaseDuration = loopC_betweenDigits;                              // Set the duration of this start sequence equal to loopC_betweenDigits.
   } else if ( currentPhase != PAUSED ) {                                      // If it was not possible to start a new sequence and the currentPhase is sequencing.
      status = SEQUENCE_RUNNING;
   } else {
      status = FAILED;
   }
   return(status);
}

