#include "stm32_systick.hpp"

#include <stm32f4xx.h>

// Add FPU initialization
static void
enable_fpu() noexcept
{
    // Enable CP10 and CP11 coprocessors (FPU)
    SCB->CPACR |= (0xF << 20);
    __DSB();
    __ISB();
}

static volatile uint32_t g_ms_ticks = 0;

extern "C" void
SysTick_Handler(void)
{
    ++g_ms_ticks;
}

void
stm32_systick::init_1ms() noexcept
{
    enable_fpu();

    // Ensure HSI is enabled and stable
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
    {
    }

    g_ms_ticks = 0;
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
    __enable_irq();
}

void
stm32_systick::delay_ms(uint32_t ms) noexcept
{
    const uint32_t start = millis();
    while ((millis() - start) < ms)
    {
    }
}

uint32_t
stm32_systick::millis() noexcept
{
    return g_ms_ticks;
}
