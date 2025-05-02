#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"

// Minimal mock for LED abstraction layer
static bool fakeLedFunc(uint8_t code)
{
    return (code == PiscoCode::LED_ON || code == PiscoCode::LED_OFF || code == PiscoCode::LED_FUNC_OK);
}

TEST_GROUP(PiscoCodeTestGroup)
{
    PiscoCode code;
    void setup() {
        code.setup(fakeLedFunc);
    }
    void teardown() {}
};

TEST(PiscoCodeTestGroup, SetupShouldSucceed)
{
    CHECK_TRUE(code.setup(fakeLedFunc));
}

TEST(PiscoCodeTestGroup, ShowCodeReturnsOK)
{
    LONGS_EQUAL(PiscoCode::OK, code.showCode(123, PiscoCode::DECIMAL));
}

TEST(PiscoCodeTestGroup, RejectNewCodeWhileRunning)
{
    code.showCode(123, PiscoCode::DECIMAL);
    LONGS_EQUAL(PiscoCode::SEQUENCE_RUNNING, code.showCode(456, PiscoCode::DECIMAL));
}
