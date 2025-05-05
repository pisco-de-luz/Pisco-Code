#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "helpers/TestLoopRunner.hpp"
#include "mocks/MockLedControlLogger.hpp"

#include <cstdint>
#include <string>
namespace
{
    std::unique_ptr<MockLedControlLogger> logger;

    // Simulated LED HAL function passed to setup() during unit testing.
    bool ledMockHAL(uint8_t ctrlLED)
    {
        return (logger != nullptr) ? logger->handle(ctrlLED) : false;
    }
} // namespace

TEST_GROUP(LoopBehaviorTest)
{
    PiscoCode code;

    void setup()
    {
        logger = std::make_unique<MockLedControlLogger>();
        code.setup(&ledMockHAL);
    }

    void teardown()
    {
        logger.reset();
    }
};

TEST(LoopBehaviorTest, ShouldHoldDimLightForDigitZero)
{
    code.showCode(0, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());
    const std::string trace = logger->traceLogToString();

    STRCMP_EQUAL("4M0S4L0M", trace.c_str());
}

TEST(LoopBehaviorTest, ShouldBlinkDigits_1_2_0)
{
    code.showCode(120, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());
    const std::string trace = logger->traceLogToString();

    STRCMP_EQUAL("4MgS4MgS4SgS4M0S4L0M", trace.c_str());
}

TEST(LoopBehaviorTest, ShouldBlinkNegativeSingleDigit)
{
    code.showCode(-2, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());
    const std::string trace = logger->traceLogToString();

    STRCMP_EQUAL("4MgM4MgS4SgS4L0M", trace.c_str());
}

TEST(LoopBehaviorTest, ShouldRepeatBlinkingSequenceTwice)
{
    code.setRepeat(2);
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());
    const std::string trace = logger->traceLogToString();

    STRCMP_EQUAL("4MgS4L0M4MgS4L0M4MgS4L0M", trace.c_str());
}

TEST(LoopBehaviorTest, ShouldEndInFinalPause)
{
    code.showCode(7, static_cast<uint8_t>(pisco::base_t::DECIMAL)); // Only one repeat
    testutils::runSequencer(code, logger.get());
    const std::string trace        = logger->traceLogToString();
    const std::string lastTwoChars = trace.substr(trace.size() - 2, 2);

    STRCMP_EQUAL("0M", lastTwoChars.c_str());
}

TEST(LoopBehaviorTest, ShouldHandleMixOfZeroAndOne)
{
    code.showCode(1010, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());
    const std::string trace = logger->traceLogToString();

    STRCMP_EQUAL("4MgS4M0S4MgS4M0S4L0M", trace.c_str());
}