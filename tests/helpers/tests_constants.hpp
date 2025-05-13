#pragma once

#include <array>

#include "pisco_constants.hpp"
#include "tests_types.hpp"

namespace testutils
{

    // Trace character constants
    inline const TraceCode LED_OFF_CHARACTER    = "_";
    inline const TraceCode LED_PULSE_CHARACTER  = "^";
    inline const TraceCode LED_DIMMED_CHARACTER = "-";

    // Duration-based repetition rules
    inline const std::array<TraceRepeatRule, 3> TRACE_REPEAT_RULES{{{0, 1}, {1101, 3}, {2201, 5}}};

    // Commonly used trace patterns
    inline const CodeTracePair DEFAULT_CODE   = {102, "___---^---_---^-^---___"};
    inline const CodeTracePair CODE_NEG_7     = {-7, "___---^^^-----^-^-^-^-^-^-^---___"};
    inline const CodeTracePair CODE_BIN_NEG_7 = {-7, "___---^^^-----^---^---^---___"};
    inline const CodeTracePair CODE_0         = {0, "___---_---___"};
    inline const CodeTracePair CODE_2         = {2, "___---^-^---___"};
    inline const CodeTracePair CODE_5         = {5, "___---^-^-^-^-^---___"};
    inline const CodeTracePair CODE_BIN_5     = {5, "___---^---_---^---___"};
    inline const CodeTracePair CODE_10        = {10, "___---^---_---___"};
    inline const CodeTracePair CODE_0010      = {10, "___---_---_---^---_---___"};
    inline const CodeTracePair CODE_OCT_28    = {28, "___---^-^-^---^-^-^-^---___"};
    inline const CodeTracePair CODE_120       = {120, "___---^---^-^---_---___"};
    inline const CodeTracePair CODE_255       = {255, "___---^---_---^-^---___"};
    inline const CodeTracePair CODE_HEX_255   = {
        255, "___---^-^-^-^-^-^-^-^-^-^-^-^-^-^-^---^-^-^-^-^-^-^-^-^-^-^-^-^-^-^---___"};
    inline const CodeTracePair CODE_1010  = {1010, "___---^---_---^---_---___"};
    inline const CodeTracePair CODE_12345 = {12345,
                                             "___---^---^-^---^-^-^---^-^-^-^---^-^-^-^-^---___"};
    inline const CodeTracePair CODE_12345_TRUNCATED_TO_2_DIGITS = {
        12345, "___---^-^-^-^---^-^-^-^-^---___"};

    constexpr pisco::LedLevel LOWEST_DIMMED_LEVEL  = 1;
    constexpr pisco::LedLevel HIGHEST_DIMMED_LEVEL = pisco::PWM_MAX - pisco::MIN_PULSE_DIMMED_GAP;
    constexpr pisco::LedLevel MID_DIMMED_LEVEL = (HIGHEST_DIMMED_LEVEL - LOWEST_DIMMED_LEVEL) / 2;
    constexpr pisco::LedLevel TOO_HIGH_DIMMED_LEVEL = 255;
    constexpr pisco::LedLevel TOO_LOW_DIMMED_LEVEL  = 0;

    constexpr std::array<pisco::LedLevel, 5> ALL_DIMMED_LEVELS = {{
        LOWEST_DIMMED_LEVEL,
        HIGHEST_DIMMED_LEVEL,
        MID_DIMMED_LEVEL,
        TOO_HIGH_DIMMED_LEVEL,
        TOO_LOW_DIMMED_LEVEL,
    }};

    constexpr pisco::LedLevel LOWEST_PULSE_LEVEL   = 1;
    constexpr pisco::LedLevel HIGHEST_PULSE_LEVEL  = pisco::PWM_MAX;
    constexpr pisco::LedLevel MID_PULSE_LEVEL      = (HIGHEST_PULSE_LEVEL - LOWEST_PULSE_LEVEL) / 2;
    constexpr pisco::LedLevel TOO_HIGH_PULSE_LEVEL = 255;
    constexpr pisco::LedLevel TOO_LOW_PULSE_LEVEL  = 0;

    constexpr std::array<pisco::LedLevel, 5> ALL_PULSE_LEVELS = {{
        LOWEST_PULSE_LEVEL,
        HIGHEST_PULSE_LEVEL,
        MID_PULSE_LEVEL,
        TOO_HIGH_PULSE_LEVEL,
        TOO_LOW_PULSE_LEVEL,
    }};

} // namespace testutils
