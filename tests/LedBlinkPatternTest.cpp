#include "helpers/LedBlinkPattern.hpp"
#include "CppUTest/TestHarness.h"

TEST_GROUP(LedBlinkPatternTests){};

TEST(LedBlinkPatternTests, ShouldOutputOneCharacterForShortDurations)
{
    LedBlinkPattern pattern;
    pattern.append(3, 100);  // DIMMED
    pattern.append(10, 100); // PULSE
    pattern.append(0, 100);  // NONE

    STRCMP_EQUAL("-^_", pattern.tracePatternToString().c_str());
}

TEST(LedBlinkPatternTests, ShouldOutputThreeCharactersForMediumDurations)
{
    LedBlinkPattern pattern;
    pattern.append(3, 1500);  // DIMMED
    pattern.append(10, 1500); // PULSE
    pattern.append(0, 1500);  // NONE

    STRCMP_EQUAL("---^^^___", pattern.tracePatternToString().c_str());
}

TEST(LedBlinkPatternTests, ShouldOutputFiveCharactersForLongDurations)
{
    LedBlinkPattern pattern;
    pattern.append(3, 3000);
    pattern.append(10, 3000);
    pattern.append(0, 3000);

    STRCMP_EQUAL("-----^^^^^_____", pattern.tracePatternToString().c_str());
}

TEST(LedBlinkPatternTests, ShouldReturnExclamationMarkIfInvalidPattern)
{
    LedBlinkPattern pattern;
    pattern.append(1, 100);
    pattern.append(2, 100);
    pattern.append(4, 100); // introduces a 3rd non-zero level -> invalid

    STRCMP_EQUAL("!", pattern.tracePatternToString().c_str());
    CHECK_FALSE(pattern.isValid());
}

TEST(LedBlinkPatternTests, ShouldReturnExclamationMarkIfUnknownLevelAppears)
{
    LedBlinkPattern pattern;
    pattern.append(3, 100); // not enough data to define dimmed/pulse

    STRCMP_EQUAL("!", pattern.tracePatternToString().c_str());
    CHECK_FALSE(pattern.isValid()); // Still valid internally, but output can't proceed
}

TEST(LedBlinkPatternTests, ShouldProvideCorrectLevelGetters)
{
    LedBlinkPattern pattern;
    pattern.append(3, 100);
    pattern.append(10, 100);

    CHECK_TRUE(pattern.isValid());
    CHECK_EQUAL(3, pattern.getDimmedLevel());
    CHECK_EQUAL(10, pattern.getPulseLevel());
}

TEST(LedBlinkPatternTests, ShouldTreatZeroAsAlwaysAllowed)
{
    LedBlinkPattern pattern;
    pattern.append(3, 100);
    pattern.append(10, 100);
    pattern.append(0, 300); // NONE — allowed

    CHECK_TRUE(pattern.isValid());
    STRCMP_EQUAL("-^_", pattern.tracePatternToString().c_str());
}
