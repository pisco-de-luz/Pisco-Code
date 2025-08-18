#include "CppUTest/TestHarness.h"

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

TEST(SignalPulseIteratorTests, NumberOne_IteratesCorrectly)
{
    const SignalElement element = signal_element_digit(1);
    signal_stack.push(element);
    SignalPulseIterator iterator(signal_stack);

    CHECK_TRUE(iterator.hasNext());
    const auto expected_gap = SIGNAL_ELEMENT_FRAMING;
    const auto actual       = iterator.next();
    CHECK_EQUAL(to_value(expected_gap.get_level()),
                to_value(actual.get_level()));
}