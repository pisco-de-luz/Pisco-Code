#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"

#include <cstdint>
#include <limits>
#include <memory>

namespace
{
    std::unique_ptr<MockLedControlLogger> logger;

    bool ledMockHAL(uint8_t ctrlLED)
    {
        return (logger != nullptr) ? logger->handle(ctrlLED) : false;
    }
} // namespace

TEST_GROUP(SetupTest)
{
    PiscoCode code;

    void setup()
    {
        logger = std::make_unique<MockLedControlLogger>();
    }

    void teardown()
    {
        logger->flush();
        logger.reset();
    }
};

TEST(SetupTest, ValidFunctionPointer_ShouldReturnTrue)
{
    const bool result = code.setup(&ledMockHAL);
    CHECK_TRUE(result);
}

TEST(SetupTest, ShouldHaveExpectedGroupedEvents)
{
    code.setup(&ledMockHAL);
    logger->flush();

    const auto& log = logger->getEvents();
    CHECK_EQUAL(6, log.size());

    CHECK_EQUAL(LED_CALL_ON, log.at(0).state);
    // CHECK_EQUAL(1, log.at(0).duration);

    CHECK_EQUAL(LED_CALL_OFF, log.at(1).state);
    // CHECK_EQUAL(1, log.at(1).duration);

    CHECK_EQUAL(LED_CALL_FUNC_FAIL, log.at(2).state);
    // constexpr uint16_t TOTAL_FAIL_BEFORE_FUNC_OK = PiscoCode::LED_FUNC_OK - 2;
    // CHECK_EQUAL(TOTAL_FAIL_BEFORE_FUNC_OK, log.at(2).duration);

    CHECK_EQUAL(LED_CALL_FUNC_OK, log.at(3).state);
    // CHECK_EQUAL(1, log.at(3).duration);

    CHECK_EQUAL(LED_CALL_FUNC_FAIL, log.at(4).state);
    // constexpr uint16_t TOTAL_FAIL_AFTER_FUNC_OK = std::numeric_limits<uint8_t>::max() -
    // PiscoCode::LED_FUNC_OK; CHECK_EQUAL(TOTAL_FAIL_AFTER_FUNC_OK, log.at(4).duration);

    CHECK_EQUAL(LED_CALL_OFF, log.at(5).state);
    // CHECK_EQUAL(1, log.at(5).duration);
}

TEST(SetupTest, FirstEventShouldBeLedOn)
{
    code.setup(&ledMockHAL);
    logger->flush();

    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_ON, log.front().state);
}

TEST(SetupTest, LastEventShouldBeLedOff)
{
    code.setup(&ledMockHAL);
    logger->flush();

    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_OFF, log.back().state);
}

TEST(SetupTest, NullFunctionPointer_ShouldReturnFalse)
{
    const bool result = code.setup(nullptr);
    CHECK_FALSE(result);
}
