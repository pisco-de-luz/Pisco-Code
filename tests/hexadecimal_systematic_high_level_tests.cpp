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

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(HexadecimalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

// - Example: 9, 88, 777, ..., 111111111
TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::HEXADECIMAL, blinker, logger);
}

TEST(HexadecimalSystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::HEXADECIMAL, blinker, logger);
}