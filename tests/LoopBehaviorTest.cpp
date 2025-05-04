#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControlLogger.hpp"

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
// #include <map>
#include <algorithm>
#include <iomanip>
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
    // code.setPWM(9);    // No max-bright blinks
    // code.setDimPWM(10); // Dim phase: LED should stay ON
    // code.setRepeat(0);  // Don't repeat
    //  code.setMinDigits(1);  // Force display of single digit
    code.showCode(0, PiscoCode::DECIMAL);

    constexpr uint8_t LOOP_COUNTER_TICKS     = 50;
    constexpr uint8_t LOOP_CALLS_PER_COUNTER = 64;
    Timestamp         loopCounter            = 0;

    uint8_t fakeMillis = 0;
    // for (uint8_t t = 0; t < LOOP_COUNTER_TICKS; ++t) {
    while (code.isSequencing())
    {
        for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i)
        {
            logger->setTime(loopCounter++); // Set the current time for the logger
            code.loop(fakeMillis);
        }

        ++fakeMillis;
    }

    logger->flush();
    const auto& log = logger->getEvents();

    // Optional: print debug info
    // for (const auto& entry : log) {
    //     if ( entry.duration > 1 ) {
    //         printf("\nT=%d  STATE=%d  DURATION=%d", entry.timestamp, entry.state,
    //         entry.duration);
    //     }
    // }
    // printf("\n");

    uint16_t onCount  = 0;
    uint16_t offCount = 0;
    for (const auto& entry : log)
    {
        if (entry.state == LED_CALL_ON)
            ++onCount;
        if (entry.state == LED_CALL_OFF)
            ++offCount;
    }

    std::string trace = logger->traceLogToString();
    printf("PWM Timeline: %s\n", trace.c_str());
    CHECK_EQUAL(10, onCount);
    CHECK_EQUAL(15, offCount);
    // These values may be adjusted based on exact dim behavior
    // CHECK_COMPARE(onCount, > 0);
    // CHECK_COMPARE(offCount, > 0);
}
