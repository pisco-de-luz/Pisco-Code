#pragma once
#include <stdint.h>

namespace stm32_systick
{
    void     init_1ms() noexcept;
    void     delay_ms(uint32_t ms) noexcept;
    uint32_t millis() noexcept;
} // namespace stm32_systick
