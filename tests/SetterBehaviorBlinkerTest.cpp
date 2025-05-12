#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "helpers/TestLoopRunner.hpp"
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
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('g') != std::string::npos); // Default peak PWM = 10
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{
    blinker.setPeakLevel(6);
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('7') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    blinker.setPeakLevel(255); // Should cap to PWM_MAX
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('g') != std::string::npos); // PWM_MAX = 15 = 'g'
}

TEST(SetterBehaviorBlinkerTest, ShouldAffectLedOnDuration)
{
    blinker.setPeakLevel(3);
    blinker.showCode(1, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('4') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultDimLevel)
{
    blinker.showCode(0, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('4') != std::string::npos); // Default dim = 1
}

TEST(SetterBehaviorBlinkerTest, ShouldSetDimLevelAffectingIdle)
{
    blinker.setDimmedLevel(4);
    blinker.showCode(0, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('5') != std::string::npos);
}

// Expect dimmed level to be clamped to safe default when above allowed maximum.
TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighDimLevel)
{
    constexpr LedLevel DIM_LEVEL_OVER_LIMIT = 255;
    constexpr LedLevel DIM_LEVEL_EXPECTED =
        pisco::DEFAULT_PULSE_LEVEL - pisco::MIN_PULSE_DIMMED_GAP;
    constexpr BlinkCode   CODE_TO_TEST = 10;
    constexpr NumDigits   NUM_DIGITS   = 0;
    constexpr RepeatTimes REPEATS      = 1;
    blinker.setDimmedLevel(DIM_LEVEL_OVER_LIMIT);
    blinker.showCode(CODE_TO_TEST, NumberBase::DECIMAL, NUM_DIGITS, REPEATS);
    logger.setBlinker(&blinker);
    runSequencer(&blinker, &logger);

    const TracerCode actual_trace = logger.traceLogToString();
    const LedLevel   dimmed_level = logger.getDimmedLevel();

    STRCMP_EQUAL("___---^---_---___", actual_trace.c_str());
    CHECK_TEXT(dimmed_level == DIM_LEVEL_EXPECTED, "Dimmed level was not clamped correctly");
}

TEST(SetterBehaviorBlinkerTest, ShouldNotAffectPeakPwmWhenSettingDim)
{
    constexpr LedLevel PULSE_LEVEL_EXPECTED = 9;
    constexpr LedLevel DIM_LEVEL_EXPECTED   = 6;
    blinker.setPeakLevel(PULSE_LEVEL_EXPECTED);
    blinker.setDimmedLevel(DIM_LEVEL_EXPECTED);

    blinker.showCode(2, NumberBase::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);

    const std::string actual_trace = logger.traceLogToString();
    const LedLevel    pulse_level  = logger.getPulseLevel();
    const LedLevel    dimmed_level = logger.getDimmedLevel();

    STRCMP_EQUAL("___---^-^---___", actual_trace.c_str());
    CHECK_EQUAL(PULSE_LEVEL_EXPECTED, pulse_level);
    CHECK_EQUAL(DIM_LEVEL_EXPECTED, dimmed_level);
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