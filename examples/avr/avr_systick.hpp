#pragma once
#include <stdint.h>

namespace avr_systick
{
    extern volatile uint32_t g_ms_ticks;

    void init_1ms() noexcept;
    void delay_ms(uint32_t ms) noexcept;

} // namespace avr_systick
