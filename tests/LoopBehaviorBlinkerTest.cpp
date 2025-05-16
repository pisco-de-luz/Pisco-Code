#include "CppUTest/TestHarness.h"

#include "BlinkerTestUtils.hpp"
#include "MockLedControlLogger.hpp"
#include "MockLedControllerAdapter.hpp"

#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

TEST_GROUP(LoopBehaviorBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(LoopBehaviorBlinkerTest, ShouldHoldDimLightForDigitZero)
{
    blinker.setDimmedLevel(testutils::MID_DIMMED_LEVEL);
    const testutils::TestBlinkerCase test_case{.blink_code     = testutils::CODE_0,
                                               .trace_check    = testutils::TraceCheck::NotEnforced,
                                               .expectedDimmed = testutils::MID_DIMMED_LEVEL};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkDigits_1_2_0)
{
    const testutils::TestBlinkerCase test_case{.blink_code  = testutils::CODE_120,
                                               .trace_check = testutils::TraceCheck::Enforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkNegativeSingleDigit)
{
    const testutils::TestBlinkerCase test_case{.blink_code  = testutils::CODE_NEG_7,
                                               .trace_check = testutils::TraceCheck::Enforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldRepeatBlinkingSequenceTwice)
{
    const testutils::TestBlinkerCase test_case{.trace_check = testutils::TraceCheck::Enforced,
                                               .repeats     = 2};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldEndInFinalDimmedPause)
{
    const testutils::TestBlinkerCase test_case{.trace_check = testutils::TraceCheck::NotEnforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
    auto trace_actual = logger.traceLogToString();

    // Find trailing LED off pattern (represented by a sequence of '-')
    const auto trail_off_start = static_cast<testutils::TraceStrIndex>(
        trace_actual.find_last_not_of(testutils::LED_DIMMED_CHARACTER) + 1);
    if (trail_off_start == testutils::TraceCode::npos)
    {
        FAIL("No trailing off pattern found");
    }
}

TEST(LoopBehaviorBlinkerTest, ShouldHandleMixOfZeroAndOne)
{
    const testutils::TestBlinkerCase test_case{.blink_code  = testutils::CODE_1010,
                                               .trace_check = testutils::TraceCheck::Enforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldNotTurnOnLedDuringIdlePhases)
{
    const testutils::TestBlinkerCase test_case{.trace_check = testutils::TraceCheck::NotEnforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);

    for (const auto& e : logger.getEvents())
    {
        if (e.state == testutils::LedEvent::On && !e.isLedBeingUsedNow)
        {
            FAIL("LED turned ON during an idle phase");
        }
    }
}
