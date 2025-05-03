#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "mocks/MockLedControl.hpp"
#include <limits>

TEST_GROUP(SetupTest)
{
    PiscoCode code;

    void setup() {
        clearLedEventLog();
    }

    void teardown() {
        clearLedEventLog();
    }
};

TEST(SetupTest, ValidFunctionPointer_ShouldReturnTrue)
{
    bool result = code.setup(&mockLedControl);
    CHECK_TRUE(result);
}

TEST(SetupTest, ShouldCallLedFuncOkFirst)
{
    code.setup(&mockLedControl);
    CHECK_EQUAL(LED_CALL_FUNC_OK, ledEventLog.at(0));
}

TEST(SetupTest, ShouldCallInvalidForAllNonAcceptedCtrlCodes)
{
    code.setup(&mockLedControl);
    size_t invalidCount = 0;
    for (size_t i = 1; i < ledEventLog.size() - 1; ++i) {
        if (ledEventLog[i] == LED_CALL_INVALID) {
            ++invalidCount;
        }
    }

    // Expected: 256 total values - 3 accepted codes = 253 invalids
    constexpr size_t acceptedCodes = 3;
    constexpr size_t totalPossible = std::numeric_limits<uint8_t>::max() + 1;
    CHECK_EQUAL(totalPossible - acceptedCodes, invalidCount);
}

TEST(SetupTest, ShouldCallLedOffLast)
{
    code.setup(&mockLedControl);
    CHECK_EQUAL(LED_CALL_OFF, ledEventLog.back());
}

TEST(SetupTest, ShouldHaveExpectedNumberOfCalls)
{
    code.setup(&mockLedControl);
    constexpr size_t totalPossible = std::numeric_limits<uint8_t>::max() + 1;  // 256
    constexpr size_t acceptedCodes = 3;  // LED_ON, LED_OFF, LED_FUNC_OK
    constexpr size_t expectedEvents = 1 /* FUNC_OK */ + (totalPossible - acceptedCodes) + 1 /* LED_OFF in setup() */;
    
    CHECK_EQUAL(expectedEvents, ledEventLog.size()); 
}

TEST(SetupTest, NullFunctionPointer_ShouldReturnFalse)
{
    bool result = code.setup(nullptr);
    CHECK_FALSE(result);
    CHECK_EQUAL(0, ledEventLog.size());
}
