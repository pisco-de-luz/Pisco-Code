#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "signal_emitter.hpp"

using namespace pisco_code;

namespace
{

    class MockLedController : public SignalController
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

        void setBlinkMode(BlinkMode mode) override
        {
            blink_mode = mode;
        }

        void update() override
        {
        }

        [[nodiscard]] bool readyForPhaseChange() const noexcept override
        {
            return true;
        }

        Int32     peak_calls   = 0;
        Int32     dimmed_calls = 0;
        Int32     off_calls    = 0;
        LedLevel  last_peak    = 0;
        LedLevel  last_dimmed  = 0;
        BlinkMode blink_mode   = BlinkMode::NONE;
    };

} // namespace

TEST_GROUP(SignalEmitterGroup)
{
    MockLedController controller;
    SignalEmitter*    blinker;

    void setup() override
    {
        blinker = new SignalEmitter(&controller);
    }

    void teardown() override
    {
        delete blinker;
    }
};

TEST(SignalEmitterGroup, InitiallyPaused)
{
    CHECK_FALSE(blinker->isRunning());
}

TEST(SignalEmitterGroup, ShowCodeStartsSequence)
{
    bool result = blinker->showCode(42, NumberBase::DEC, 2, 1);
    CHECK_TRUE(result);
    CHECK_TRUE(blinker->isRunning());
}

TEST(SignalEmitterGroup, LoopTriggersDimmedStart)
{
    blinker->showCode(5, NumberBase::DEC, 1, 1);
    blinker->loop(); // simulate loop trigger
    CHECK_EQUAL(1, controller.dimmed_calls);
    CHECK_EQUAL(DEFAULT_DIMMED_LEVEL, controller.last_dimmed);
}
