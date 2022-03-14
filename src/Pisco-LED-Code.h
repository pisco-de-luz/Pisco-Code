#ifndef PISCO_CODE
#define PISCO_CODE

#include <stdint.h>


const uint32_t            mSec_longBlink =            550;          // Set the LED's duration during a long blink in milliseconds.
const uint32_t            mSec_shortBlink =           350;          // Set the LED's duration during a short blink in milliseconds.
const uint32_t            mSec_veryShortBlink =       440;          // Set the LED's duration during a very short blink in milliseconds. It has to be less than longBlink and shortBlink
const uint32_t            mSec_betweenBlink =         350;          // Set the pause duration between blinks in milliseconds.
const uint32_t            mSec_betweenDigits =       1300;          // Set the pause duration between digits in milliseconds.
const uint32_t            mSec_betweenCodes =        1500;          // Set the pause duration between Codes in milliseconds.
const uint8_t             MAX_DIGITS =                  5;          // The maximum number of digits a sequence will process.
const uint8_t             initialDimmedPWM =            0;          // The default PWM set value for the dimmed phase of the sequence.
const uint8_t             pwmMax  =                    15;          // The maximum value the PWM scale could have beginning with zero.




class PiscoCode {
    public:

                               PiscoCode(void);
      void                     setup(void (*ledOnFunc)(void), void(*LedOffFunc)(void));
      void                     setup(void (*ledOnFunc)(void), void(*LedOffFunc)(void), uint8_t dimPWM);
      void                     loop(uint32_t Millis);
      uint8_t                  showDec(int32_t codigo, uint8_t pwm, uint8_t vezes);
      bool                     isSequencing(void);

      
      enum Errors {                                                 // Errors codes returned from showDec, showHex, and showBin functions.
         OK = 0,                                                    // The Pisco Code was accepted and will soon start sequencing. 
         SEQUENCE_RUNNING = 1                                       // The PiscoCode was rejected because another sequencing is running.
      };



    private:

     enum CurrentPhases {
        NOT_SEQUENCING =     10,
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

    int16_t                               ledPort;                                                 // I/O port of LED
    uint8_t                               digitToShow[MAX_DIGITS];                                 // Valor a ser sinalizado
    int8_t                                blinksToShow[MAX_DIGITS];                                // Piscos pendentes para showDec
    uint8_t                               currentDigit;                                             // Digito onde os piscos estão pendentes para showDec
    uint8_t                               lastCurrentDigit;                                       // Digito inicial onde o codigo original começou a showDec.
    uint8_t                               pwmSequence;                                          // PWM para controlar intensidade do sinalizador
    uint8_t                               pwmCounter;                                        
    uint8_t                               sequenceTimes;                                        // Quantas vezes o codigo deverá ser repetido
    uint8_t                               currentPhase;                                              // Indica em qual etapa está sendo sinalizada    
    uint8_t                               dimmedPWM;                                                // Fase do pwm para desligar o sinalizador indicando a iluminação base para os piscos.
    uint32_t                              millisUltimaEtapa;                                       // Registra o millis da ultima etapa alterada 
    uint32_t                              currentPhaseDuration;                                       // Armazena os uSec necessários para a etapa atual  
    bool                                  isNegative;                                                  // Sempre que vier um código negativo, essa variável será true                   
    void                                  (*LedOn)(void);
    void                                  (*LedOff)(void);
     
};

#endif