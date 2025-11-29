// //--------------------------------------------------------------------------------------------
// // HAL LED implementation for Nucleo-F410RB
// //
// // LED1 (Onboard): PA5 - Software PWM (on/off toggle)
// // LED2 (PWM):     PA0 - Hardware PWM via TIM2_CH1
// //--------------------------------------------------------------------------------------------

#include <stm32f4xx.h>

#include "board_config.hpp"
#include "hal_led.hpp"

using LedControlCode = pisco_code::LedControlCode;
using IntensityLevel = pisco_code::IntensityLevel;

namespace
{
    inline void ledOnboardOn() noexcept
    {
        LED_ONBOARD_GPIO->BSRR = (1 << LED_ONBOARD_PIN);
    }

    inline void ledOnboardOff() noexcept
    {
        LED_ONBOARD_GPIO->BSRR = (1 << (LED_ONBOARD_PIN + 16));
    }

    void initOnboardLed() noexcept
    {
        // Enable GPIOA clock
        RCC->AHB1ENR |= (1 << LED_ONBOARD_RCC_BIT);

        // Configure PA5 as output
        LED_ONBOARD_GPIO->MODER &= ~(0b11 << (2 * LED_ONBOARD_PIN));
        LED_ONBOARD_GPIO->MODER |= (0b01 << (2 * LED_ONBOARD_PIN));

        ledOnboardOff();
    }

    void initPwmLed() noexcept
    {
        // Enable TIM2 clock
        RCC->APB1ENR |= LED_PWM_TIMER_RCC;

        // Configure PA0 as alternate function (TIM2_CH1)
        LED_PWM_GPIO->MODER &= ~(0b11 << (2 * LED_PWM_PIN));
        LED_PWM_GPIO->MODER |= (0b10 << (2 * LED_PWM_PIN)); // AF mode
        LED_PWM_GPIO->AFR[0] &= ~(0xF << (4 * LED_PWM_PIN));
        LED_PWM_GPIO->AFR[0] |= (LED_PWM_AF << (4 * LED_PWM_PIN));

        // Configure TIM2 for PWM mode
        // Assuming 16 MHz clock, prescaler=0, ARR=255 for 8-bit PWM
        LED_PWM_TIMER->PSC  = 0;
        LED_PWM_TIMER->ARR  = 255;
        LED_PWM_TIMER->CCR1 = 0; // Start OFF

        // PWM mode 1 on channel 1, preload enabled
        LED_PWM_TIMER->CCMR1 = (0b110 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;

        // Enable channel 1 output
        LED_PWM_TIMER->CCER |= TIM_CCER_CC1E;

        // Enable counter
        LED_PWM_TIMER->CR1 |= TIM_CR1_CEN;
    }

} // namespace

void
hal_led::init() noexcept
{
    initOnboardLed();
    initPwmLed();
}

bool
hal_led::ledOnboard(LedControlCode code) noexcept
{
    switch (code)
    {
        case LedControlCode::ON:
            ledOnboardOn();
            return true;
        case LedControlCode::OFF:
            ledOnboardOff();
            return true;
        case LedControlCode::FUNC_OK:
            return true;
        default:
            return false;
    }
}

void
hal_led::ledPwmSetLevel(IntensityLevel level) noexcept
{
    LED_PWM_TIMER->CCR1 = level;
}
