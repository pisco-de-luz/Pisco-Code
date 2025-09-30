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
        void setPeakLevel(IntensityLevel led_level) override
        {
            peak_calls++;
            last_peak = led_level;
        }

        void setBaseLevel(IntensityLevel led_level) override
        {
            base_calls++;
            last_base = led_level;
        }

        void setCurrentSignalMode(SignalMode mode) override
        {
            blink_mode = mode;
        }

        [[nodiscard]] IntensityLevel getPeakLevel() const noexcept override
        {
            return last_peak;
        }

        [[nodiscard]] IntensityLevel getBaseLevel() const noexcept override
        {
            return last_base;
        }

        [[nodiscard]] SignalMode getCurrentSignalMode() const noexcept override
        {
            return blink_mode;
        }

        [[nodiscard]] Int32 getBaseCalls() const noexcept
        {
            return base_calls;
        }

        [[nodiscard]] IntensityLevel getLastBase() const noexcept
        {
            return last_base;
        }

        void update() override
        {
        }

        [[nodiscard]] bool readyForPhaseChange() const noexcept override
        {
            return phase_ready_;
        }

      private:
        Int32          peak_calls   = 0;
        Int32          base_calls   = 0;
        Int32          off_calls    = 0;
        IntensityLevel last_peak    = 0;
        IntensityLevel last_base    = 0;
        SignalMode     blink_mode   = SignalMode::NOT_DEFINED;
        bool           phase_ready_ = true;
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

TEST(SignalEmitterGroup, LoopTriggersBaseStart)
{
    blinker->showCode(testutils::CODE_5, NumberBase::DEC, 1);
    blinker->loop();
    CHECK_EQUAL(1, controller.getBaseCalls());
    CHECK_EQUAL(DEFAULT_BASE_LEVEL, controller.getLastBase());
}
