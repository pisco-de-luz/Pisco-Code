#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define AVR_SYSTICK_USE_TIMER1 1

// LED1 (Onboard): Arduino Nano built-in LED on D13 (PB5)
#define LED_ONBOARD_PORT PORTB
#define LED_ONBOARD_DDR DDRB
#define LED_ONBOARD_BIT 5

// LED2 (PWM): External LED on D3 (PD3) - Timer2/OC2B hardware PWM
#define LED_PWM_PORT PORTD
#define LED_PWM_DDR DDRD
#define LED_PWM_BIT 3
