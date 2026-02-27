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
        [[nodiscard]] Int32 getBaseCalls() const noexcept
        {
            return base_calls;
        }

        [[nodiscard]] IntensityLevel getLastBase() const noexcept
        {
            return last_base;
        }

        void update() noexcept override
        {
        }

        [[nodiscard]] bool readyForPhaseChange() const noexcept override
        {
            return phase_ready_;
        }

      private:
        Int32          high_level_calls = 0;
        Int32          base_calls       = 0;
        Int32          off_calls        = 0;
        IntensityLevel last_high_level  = 0;
        IntensityLevel last_base        = 0;
        SignalMode     blink_mode       = SignalMode::NOT_DEFINED;
        bool           phase_ready_     = true;
    };

} // namespace

TEST_GROUP(SignalEmitterGroup)
{
    MockLedController controller;
    SignalEmitter*    blinker;

    void setup() override
    {
        blinker = new SignalEmitter(controller);
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
