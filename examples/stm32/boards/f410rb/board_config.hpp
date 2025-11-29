#pragma once
#include <stm32f4xx.h>

// LED1 (Onboard): Nucleo board LED on PA5
#define LED_ONBOARD_GPIO GPIOA
#define LED_ONBOARD_PIN 5
#define LED_ONBOARD_RCC_BIT 0 // RCC_AHB1ENR bit for GPIOA

// LED2 (PWM): External LED on PA0 - TIM5_CH1 hardware PWM
// Note: STM32F410RB has TIM1, TIM5, TIM9, TIM11, LPTIM1 (no TIM2/3/4)
#define LED_PWM_GPIO GPIOA
#define LED_PWM_PIN 0
#define LED_PWM_AF 2 // AF2 = TIM5
#define LED_PWM_TIMER TIM5
#define LED_PWM_TIMER_RCC RCC_APB1ENR_TIM5EN
