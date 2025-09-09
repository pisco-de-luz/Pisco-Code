// src/main.cpp
#include <stdint.h>

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "software_pwm_led_controller.hpp"

using namespace pisco_code;

namespace
{
    // STM32F410RB minimal regs we need
    constexpr uint32_t PERIPH_BASE     = 0x40000000UL;
    constexpr uint32_t AHB1PERIPH_BASE = PERIPH_BASE + 0x00020000UL;
    constexpr uint32_t RCC_BASE        = AHB1PERIPH_BASE + 0x3800UL;
    constexpr uint32_t GPIOA_BASE      = AHB1PERIPH_BASE + 0x0000UL;

    struct RCC_Type
    {
        volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
            RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR,
            AHB3ENR, RESERVED2, APB1ENR, APB2ENR;
    };
    struct GPIO_Type
    {
        volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR,
            AFR[2];
    };

#define RCC ((RCC_Type*) RCC_BASE)
#define GPIOA ((GPIO_Type*) GPIOA_BASE)

    // SysTick
    constexpr uint32_t SYSCLK_HZ = 16000000U; // HSI default
#define SYST_CSR (*(volatile uint32_t*) 0xE000E010UL)
#define SYST_RVR (*(volatile uint32_t*) 0xE000E014UL)
#define SYST_CVR (*(volatile uint32_t*) 0xE000E018UL)
    constexpr uint32_t SYST_CSR_ENABLE  = (1U << 0);
    constexpr uint32_t SYST_CSR_TICKINT = (1U << 1);
    constexpr uint32_t SYST_CSR_CLKSRC  = (1U << 2);

    constexpr uint32_t LD2_PIN = 5U; // PA5

    volatile uint32_t g_ms_ticks = 0;

    inline void systick_init_1ms() noexcept
    {
        SYST_RVR = (SYSCLK_HZ / 1000U) - 1U;
        SYST_CVR = 0U;
        SYST_CSR = SYST_CSR_CLKSRC | SYST_CSR_TICKINT | SYST_CSR_ENABLE;

        __asm volatile("cpsie i" ::
                           : "memory"); // make global IRQ enable explicit
    }

    inline void ledInit() noexcept
    {
        RCC->AHB1ENR |= (1U << 0); // GPIOA
        (void) RCC->AHB1ENR;
        GPIOA->MODER &= ~(3U << (LD2_PIN * 2U));
        GPIOA->MODER |= (1U << (LD2_PIN * 2U)); // output
    }

    inline void ledOn() noexcept
    {
        GPIOA->ODR |= (1U << LD2_PIN);
    } // LD2 is active-high
    inline void ledOff() noexcept
    {
        GPIOA->ODR &= ~(1U << LD2_PIN);
    }

    // Adapter from library control codes to board I/O
    bool swPwmLed1(LedControlCode code) noexcept
    {
        switch (code)
        {
            case LedControlCode::ON:
                ledOn();
                return true;
            case LedControlCode::OFF:
                ledOff();
                return true;
            case LedControlCode::FUNC_OK:
                return true;
            default:
                return false;
        }
    }

    inline void delay_ms(uint32_t ms) noexcept
    {
        const uint32_t start = g_ms_ticks;
        while ((g_ms_ticks - start) < ms)
        {
        }
    }
} // namespace

extern "C" void
SysTick_Handler(void)
{
    ++g_ms_ticks;
}

int
main()
{
    systick_init_1ms();
    ledInit();
    ledOn();
    delay_ms(250);
    ledOff();

    SoftwarePwmLedController controller_led1(swPwmLed1);
    SignalEmitter            emitter_led1(&controller_led1);

    emitter_led1.setDimmedLevel(7);

    const RepeatTimes repeats{1};
    const NumDigits   num_digits{0};
    const SignalCode  signal_code{2};

    emitter_led1.showCode(signal_code, NumberBase::DEC, num_digits, repeats);

    int i = 0;
    while (emitter_led1.isRunning())
    {
        emitter_led1.loop();
        delay_ms(1);
        ledOn();
        delay_ms(250);
        ledOff();
        delay_ms(250);
    }

    ledOn();
    delay_ms(2500);
    ledOff();
    delay_ms(250);

    for (;;)
    {
    }
}
