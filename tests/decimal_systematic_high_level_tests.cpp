#include "CppUTest/TestHarness.h"

#include <cstdint>
#include <iostream>

#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(DecimalSystematicHighLevelTests)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSequentialDigitsInDecimalUpToMaxDigits)
{
    const auto number_base = NumberBase::DECIMAL;

    std::cout << std::endl;

    for (testutils::Counter digit_index = 1; digit_index <= pisco::MAX_DIGITS; ++digit_index)
    {
        logger.clear();
        BlinkCode code_to_show = 0;
        for (testutils::Counter i = 0; i < digit_index; ++i)
        {
            const testutils::Counter digit_to_show = ((i + 1) % to_value(number_base));
            code_to_show = code_to_show * to_value(number_base) + digit_to_show;
        }

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(DecimalSystematicHighLevelTests, ShouldBlinkSameDigitsInDecimalUpToMaxDigits)
{
    const auto number_base = NumberBase::DECIMAL;

    for (testutils::Counter digit_index = 1; digit_index <= pisco::MAX_DIGITS; ++digit_index)
    {
        logger.clear();
        BlinkCode code_to_show = 0;
        for (testutils::Counter i = 0; i < digit_index; ++i)
        {
            const testutils::Counter digit_to_show =
                ((pisco::MAX_DIGITS - digit_index) % (to_value(number_base) - 1)) + 1;
            code_to_show = code_to_show * to_value(number_base) + digit_to_show;
        }

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code_to_show,
            .number_base = number_base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}
