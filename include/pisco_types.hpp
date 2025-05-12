// pisco_types.hpp
#pragma once
#include <stdint.h>

namespace pisco
{

    using Timestamp     = uint32_t;
    using DurationMs    = uint16_t;
    using LedLevel      = uint8_t;
    using RepeatTimes   = uint8_t;
    using NumDigits     = uint8_t;
    using DigitValue    = uint8_t;
    using LoopCounter   = uint8_t;
    using PwmCounter    = uint8_t;
    using PhaseDuration = uint8_t;
    using BlinkCode     = int32_t;

} // namespace pisco
