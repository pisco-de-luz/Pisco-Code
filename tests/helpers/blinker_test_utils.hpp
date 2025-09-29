#pragma once

#include <cstdint>

#include "mock_led_control_logger.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_sequencer.hpp"
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
    // Two LUTs for GAP and PEAK (SignalMode::GAP == 1, PEAK == 2)
    constexpr const char* GAP_LUT[]    = {"_", "__", "___", "______"};
    constexpr const char* MIDDLE_LUT[] = {"-", "--", "---", "------"};
    constexpr const char* PEAK_LUT[]   = {"^", "^^", "^^^", "^^^^^^"};

    // Returns a pointer to a static string literal (no dynamic allocation)
    [[nodiscard]] inline TraceCode toTraceCode(SignalMode     mode,
                                               SignalDuration duration) noexcept
    {
        const auto duration_value = to_value(duration);

        if (duration_value >= 4)
        {
            return ""; // fail-safe: invalid duration
        }
        switch (mode)
        {
            case SignalMode::GAP:
                return GAP_LUT[duration_value];
            case SignalMode::BASE:
                return MIDDLE_LUT[duration_value];
            case SignalMode::PEAK:
                return PEAK_LUT[duration_value];
            default:
                return ""; // NOT_DEFINED or invalid
        }
    }

    // Drives the loop for a maximum simulated time, default 64 seconds.
    inline void runSequencer(SignalEmitter* code, MockLedControlLogger* logger)
    {
        constexpr uint8_t   LOOP_CALLS_PER_COUNTER = 64;
        constexpr Timestamp MAX_SEQUENCING_LOOPS =
            3600000UL; // Limit simulation of 1h to prevent infinite loop
        Timestamp sequencing_counter = 0;
        uint8_t   fake_millis        = 0;

        while (code->isRunning() && sequencing_counter < MAX_SEQUENCING_LOOPS)
        {
            for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i)
            {
                logger->setTime(sequencing_counter++);
                // code->loop(fake_millis);
                code->loop();
            }

            ++fake_millis;
        }

        logger->flush();
    }

    inline TraceCode repeatTracePattern(const TraceCode& trace_code,
                                        RepeatTimes      repeat_count)
    {
        if (repeat_count <= 1 || trace_code.empty())
        {
            return {};
        }

        // Find trailing LED off pattern (represented by a sequence of '_')
        using size_type = TraceCode::size_type;
        const size_type trail_off_start =
            trace_code.find_last_not_of(LED_OFF_CHARACTER) + 1;
        if (trail_off_start == TraceCode::npos)
        {
            return "No trailing off pattern found";
        }

        const TraceCode leading_off = trace_code.substr(0, trail_off_start);
        TraceCode       result      = leading_off;
        for (RepeatTimes i = 1; i < repeat_count; ++i)
        {
            result += trace_code; // skip first repetition's trailing off
        }

        return result;
    }

    inline TraceCode generateExpectedTrace(SignalCode code, NumberBase base,
                                           NumDigits   num_digits = 0,
                                           RepeatTimes repeats    = 1)
    {
        SignalSequencer sequencer;
        sequencer.loadSignalCode(code, base, num_digits);
        SignalPulseIterator pulse_iterator = sequencer.createPulseIterator();

        TraceCode trace{};
        for (RepeatTimes i = 0; i < repeats; ++i)
        {
            pulse_iterator.reset();
            while (pulse_iterator.hasNext())
            {
                auto element = pulse_iterator.next();
                trace +=
                    toTraceCode(element.get_mode(), element.get_duration());
            }
        }

        return trace;
    }

    // Check the behavior of the blinker against the expected values.
    inline void checkBlinkerBehavior(SignalEmitter&         blinker,
                                     MockLedControlLogger&  logger,
                                     const TestBlinkerCase& testCase)
    {
        const SignalCode code_to_show =
            testCase.blink_code.value_or(DEFAULT_CODE);
        const NumDigits   num_digits = testCase.numDigits.value_or(0);
        const RepeatTimes repeats    = testCase.repeats.value_or(1);
        const NumberBase  base = testCase.number_base.value_or(NumberBase::DEC);

        const TraceCode expected_trace =
            generateExpectedTrace(code_to_show, base, num_digits, repeats);

        blinker.setRepeatTimes(repeats);
        blinker.showCode(code_to_show, base, num_digits);
        logger.setBlinker(&blinker);
        runSequencer(&blinker, &logger);
        const TraceCode actual_trace = logger.traceLogToString();

        // Check the trace log
        if (testCase.trace_check == TraceCheck::ENFORCED)
        {
            STRCMP_EQUAL(expected_trace.c_str(), actual_trace.c_str());
        }

        // Check the base and pulse levels
        if (testCase.expectedBase.has_value())
        {
            CHECK_EQUAL_TEXT(testCase.expectedBase.value(),
                             logger.getBaseLevel(), "BASE level mismatch");
        }
        if (testCase.expectedPulse.has_value())
        {
            CHECK_EQUAL_TEXT(testCase.expectedPulse.value(),
                             logger.getPulseLevel(), "Pulse level mismatch");
        }
    }

    // Generate a SignalCode code using a pattern of digits
    inline SignalCode
    generatePatternOfDigits(const GeneratePatternParams& params)
    {
        SignalCode code_to_show = 0;
        const auto base_value   = to_value(params.number_base);

        PatternOption pattern_to_use = params.pattern;
        if (params.pattern == PatternOption::SEQUENCIAL_DOWN &&
            params.num_digits >= base_value)
        {
            pattern_to_use = PatternOption::DESCENDING_FROM_MAX;
        }

        for (Counter i = 0; i < params.num_digits; ++i)
        {
            DigitValue digit_to_show = 0;

            switch (pattern_to_use)
            {
                case PatternOption::SEQUENCIAL_UP:
                    digit_to_show = (i + 1) % base_value;
                    break;

                case PatternOption::SEQUENCIAL_DOWN:
                    digit_to_show = (params.num_digits - i) % base_value;
                    break;

                case PatternOption::DESCENDING_FROM_MAX:
                    digit_to_show = static_cast<DigitValue>(
                        (base_value - 1 - (i % base_value)));
                    break;

                case PatternOption::SAME_DIGIT:
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
