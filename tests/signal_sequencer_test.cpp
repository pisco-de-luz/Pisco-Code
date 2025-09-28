#include "CppUTest/TestHarness.h"

#include "helpers/signal_element_test.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalSequencerTests)
{
    SignalSequencer sequencer;

    void teardown() override
    {
        sequencer.clear();
    }
};

TEST(SignalSequencerTests, DefaultConstructor_InitializesCorrectly)
{
    CHECK_FALSE(sequencer.shouldRepeat());
    const SignalPulseIterator iterator = sequencer.createPulseIterator();
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalSequencerTests, ShouldHaveNextIterator_AfterLoad)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    const SignalPulseIterator iterator = sequencer.createPulseIterator();
    CHECK_TRUE(iterator.hasNext());
}

TEST(SignalSequencerTests, ShouldNotHaveNextIterator_AfterLoadAndClear)
{
    sequencer.loadSignalCode(CODE_0, NumberBase::DEC, 0);
    sequencer.clear();
    const SignalPulseIterator iterator = sequencer.createPulseIterator();
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalSequencerTests, CodeMinus103_IteratesCorrectly)
{
    sequencer.loadSignalCode(CODE_NEG_103, NumberBase::DEC, 0);

    SignalPulseIterator iterator = sequencer.createPulseIterator();

    const auto expected_framing      = SIGNAL_ELEMENT_FRAMING;
    const auto expected_inter_symbol = SIGNAL_ELEMENT_INTER_SYMBOL;
    const auto expected_negative     = SIGNAL_ELEMENT_NEGATIVE;
    const auto expected_zero         = SIGNAL_ELEMENT_ZERO;
    const auto expected_digit        = SIGNAL_ELEMENT_DIGIT;
    const auto expected_intra_digit  = SIGNAL_ELEMENT_INTRA_DIGIT;

    CHECK_TRUE(iterator.hasNext());
    auto actual = iterator.next(); // leading framing
    CHECK_EQUAL(expected_framing, actual);

    actual = iterator.next(); // inter before negative
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next(); // negative sign
    CHECK_EQUAL(expected_negative, actual);

    actual = iterator.next(); // inter before '1'
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next(); // digit '1'
    CHECK_EQUAL(expected_digit, actual);

    actual = iterator.next(); // inter before '0'
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next(); // zero digit
    CHECK_EQUAL(expected_zero, actual);

    actual = iterator.next(); // inter before '3'
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next(); // first pulse of '3'
    CHECK_EQUAL(expected_digit, actual);
    actual = iterator.next(); // intra digit gap
    CHECK_EQUAL(expected_intra_digit, actual);
    actual = iterator.next(); // second pulse of '3'
    CHECK_EQUAL(expected_digit, actual);
    actual = iterator.next(); // intra digit gap
    CHECK_EQUAL(expected_intra_digit, actual);
    actual = iterator.next(); // third pulse of '3'
    CHECK_EQUAL(expected_digit, actual);

    actual = iterator.next(); // trailing inter and framing
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);

    CHECK_FALSE(iterator.hasNext());
}
