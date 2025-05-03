#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"

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

TEST_GROUP(LoopBehaviorTest)
{
    PiscoCode code;

    void setup() {
        logger = std::make_unique<MockLedControlLogger>();
        code.setup(&ledMockHAL);
    }

    void teardown() {
        logger.reset();
    }
};

IGNORE_TEST(LoopBehaviorTest, ShouldHoldDimLightForDigitZero)
{
    code.setPWM(10);        // No max-bright blinks
    code.setDimPWM(5);     // Dim phase: LED should stay ON
    code.setRepeat(0);     // Don't repeat
    code.setMinDigits(1);  // Force display of single digit
    code.showCode(0, PiscoCode::DECIMAL);

    constexpr uint8_t LOOP_COUNTER_TICKS = 20;
    constexpr uint8_t LOOP_CALLS_PER_COUNTER = 64;

    uint8_t loopCounter = 0;
    for (uint8_t t = 0; t < LOOP_COUNTER_TICKS; ++t) {
        //logger->setTime(loopCounter);

        for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i) {
            code.loop(loopCounter);
        }

        ++loopCounter;
    }

    const auto& log = logger->getEvents();

    // Optional: print debug info
    for (const auto& entry : log) {
        printf("T=%d  STATE=%d  DURATION=%d\n", entry.timestamp, entry.state, entry.duration);
    }

    uint16_t onCount = 0;
    uint16_t offCount = 0;
    for (const auto& entry : log) {
        if (entry.state == LED_CALL_ON)  ++onCount;
        if (entry.state == LED_CALL_OFF) ++offCount;
    }
    CHECK_EQUAL(10, onCount);
    CHECK_EQUAL(15, offCount);
    // These values may be adjusted based on exact dim behavior
    // CHECK_COMPARE(onCount, > 0);
    // CHECK_COMPARE(offCount, > 0);
}
