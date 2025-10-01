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
    SignalEmitter            blinker{&controller};
};

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultPwmLevel)
{
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = DEFAULT_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    controller.setPeakLevel(MID_PULSE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = MID_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    controller.setPeakLevel(TOO_HIGH_PULSE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = HIGHEST_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectSlightHighPwmLevel)
{
    controller.setPeakLevel(HIGHEST_PULSE_LEVEL + 1);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = HIGHEST_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultBaseLevel)
{
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = DEFAULT_BASE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldUseCustomBaseLevel)
{

    controller.setBaseLevel(MID_BASE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = MID_BASE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect base level to be clamped to safe default when above allowed maximum.
IGNORE_TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighBaseLevel)
{
    controller.setBaseLevel(TOO_HIGH_BASE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = HIGHEST_BASE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest,
     ShouldNotAffectDefaultPulseLevelWhenSettingBaseLevel)
{
    for (auto led_level : ALL_BASE_LEVELS)
    {
        controller.setBaseLevel(led_level);

        const TestBlinkerCase test_case{
            .trace_check   = TraceCheck::NOT_ENFORCED,
            .expectedPulse = DEFAULT_PULSE_LEVEL,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

IGNORE_TEST(SetterBehaviorBlinkerTest,
            ShouldNotAffecLowestBaseLevelWhenSettingPulseLevel)
{
    for (auto led_level : ALL_PULSE_LEVELS)
    {
        controller.setPeakLevel(led_level);
        controller.setBaseLevel(LOWEST_BASE_LEVEL);
        const TestBlinkerCase test_case{
            .trace_check  = TraceCheck::NOT_ENFORCED,
            .expectedBase = LOWEST_BASE_LEVEL,
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