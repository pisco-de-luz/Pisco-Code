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
    // void check_next(const SignalElement& expected)
    // {
    //     const auto actual = sequence_stack.pop();
    //     CHECK_EQUAL(to_value(expected.get_level()),
    //                 to_value(actual.get_level()));
    //     CHECK_EQUAL(to_value(expected.get_times()),
    //                 to_value(actual.get_times()));
    //     CHECK_EQUAL(to_value(expected.get_duration()),
    //                 to_value(actual.get_duration()));
    // }

    SignalStack signal_stack;

    void teardown() override
    {
        signal_stack.clear();
    }
};

TEST(SignalPulseIteratorTests, SignalStackEmpty_IteratesCorrectly)
{
    signal_stack.clear();
    SignalPulseIterator iterator(signal_stack);

    CHECK_TRUE(iterator.hasNext());
    const auto expected_framing      = SIGNAL_ELEMENT_FRAMING;
    const auto expected_inter_symbol = SIGNAL_ELEMENT_INTER_SYMBOL;

    auto actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_inter_symbol, actual);
    actual = iterator.next();
    CHECK_EQUAL(expected_framing, actual);
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