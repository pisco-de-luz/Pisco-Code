#pragma once

#include "pisco_code.hpp"

namespace hal_led
{
    void init() noexcept;
    bool led1(LedControlCode code) noexcept;
} // namespace hal_led
