#include "stm32_systick.hpp"

#include <stm32f4xx.h>

static volatile uint32_t g_ms_ticks = 0;

extern "C" void
SysTick_Handler(void)
{
    ++g_ms_ticks;
}

void
stm32_systick::init_1ms() noexcept
{
    g_ms_ticks = 0;
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
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
