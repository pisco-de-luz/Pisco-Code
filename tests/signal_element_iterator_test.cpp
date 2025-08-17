#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_element_iterator.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

TEST_GROUP(SignalElementIteratorTests){};

TEST(SignalElementIteratorTests, SIGNAL_ELEMENT_ZERO_GAP_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_ZERO_GAP;
    SignalElementIterator iterator(expected);
    CHECK_TRUE(iterator.hasNext());
    const auto actual = iterator.next();
    CHECK_EQUAL(to_value(expected.get_level()), to_value(actual.get_level()));
    CHECK_EQUAL(to_value(expected.get_duration()),
                to_value(actual.get_duration()));
    CHECK_EQUAL(to_value(expected.get_times()), to_value(actual.get_times()));
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, SIGNAL_ELEMENT_NEGATIVE_PEAK_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_NEGATIVE_PEAK;
    SignalElementIterator iterator(expected);
    CHECK_TRUE(iterator.hasNext());
    const auto actual = iterator.next();
    CHECK_EQUAL(to_value(expected.get_level()), to_value(actual.get_level()));
    CHECK_EQUAL(to_value(expected.get_duration()),
                to_value(actual.get_duration()));
    CHECK_EQUAL(to_value(expected.get_times()), to_value(actual.get_times()));
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, SIGNAL_ELEMENT_NUMBER_FIVE_IteratesCorrectly)
{
    const SignalTimesType digit_value = 5;
    const auto digit_element          = signal_element_digit_peak(digit_value);
    const auto expected_peak          = SIGNAL_ELEMENT_DIGIT_PEAK;
    const auto expected_between       = SIGNAL_ELEMENT_BETWEEN_PEAK;

    SignalElementIterator iterator(digit_element);
    SignalTimesType       peak_pulse_count = 0;
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(to_value(expected_peak.get_level()),
                    to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected_peak.get_duration()),
                    to_value(actual.get_duration()));
        CHECK_EQUAL(to_value(expected_peak.get_times()),
                    to_value(actual.get_times()));
        ++peak_pulse_count;
        if (iterator.hasNext())
        {
            const auto between = iterator.next();
            CHECK_EQUAL(to_value(expected_between.get_level()),
                        to_value(between.get_level()));
            CHECK_EQUAL(to_value(expected_between.get_duration()),
                        to_value(between.get_duration()));
            CHECK_EQUAL(to_value(expected_between.get_times()),
                        to_value(between.get_times()));
        }
    }
    CHECK_EQUAL(digit_value, peak_pulse_count);
}
