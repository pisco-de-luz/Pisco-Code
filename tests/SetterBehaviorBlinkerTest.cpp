#include "CppUTest/TestHarness.h"

#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
#include "helpers/tests_types.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(SetterBehaviorBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultPwmLevel)
{
    const testutils::TestBlinkerCase test_case{
        .trace_check   = testutils::TraceCheck::NotEnforced,
        .expectedPulse = pisco::DEFAULT_PULSE_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{

    blinker.setPeakLevel(testutils::MID_PULSE_LEVEL);
    const testutils::TestBlinkerCase test_case{
        .trace_check   = testutils::TraceCheck::NotEnforced,
        .expectedPulse = testutils::MID_PULSE_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    blinker.setPeakLevel(testutils::TOO_HIGH_PULSE_LEVEL);
    const testutils::TestBlinkerCase test_case{
        .trace_check   = testutils::TraceCheck::NotEnforced,
        .expectedPulse = testutils::HIGHEST_PULSE_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectSlightHighPwmLevel)
{
    blinker.setPeakLevel(testutils::HIGHEST_PULSE_LEVEL + 1);
    const testutils::TestBlinkerCase test_case{
        .trace_check   = testutils::TraceCheck::NotEnforced,
        .expectedPulse = testutils::HIGHEST_PULSE_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultDimmedLevel)
{
    const testutils::TestBlinkerCase test_case{
        .trace_check    = testutils::TraceCheck::NotEnforced,
        .expectedDimmed = pisco::DEFAULT_DIMMED_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomDimmedLevel)
{

    blinker.setDimmedLevel(testutils::MID_DIMMED_LEVEL);
    const testutils::TestBlinkerCase test_case{
        .trace_check    = testutils::TraceCheck::NotEnforced,
        .expectedDimmed = testutils::MID_DIMMED_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

// Expect dimmed level to be clamped to safe default when above allowed maximum.
TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighDimmedLevel)
{
    blinker.setDimmedLevel(testutils::TOO_HIGH_DIMMED_LEVEL);
    const testutils::TestBlinkerCase test_case{
        .trace_check    = testutils::TraceCheck::NotEnforced,
        .expectedDimmed = testutils::HIGHEST_DIMMED_LEVEL,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldNotAffectDefaultPulseLevelWhenSettingDimmedLevel)
{
    for (auto led_level : testutils::ALL_DIMMED_LEVELS)
    {
        blinker.setDimmedLevel(led_level);

        const testutils::TestBlinkerCase test_case{
            .trace_check   = testutils::TraceCheck::NotEnforced,
            .expectedPulse = pisco::DEFAULT_PULSE_LEVEL,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest, ShouldNotAffecLowestDimmedLevelWhenSettingPulseLevel)
{
    for (auto led_level : testutils::ALL_PULSE_LEVELS)
    {
        blinker.setPeakLevel(led_level);
        blinker.setDimmedLevel(testutils::LOWEST_DIMMED_LEVEL);
        const testutils::TestBlinkerCase test_case{
            .trace_check    = testutils::TraceCheck::NotEnforced,
            .expectedDimmed = testutils::LOWEST_DIMMED_LEVEL,
        };

        testutils::checkBlinkerBehavior(blinker, logger, test_case);
    }
}

TEST(SetterBehaviorBlinkerTest, ShouldPadWithLeadingZeros)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_0010,
        .trace_check = testutils::TraceCheck::Enforced,
        .numDigits   = 4,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldNotPadIfNotNeeded)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_12345,
        .trace_check = testutils::TraceCheck::Enforced,
        .numDigits   = 5,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldTruncateToNumDigits)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_12345_TRUNCATED_TO_2_DIGITS,
        .trace_check = testutils::TraceCheck::Enforced,
        .numDigits   = 2,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighNumDigits)
{
    const pisco::NumDigits           max_digits = maxDigitsForBase(pisco::NumberBase::DECIMAL);
    const testutils::TestBlinkerCase test_case{
        .trace_check = testutils::TraceCheck::Enforced,
        .numDigits   = max_digits + 1,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooLowNumDigits)
{
    const testutils::TestBlinkerCase test_case{
        .trace_check = testutils::TraceCheck::Enforced,
        .numDigits   = 0,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(SetterBehaviorBlinkerTest, ShouldHandleSingleDigitZero)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_0,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}