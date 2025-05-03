#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"

// Required for std::numeric_limits — clang-tidy false positive
#include <limits>
#include <cstdint>
#include <memory>

namespace {
    std::unique_ptr<MockLedControlLogger> logger;

    // Simulated LED HAL function passed to setup() during unit testing.
    bool ledMockHAL(uint8_t ctrlLED) {
        return (logger != nullptr) ? logger->handle(ctrlLED) : false;
    }
}

TEST_GROUP(SetupTest)
{
    PiscoCode code;

    void setup() {
        logger = std::make_unique<MockLedControlLogger>();
    }

    void teardown() {
        logger.reset();
    }
};

IGNORE_TEST(SetupTest, ValidFunctionPointer_ShouldReturnTrue)
{
    const bool result = code.setup(&ledMockHAL);
    CHECK_TRUE(result);
}

TEST(SetupTest, ShouldHaveExpectedNumberOfCalls)
{
    code.setup(&ledMockHAL);

    // Expect: 1 (FUNC_OK) + all invalids + 1 (TURN_LED_OFF)
    constexpr uint16_t TOTAL_POSSIBLE = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t ACCEPTED       = 3;  // LED_ON, LED_OFF, LED_FUNC_OK
    constexpr uint16_t EXPECTED       = 1 + (TOTAL_POSSIBLE - ACCEPTED) + 1;

    const auto actualSize = static_cast<uint16_t>(logger->getEvents().size());
    CHECK_EQUAL(EXPECTED, actualSize);
}

IGNORE_TEST(SetupTest, FirstEventShouldBeFuncOk)
{
    code.setup(&ledMockHAL);

    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_FUNC_OK, log.at(0).state);
}

IGNORE_TEST(SetupTest, LastEventShouldBeLedOff)
{
    code.setup(&ledMockHAL);

    const auto& log = logger->getEvents();
    CHECK_EQUAL(LED_CALL_OFF, log.back().state);
}

IGNORE_TEST(SetupTest, ShouldHaveExactly253InvalidCalls)
{
    code.setup(&ledMockHAL);

    const auto& log = logger->getEvents();
    uint16_t invalidCount = 0;

    for (const auto& entry : log) {
        if (entry.state == LED_CALL_INVALID) {
            ++invalidCount;
        }
    }

    constexpr uint16_t TOTAL_POSSIBLE = std::numeric_limits<uint8_t>::max() + 1;
    constexpr uint16_t ACCEPTED       = 3;
    constexpr uint16_t EXPECTED_INVALIDS = TOTAL_POSSIBLE - ACCEPTED;

    CHECK_EQUAL(EXPECTED_INVALIDS, invalidCount);
}

IGNORE_TEST(SetupTest, NullFunctionPointer_ShouldReturnFalse)
{
    const bool result = code.setup(nullptr);
    CHECK_FALSE(result);
}
