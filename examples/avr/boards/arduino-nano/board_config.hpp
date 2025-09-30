#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define AVR_SYSTICK_USE_TIMER1 1

// On Arduino Nano, the built-in LED is on digital pin 13, which is PORTB5
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_BIT 5

#define PWM_LED_PORT PORTD
#define PWM_LED_DDR DDRD
#define PWM_LED_BIT 3
