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
    blinker.showCode(12, NumberBase::DECIMAL, 4, 1); // Expect: 0012
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---_---_---^---^-^---___", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldNotPadIfNotNeeded)
{
    blinker.showCode(23, NumberBase::DECIMAL, 2, 1); // Expect: 23
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^-^---^-^-^---___", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldTruncateToNumDigits)
{
    blinker.showCode(321, NumberBase::DECIMAL, 2, 1); // Expect: 21
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^-^---^---___", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighMinDigits)
{
    blinker.showCode(1, NumberBase::DECIMAL, 99, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^---___", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldHandleSingleDigitZero)
{
    blinker.showCode(0, NumberBase::DECIMAL, 1, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---_---___", logger.traceLogToString().c_str());
}