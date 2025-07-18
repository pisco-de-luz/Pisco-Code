#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

#include "pisco_types.hpp"

using namespace pisco_code;
namespace testutils
{

    using TraceCode     = std::string;
    using TraceStrIndex = int16_t;
    using Index         = int32_t;
    using Counter       = int32_t;

    struct LedLevelDuration
    {
        LedLevel  led_level{0};
        Timestamp duration{0};
    };

    struct TraceRepeatRule
    {
        Timestamp min_duration;
        uint8_t   repeat_count;
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
        std::optional<SignalCode>  blink_code{std::nullopt};
        std::optional<NumberBase>  number_base{std::nullopt};
        std::optional<TraceCheck>  trace_check{std::nullopt};
        std::optional<NumDigits>   numDigits{std::nullopt};
        std::optional<RepeatTimes> repeats{std::nullopt};
        std::optional<LedLevel>    expectedDimmed{};
        std::optional<LedLevel>    expectedPulse{};
    };

    struct CodeDigitInfo
    {
        bool                                     is_negative;
        std::array<uint8_t, MAX_DIGITS_ABSOLUTE> digits;
        uint8_t                                  first_nonzero_digit_index;
    };

    // PatternOption — Digit composition strategies used in test generation.
    //
    // SequencialUp
    // - Example: 1, 12, 123, 1234, ..., 123456789
    //
    // SequencialDown
    // - Example: 1, 21, 321, 4321, ..., 987654321
    //
    // DescendingFromMax (use the base-1 value to determine the max digit)
    // - Example: 9, 98, 987, 9876, ..., 987654321
    //
    // SameDigit (use the digit value parameter to determine the digit)
    // - Example: 9, 88, 777, ..., 111111111
    enum class PatternOption
    {
        SequencialUp,
        SequencialDown,
        DescendingFromMax,
        SameDigit,
    };
    struct GeneratePatternParams
    {
        PatternOption pattern{PatternOption::SequencialUp};
        NumberBase    number_base{NumberBase::DEC};
        NumDigits     num_digits{};
        DigitValue    digit{0};
    };

} // namespace testutils
