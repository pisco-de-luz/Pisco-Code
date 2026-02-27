#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "helpers/test_patterns_common.hpp"
#include "pisco_constants.hpp"

// clang-format off

// Stamps out one complete TEST_GROUP with 8 tests (7 active + 1 ignored)
// for a given NumberBase.  Group names are kept so that CTest filtering
// (e.g. -g BinarySystematic...) and IGNORE_TEST behaviour are unchanged.
#define SYSTEMATIC_TESTS_FOR_BASE(GroupName, Base)                             \
                                                                               \
    TEST_GROUP_BASE(GroupName, BlinkerTestFixture){};                           \
                                                                               \
    TEST(GroupName, ShouldBlinkSameDigitsUpToMaxDigits)                         \
    {                                                                          \
        runSameDigitsUpToMaxDigitsForBase(Base, blinker, logger);              \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSequentialUpDigitsUpToMaxDigits)                 \
    {                                                                          \
        runSequentialUpDigitsUpToMaxDigitsForBase(Base, blinker, logger);      \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSequentialDownDigitsUpToMaxDigits)               \
    {                                                                          \
        runSequentialDownDigitsUpToMaxDigitsForBase(Base, blinker, logger);    \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToMaxDigits)            \
    {                                                                          \
        runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(              \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName, ShouldBlinkSameDigitsUpToMaxDigitsPaddedToMaxDigits)        \
    {                                                                          \
        runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(                    \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigits)        \
    {                                                                          \
        runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(          \
            Base, blinker, logger);                                            \
    }                                                                          \
    TEST(GroupName,                                                            \
         ShouldBlinkSameDigitsUpToMaxDigitsPaddedToHalfMaxDigits)              \
    {                                                                          \
        runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(                \
            Base, blinker, logger);                                            \
    }                                                                          \
    IGNORE_TEST(GroupName,                                                     \
                ShouldBlinkSameMaxBaseDigitUpToMaxDigitsNineTimes)             \
    {                                                                          \
        runSameMaxBaseDigitUpToMaxDigitsNineTimesForBase(                      \
            Base, blinker, logger);                                            \
    }

// clang-format on

SYSTEMATIC_TESTS_FOR_BASE(BinarySystematicHighLevelTests,      NumberBase::BIN)
SYSTEMATIC_TESTS_FOR_BASE(OctalSystematicHighLevelTests,       NumberBase::OCT)
SYSTEMATIC_TESTS_FOR_BASE(DecimalSystematicHighLevelTests,     NumberBase::DEC)
SYSTEMATIC_TESTS_FOR_BASE(HexadecimalSystematicHighLevelTests, NumberBase::HEX)
