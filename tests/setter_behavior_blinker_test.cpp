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

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultPwmLevel)
{
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = DEFAULT_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    blinker.setPeakLevel(MID_PULSE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = MID_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    blinker.setPeakLevel(TOO_HIGH_PULSE_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = HIGHEST_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectSlightHighPwmLevel)
{
    blinker.setPeakLevel(HIGHEST_PULSE_LEVEL + 1);
    const TestBlinkerCase test_case{
        .trace_check   = TraceCheck::NOT_ENFORCED,
        .expectedPulse = HIGHEST_PULSE_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultDimmedLevel)
{
    const TestBlinkerCase test_case{
        .trace_check    = TraceCheck::NOT_ENFORCED,
        .expectedDimmed = DEFAULT_DIMMED_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomDimmedLevel)
{

    blinker.setBaseLevel(MID_DIMMED_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check    = TraceCheck::NOT_ENFORCED,
        .expectedDimmed = MID_DIMMED_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect dimmed level to be clamped to safe default when above allowed maximum.
TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighDimmedLevel)
{
    blinker.setBaseLevel(TOO_HIGH_DIMMED_LEVEL);
    const TestBlinkerCase test_case{
        .trace_check    = TraceCheck::NOT_ENFORCED,
        .expectedDimmed = HIGHEST_DIMMED_LEVEL,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest,
     ShouldNotAffectDefaultPulseLevelWhenSettingDimmedLevel)
{
    for (auto led_level : ALL_DIMMED_LEVELS)
    {
        blinker.setBaseLevel(led_level);

        const TestBlinkerCase test_case{
            .trace_check   = TraceCheck::NOT_ENFORCED,
            .expectedPulse = DEFAULT_PULSE_LEVEL,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest,
     ShouldNotAffecLowestDimmedLevelWhenSettingPulseLevel)
{
    for (auto led_level : ALL_PULSE_LEVELS)
    {
        blinker.setPeakLevel(led_level);
        blinker.setBaseLevel(LOWEST_DIMMED_LEVEL);
        const TestBlinkerCase test_case{
            .trace_check    = TraceCheck::NOT_ENFORCED,
            .expectedDimmed = LOWEST_DIMMED_LEVEL,
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