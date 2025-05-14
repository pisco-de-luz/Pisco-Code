#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

#include "pisco_types.hpp"

namespace testutils
{

    using TraceCode     = std::string;
    using TraceStrIndex = int16_t;
    using Index         = int32_t;
    using Counter       = int32_t;

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
        std::optional<pisco::BlinkCode>   blink_code{std::nullopt};
        std::optional<pisco::NumberBase>  number_base{std::nullopt};
        std::optional<TraceCheck>         trace_check{std::nullopt};
        std::optional<pisco::NumDigits>   numDigits{std::nullopt};
        std::optional<pisco::RepeatTimes> repeats{std::nullopt};
        std::optional<pisco::LedLevel>    expectedDimmed{};
        std::optional<pisco::LedLevel>    expectedPulse{};
    };

    struct CodeDigitInfo
    {
        bool                                   is_negative;
        std::array<uint8_t, pisco::MAX_DIGITS> digits;
        uint8_t                                first_nonzero_digit_index;
    };

} // namespace testutils
