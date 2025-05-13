#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(LoopBehaviorBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(LoopBehaviorBlinkerTest, ShouldHoldDimLightForDigitZero)
{
    blinker.setDimmedLevel(testutils::MID_DIMMED_LEVEL);
    const testutils::TestBlinkerCase test_case{.code_pair      = testutils::CODE_0,
                                               .trace_check    = testutils::TraceCheck::NotEnforced,
                                               .expectedDimmed = testutils::MID_DIMMED_LEVEL};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkDigits_1_2_0)
{
    const testutils::TestBlinkerCase test_case{.code_pair   = testutils::CODE_120,
                                               .trace_check = testutils::TraceCheck::Enforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkNegativeSingleDigit)
{
    const testutils::TestBlinkerCase test_case{.code_pair   = testutils::CODE_NEG_7,
                                               .trace_check = testutils::TraceCheck::Enforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldRepeatBlinkingSequenceTwice)
{
    const testutils::TestBlinkerCase test_case{.trace_check = testutils::TraceCheck::Enforced,
                                               .repeats     = 2};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldEndInFinalPause)
{
    const testutils::TestBlinkerCase test_case{.trace_check = testutils::TraceCheck::NotEnforced};

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
    auto trace_actual = logger.traceLogToString();
    // Find trailing LED off pattern (represented by a sequence of '_')
    const auto trail_off_start =
        static_cast<TraceStrIndex>(trace_actual.find_last_not_of(testutils::LED_OFF_CHARACTER) + 1);
    if (trail_off_start == TraceCode::npos)
    {
        FAIL("No trailing off pattern found");
    }

    CHECK_TRUE_TEXT(trace_actual.length() > trail_off_start, "");
}

TEST(LoopBehaviorBlinkerTest, ShouldHandleMixOfZeroAndOne)
{
    blinker.showCode(1010, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4MgS4M0S4MgS4M0S4L0M", logger.traceLogToString().c_str());
}

TEST(LoopBehaviorBlinkerTest, ShouldStartAndEndWithLedOff)
{
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();

    // Ensure the trace starts and ends with 0M (off periods)
    CHECK_TRUE(trace.substr(0, 2) == "0M");
    CHECK_TRUE(trace.substr(trace.size() - 2, 2) == "0M");
}

TEST(LoopBehaviorBlinkerTest, ShouldNotTurnOnLedDuringIdlePhases)
{
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    logger.setBlinker(&blinker);

    runSequencer(&blinker, &logger);

    for (const auto& e : logger.getEvents())
    {
        if (e.state == LedEvent::On && !e.isLedBeingUsedNow)
        {
            FAIL("LED turned ON during an idle phase");
        }
    }
}
