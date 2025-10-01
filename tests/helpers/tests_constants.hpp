#pragma once

#include <array>

#include "pisco_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
namespace testutils
{

    // Trace character constants
    inline const TraceCode LED_OFF_CHARACTER   = "_";
    inline const TraceCode LED_PULSE_CHARACTER = "^";
    inline const TraceCode LED_BASE_CHARACTER  = "-";

    // Duration-based repetition rules
    inline const std::array<TraceRepeatRule, 3> TRACE_REPEAT_RULES{
        {{0U, 1U}, {1101U, 3U}, {2201U, 6U}}
    };

    // Commonly used trace patterns
    inline constexpr SignalCode DEFAULT_CODE                     = 102;
    inline constexpr SignalCode CODE_NEG_7                       = -7;
    inline constexpr SignalCode CODE_NEG_103                     = -103;
    inline constexpr SignalCode CODE_BIN_NEG_7                   = -7;
    inline constexpr SignalCode CODE_0                           = 0;
    inline constexpr SignalCode CODE_2                           = 2;
    inline constexpr SignalCode CODE_5                           = 5;
    inline constexpr SignalCode CODE_BIN_5                       = 5;
    inline constexpr SignalCode CODE_10                          = 10;
    inline constexpr SignalCode CODE_0010                        = 10;
    inline constexpr SignalCode CODE_OCT_28                      = 28;
    inline constexpr SignalCode CODE_120                         = 120;
    inline constexpr SignalCode CODE_255                         = 255;
    inline constexpr SignalCode CODE_HEX_255                     = 255;
    inline constexpr SignalCode CODE_1010                        = 1010;
    inline constexpr SignalCode CODE_12345                       = 12345;
    inline constexpr SignalCode CODE_12345_TRUNCATED_TO_2_DIGITS = 12345;

    constexpr IntensityLevel LOWEST_LOW_LEVEL = 16U;
    constexpr IntensityLevel HIGHEST_LOW_LEVEL =
        MAX_BYTE_VALUE - MIN_INTENSITY_DIFFERENCE;
    constexpr IntensityLevel MID_LOW_LEVEL =
        (HIGHEST_LOW_LEVEL - LOWEST_LOW_LEVEL) / 2U;
    constexpr IntensityLevel TOO_HIGH_LOW_LEVEL = MAX_BYTE_VALUE;
    constexpr IntensityLevel TOO_LOW_LOW_LEVEL  = 0U;

    constexpr std::array<IntensityLevel, 5> ALL_LOW_LEVELS = {
        {
         LOWEST_LOW_LEVEL, HIGHEST_LOW_LEVEL,
         MID_LOW_LEVEL, TOO_HIGH_LOW_LEVEL,
         TOO_LOW_LOW_LEVEL, }
    };

    constexpr IntensityLevel LOWEST_HIGH_LEVEL  = 16U;
    constexpr IntensityLevel HIGHEST_HIGH_LEVEL = MAX_BYTE_VALUE;
    constexpr IntensityLevel MID_HIGH_LEVEL =
        (HIGHEST_HIGH_LEVEL - LOWEST_HIGH_LEVEL) / 2U;
    constexpr IntensityLevel TOO_HIGH_HIGH_LEVEL = MAX_BYTE_VALUE;
    constexpr IntensityLevel TOO_LOW_HIGH_LEVEL  = 0U;

    constexpr std::array<IntensityLevel, 5> ALL_HIGH_LEVELS = {
        {
         LOWEST_HIGH_LEVEL, HIGHEST_HIGH_LEVEL,
         MID_HIGH_LEVEL, TOO_HIGH_HIGH_LEVEL,
         TOO_LOW_HIGH_LEVEL, }
    };

} // namespace testutils
