#pragma once

#include <array>

#include "pisco_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
namespace testutils
{

    // Trace character constants
    inline const TraceCode LED_OFF_CHARACTER    = "_";
    inline const TraceCode LED_PULSE_CHARACTER  = "^";
    inline const TraceCode LED_DIMMED_CHARACTER = "-";

    // Duration-based repetition rules
    inline const std::array<TraceRepeatRule, 3> TRACE_REPEAT_RULES{
        {{0, 1}, {1101, 3}, {2201, 5}}
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

    constexpr LedLevel LOWEST_DIMMED_LEVEL  = 1;
    constexpr LedLevel HIGHEST_DIMMED_LEVEL = PWM_MAX - MIN_PULSE_DIMMED_GAP;
    constexpr LedLevel MID_DIMMED_LEVEL =
        (HIGHEST_DIMMED_LEVEL - LOWEST_DIMMED_LEVEL) / 2;
    constexpr LedLevel TOO_HIGH_DIMMED_LEVEL = 255;
    constexpr LedLevel TOO_LOW_DIMMED_LEVEL  = 0;

    constexpr std::array<LedLevel, 5> ALL_DIMMED_LEVELS = {
        {
         LOWEST_DIMMED_LEVEL, HIGHEST_DIMMED_LEVEL,
         MID_DIMMED_LEVEL, TOO_HIGH_DIMMED_LEVEL,
         TOO_LOW_DIMMED_LEVEL, }
    };

    constexpr LedLevel LOWEST_PULSE_LEVEL  = 1;
    constexpr LedLevel HIGHEST_PULSE_LEVEL = PWM_MAX;
    constexpr LedLevel MID_PULSE_LEVEL =
        (HIGHEST_PULSE_LEVEL - LOWEST_PULSE_LEVEL) / 2;
    constexpr LedLevel TOO_HIGH_PULSE_LEVEL = 255;
    constexpr LedLevel TOO_LOW_PULSE_LEVEL  = 0;

    constexpr std::array<LedLevel, 5> ALL_PULSE_LEVELS = {
        {
         LOWEST_PULSE_LEVEL, HIGHEST_PULSE_LEVEL,
         MID_PULSE_LEVEL, TOO_HIGH_PULSE_LEVEL,
         TOO_LOW_PULSE_LEVEL, }
    };

} // namespace testutils
