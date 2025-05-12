#ifndef TEST_LOOP_RUNNER_HPP
#define TEST_LOOP_RUNNER_HPP

#include "../mocks/MockLedControlLogger.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include <cstdint>
#include <optional>

namespace testutils
{
    enum class TraceCheck
    {
        NotEnforced,
        Enforced,
    };

    struct CodeTracePair
    {
        pisco::BlinkCode code{0};
        TraceCode        trace{};
    };

    struct TestBlinkerCase
    {
        std::optional<testutils::CodeTracePair> code_pair{std::nullopt};
        testutils::TraceCheck                   trace_check{TraceCheck::NotEnforced};
        std::optional<pisco::NumDigits>         numDigits{std::nullopt};
        std::optional<pisco::RepeatTimes>       repeats{std::nullopt};
        std::optional<pisco::LedLevel>          expectedDimmed{};
        std::optional<pisco::LedLevel>          expectedPulse{};
    };

    // Example predefined codes
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

    // Drives the loop for a maximum simulated time, default 64 seconds.
    inline void runSequencer(pisco::CodeBlinker* code, MockLedControlLogger* logger)
    {
        constexpr uint8_t   LOOP_CALLS_PER_COUNTER = 64;
        constexpr Timestamp MAX_SEQUENCING_LOOP =
            64000UL; // Limit of 64 seconds to prevent infinite loop
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

    // Check the behavior of the blinker against the expected values.
    inline void checkBlinkerBehavior(pisco::CodeBlinker& blinker, MockLedControlLogger& logger,
                                     const TestBlinkerCase& testCase)
    {
        // constexpr pisco::BlinkCode CODE_WITH_PULSE_ZERO_AND_PULSES = 102;
        const CodeTracePair default_code_pair = DEFAULT_CODE;
        pisco::BlinkCode    code_to_show      = default_code_pair.code;
        TraceCode           expected_trace    = default_code_pair.trace;
        if (testCase.code_pair.has_value())
        {
            code_to_show   = testCase.code_pair->code;
            expected_trace = testCase.code_pair->trace;
        }

        const pisco::NumDigits   num_digits = testCase.numDigits.value_or(0);
        const pisco::RepeatTimes repeats    = testCase.repeats.value_or(1);

        blinker.showCode(code_to_show, pisco::NumberBase::DECIMAL, num_digits, repeats);
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

} // namespace testutils

#endif // TEST_LOOP_RUNNER_HPP
