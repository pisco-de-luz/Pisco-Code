#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "mock_led_control_logger.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using pisco_code::DigitValue;
using pisco_code::NumberBase;
using pisco_code::NumDigits;
using pisco_code::RepeatTimes;
using pisco_code::SignalCode;
using pisco_code::SignalEmitter;

namespace testutils
{
    // Drives the loop for a maximum simulated time, default 64 seconds.
    inline void runSequencer(SignalEmitter* code, MockLedControlLogger* logger)
    {
        constexpr uint8_t   LOOP_CALLS_PER_COUNTER = 64;
        constexpr Timestamp MAX_SEQUENCING_LOOP =
            3600000UL; // Limit simulation of 1h to prevent infinite loop
        Timestamp loop_counter = 0;
        uint8_t   fake_millis  = 0;

        while (code->isRunning() && loop_counter < MAX_SEQUENCING_LOOP)
        {
            for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i)
            {
                logger->setTime(loop_counter++);
                code->loop(fake_millis);
            }

            ++fake_millis;
        }

        logger->flush();
    }

    inline TraceCode repeatTracePattern(const TraceCode& trace_code, RepeatTimes repeat_count)
    {
        if (repeat_count <= 1 || trace_code.empty())
            return {};

        // Find trailing LED off pattern (represented by a sequence of '_')
        const auto trail_off_start =
            static_cast<TraceStrIndex>(trace_code.find_last_not_of(LED_OFF_CHARACTER) + 1);
        if (trail_off_start == TraceCode::npos)
            return "No trailing off pattern found";

        const TraceCode leading_off = trace_code.substr(0, trail_off_start);
        TraceCode       result      = leading_off;
        for (RepeatTimes i = 1; i < repeat_count; ++i)
        {
            result += trace_code; // skip first repetition's trailing off
        }

        return result;
    }

    inline CodeDigitInfo convertCodeToDigits(SignalCode code, NumberBase base, NumDigits num_digits)
    {
        CodeDigitInfo result{};
        result.is_negative  = (code < 0);
        SignalCode abs_code = result.is_negative ? -code : code;

        const NumDigits max_digits    = max_digits_for_base(base);
        uint8_t         first_nonzero = max_digits - 1;

        for (Index i = max_digits - 1; i >= 0; --i)
        {
            const auto digit = static_cast<uint8_t>(abs_code % static_cast<uint8_t>(base));
            result.digits[i] = digit;
            if (digit > 0)
            {
                first_nonzero = i;
            }
            abs_code /= static_cast<uint8_t>(base);
        }

        if (num_digits > 0 && num_digits <= max_digits)
        {
            result.first_nonzero_digit_index = max_digits - num_digits;
        }
        else
        {
            result.first_nonzero_digit_index = first_nonzero;
        }

        return result;
    }

    inline TraceCode generateExpectedTrace(SignalCode code, NumberBase base,
                                           NumDigits min_digits = 0, RepeatTimes repeats = 1)
    {
        const CodeDigitInfo info       = convertCodeToDigits(code, base, min_digits);
        TraceCode           trace      = "___";
        const NumDigits     max_digits = max_digits_for_base(base);

        for (Counter r = 0; r < repeats; ++r)
        {
            trace += "---";
            if (info.is_negative)
            {
                trace += "^^^---";
            }

            for (Index index = info.first_nonzero_digit_index; index < max_digits; ++index)
            {
                const Counter digit = info.digits[index];
                if (digit == 0)
                {
                    trace += "_-";
                }
                else
                {
                    for (Counter j = 0; j < digit; ++j)
                    {
                        trace += "^-";
                    }
                }
                trace += "--";
            }

            trace += "___";
        }

        return trace;
    }

    // Check the behavior of the blinker against the expected values.
    inline void checkBlinkerBehavior(SignalEmitter& blinker, MockLedControlLogger& logger,
                                     const TestBlinkerCase& testCase)
    {
        SignalCode        code_to_show = testCase.blink_code.value_or(DEFAULT_CODE);
        const NumDigits   num_digits   = testCase.numDigits.value_or(0);
        const RepeatTimes repeats      = testCase.repeats.value_or(1);
        const NumberBase  base         = testCase.number_base.value_or(NumberBase::DEC);

        TraceCode expected_trace = generateExpectedTrace(code_to_show, base, num_digits, repeats);

        blinker.showCode(code_to_show, base, num_digits, repeats);
        logger.setBlinker(&blinker);
        runSequencer(&blinker, &logger);
        const TraceCode actual_trace = logger.traceLogToString();

        // Check the trace log
        if (testCase.trace_check == TraceCheck::Enforced)
        {
            STRCMP_EQUAL(expected_trace.c_str(), actual_trace.c_str());
        }

        // Check the dimmed and pulse levels
        if (testCase.expectedDimmed.has_value())
        {
            CHECK_EQUAL_TEXT(testCase.expectedDimmed.value(), logger.getDimmedLevel(),
                             "Dimmed level mismatch");
        }
        if (testCase.expectedPulse.has_value())
        {
            CHECK_EQUAL_TEXT(testCase.expectedPulse.value(), logger.getPulseLevel(),
                             "Pulse level mismatch");
        }
    }

    // Generate a SignalCode code using a pattern of digits
    inline SignalCode generatePatternOfDigits(const GeneratePatternParams& params)
    {
        SignalCode code_to_show = 0;
        const auto base_value   = to_value(params.number_base);

        PatternOption pattern_to_use = params.pattern;
        if (params.pattern == PatternOption::SequencialDown && params.num_digits >= base_value)
        {
            pattern_to_use = PatternOption::DescendingFromMax;
        }

        for (Counter i = 0; i < params.num_digits; ++i)
        {
            DigitValue digit_to_show = 0;

            switch (pattern_to_use)
            {
                case PatternOption::SequencialUp:
                    digit_to_show = (i + 1) % base_value;
                    break;

                case PatternOption::SequencialDown:
                    digit_to_show = (params.num_digits - i) % base_value;
                    break;

                case PatternOption::DescendingFromMax:
                    digit_to_show = static_cast<DigitValue>((base_value - 1 - (i % base_value)));
                    break;

                case PatternOption::SameDigit:
                    digit_to_show = params.digit;
                    break;

                default:
                    digit_to_show = 0;
                    break;
            }

            code_to_show = code_to_show * base_value + digit_to_show;
        }

        return code_to_show;
    }

} // namespace testutils

/*
if (num_digits >= base_value) { start_digit = (num_digits - base_value + 1);}
i, (num_digits - i),  (num_digits - i) % base_value, (start_digit - i)
0,  9,                  1, 2
1,  8,                  0
2,  7,                  7
3, 6, 6
4, 5, 5
5, 4, 4
6, 3, 3
7, 2, 2
8, 1, 1


*/
