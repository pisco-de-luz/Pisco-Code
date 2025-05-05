#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"

using namespace pisco;

// namespace
// {

//     class MockLedController : public LedController
//     {
//       public:
//         void setPeakLevel(uint8_t level) override
//         {
//             peak_calls++;
//             last_peak = level;
//         }
//         void setDimmedLevel(uint8_t level) override
//         {
//             dimmed_calls++;
//             last_dimmed = level;
//         }
//         void turnOff() override { off_calls++; }

//         int     peak_calls   = 0;
//         int     dimmed_calls = 0;
//         int     off_calls    = 0;
//         uint8_t last_peak    = 0;
//         uint8_t last_dimmed  = 0;
//     };

// } // namespace

// TEST_GROUP(CodeBlinkerGroup)
// {
//     MockLedController controller;
//     CodeBlinker*      blinker;

//     void setup() override
//     {
//         blinker = new CodeBlinker(&controller);
//     }

//     void teardown() override
//     {
//         delete blinker;
//     }
// };

// TEST(CodeBlinkerGroup, InitiallyPaused)
// {
//     CHECK_FALSE(blinker->isRunning());
// }

// TEST(CodeBlinkerGroup, ShowCodeStartsSequence)
// {
//     bool result = blinker->showCode(42, base_t::DECIMAL, 2, 1);
//     CHECK_TRUE(result);
//     CHECK_TRUE(blinker->isRunning());
// }

// TEST(CodeBlinkerGroup, LoopTriggersDimmedStart)
// {
//     blinker->showCode(5, base_t::DECIMAL, 1, 1);
//     blinker->loop(1); // simulate loop trigger
//     CHECK_EQUAL(1, controller.dimmed_calls);
//     CHECK_EQUAL(INITIAL_DIMMED_PWM, controller.last_dimmed);
// }

// TEST(CodeBlinkerGroup, LoopEventuallyTurnsOff)
// {
//     blinker->showCode(1, base_t::DECIMAL, 1, 1);
//     for (uint8_t i = 0; i < 255; ++i)
//     {
//         blinker->loop(i);
//     }
//     CHECK(controller.off_calls > 0);
// }
