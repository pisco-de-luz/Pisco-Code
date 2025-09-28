#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"

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

        void setBaseLevel(LedLevel led_level) override
        {
            dimmed_calls++;
            last_dimmed = led_level;
        }

        void setCurrentSignalLevel(BlinkMode mode) override
        {
            blink_mode = mode;
        }

        [[nodiscard]] Int32 getDimmedCalls() const noexcept
        {
            return dimmed_calls;
        }

        [[nodiscard]] LedLevel getLastDimmed() const noexcept
        {
            return last_dimmed;
        }

        void update() override
        {
        }

        [[nodiscard]] bool readyForPhaseChange() const noexcept override
        {
            return phase_ready_;
        }

      private:
        Int32     peak_calls   = 0;
        Int32     dimmed_calls = 0;
        Int32     off_calls    = 0;
        LedLevel  last_peak    = 0;
        LedLevel  last_dimmed  = 0;
        BlinkMode blink_mode   = BlinkMode::NONE;
        bool      phase_ready_ = true;
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
    const bool result =
        blinker->showCode(testutils::DEFAULT_CODE, NumberBase::DEC, 2);
    CHECK_TRUE(result);
    CHECK_TRUE(blinker->isRunning());
}

TEST(SignalEmitterGroup, LoopTriggersDimmedStart)
{
    blinker->showCode(testutils::CODE_5, NumberBase::DEC, 1);
    blinker->loop();
    CHECK_EQUAL(1, controller.getDimmedCalls());
    CHECK_EQUAL(DEFAULT_DIMMED_LEVEL, controller.getLastDimmed());
}
