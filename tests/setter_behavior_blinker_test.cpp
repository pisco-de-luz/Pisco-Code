#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "helpers/tests_types.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

using namespace testutils;

TEST_GROUP_BASE(SetterBehaviorBlinkerTest, BlinkerTestFixture){};

// The mock measures SW PWM levels (intensity / SW_PWM_LEVEL_FACTOR),
// not raw intensity levels — hence all expected values use to_sw_pwm_level().
TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultPwmLevel)
{
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = to_sw_pwm_level(DEFAULT_HIGH_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    controller.setHighLevel(MID_HIGH_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = to_sw_pwm_level(MID_HIGH_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    controller.setHighLevel(TOO_HIGH_HIGH_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = to_sw_pwm_level(HIGHEST_HIGH_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultBaseLevel)
{
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(DEFAULT_LOW_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomBaseLevel)
{

    controller.setLowLevel(MID_LOW_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(MID_LOW_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect base level to be clamped to safe default when above allowed maximum.
TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighBaseLevel)
{
    controller.setLowLevel(TOO_HIGH_LOW_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(HIGHEST_LOW_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest,
            ShouldNotAffectDefaultPulseLevelWhenSettingBaseLevel)
{
    for (auto led_level : ALL_LOW_LEVELS)
    {
        controller.setLowLevel(led_level);

        const TestBlinkerCase test_case{
            .trace_check   = TraceCheck::NOT_ENFORCED,
            .expectedPulse = to_sw_pwm_level(DEFAULT_HIGH_LEVEL),
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest,
            ShouldNotAffecLowestBaseLevelWhenSettingPulseLevel)
{
    for (auto led_level : ALL_HIGH_LEVELS)
    {
        controller.setHighLevel(led_level);
        controller.setLowLevel(LOWEST_LOW_LEVEL);
        const TestBlinkerCase test_case{
            .trace_check  = TraceCheck::NOT_ENFORCED,
            .expectedBase = to_sw_pwm_level(LOWEST_LOW_LEVEL),
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