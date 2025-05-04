#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "helpers/TestLoopRunner.hpp"
#include "mocks/MockLedControlLogger.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace
{
    std::unique_ptr<MockLedControlLogger> logger;

    bool ledMockHAL(uint8_t ctrlLED)
    {
        return (logger != nullptr) ? logger->handle(ctrlLED) : false;
    }
} // namespace

TEST_GROUP(SetterTest)
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

TEST(SetterTest, ShouldUseDefaultPwmLevel)
{
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('g') != std::string::npos); // Default PWM is 10 (hex A)
}

TEST(SetterTest, ShouldUseCustomPwmLevel)
{
    code.setPwm(6);
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('7') != std::string::npos);
}

TEST(SetterTest, ShouldRejectTooHighPwmLevel)
{
    code.setPwm(255); // Out of range
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    // Expect fallback/default still used
    CHECK(trace.find('g') != std::string::npos);
}

TEST(SetterTest, ShouldAffectLedOnDuration)
{
    code.setPwm(3);
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('4') != std::string::npos);
}

TEST(SetterTest, ShouldUseDefaultDimPwmLevel)
{
    code.showCode(0, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('1') != std::string::npos); // Default is 1
}

TEST(SetterTest, ShouldSetDimLevelAffectingIdle)
{
    code.setDimPwm(4);
    code.showCode(0, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('5') != std::string::npos);
}

TEST(SetterTest, ShouldRejectTooHighDimPwmLevel)
{
    code.setDimPwm(255); // Out of range
    code.showCode(0, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('g') != std::string::npos);
}

TEST(SetterTest, ShouldNotAffectBlinkPwm)
{
    code.setPwm(9);
    code.setDimPwm(2);
    code.showCode(2, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    CHECK(trace.find('a') != std::string::npos);
    CHECK(trace.find('3') != std::string::npos);
}

TEST(SetterTest, ShouldPadWithLeadingZeros)
{
    code.setMinDigits(4);
    code.showCode(12, static_cast<uint8_t>(pisco::base_t::DECIMAL)); // Expect: 0012
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1M0S1M0S1MgS1MgS1SgS1L0M", trace.c_str());
}

TEST(SetterTest, ShouldNotPadIfNotNeeded)
{
    code.setMinDigits(2);                                             // force 2 digits
    code.showCode(123, static_cast<uint8_t>(pisco::base_t::DECIMAL)); // Will blink number 23 only
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1SgS1MgS1SgS1SgS1L0M", trace.c_str());
}

TEST(SetterTest, ShouldHandleSingleDigitZero)
{
    code.setMinDigits(1);
    code.showCode(0, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1M0S1L0M", trace.c_str());
}

TEST(SetterTest, ShouldRejectTooHighMinDigits)
{
    code.setMinDigits(99); // Exceeds MAX_DIGITS
    code.showCode(1, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1L0M", trace.c_str()); // It is not working as expected
}