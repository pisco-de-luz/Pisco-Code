#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_utils.hpp"
#include "helpers/tests_types.hpp"
#include "mocks/mock_led_control_logger.hpp"
#include "mocks/mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SetterBehaviorBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{controller};
};

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultPwmLevel)
{
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = DEFAULT_HIGH_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    controller.setHighLevel(MID_HIGH_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = MID_HIGH_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    controller.setHighLevel(TOO_HIGH_HIGH_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = HIGHEST_HIGH_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultBaseLevel)
{
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = DEFAULT_LOW_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseCustomBaseLevel)
{

    controller.setLowLevel(MID_LOW_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = MID_LOW_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect base level to be clamped to safe default when above allowed maximum.
IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighBaseLevel)
{
    controller.setLowLevel(TOO_HIGH_LOW_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = HIGHEST_LOW_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest,
            ShouldNotAffectDefaultPulseLevelWhenSettingBaseLevel)
{
    for (auto led_level : ALL_LOW_LEVELS)
    {
        controller.setLowLevel(led_level);

        const TestBlinkerCase test_case{
            .trace_check   = TraceCheck::NOT_ENFORCED,
            .expectedPulse = DEFAULT_HIGH_LEVEL,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

IGNORE_TEST(SetterBehaviorBlinkerTest,
            ShouldNotAffecLowestBaseLevelWhenSettingPulseLevel)
{
    for (auto led_level : ALL_HIGH_LEVELS)
    {
        controller.setHighLevel(led_level);
        controller.setLowLevel(LOWEST_LOW_LEVEL);
        const TestBlinkerCase test_case{
            .trace_check  = TraceCheck::NOT_ENFORCED,
            .expectedBase = LOWEST_LOW_LEVEL,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest, ShouldPadWithLeadingZeros)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_0010,
        .trace_check = TraceCheck::ENFORCED,
        .numDigits   = 4,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldNotPadIfNotNeeded)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_12345,
        .trace_check = TraceCheck::ENFORCED,
        .numDigits   = 5,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldTruncateToNumDigits)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_12345_TRUNCATED_TO_2_DIGITS,
        .trace_check = TraceCheck::ENFORCED,
        .numDigits   = 2,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighNumDigits)
{
    const NumDigits       max_digits = max_digits_for_base(NumberBase::DEC);
    const TestBlinkerCase test_case{
        .trace_check = TraceCheck::ENFORCED,
        .numDigits   = max_digits + 1,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooLowNumDigits)
{
    const TestBlinkerCase test_case{
        .trace_check = TraceCheck::ENFORCED,
        .numDigits   = 0,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldHandleSingleDigitZero)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_0,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}