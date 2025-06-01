#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

TEST_GROUP(SignalSequencerTests)
{
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

// TEST(SignalUnitsGroup, ShouldEncodeZeroAsGap)
// {
//     sequence_stack.loadSignalCode(CODE_0, NumberBase::DEC, 0);
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
// }

// TEST(SignalUnitsGroup, ShouldEncodeSingleDigitPositiveNumber)
// {
//     sequence_stack.loadSignalCode(CODE_2, NumberBase::DEC, 0);
//     CHECK_EQUAL(1, sequence_stack.size());
//     check_next(signal_element_digit_peak(2));
// }

// TEST(SignalUnitsGroup, ShouldRespectHasNextSignalUnit)
// {
//     sequence_stack.loadSignalCode(CODE_2, NumberBase::DEC, 0);
//     CHECK_EQUAL(1, sequence_stack.size());
//     CHECK_TRUE(sequence_stack.hasNextSignalUnit());
//     check_next(signal_element_digit_peak(2));
//     CHECK_FALSE(sequence_stack.hasNextSignalUnit());
// }

// TEST(SignalUnitsGroup, ShouldEncodeNegativeNumberWithLeadingPeak)
// {
//     sequence_stack.loadSignalCode(CODE_NEG_7, NumberBase::DEC, 0);
//     CHECK_EQUAL(2, sequence_stack.size());
//     check_next(SIGNAL_ELEMENT_NEGATIVE_PEAK);
//     check_next(signal_element_digit_peak(7));
// }

// TEST(SignalUnitsGroup, ShouldRespectMinDigitsWithPadding)
// {
//     sequence_stack.loadSignalCode(CODE_120, NumberBase::DEC, 5);
//     CHECK_EQUAL(5, sequence_stack.size());
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
//     check_next(signal_element_digit_peak(1));
//     check_next(signal_element_digit_peak(2));
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
// }

// TEST(SignalUnitsGroup, ShouldEncodeMultiDigitNumber)
// {
//     sequence_stack.loadSignalCode(CODE_12345, NumberBase::DEC, 0);
//     CHECK_EQUAL(5, sequence_stack.size());
//     check_next(signal_element_digit_peak(1));
//     check_next(signal_element_digit_peak(2));
//     check_next(signal_element_digit_peak(3));
//     check_next(signal_element_digit_peak(4));
//     check_next(signal_element_digit_peak(5));
// }

// TEST(SignalUnitsGroup, ShouldRespectRewindCommand)
// {
//     sequence_stack.loadSignalCode(CODE_120, NumberBase::DEC, 0);
//     CHECK_EQUAL(3, sequence_stack.size());
//     check_next(signal_element_digit_peak(1));
//     check_next(signal_element_digit_peak(2));
//     sequence_stack.rewind();
//     check_next(signal_element_digit_peak(1));
//     check_next(signal_element_digit_peak(2));
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
// }

// TEST(SignalUnitsGroup, ShouldRespectEndOfStack)
// {
//     sequence_stack.loadSignalCode(CODE_120, NumberBase::DEC, 0);
//     CHECK_EQUAL(3, sequence_stack.size());
//     check_next(signal_element_digit_peak(1));
//     check_next(signal_element_digit_peak(2));
//     check_next(SIGNAL_ELEMENT_ZERO_GAP);
//     check_next(SIGNAL_ELEMENT_NOT_DEFINED);
//     check_next(SIGNAL_ELEMENT_NOT_DEFINED);
// }