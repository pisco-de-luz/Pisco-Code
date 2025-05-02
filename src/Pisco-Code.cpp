#include "Pisco-Code.hpp"

PiscoCode::PiscoCode(void) {   
   currentDigit = 0;       
   leastSignificantDigit = 0;       
   currentPhase = PAUSED;       
   isNegative = false;       
   minNumDigits = 0;       

   // Set the initial PWM sequence to the maximum value
   pwmSequence = pwmMax;       
   _pwmSequence = pwmSequence;       
   pwmCounter = 0;       
   sequenceTimes = 0;       
   _sequenceTimes = sequenceTimes;       

   // Set the initial dimmed PWM to the predefined initial value
   dimmedPWM = initialDimmedPWM;       
   _dimmedPWM = dimmedPWM;       

   // Initialize the start time of the last phase to zero
   startTimeLastPhase = 0;       
   currentPhaseDuration = 0;       

   // Initialize the LED function pointer to null
   LedOnOff = nullptr;       
}

bool PiscoCode::setup(bool (*LedOnOffFunc)(uint8_t ctrlLED)) {
   
   bool setupOK = false;
   currentPhase = PAUSED;  
   pwmCounter = 0; 

   // Set the LED activation function
   LedOnOff = LedOnOffFunc; 

   // Check if the external LED activation function is working correctly
   if ( _isExternalLedFuncOk() ) { 
      setupOK = true; 
      (void)_switchLED(TURN_LED_OFF); 
   }

   return setupOK;
}


// Function to check if there is a current sequence running. 
bool PiscoCode::isSequencing(void) {
   return( currentPhase != PAUSED );
}

void PiscoCode::setPWM(uint8_t pwm) {
   pwmSequence = pwm;
}

void PiscoCode::setDimPWM(uint8_t dimPWM) {
   dimmedPWM = dimPWM;
}

void PiscoCode::setRepeat(uint8_t times) {
   sequenceTimes = times;
}

// Define the minimum number of digits to show.
void PiscoCode::setMinDigits(uint8_t minDigits) {
   minNumDigits = minDigits;
}


// This method encapsulates the hardware-dependent LED function.
bool PiscoCode::_switchLED(bool turnItON) {
   // Initialize the function status to false.
   // The function will return true only if the LedOnOff external function returns true.
   bool statusFuncOK = false;                               

   // Check if the hardware-dependent LED function is set correctly.
   // If the function pointer is not null, it means it's set correctly.
   if ( LedOnOff != nullptr ) {
      // If the turnItON parameter is true, we want to turn the LED on.
      if ( turnItON ) {
         // Call the external function to turn the LED on.
         // The return value of the external function is stored in statusFuncOK.
         statusFuncOK = LedOnOff(LED_ON);
      } else {
         // Call the external function to turn the LED off.
         // The return value of the external function is stored in statusFuncOK.
         statusFuncOK = LedOnOff(LED_OFF);
      }
   } else {
      // If the function pointer is null, it means the external function is not set correctly.
      // In this case, set the function status to false.
      statusFuncOK = false;
   }

   // Return the function status.
   // This will be true if the external function was called and returned true, and false otherwise.
   return(statusFuncOK);
}

// This method checks if the external function for controlling the LED is working correctly.
bool PiscoCode::_isExternalLedFuncOk(void) {
   // Initialize the function status to true.
   bool statusFuncOK = true;

   // Check if the function pointer is not null (which is a good sign) and
   // if calling the function with the LED_FUNC_OK code returns true (as it should).
   if ( LedOnOff != nullptr && LedOnOff(LED_FUNC_OK) == true) {
      // Call the external function with all possible values to check if it's working correctly.
      for(uint8_t ctrlLED=0;ctrlLED < 255;ctrlLED++) {
         // If the ctrlLED codes are not one of the valid options (LED_ON, LED_OFF, LED_FUNC_OK) and
         // calling the function with an invalid code returns true, then something is wrong.
         if ( ctrlLED != LED_ON && ctrlLED != LED_OFF && ctrlLED != LED_FUNC_OK && LedOnOff(ctrlLED) == true ) {
            // Set the function status to false.
            statusFuncOK = false;
         }
      }
   } else {
      // If the function pointer is null, the external function is not set correctly.
      // In this case, set the function status to false.
      statusFuncOK = false;
   }

   // Return the function status.
   // This will be true if the external function was called and returned true for all valid codes, and false otherwise.
   return(statusFuncOK);
}