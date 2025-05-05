#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "helpers/TestLoopRunner.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"

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
    blinker.showCode(1, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('g') != std::string::npos); // Default peak PWM = 10
}

TEST(SetterBehaviorBlinkerTest, ShouldUseCustomPwmLevel)
{
    blinker.setPeakLevel(6);
    blinker.showCode(1, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('7') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighPwmLevel)
{
    blinker.setPeakLevel(255); // Should cap to PWM_MAX
    blinker.showCode(1, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('g') != std::string::npos); // PWM_MAX = 15 = 'g'
}

TEST(SetterBehaviorBlinkerTest, ShouldAffectLedOnDuration)
{
    blinker.setPeakLevel(3);
    blinker.showCode(1, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('4') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldUseDefaultDimLevel)
{
    blinker.showCode(0, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('4') != std::string::npos); // Default dim = 1
}

TEST(SetterBehaviorBlinkerTest, ShouldSetDimLevelAffectingIdle)
{
    blinker.setDimmedLevel(4);
    blinker.showCode(0, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('5') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighDimLevel)
{
    blinker.setDimmedLevel(255);
    blinker.showCode(0, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    CHECK(trace.find('0') != std::string::npos);
}

TEST(SetterBehaviorBlinkerTest, ShouldNotAffectPeakPwmWhenSettingDim)
{
    blinker.setPeakLevel(9);
    blinker.setDimmedLevel(2);
    blinker.showCode(2, base_t::DECIMAL, 0, 1);
    runSequencer(&blinker, &logger);
    const std::string trace = logger.traceLogToString();
    STRCMP_EQUAL("4M0S4M0S4MgS4MgS4SgS4L0M", logger.traceLogToString().c_str());
    CHECK(trace.find('a') != std::string::npos); // 9 + 1 = 'a'
    CHECK(trace.find('3') != std::string::npos); // 2 + 1 = '3'
}

TEST(SetterBehaviorBlinkerTest, ShouldPadWithLeadingZeros)
{
    blinker.showCode(12, base_t::DECIMAL, 4, 1); // Expect: 0012
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4M0S4M0S4MgS4MgS4SgS4L0M", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldNotPadIfNotNeeded)
{
    blinker.showCode(123, base_t::DECIMAL, 2, 1); // Expect: 23
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4MgS4SgS4MgS4SgS4SgS4L0M", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldHandleSingleDigitZero)
{
    blinker.showCode(0, base_t::DECIMAL, 1, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4M0S4L0M", logger.traceLogToString().c_str());
}

TEST(SetterBehaviorBlinkerTest, ShouldRejectTooHighMinDigits)
{
    blinker.showCode(1, base_t::DECIMAL, 99, 1);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("4MgS4L0M", logger.traceLogToString().c_str());
}
