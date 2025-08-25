#include "CppUTest/TestHarness.h"

#include "blinker_test_utils.hpp"
#include "mock_led_control_logger.hpp"
#include "mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(LoopBehaviorBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{&controller};
};

TEST(LoopBehaviorBlinkerTest, ShouldHoldDimLightForDigitZero)
{
    blinker.setDimmedLevel(MID_DIMMED_LEVEL);
    const TestBlinkerCase test_case{.blink_code     = CODE_0,
                                    .trace_check    = TraceCheck::NOT_ENFORCED,
                                    .expectedDimmed = MID_DIMMED_LEVEL};

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkDigits_1_2_0)
{
    const TestBlinkerCase test_case{.blink_code  = CODE_120,
                                    .trace_check = TraceCheck::ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldBlinkNegativeSingleDigit)
{
    const TestBlinkerCase test_case{.blink_code  = CODE_NEG_7,
                                    .trace_check = TraceCheck::ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldRepeatBlinkingSequenceTwice)
{
    const TestBlinkerCase test_case{.trace_check = TraceCheck::ENFORCED,
                                    .repeats     = 2};

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldEndInFinalDimmedPause)
{
    const TestBlinkerCase test_case{.trace_check = TraceCheck::NOT_ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);
    auto trace_actual = logger.traceLogToString();

    // Find trailing LED off pattern (represented by a sequence of '-')
    const auto trail_off_start = static_cast<TraceStrIndex>(
        trace_actual.find_last_not_of(LED_DIMMED_CHARACTER) + 1);
    if (trail_off_start == TraceCode::npos)
    {
        FAIL("No trailing off pattern found");
    }
}

TEST(LoopBehaviorBlinkerTest, ShouldHandleMixOfZeroAndOne)
{
    const TestBlinkerCase test_case{.blink_code  = CODE_1010,
                                    .trace_check = TraceCheck::ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(LoopBehaviorBlinkerTest, ShouldNotTurnOnLedDuringIdlePhases)
{
    const TestBlinkerCase test_case{.trace_check = TraceCheck::NOT_ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);

    for (const auto& e : logger.getEvents())
    {
        if (e.state == LedEvent::On && !e.isLedBeingUsedNow)
        {
            FAIL("LED turned ON during an idle phase");
        }
    }
}
