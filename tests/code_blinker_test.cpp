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
        void update() noexcept override
        {
        }

        [[nodiscard]] bool readyForPhaseChange() const noexcept override
        {
            return phase_ready_;
        }

      private:
        bool phase_ready_ = true;
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
        blinker->showCode(testutils::DEFAULT_CODE, Radix::DEC, 2);
    CHECK_TRUE(result);
    CHECK_TRUE(blinker->isRunning());
}

TEST(SignalEmitterGroup, StopWhenIdle_IsHarmless)
{
    CHECK_FALSE(blinker->isRunning());
    blinker->stop(); // should not crash or change anything meaningful
    CHECK_FALSE(blinker->isRunning());
}

TEST(SignalEmitterGroup, StopWhenRunning_SetsIsRunningToFalse)
{
    blinker->showCode(testutils::DEFAULT_CODE, Radix::DEC, 2);
    CHECK_TRUE(blinker->isRunning());

    blinker->stop();
    CHECK_FALSE(blinker->isRunning());
}

TEST(SignalEmitterGroup, StopAllowsShowCodeImmediately)
{
    blinker->showCode(testutils::DEFAULT_CODE, Radix::DEC, 2);
    CHECK_TRUE(blinker->isRunning());

    blinker->stop();

    const bool result =
        blinker->showCode(testutils::DEFAULT_CODE, Radix::DEC, 2);
    CHECK_TRUE(result);
    CHECK_TRUE(blinker->isRunning());
}
