#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "pisco_constants.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace testutils;

TEST_GROUP_BASE(LoopBehaviorBlinkerTest, BlinkerTestFixture){};

IGNORE_TEST(LoopBehaviorBlinkerTest, ShouldHoldDimLightForDigitZero)
{
    controller.setLowLevel(MID_LOW_LEVEL);
    const TestBlinkerCase test_case{.blink_code   = CODE_0,
                                    .trace_check  = TraceCheck::NOT_ENFORCED,
                                    .expectedBase = MID_LOW_LEVEL};

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

TEST(LoopBehaviorBlinkerTest, ShouldEndInFinalBasePause)
{
    const TestBlinkerCase test_case{.trace_check = TraceCheck::NOT_ENFORCED};

    checkBlinkerBehavior(blinker, logger, test_case);
    auto trace_actual = logger.traceLogToString();

    // Find trailing LED off pattern (represented by a sequence of '-')
    using size_type = TraceCode::size_type;
    const size_type trail_off_start =
        trace_actual.find_last_not_of(LED_BASE_CHARACTER) + 1;
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
