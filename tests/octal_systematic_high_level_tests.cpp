#include "CppUTest/TestHarness.h"

#include <cstdint>
#include <iostream>

#include "code_blinker.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "mocks/mock_led_control_logger.hpp"
#include "mocks/mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

TEST_GROUP(OctalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{&controller};
};

TEST(OctalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSequentialDownDigitsUpToMaxDigits)
{
    runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

TEST(OctalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase::OCT, blinker, logger);
}

// Ignored due to long runtime from many full-digit blink sequences.
// Useful for stress/regression tests, not routine execution.
IGNORE_TEST(OctalSystematicHighLevelTests, ShouldBlinkSameMaxBaseDigitUpToMaxDigitsNineTimes)
{
    runSameMaxBaseDigitUpToMaxDigitsNineTimesForBase(NumberBase::OCT, blinker, logger);
}