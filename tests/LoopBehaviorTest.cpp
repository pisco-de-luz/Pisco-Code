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
    code.showCode(0, PiscoCode::DECIMAL);

    constexpr uint8_t LOOP_COUNTER_TICKS     = 50;
    constexpr uint8_t LOOP_CALLS_PER_COUNTER = 64;
    Timestamp         loopCounter            = 0;

    uint8_t fakeMillis = 0;

    while (code.isSequencing())
    {
        for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i)
        {
            logger->setTime(loopCounter++);
            code.loop(fakeMillis);
        }

        ++fakeMillis;
    }

    logger->flush();

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1M0S1L0M", trace.c_str());
}
