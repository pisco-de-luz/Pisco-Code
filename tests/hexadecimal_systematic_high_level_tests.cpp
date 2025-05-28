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

TEST_GROUP(HexadecimalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::HEX, blinker, logger);
}

TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::HEX, blinker, logger);
}

TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSequentialDownDigitsUpToMaxDigits)
{
    runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase::HEX, blinker, logger);
}

TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)
{
    runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase::HEX, blinker, logger);
}