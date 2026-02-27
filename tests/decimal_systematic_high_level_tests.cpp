#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_utils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "mocks/mock_led_control_logger.hpp"
#include "mocks/mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"

using namespace pisco_code;

TEST_GROUP(DecimalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{controller};
};

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::DEC, blinker, logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::DEC, blinker, logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSequentialDownDigitsUpToMaxDigits)
{
    runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase::DEC, blinker,
                                                logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::DEC,
                                                             blinker, logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::DEC, blinker,
                                                       logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(
        NumberBase::DEC, blinker, logger);
}

TEST(DecimalSystematicHighLevelTests,
     ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)
{
    runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase::DEC,
                                                           blinker, logger);
}

// Ignored due to long runtime from many full-digit blink sequences.
// Useful for stress/regression tests, not routine execution.
IGNORE_TEST(DecimalSystematicHighLevelTests,
            ShouldBlinkSameMaxBaseDigitUpToMaxDigitsNineTimes)
{
    runSameMaxBaseDigitUpToMaxDigitsNineTimesForBase(NumberBase::DEC, blinker,
                                                     logger);
}
