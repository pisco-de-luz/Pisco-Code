// pisco_types.hpp
#pragma once
#include <stdint.h>

namespace pisco_code
{

    using Timestamp     = uint32_t;
    using DurationMs    = uint32_t;
    using LedLevel      = uint8_t;
    using RepeatTimes   = uint8_t;
    using NumDigits     = uint8_t;
    using DigitValue    = uint8_t;
    using LoopCounter   = uint8_t;
    using PhaseDuration = uint8_t;
    using BlinkCode     = int32_t;
    using Index8        = int8_t;
    using Index16       = int16_t;
    using Counter8      = int8_t;
    using Counter16     = int16_t;
    using Byte          = uint8_t;

    using BaseType      = Byte; // Used as underlying type for enum class NumberBase
    using LedCodeType   = Byte; // Used for LedControlCode
    using PhaseType     = Byte; // Used for Phase enum class
    using BlinkModeType = Byte; // Used for BlinkMode enum class
    using Counter       = Counter8;
    using Index         = Index8;

} // namespace pisco_code
