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

TEST_GROUP(BinarySystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(BinarySystematicHighLevelTests, ShouldBlinkSameDigitsUpToMaxDigits)
{
    runSameDigitsUpToMaxDigitsForBase(NumberBase::BINARY, blinker, logger);
}

TEST(BinarySystematicHighLevelTests, ShouldBlinkSequentialUpDigitsUpToMaxDigits)
{
    runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase::BINARY, blinker, logger);
}