// pisco_types.hpp
#pragma once
#include <stdint.h>

namespace pisco_code
{

    using UInt32          = uint32_t; // 32-bit unsigned integer
    using Int32           = int32_t;  // 32-bit signed integer
    using UInt16          = uint16_t; // 16-bit unsigned integer
    using Int16           = int16_t;  // 16-bit signed integer
    using UInt8           = uint8_t;  // 8-bit unsigned integer
    using Int8            = int8_t;   // 8-bit signed integer
    using Timestamp       = UInt32;
    using DurationMs      = UInt32;
    using LedLevel        = UInt8;
    using RepeatTimes     = UInt8;
    using NumDigits       = UInt8;
    using DigitValue      = UInt8;
    using TickCounter     = UInt8;
    using PwmTickPosition = UInt8;
    using PhaseDuration   = UInt8;
    using SignalCode      = Int32;
    using Index8          = UInt8;
    using Index16         = UInt16;
    using Counter8        = Int8;
    using Counter16       = Int16;
    using Byte            = UInt8;

    // Type aliases for various enums and types used in the code
    using BaseType = Byte; // Used as underlying type for enum class NumberBase
    using LedCodeType        = Byte; // Used for LedControlCode
    using PhaseType          = Byte; // Used for Phase enum class
    using BlinkModeType      = Byte; // Used for BlinkMode enum class
    using SignalLevelType    = Byte; // Used for SignalLevel enum class
    using SignalDurationType = Byte; // Used for SignalDuration enum class
    using SignalTimesType    = Byte; // Used for SignalTimes enum class
    using Counter            = Counter8;
    using Index              = Index8;

} // namespace pisco_code
