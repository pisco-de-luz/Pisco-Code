#include "CppUTest/TestHarness.h"

#include "code_blinker.hpp"
#include "pisco_constants.hpp"

using namespace pisco_code;

namespace
{

    class MockLedController : public LedController
    {
      public:
        void setPeakLevel(LedLevel led_level) override
        {
            peak_calls++;
            last_peak = led_level;
        }

        void setDimmedLevel(LedLevel led_level) override
        {
            dimmed_calls++;
            last_dimmed = led_level;
        }

        void setBlinkMode(BlinkMode mode) override { blink_mode = mode; }

        Int32     peak_calls   = 0;
        Int32     dimmed_calls = 0;
        Int32     off_calls    = 0;
        LedLevel  last_peak    = 0;
        LedLevel  last_dimmed  = 0;
        BlinkMode blink_mode   = BlinkMode::None;
    };

} // namespace

TEST_GROUP(CodeBlinkerGroup)
{
    MockLedController controller;
    CodeBlinker*      blinker;

    void setup() override
    {
        blinker = new CodeBlinker(&controller);
    }

    void teardown() override
    {
        delete blinker;
    }
};

TEST(CodeBlinkerGroup, InitiallyPaused)
{
    CHECK_FALSE(blinker->isRunning());
}

TEST(CodeBlinkerGroup, ShowCodeStartsSequence)
{
    bool result = blinker->showCode(42, NumberBase::DEC, 2, 1);
    CHECK_TRUE(result);
    CHECK_TRUE(blinker->isRunning());
}

TEST(CodeBlinkerGroup, LoopTriggersDimmedStart)
{
    blinker->showCode(5, NumberBase::DEC, 1, 1);
    blinker->loop(1); // simulate loop trigger
    CHECK_EQUAL(1, controller.dimmed_calls);
    CHECK_EQUAL(DEFAULT_DIMMED_LEVEL, controller.last_dimmed);
}
