#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include <limits>

// This is the simulated LED HAL function passed to setup() during unit testing.
static MockLedControlLogger* logger = nullptr;
static bool ledMockHAL(uint8_t ctrlLED) {
    return logger ? logger->handle(ctrlLED) : false;
}

TEST_GROUP(SetupTest)
{
    PiscoCode code;

    void setup() {
        logger = new MockLedControlLogger();
    }

    void teardown() {
        delete logger;
        logger = nullptr;
    }
};

TEST(SetupTest, ValidFunctionPointer_ShouldReturnTrue)
{
    bool result = code.setup(&ledMockHAL);
    CHECK_TRUE(result);
}

TEST(SetupTest, ShouldHaveExpectedNumberOfCalls)
{
    code.setup(&ledMockHAL);

    constexpr uint16_t totalPossible = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t accepted = 3; // LED_ON, LED_OFF, LED_FUNC_OK
    constexpr uint16_t expected = 1 /* FUNC_OK */ + (totalPossible - accepted) + 1 /* TURN_LED_OFF */;
    CHECK_EQUAL(expected, logger->getEvents().size());
}

TEST(SetupTest, FirstEventShouldBeFuncOk)
{
    code.setup(&ledMockHAL);
    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_FUNC_OK, log.at(0).state);
}

TEST(SetupTest, LastEventShouldBeLedOff)
{
    code.setup(&ledMockHAL);
    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_OFF, log.back().state);
}

TEST(SetupTest, ShouldHaveExactly253InvalidCalls)
{
    code.setup(&ledMockHAL);
    const auto& log = logger->getEvents();

    uint16_t count = 0;
    for (const auto& entry : log) {
        if (entry.state == LED_CALL_INVALID) ++count;
    }

    constexpr uint16_t totalPossible = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t accepted = 3;
    CHECK_EQUAL(totalPossible - accepted, count);
}

TEST(SetupTest, NullFunctionPointer_ShouldReturnFalse)
{
    bool result = code.setup(nullptr);
    CHECK_FALSE(result);
}
