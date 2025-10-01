// pisco_types.hpp
#pragma once

#if defined(__cplusplus)
#if __has_include(<cstdint>)
#include <cstdint>
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;
#else
#include <stdint.h>
#endif
#else
#include <stdint.h>
#endif

namespace pisco_code
{

    using UInt32          = uint32_t;
    using Int32           = int32_t;
    using UInt16          = uint16_t;
    using Int16           = int16_t;
    using UInt8           = uint8_t;
    using Int8            = int8_t;
    using Timestamp       = UInt32;
    using DurationMs      = UInt32;
    using IntensityLevel  = UInt8;
    using RepeatTimes     = UInt8;
    using NumDigits       = UInt8;
    using DigitValue      = UInt8;
    using PwmTickPosition = UInt8;
    using SignalCode      = Int32;
    using Byte            = UInt8;
    using NumBits         = UInt8;
    using Counter         = UInt8;
    using Index           = UInt8;

    // Type aliases for various enums and types used in the code
    using BaseType           = Byte;
    using LedCodeType        = Byte;
    using PhaseType          = Byte;
    using SignalModeType     = Byte;
    using SignalDurationType = Byte;
    using SignalTimesType    = Byte;

    constexpr UInt8 MAX_BYTE_VALUE = 255;

} // namespace pisco_code
