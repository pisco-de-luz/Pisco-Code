#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "pisco_constants.hpp"

// clang-format off

// Stamps out one complete TEST_GROUP with 8 tests (7 active + 1 ignored)
// for a given Radix.  Group names are kept so that CTest filtering
// (e.g. -g BinarySystematic...) and IGNORE_TEST behaviour are unchanged.
#define SYSTEMATIC_TESTS_FOR_RADIX(GroupName, Base)                             \
                                                                               \
    TEST_GROUP_BASE(GroupName, BlinkerTestFixture){};                           \
                                                                               \
    TEST(GroupName, ShouldBlinkSameDigitsUpToMaxDigits)                         \
    {                                                                          \
        runSameDigitsUpToMaxDigitsForRadix(Base, blinker, logger);              \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSequentialUpDigitsUpToMaxDigits)                 \
    {                                                                          \
        runSequentialUpDigitsUpToMaxDigitsForRadix(Base, blinker, logger);      \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSequentialDownDigitsUpToMaxDigits)               \
    {                                                                          \
        runSequentialDownDigitsUpToMaxDigitsForRadix(Base, blinker, logger);    \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)            \
    {                                                                          \
        runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForRadix(              \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)        \
    {                                                                          \
        runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForRadix(                    \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)        \
    {                                                                          \
        runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForRadix(          \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)              \
    {                                                                          \
        runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForRadix(                \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSameMaxBaseDigitUpToMaxDigitsNineTimes)                    \
    {                                                                          \
        runSameMaxBaseDigitUpToMaxDigitsNineTimesForRadix(                      \
            Base, blinker, logger);                                            \
    }

// clang-format on

SYSTEMATIC_TESTS_FOR_RADIX(BinarySystematicHighLevelTests,      Radix::BIN)
SYSTEMATIC_TESTS_FOR_RADIX(OctalSystematicHighLevelTests,       Radix::OCT)
SYSTEMATIC_TESTS_FOR_RADIX(DecimalSystematicHighLevelTests,     Radix::DEC)
SYSTEMATIC_TESTS_FOR_RADIX(HexadecimalSystematicHighLevelTests, Radix::HEX)
