#include "Pisco-LED-Code.h"

PiscoCode::PiscoCode(void) {
    
}

void PiscoCode::setup(void (*LedOnFunc)(void), void (*LedOffFunc)(void)) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             initialDimmedPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
      LedOn =                 LedOnFunc;
      LedOff =                LedOffFunc;
      LedOff();
}


void PiscoCode::setup(void (*LedOnFunc)(void), void (*LedOffFunc)(void), uint8_t dimPWM) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             dimPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
      LedOn =                 LedOnFunc;
      LedOff =                LedOffFunc;
      LedOff();
}



bool PiscoCode::isSequencing(void) {
   return( currentPhase != NOT_SEQUENCING );
}

