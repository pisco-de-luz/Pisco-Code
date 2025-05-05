#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "helpers/TestLoopRunner.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(BaseEncodingBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(BaseEncodingBlinkerTest, CanShowDecimal42)
{
    // controller.setDimmedLevel(1);
    bool started = blinker.showCode(120, base_t::DECIMAL, 0, 1);
    CHECK_TRUE(started);

    runSequencer(&blinker, &logger);

    STRCMP_EQUAL("4MgS4MgS4SgS4M0S4L0M", logger.traceLogToString().c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowZero)
{
    bool started = blinker.showCode(0, base_t::DECIMAL, 2, 1);
    CHECK_TRUE(started);

    runSequencer(&blinker, &logger);

    STRCMP_EQUAL("4M0S4L0M", logger.traceLogToString().c_str());
}
