#include "Pisco-LED-Code.h"

PiscoCode::PiscoCode(void) {
}

void PiscoCode::setup(void (*LedOnOffFunc)(bool turnItON)) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             initialDimmedPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
      LedOnOff =               LedOnOffFunc;
      LedOnOff(LED_OFF);
}

void PiscoCode::setup(void (*LedOnOffFunc)(bool turnItON), uint8_t dimPWM) {
      currentPhase =           NOT_SEQUENCING;  
      dimmedPWM =             dimPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
      LedOnOff =               LedOnOffFunc;
      LedOnOff(LED_OFF);
}


bool PiscoCode::isSequencing(void) {
   return( currentPhase != NOT_SEQUENCING );
}

