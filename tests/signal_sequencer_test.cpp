#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalSequencerTests)
{
    void check_next_signal_element(const SignalElement& expected)
    {
        auto actual = sequencer.popNextSignalElement();
        CHECK_EQUAL(to_value(expected.get_level()), to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected.get_times()), to_value(actual.get_times()));
        CHECK_EQUAL(to_value(expected.get_duration()), to_value(actual.get_duration()));
    }

    SignalSequencer sequencer;

    void teardown() override
    {
        sequencer.clear();
    }
};

TEST(SignalSequencerTests, DefaultConstructor_InitializesToZero)
{
    CHECK_EQUAL(0, sequencer.getElementCount());
    CHECK_FALSE(sequencer.hasMoreSignalCodeToSequence());
    CHECK_FALSE(sequencer.hasMoreSignalElements());
    CHECK_FALSE(sequencer.hasMorePulse());
}

TEST(SignalSequencerTests, ShouldHaveMoreSignalCodeToSequence_AfterLoad)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    CHECK_TRUE(sequencer.hasMoreSignalCodeToSequence());
}
TEST(SignalSequencerTests, ShouldNotHaveMoreSignalCodeToSequence_AfterClear)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    CHECK_TRUE(sequencer.hasMoreSignalCodeToSequence());
    sequencer.clear();
    CHECK_FALSE(sequencer.hasMoreSignalCodeToSequence());
}
TEST(SignalSequencerTests, ShouldNotHaveMoreSignalElements_AfterLoad)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    CHECK_FALSE(sequencer.hasMoreSignalElements());
}
TEST(SignalSequencerTests, ShouldHaveMoreSignalElements_AfterPopNextCodeToSequence)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    CHECK_TRUE(sequencer.hasMoreSignalElements());
}

TEST(SignalSequencerTests, ShouldNotHaveMorePulse_AfterLoad)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    CHECK_FALSE(sequencer.hasMorePulse());
}
TEST(SignalSequencerTests, ShouldNotHaveMorePulse_AfterClear)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    sequencer.clear();
    CHECK_FALSE(sequencer.hasMorePulse());
}

TEST(SignalSequencerTests, ShouldEncodeSingleDigitPositiveNumber)
{
    sequencer.loadSignalCode(CODE_2, NumberBase::DEC, 0);
    CHECK_TRUE(sequencer.hasMoreSignalCodeToSequence());
    sequencer.popNextCodeToSequence();
    CHECK_TRUE(sequencer.hasMoreSignalElements());
    check_next_signal_element(signal_element_digit_peak(2));
}

TEST(SignalSequencerTests, ShouldNotHaveMoreSignalElements_ReadLastOne)
{
    sequencer.loadSignalCode(CODE_2, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    CHECK_TRUE(sequencer.hasMoreSignalElements());
    check_next_signal_element(signal_element_digit_peak(2));
    CHECK_FALSE(sequencer.hasMoreSignalElements());
}

TEST(SignalSequencerTests, ShouldHaveMorePulse_AfterPopNextSignalElement)
{
    sequencer.loadSignalCode(CODE_2, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    CHECK_TRUE(sequencer.hasMoreSignalElements());
    CHECK_FALSE(sequencer.hasMorePulse());
    sequencer.popNextSignalElement();
    CHECK_TRUE(sequencer.hasMorePulse());
}

TEST(SignalSequencerTests, ShouldPulseFiveTimesForCode5)
{
    sequencer.loadSignalCode(CODE_5, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    sequencer.popNextSignalElement();
    testutils::Counter actual_pulse_count = 0;
    while (sequencer.hasMorePulse())
    {
        sequencer.popNextPulse();
        ++actual_pulse_count;
    }
    CHECK_EQUAL(to_value(CODE_5), actual_pulse_count);
}

TEST(SignalSequencerTests, ShouldEncodeZeroAsGap)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    check_next_signal_element(SIGNAL_ELEMENT_ZERO_GAP);
}
TEST(SignalSequencerTests, ShouldEncodeZeroWithMinDigits)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 3);
    sequencer.popNextCodeToSequence();
    check_next_signal_element(SIGNAL_ELEMENT_ZERO_GAP);
    check_next_signal_element(SIGNAL_ELEMENT_ZERO_GAP);
    check_next_signal_element(SIGNAL_ELEMENT_ZERO_GAP);
}

TEST(SignalSequencerTests, ShouldPulseFiveTimesOnTheLastDigitForCode12345)
{
    sequencer.loadSignalCode(CODE_12345, NumberBase::DEC, 0);
    sequencer.popNextCodeToSequence();
    testutils::Counter actual_pulse_count{0};
    while (sequencer.hasMoreSignalElements())
    {
        sequencer.popNextSignalElement();
        actual_pulse_count = 0;
        while (sequencer.hasMorePulse())
        {
            sequencer.popNextPulse();
            ++actual_pulse_count;
        }
    }
    CHECK_EQUAL(to_value(CODE_5), actual_pulse_count);
}

TEST(SignalSequencerTests, ShouldRepeatCode120Twice)
{
    sequencer.loadSignalCode(CODE_120, NumberBase::DEC, 0);
    const auto expected_element_count = 3;
    const auto expected_repeat_count  = 2;
    sequencer.setRepeatTimes(expected_repeat_count);
    testutils::Counter actual_repeat_count{0};
    testutils::Counter actual_element_count{0};
    while (sequencer.hasMoreSignalCodeToSequence())
    {
        sequencer.popNextCodeToSequence();
        actual_element_count = 0;
        while (sequencer.hasMoreSignalElements())
        {
            sequencer.popNextSignalElement();
            while (sequencer.hasMorePulse())
            {
                sequencer.popNextPulse();
            }
            ++actual_element_count;
        }
        ++actual_repeat_count;
    }
    CHECK_EQUAL(expected_repeat_count, actual_repeat_count);
    CHECK_EQUAL(expected_element_count, actual_element_count);
}
