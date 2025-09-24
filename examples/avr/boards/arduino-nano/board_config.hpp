#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define AVR_SYSTICK_USE_TIMER1 1

// // TX Pin for Serial1 is D1 (PD1)
// #define NANO_LED_PORT PORTD
// #define NANO_LED_DDR DDRD
// #define NANO_LED_BIT 1

// RX Pin for Serial1 is D0 (PD0)
#define NANO_LED_PORT PORTD
#define NANO_LED_DDR DDRD
#define NANO_LED_BIT 0

// // On Arduino Nano, the built-in LED is on digital pin 13, which is PORTB5
// #define NANO_LED_PORT PORTB
// #define NANO_LED_DDR DDRB
// #define NANO_LED_BIT 5
