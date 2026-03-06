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
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedPeak = to_sw_pwm_level(DEFAULT_PEAK_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    controller.setPeakLevel(MID_PEAK_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedPeak = to_sw_pwm_level(MID_PEAK_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    controller.setPeakLevel(TOO_HIGH_PEAK_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedPeak = to_sw_pwm_level(HIGHEST_PEAK_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultBaseLevel)
{
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(DEFAULT_BASE_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomBaseLevel)
{

    controller.setBaseLevel(MID_BASE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(MID_BASE_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect base level to be clamped to safe default when above allowed maximum.
TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighBaseLevel)
{
    controller.setBaseLevel(TOO_HIGH_BASE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check  = TraceCheck::NOT_ENFORCED,
        .expectedBase = to_sw_pwm_level(HIGHEST_BASE_LEVEL),
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest,
     ShouldNotAffectDefaultPeakLevelWhenSettingBaseLevel)
{
    for (auto led_level : ALL_BASE_LEVELS)
    {
        controller.setBaseLevel(led_level);

        const TestBlinkerCase test_case{
            .trace_check  = TraceCheck::NOT_ENFORCED,
            .expectedPeak = to_sw_pwm_level(DEFAULT_PEAK_LEVEL),
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest,
     ShouldNotAffectLowestBaseLevelWhenSettingPeakLevel)
{
    for (auto led_level : ALL_PEAK_LEVELS)
    {
        controller.setPeakLevel(led_level);
        controller.setBaseLevel(LOWEST_BASE_LEVEL);
        const TestBlinkerCase test_case{
            .trace_check  = TraceCheck::NOT_ENFORCED,
            .expectedBase = to_sw_pwm_level(LOWEST_BASE_LEVEL),
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