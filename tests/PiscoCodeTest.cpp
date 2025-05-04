#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"

// Minimal mock for LED abstraction layer
static bool fakeLedFunc(uint8_t code)
{
    return (code == pisco::LED_ON || code == pisco::LED_OFF || code == pisco::LED_FUNC_OK);
}

TEST_GROUP(PiscoCodeTestGroup)
{
    PiscoCode code;
    void      setup()
    {
        code.setup(fakeLedFunc);
    }
    void teardown()
    {
    }
};

TEST(PiscoCodeTestGroup, SetupShouldSucceed)
{
    CHECK_TRUE(code.setup(fakeLedFunc));
}

TEST(PiscoCodeTestGroup, ShowCodeReturnsOK)
{
    LONGS_EQUAL(static_cast<uint8_t>(pisco::status_t::OK),
                code.showCode(123, static_cast<uint8_t>(pisco::base_t::DECIMAL)));
}

TEST(PiscoCodeTestGroup, RejectNewCodeWhileRunning)
{
    code.showCode(123, static_cast<uint8_t>(pisco::base_t::DECIMAL));
    LONGS_EQUAL(static_cast<uint8_t>(pisco::status_t::SEQUENCE_RUNNING),
                code.showCode(456, static_cast<uint8_t>(pisco::base_t::DECIMAL)));
}
