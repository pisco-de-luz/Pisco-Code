#include "CppUTest/TestHarness.h"

#include "helpers/signal_element_test.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"
#include <type_traits>

using namespace pisco_code;

static_assert(std::is_trivially_copyable<SignalStack>::value,
              "Copy must stay cheap and predictable");

TEST_GROUP(SignalPulseIteratorTests)
{
    SignalStack signal_stack;

    void teardown() override
    {
        signal_stack.clear();
    }
};

TEST(SignalPulseIteratorTests, SignalStackEmpty_IteratesCorrectly)
{
    signal_stack.clear();
    const SignalPulseIterator iterator(signal_stack);

    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalPulseIteratorTests, NumberOne_IteratesCorrectly)
{
    const SignalElement element = signal_element_digit(1);
    signal_stack.push(element);
    SignalPulseIterator iterator(signal_stack);

    const auto expected_framing      = SIGNAL_ELEMENT_FRAMING;
    const auto expected_inter_symbol = SIGNAL_ELEMENT_INTER_SYMBOL;
    const auto expected_digit        = SIGNAL_ELEMENT_DIGIT;

    CHECK_TRUE(iterator.hasNext());
    auto actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_digit, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalPulseIteratorTests, NumberTwo_IteratesCorrectly)
{
    const SignalElement element = signal_element_digit(2);
    signal_stack.push(element);
    SignalPulseIterator iterator(signal_stack);

    const auto expected_framing      = SIGNAL_ELEMENT_FRAMING;
    const auto expected_inter_symbol = SIGNAL_ELEMENT_INTER_SYMBOL;
    const auto expected_digit        = SIGNAL_ELEMENT_DIGIT;
    const auto expected_intra_digit  = SIGNAL_ELEMENT_INTRA_DIGIT;

    CHECK_TRUE(iterator.hasNext());
    auto actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_digit, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_intra_digit, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_digit, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalPulseIteratorTests, CodeMinus103_IteratesCorrectly)
{
    signal_stack.clear();
    signal_stack.push(signal_element_digit(3)); // 3
    signal_stack.push(SIGNAL_ELEMENT_ZERO);     // 0
    signal_stack.push(signal_element_digit(1)); // 1
    signal_stack.push(SIGNAL_ELEMENT_NEGATIVE); // -

    SignalPulseIterator iterator(signal_stack);

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