#include "show.h"


uint8_t PiscoCode::showDec(int32_t codigo, uint8_t pwm, uint8_t vezes) {
  
     if ( currentPhase == NOT_SEQUENCING && pwm > 0 && pwm <=pwmMax && vezes > 0 ) {                                       // Não está num processo de sinalização, sinalização pode ser aceita
        if ( codigo >= 0 ) {                                                                                                // Se o código a ser sinalizado for possitivo
           isNegative = false;                                                                                                  // Indicar que é um valor positivo
        } else {                                                                                                            // Se for negativo
           isNegative = true;                                                                                                   // Indicar que é um valor negativo
           codigo = -codigo;                                                                                                // Inverte o sinal
        }
        
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


        currentPhase = START_SEQUENCE;                                                                                            // Armazena a nova etapa
        millisUltimaEtapa = 0;                                                                                       // Registra inicio de uma nova etapa
        currentPhaseDuration = mSec_betweenDigits;                                                                           // Seta a duração atual para tempo apagado entre dígitos
        
        
     } else if ( currentPhase != NOT_SEQUENCING ) {
        return ( SEQUENCE_RUNNING );
     } else {
        return ( OK );
     }
}

