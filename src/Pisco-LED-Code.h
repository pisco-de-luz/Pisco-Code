#ifndef PISCO_CODE
#define PISCO_CODE

// Central include file for Pisco-LED-Code and defines the PiscoCode class.

#include <stdint.h>
#include "loop.h"
#include "show.h"

// CONSTRUCTORS
class PiscoCode {
    // PUBLIC MEMBER FUNCTIONS
    public:

                               PiscoCode(void);
                               // Defining all class members. 
      bool                     setup(bool (*ledOnOffFunc)(uint8_t ctrlLED));
      void                     loop(uint32_t Millis);
      uint8_t                  showCode(int32_t codeToShow, uint8_t base);
      bool                     isSequencing(void);
      void                     setPWM(uint8_t pwm);
      void                     setDimPWM(uint8_t dimPWM);
      void                     setRepeat(uint8_t times);
      void                     setMinDigits(uint8_t minDigits);
      static const uint8_t     LED_ON =                      0;
      static const uint8_t     LED_OFF =                     1;
      static const uint8_t     LED_FUNC_OK =               100;
      static const uint8_t     BINARY =                      2;
      static const uint8_t     OCTAL =                       8;
      static const uint8_t     DECIMAL =                    10;
      static const uint8_t     HEXADECIMAL =                16;


      
      enum Errors {                                                 // Errors codes returned from showDec, showHex, and showBin functions.
         OK = 0,                                                    // The Pisco Code was accepted and will soon start sequencing. 
         SEQUENCE_RUNNING = 1,                                      // The PiscoCode was rejected because another sequencing is running.
         FAILED = 2                                                 // Some errors occurred. 
      };


    // PRIVATE MEMBER FUNCTIONS
    private:

     enum CurrentPhases {
        PAUSED =             10,
        START_SEQUENCE =     20,
        NEGATIVE_SIGN_ON =   30,
        NEGATIVE_SIGN_OFF =  40,
        READ_NEXT_DIGIT =    50,
        SEQUENCING_ON =      60,
        SEQUENCING_OFF =     70,
        FINAL_PAUSE =        80,
        REPEAT_SEQUENCE =    90,
        END_SEQUENCE =      100
     };

static const bool                TURN_LED_ON =              true;
static const bool                TURN_LED_OFF =            false;
static const uint32_t            mSec_negativeLongBlink =   1800;          // Set the LED's duration during a long blink in milliseconds.
static const uint32_t            mSec_shortBlink =           350;          // Set the LED's duration during a short blink in milliseconds.
static const uint32_t            mSec_Blink4DigitZero =      440;          // Set the LED's duration during a very short blink in milliseconds. It has to be less than longBlink and shortBlink
static const uint32_t            mSec_betweenBlink =         350;          // Set the pause duration between blinks in milliseconds.
static const uint32_t            mSec_betweenDigits =       1700;          // Set the pause duration between digits in milliseconds.
static const uint32_t            mSec_betweenCodes =        1500;          // Set the pause duration between Codes in milliseconds.
static const uint8_t             MAX_DIGITS =                 10;          // The maximum number of digits a sequence will process.
static const uint8_t             initialDimmedPWM =            0;          // The default PWM set value for the dimmed phase of the sequence.
static const uint8_t             pwmMax  =                    15;          // The maximum value the PWM scale could have beginning with zero.



    uint8_t                           digitToShow[MAX_DIGITS];             // Separated digits to be displayed.
    int8_t                            blinksToShow[MAX_DIGITS];            // The number of blinks is still pending to be displayed.
    uint8_t                           currentDigit;                        // Current digit to show. 
    uint8_t                           lessSignificantDigit;                // The less significant digit to be displayed.
    uint8_t                           pwmSequence;                         // PWM value of the most bright light the LED should blink. 
    uint8_t                           _pwmSequence;                         // PWM value of the most bright light the LED should blink. 
    uint8_t                           pwmCounter;                          // PWM counter from zero to pwmMax was used to set the PWM levels' timing.              
    uint8_t                           sequenceTimes;                       // Register the number of times we should repeat the PiscoCode. 
    uint8_t                           _sequenceTimes;                      // Copied from sequenceTimes variable always when a new code starts been shown. 
    uint8_t                           currentPhase;                        // The current phase we are working on now. 
    uint8_t                           dimmedPWM;                           // PWM value of the dimmed light the LED should stay on during the hole sequence. 
    uint8_t                           _dimmedPWM;                          // PWM value of the dimmed light the LED should stay on during the hole sequence. 
    uint8_t                           minNumDigits;                        // Minimum number of digits should be show.
    uint32_t                          startTimeLastPhase;                  // Start time of the last phase. 
    uint32_t                          currentPhaseDuration;                // Register the total milliseconds this phase should last.
    bool                              isNegative;                          // It is true if the number to show is negative. 
    bool                              isExternalLedFuncOk(void);
    bool                              switchLED(bool turnItON);
    bool                              currentPhaseFinished(uint32_t millis);

    // LedOnOff() - Pointer to an external function used to switch LED on and off. 
    // ------------------------------------------------------------------------------------------------
    // Before using this function to turn the LED on and off, the caller will check if it is a valid
    // pointer to a correct function, and it should respond to a LED_FUNC_OK call returning true. 
    //
    // This function will return true only if one of these three commands are received, LED_ON,
    // LED_OFF, and LED_FUNC_OK. All other values will return false. 
    bool                                  (*LedOnOff)(uint8_t);                // Pointer variable to the LED activation function. 
     
};

#endif