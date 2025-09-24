#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define AVR_SYSTICK_USE_TIMER1 1

// On Arduino Nano, the built-in LED is on digital pin 13, which is PORTB5
#define NANO_LED_PORT PORTB
#define NANO_LED_DDR DDRB
#define NANO_LED_BIT 5
