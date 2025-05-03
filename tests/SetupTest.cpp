#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"
// Required for std::numeric_limits — clang-tidy false positive
#include <limits>
#include <cstdint>
#include <memory>

// This is the simulated LED HAL function passed to setup() during unit testing.
namespace {
    std::unique_ptr<MockLedControlLogger> logger;

    // Simulated LED HAL function passed to setup()
    bool ledMockHAL(uint8_t ctrlLED) {
        return logger ? logger->handle(ctrlLED) : false;
    }
}

TEST_GROUP(SetupTest)
{
    PiscoCode code;

    void setup() {
        logger = std::make_unique<MockLedControlLogger>();
    }

    void teardown() {
        logger.reset();  // optional: happens automatically
    }
};

TEST(SetupTest, ValidFunctionPointer_ShouldReturnTrue)
{
    const bool Result = code.setup(&ledMockHAL);
    CHECK_TRUE(Result);
}

TEST(SetupTest, ShouldHaveExpectedNumberOfCalls)
{
    code.setup(&ledMockHAL);

    constexpr uint16_t TotalPossible = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t Accepted = 3; // LED_ON, LED_OFF, LED_FUNC_OK
    // Expect: 1 (FUNC_OK) + all invalids + 1 (TURN_LED_OFF)
    constexpr uint16_t Expected = 1 + (TotalPossible - Accepted) + 1;
    CHECK_EQUAL(Expected, static_cast<uint16_t>(logger->getEvents().size()));
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

    constexpr uint16_t TotalPossible = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t Accepted = 3;
    CHECK_EQUAL(TotalPossible - Accepted, count);
}

TEST(SetupTest, NullFunctionPointer_ShouldReturnFalse)
{
    const bool Result = code.setup(nullptr);
    CHECK_FALSE(Result);
}
