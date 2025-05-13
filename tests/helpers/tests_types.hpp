#pragma once

#include "../../include/pisco_types.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace testutils
{

    using TraceCode     = std::string;
    using TraceStrIndex = int16_t;

    struct LedLevelDuration
    {
        pisco::LedLevel  led_level{0};
        pisco::Timestamp duration{0};
    };

    struct TraceRepeatRule
    {
        pisco::Timestamp min_duration;
        uint8_t          repeat_count;
    };

    struct CodeTracePair
    {
        pisco::BlinkCode code{0};
        TraceCode        trace{};
    };

    enum class TraceCheck
    {
        NotEnforced,
        Enforced,
    };

    enum class LedEvent : uint8_t
    {
        Invalid,
        On,
        Off,
        FuncOk,
        FuncFail
    };

    struct TestBlinkerCase
    {
        std::optional<CodeTracePair>      code_pair{std::nullopt};
        std::optional<TraceCheck>         trace_check{std::nullopt};
        std::optional<pisco::NumDigits>   numDigits{std::nullopt};
        std::optional<pisco::RepeatTimes> repeats{std::nullopt};
        std::optional<pisco::LedLevel>    expectedDimmed{};
        std::optional<pisco::LedLevel>    expectedPulse{};
    };

} // namespace testutils
