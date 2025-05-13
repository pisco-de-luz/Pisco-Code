#pragma once

#include "tests_types.hpp"
#include <array>

namespace testutils
{

    // Trace character constants
    inline const TraceCode LED_OFF_CHARACTER    = "_";
    inline const TraceCode LED_ON_CHARACTER     = "^";
    inline const TraceCode LED_DIMMED_CHARACTER = "-";

    // Duration-based repetition rules
    inline const std::array<TraceRepeatRule, 3> TRACE_REPEAT_RULES{{{0, 1}, {1101, 3}, {2201, 5}}};

    // Commonly used trace patterns
    inline const CodeTracePair DEFAULT_CODE = {102, "___---^---_---^-^---___"};
    inline const CodeTracePair CODE_10      = {10, "___---^---_---___"};
    inline const CodeTracePair CODE_2       = {2, "___---^-^---___"};
    inline const CodeTracePair CODE_120     = {120, "___^^^-_^^^___"};
    inline const CodeTracePair CODE_1010    = {1010, "4MgS4M0S4MgS4M0S4L0M"};
    inline const CodeTracePair CODE_12345   = {12345,
                                               "___---^---^-^---^-^-^---^-^-^-^---^-^-^-^-^---___"};
    inline const CodeTracePair CODE_0       = {0, "___---_---___"};
    inline const CodeTracePair CODE_255     = {255, "___---^---_---^-^---___"};
    inline const CodeTracePair CODE_5       = {5, "___---^---_---^---___"};
    inline const CodeTracePair CODE_NEG_7   = {-7, "4MgS4L0M"};

} // namespace testutils
