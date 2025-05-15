#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "../helpers/tests_constants.hpp"
#include "../helpers/tests_types.hpp"
#include "../mocks/MockLedControlLogger.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace testutils
{
    // Drives the loop for a maximum simulated time, default 64 seconds.
    inline void runSequencer(pisco::CodeBlinker* code, MockLedControlLogger* logger)
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

    inline TraceCode repeatTracePattern(const TraceCode&   trace_code,
                                        pisco::RepeatTimes repeat_count)
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
        for (pisco::RepeatTimes i = 1; i < repeat_count; ++i)
        {
            result += trace_code; // skip first repetition's trailing off
        }

        return result;
    }

    inline CodeDigitInfo convertCodeToDigits(pisco::BlinkCode code, pisco::NumberBase base,
                                             pisco::NumDigits num_digits)
    {
        CodeDigitInfo result{};
        result.is_negative        = (code < 0);
        pisco::BlinkCode abs_code = result.is_negative ? -code : code;

        const pisco::NumDigits max_digits    = maxDigitsForBase(base);
        uint8_t                first_nonzero = max_digits - 1;

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

        if (num_digits > 0 && num_digits < max_digits)
        {
            result.first_nonzero_digit_index = max_digits - num_digits;
        }
        else
        {
            result.first_nonzero_digit_index = first_nonzero;
        }

        return result;
    }

    inline TraceCode generateExpectedTrace(pisco::BlinkCode code, pisco::NumberBase base,
                                           pisco::NumDigits   min_digits = 0,
                                           pisco::RepeatTimes repeats    = 1)
    {
        const CodeDigitInfo    info       = convertCodeToDigits(code, base, min_digits);
        TraceCode              trace      = "___";
        const pisco::NumDigits max_digits = maxDigitsForBase(base);

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
    inline void checkBlinkerBehavior(pisco::CodeBlinker& blinker, MockLedControlLogger& logger,
                                     const TestBlinkerCase& testCase)
    {
        pisco::BlinkCode         code_to_show = testCase.blink_code.value_or(DEFAULT_CODE);
        const pisco::NumDigits   num_digits   = testCase.numDigits.value_or(0);
        const pisco::RepeatTimes repeats      = testCase.repeats.value_or(1);
        const pisco::NumberBase  base = testCase.number_base.value_or(pisco::NumberBase::DECIMAL);

        // if (testCase.code_pair.has_value())
        // {
        //     code_to_show = testCase.code_pair.value();
        // }
        TraceCode expected_trace =
            testutils::generateExpectedTrace(code_to_show, base, num_digits, repeats);

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

    // Generate a BlinkCode code using a pattern of digits
    inline pisco::BlinkCode generatePatternOfDigits(const GeneratePatternParams& params)
    {
        pisco::BlinkCode code_to_show = 0;
        for (Counter i = 0; i < params.num_digits; ++i)
        {
            pisco::DigitValue digit_to_show = 0;
            switch (params.pattern)
            {
                case PatternOption::SequencialUp:
                    digit_to_show = ((i + 1) % to_value(params.number_base));
                    break;
                case PatternOption::SequencialDown:
                    digit_to_show = ((params.num_digits - i) % to_value(params.number_base));
                    break;
                case PatternOption::DescendingFromMax:
                    digit_to_show =
                        ((to_value(params.number_base) - i - 1) % to_value(params.number_base));
                    break;
                case PatternOption::SameDigit:
                    digit_to_show = params.digit;
                    break;
                default:
                    digit_to_show =
                        0; // Make the generator return 0 if the pattern is not recognized
                    break;
            }
            code_to_show = code_to_show * to_value(params.number_base) + digit_to_show;
        }
        return code_to_show;
    }

} // namespace testutils
