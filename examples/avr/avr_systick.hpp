#pragma once
#include <stdint.h>

namespace avr_systick
{
    extern volatile uint32_t g_ms_ticks;

    void systick_init_1ms() noexcept;

    // Busy wait using the millisecond tick
    inline void delay_ms(uint32_t ms) noexcept
    {
        const uint32_t start = g_ms_ticks;
        while ((g_ms_ticks - start) < ms)
        {
        }
    }
} // namespace avr_systick
