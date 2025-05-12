#include "CppUTest/TestHarness.h"
#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
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
    bool started = blinker.showCode(42, NumberBase::DECIMAL, 0, 1);
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);

    runSequencer(&blinker, &logger);

    STRCMP_EQUAL("___---^-^-^-^---^-^---___", logger.traceLogToString().c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowZero)
{
    bool started = blinker.showCode(0, NumberBase::DECIMAL, 0, 1);
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);

    runSequencer(&blinker, &logger);
    std::string expected = logger.traceLogToString();
    STRCMP_EQUAL("___---_---___", expected.c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowDecimal12345)
{
    bool started = blinker.showCode(12345, NumberBase::DECIMAL, 0, 1);
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^---^-^---^-^-^---^-^-^-^---^-^-^-^-^---___",
                 logger.traceLogToString().c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowBinary5)
{
    bool started = blinker.showCode(5, NumberBase::BINARY, 0, 1); // binary: 101
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^---_---^---___", logger.traceLogToString().c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowHex255)
{
    bool started = blinker.showCode(255, NumberBase::HEXADECIMAL, 0, 1); // hex: FF
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^-^-^-^-^-^-^-^-^-^-^-^-^-^-^---^-^-^-^-^-^-^-^-^-^-^-^-^-^-^---___",
                 logger.traceLogToString().c_str());
}

TEST(BaseEncodingBlinkerTest, CanShowOctal28)
{
    bool started = blinker.showCode(28, NumberBase::OCTAL, 0, 1); // octal: 34
    logger.setBlinker(&blinker);
    CHECK_TRUE(started);
    runSequencer(&blinker, &logger);
    STRCMP_EQUAL("___---^-^-^---^-^-^-^---___", logger.traceLogToString().c_str());
}
