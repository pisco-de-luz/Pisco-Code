#include "Pisco-LED-Code.h"
//#include "Arduino.h"
//#define pwmFimPiscoInterruptor 5


PiscoCode::PiscoCode(void) {
    
}

void PiscoCode::setup(int16_t selectedPort) {
      ledPort =               selectedPort;
      currentStep =           NOT_SEQUENCING;  
      dimmedPWM =             initialDimmedPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
//      digitalWrite(ledPort, ledOFF);
}

void PiscoCode::setup(int16_t selectedPort, uint8_t dimPWM) {
      ledPort =               selectedPort;
      currentStep =           NOT_SEQUENCING;  
      dimmedPWM =             dimPWM;                                                                       // Seta um valor inicial para o dimmedPWM
      pwmCounter =            0;                                                                                      // Varia entre 0-7
      digitalWrite(ledPort, ledOFF);
}

void PiscoCode::loop(uint32_t Millis) {  
     if ( pwmCounter == 0 ) {
        if ( currentStep == NOT_SEQUENCING ) {
           digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
        }
     }
     if ( currentStep != NOT_SEQUENCING && (pwmCounter == 0 || pwmCounter == dimmedPWM || pwmCounter == pwmSequence) ) {              // Indica que há sinalizacao a fazer
        if ( pwmCounter == 0 && currentStepDuration != mSec_veryShortBlink && currentStep != REPEAT_SEQUENCE &&                     // Está no inicio do ciclo de pwm, e o digito não é o zero e não está repetindo o codigo e...
             currentStep != FINAL_PAUSE ) {                                                                                       // não está na pausa final
           digitalWrite(ledPort, ledON);                                                                                // Acende o LED de sinalização de fundo
        } 

        switch (currentStep) {
            case START_SEQUENCE:
                 if ( millisUltimaEtapa == 0 ) { millisUltimaEtapa = Millis; }                                                    // No inicio do processo, quando a sinalizacao é criada, o millisUltimaEtapa é zerado.
                 if ( pwmCounter == dimmedPWM  ) {                                                                                // Está no pwm de fundo desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                    if ( isNegative ) {
                       currentStep = NEGATIVE_SIGN_ON;                                                                                // Altera a etapa atual para ligar o LED      
                       currentStepDuration = mSec_longBlink*4;
                    } else {
                       currentStep = READ_NEXT_DIGIT;                                                                                // Altera a etapa atual para ligar o LED  
                    }               
                    millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 }
                 break;
            case NEGATIVE_SIGN_ON:
                 if ( pwmCounter == pwmSequence  ) {                                                                                // Está no pwm de fundo desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }                
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                     currentStep = NEGATIVE_SIGN_OFF;                                                                                   // Finaliza de vez a sinalização                      
                     currentStepDuration = mSec_longBlink*4;
                     millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 }
                 break;
            case NEGATIVE_SIGN_OFF:
                 if ( pwmCounter == dimmedPWM  ) {                                                                                       // Está no pwm de fundo desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }                
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                     currentStep = READ_NEXT_DIGIT;                                                                                   // Finaliza de vez a sinalização                      
                     currentStepDuration = mSec_betweenDigits;
                     millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 }
                 break;
            case READ_NEXT_DIGIT:
                 if ( currentDigit == MAX_DIGITS && sequenceTimes-- ) {                                                              // Indica que não há mais digitos para showDec, final do codigo
                    currentStep = END_SEQUENCE;                                                                                // Desativa sinalização.
                    currentStepDuration = mSec_betweenDigits;                                                                           // Seta a duração atual para tempo apagado entre dígitos
                    millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 } else {                                                                                                               // Indica que há números para piscar
                    currentStepDuration = mSec_shortBlink;                                                                               // Seta quantos uSec será a duração do pisco longo
                    if ( blinksToShow[currentDigit] == 0 ) {                                                                          // Indica que esse digito iniciou em zero e deve ter um pisco super curto
                       currentStepDuration = mSec_veryShortBlink;                                                                        // Ativar pisco super curto pois digito é igual a zero
                    }
                    
                    currentStep = SEQUENCING_ON;                                                                                       // Altera a etapa atual para ligar o LED
                    millisUltimaEtapa = Millis;                                                                                         // Registra inicio de uma nova etapa
                 }
                 break;
            case SEQUENCING_ON:
                 if ( pwmCounter == pwmSequence  ) {                                                                          // Está no pwm do brilho desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                    currentStep = SEQUENCING_OFF;                                                                                      // Altera a etapa atual para desligar o LED
                    millisUltimaEtapa = Millis;                                                                                         // Registra inicio de uma nova etapa
                    blinksToShow[currentDigit]--;                                                                                     // Decrementa a quantidade de piscos do digito atual
                    if ( blinksToShow[currentDigit] > 0 ) {                                                                           // Indica que ainda não mudou para outro digito
                       currentStepDuration = mSec_betweenBlink;                                                                            // Seta a duração atual para tempo apagado entre piscos              
                    } else {
                       currentStepDuration = mSec_betweenDigits;                                                                           // Seta a duração atual para tempo apagado entre dígitos
                    }
                 }
                 break;
            case SEQUENCING_OFF:
                 if ( pwmCounter == dimmedPWM  ) {                                                                                // Está no pwm de fundo desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }
                
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                    if ( blinksToShow[currentDigit] > 0 ) {                                                                           // Indica que ainda não mudou para outro digito
                       currentStep = SEQUENCING_ON;                                                                                    // Altera a etapa atual para ligar o LED
                       millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa
                          currentStepDuration = mSec_shortBlink;                                                                          // Seta quantos uSec será a duração do pisco longo
                    } else {                 
                       if ( currentDigit < MAX_DIGITS) {
                          currentDigit++;                                                                                                // Pula para o próximo digito.
                       }
                       currentStep = READ_NEXT_DIGIT;                                                                                // Altera a etapa atual para ligar o LED
                       millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                    }
                 }
                 break;
            case END_SEQUENCE:
                 if ( pwmCounter == dimmedPWM  ) {                                                                                // Está no pwm de fundo desejado    
                    digitalWrite(ledPort, ledOFF);                                                                               // Apaga o LED de sinalização
                 }                
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                     if ( sequenceTimes == 0 ) {
                        currentStep = FINAL_PAUSE;                                                                                   // Finaliza de vez a sinalização                      
                        currentStepDuration = mSec_betweenDigits;
                     } else {
                        currentStep = REPEAT_SEQUENCE;                                                                                // Finaliza de vez a sinalização                      
                        currentStepDuration = mSec_betweenCodes;                                                                          // Seta quantos uSec será a duração entre códigos
                     }
                     millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 }
                 break;
            case FINAL_PAUSE:
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                    currentStep = NOT_SEQUENCING;                                                                                // Finaliza de vez a sinalização                      
                    millisUltimaEtapa = Millis;                                                                                      // Registra inicio de uma nova etapa                  
                 }
                 break;
             case REPEAT_SEQUENCE:
                 //if ( ((unsigned long)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                 if ( ((uint32_t)(Millis - millisUltimaEtapa) > currentStepDuration) && pwmCounter == pwmSequence ) {                          // Tempo da etapa atual esgotado
                    for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {
                        blinksToShow[dig] = digitToShow[dig];                                                                          // Copia os valores para a variavel blinksToShow
                    }
                    currentDigit = lastCurrentDigit;
                    currentStep = START_SEQUENCE;                                                                                            // Armazena a nova etapa
                    millisUltimaEtapa = Millis;                                                                                       // Registra inicio de uma nova etapa
                    currentStepDuration = mSec_betweenDigits;                                                                           // Seta a duração atual para tempo apagado entre dígitos
                 }
                 break;
                
        }
     }
     if ( pwmCounter++ > pwmMax ) {
         pwmCounter = 0;
     }
}






uint8_t PiscoCode::showDec(int32_t codigo, uint8_t pwm, uint8_t vezes) {
  
     if ( currentStep == NOT_SEQUENCING && pwm > 0 && pwm <=pwmMax && vezes > 0 ) {                                       // Não está num processo de sinalização, sinalização pode ser aceita
//      digitalWrite(ledPort, LOW);                                                                                  // Acende o LED de sinalização
//      delay(100*codigo);
//      digitalWrite(ledPort, HIGH);                                                                                  // Apaga o LED de sinalização

        if ( codigo >= 0 ) {                                                                                                // Se o código a ser sinalizado for possitivo
           isNegative = false;                                                                                                  // Indicar que é um valor positivo
        } else {                                                                                                            // Se for negativo
           isNegative = true;                                                                                                   // Indicar que é um valor negativo
           codigo = -codigo;                                                                                                // Inverte o sinal
        }
        
        //if ( pwmSequence > pwmMax ) { pwmSequence = pwmMax; }
        pwmSequence = pwm;
        sequenceTimes = vezes;

        currentDigit = (MAX_DIGITS - 1);
        for(int8_t dig=(MAX_DIGITS - 1); dig>=0 ; dig--) {
           digitToShow[dig] =  (codigo % 10);                                                                               // Captura o digito mais a direita do código
           blinksToShow[dig] = digitToShow[dig];                                                                          // Copia os valores para a variavel blinksToShow
           if ( digitToShow[dig] > 0 ) { currentDigit = dig; }
           codigo /= 10;                                                                                                        // Pega o próximo dígito do código
        }
        lastCurrentDigit = currentDigit;


        currentStep = START_SEQUENCE;                                                                                            // Armazena a nova etapa
        millisUltimaEtapa = 0;                                                                                       // Registra inicio de uma nova etapa
        currentStepDuration = mSec_betweenDigits;                                                                           // Seta a duração atual para tempo apagado entre dígitos
        
        
     } else if ( currentStep != NOT_SEQUENCING ) {
        return ( SEQUENCE_RUNNING );
     } else {
        return ( OK );
     }
}


bool PiscoCode::isSequencing(void) {
   return( currentStep != NOT_SEQUENCING );
}

