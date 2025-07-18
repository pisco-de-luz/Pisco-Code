#include "CppUTest/TestHarness.h"

#include <cstdint>
#include <iostream>

#include "helpers/blinker_test_utils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "mocks/mock_led_control_logger.hpp"
#include "mocks/mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

TEST_GROUP(BinarySystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{&controller};
};

TEST(BinarySystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSequentialDownDigitsUpToMaxDigits)
{
    runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase::BIN, blinker, logger);
}

// Ignored due to long runtime from many full-digit blink sequences.
// Useful for stress/regression tests, not routine execution.
IGNORE_TEST(BinarySystematicHighLevelTests, ShouldBlinkSameMaxBaseDigitUpToMaxDigitsNineTimes)
{
    runSameMaxBaseDigitUpToMaxDigitsNineTimesForBase(NumberBase::BIN, blinker, logger);
}