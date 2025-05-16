#include "CppUTest/TestHarness.h"

#include <cstdint>
#include <iostream>

#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using testutils::runSequencer;

TEST_GROUP(DecimalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

// - Example: 9, 88, 777, ..., 111111111
TEST(DecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::DEC, blinker, logger);
}

// - Example: 1, 12, 123, 1234, ..., 123456789
TEST(DecimalSystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::DEC, blinker, logger);
}

// - Example: 1, 21, 321, 4321, ..., 987654321
TEST(DecimalSystematicHighLevelTests, ShouldBlinkSequentialDownDigitsUpToMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SequencialDown,
            .number_base = number_base,
            .num_digits  = num_digits,
        };
        const BlinkCode                  code_to_show = testutils::generatePatternOfDigits(params);
        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SequencialUp,
            .number_base = number_base,
            .num_digits  = num_digits,
        };
        const BlinkCode                  code_to_show = testutils::generatePatternOfDigits(params);
        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const DigitValue digit_to_show =
            ((max_digits - num_digits) % (to_value(number_base) - 1)) + 1;
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SameDigit,
            .number_base = number_base,
            .num_digits  = num_digits,
            .digit       = digit_to_show,
        };
        const BlinkCode code_to_show = testutils::generatePatternOfDigits(params);

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SequencialUp,
            .number_base = number_base,
            .num_digits  = num_digits,
        };
        const BlinkCode                  code_to_show = testutils::generatePatternOfDigits(params);
        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits / 2,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const DigitValue digit_to_show =
            ((max_digits - num_digits) % (to_value(number_base) - 1)) + 1;
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SameDigit,
            .number_base = number_base,
            .num_digits  = num_digits,
            .digit       = digit_to_show,
        };
        const BlinkCode code_to_show = testutils::generatePatternOfDigits(params);

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits / 2,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkFullPatternOfMaxBaseDigitUpToMaxDigits)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SameDigit,
            .number_base = number_base,
            .num_digits  = num_digits,
            .digit       = (to_value(number_base) - 1),
        };
        const BlinkCode code_to_show = testutils::generatePatternOfDigits(params);

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldRepeatBlinkOfMaxBaseDigitUpToMaxDigitsNineTimes)
{
    const auto      number_base = NumberBase::DEC;
    const NumDigits max_digits  = maxDigitsForBase(number_base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const testutils::GeneratePatternParams params{
            .pattern     = testutils::PatternOption::SameDigit,
            .number_base = number_base,
            .num_digits  = num_digits,
            .digit       = (to_value(number_base) - 1),
        };
        const BlinkCode code_to_show = testutils::generatePatternOfDigits(params);

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
            .repeats     = 9,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}