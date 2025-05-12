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
    blinker.showCode(0, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---_---___", logger.traceLogToString().c_str());
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkDigits_1_2_0)
{
    blinker.showCode(120, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___^^^-_^^^___", logger.traceLogToString().c_str());
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkNegativeSingleDigit)
{
    blinker.showCode(-2, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4MgM4MgS4SgS4L0M", logger.traceLogToString().c_str());
}

TEST(LoopBehaviorBlinkerTest, ShouldRepeatBlinkingSequenceTwice)
{
    blinker.showCode(1, NumberBase::DECIMAL, 0, 2);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4MgS4L0M4MgS4L0M", logger.traceLogToString().c_str());
}

TEST(LoopBehaviorBlinkerTest, ShouldEndInFinalPause)
{
    blinker.showCode(7, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    std::string trace   = logger.traceLogToString();
    std::string lastTwo = trace.substr(trace.size() - 2, 2);
    STRCMP_EQUAL("0M", lastTwo.c_str());
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
