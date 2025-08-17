
#include "helpers/signal_element_iterator_test.hpp"

#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_element_iterator.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

TEST_GROUP(SignalElementIteratorTests){};

TEST(SignalElementIteratorTests, ZERO_GAP_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_ZERO_GAP;
    SignalElementIterator iterator(expected);
    CHECK_TRUE(iterator.hasNext());
    const auto actual = iterator.next();
    CHECK_EQUAL(expected, actual);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NEGATIVE_PEAK_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_NEGATIVE_PEAK;
    SignalElementIterator iterator(expected);
    CHECK_TRUE(iterator.hasNext());
    const auto actual = iterator.next();
    CHECK_EQUAL(expected, actual);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NUMBER_One_IteratesCorrectly)
{
    const SignalTimesType digit_value = 1;
    const auto digit_element          = signal_element_digit_peak(digit_value);
    const auto expected_peak          = SIGNAL_ELEMENT_DIGIT_PEAK;

    SignalElementIterator iterator(digit_element);

    CHECK_TRUE(iterator.hasNext());
    if (iterator.hasNext())
    {
        const auto between = iterator.next();
        CHECK_EQUAL(expected_peak, between);
    }
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NUMBER_Two_IteratesCorrectly)
{
    const SignalTimesType digit_value = 2;
    const auto digit_element          = signal_element_digit_peak(digit_value);
    const auto expected_peak          = SIGNAL_ELEMENT_DIGIT_PEAK;
    const auto expected_between       = SIGNAL_ELEMENT_BETWEEN_PEAK;

    SignalElementIterator iterator(digit_element);
    SignalTimesType       peak_pulse_count = 0;
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(expected_peak, actual);
        ++peak_pulse_count;
        if (iterator.hasNext())
        {
            const auto between = iterator.next();
            CHECK_EQUAL(expected_between, between);
        }
    }
    CHECK_EQUAL(digit_value, peak_pulse_count);
}

TEST(SignalElementIteratorTests, NUMBER_Fifteen_IteratesCorrectly)
{
    const SignalTimesType digit_value = 15;
    const auto digit_element          = signal_element_digit_peak(digit_value);
    const auto expected_peak          = SIGNAL_ELEMENT_DIGIT_PEAK;
    const auto expected_between       = SIGNAL_ELEMENT_BETWEEN_PEAK;

    SignalElementIterator iterator(digit_element);
    SignalTimesType       peak_pulse_count = 0;
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(expected_peak, actual);
        ++peak_pulse_count;
        if (iterator.hasNext())
        {
            const auto between = iterator.next();
            CHECK_EQUAL(expected_between, between);
        }
    }
    CHECK_EQUAL(digit_value, peak_pulse_count);
}

TEST(SignalElementIteratorTests,
     GAP_SHORT_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times    = 3;
    const SignalElement   expected = {SignalLevel::GAP, times,
                                      SignalDuration::SHORT};
    SignalElementIterator iterator(expected);
    SignalTimesType       peak_pulse_count = 0;
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(to_value(expected.get_level()),
                    to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected.get_duration()),
                    to_value(actual.get_duration()));
        CHECK_EQUAL(Byte{1}, to_value(actual.get_times()));
        ++peak_pulse_count;
    }
    CHECK_EQUAL(times, peak_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests,
     PEAK_LONG_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times    = 2;
    const SignalElement   expected = {SignalLevel::PEAK, times,
                                      SignalDuration::LONG};
    SignalElementIterator iterator(expected);
    SignalTimesType       peak_pulse_count = 0;
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(to_value(expected.get_level()),
                    to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected.get_duration()),
                    to_value(actual.get_duration()));
        CHECK_EQUAL(Byte{1}, to_value(actual.get_times()));
        ++peak_pulse_count;
    }
    CHECK_EQUAL(times, peak_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests,
     PEAK_EXTRA_LONG_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times    = 3;
    const SignalElement   expected = {SignalLevel::PEAK, times,
                                      SignalDuration::EXTRA_LONG};
    SignalElementIterator iterator(expected);
    SignalTimesType       peak_pulse_count = 0;
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        CHECK_EQUAL(to_value(expected.get_level()),
                    to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected.get_duration()),
                    to_value(actual.get_duration()));
        CHECK_EQUAL(Byte{1}, to_value(actual.get_times()));
        ++peak_pulse_count;
    }
    CHECK_EQUAL(times, peak_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, TimesEqualToZero_MustNotEmitPulse)
{
    const SignalTimesType times    = 0;
    const SignalElement   expected = {SignalLevel::PEAK, times,
                                      SignalDuration::EXTRA_LONG};
    SignalElementIterator iterator(expected);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, ResetMustIterateAgain)
{
    const SignalTimesType digit_value = 3;
    Byte                  reset_count = 2;
    const auto digit_element          = signal_element_digit_peak(digit_value);
    const auto expected_peak          = SIGNAL_ELEMENT_DIGIT_PEAK;
    const auto expected_between       = SIGNAL_ELEMENT_BETWEEN_PEAK;

    SignalElementIterator iterator(digit_element);
    while (reset_count > 0)
    {
        SignalTimesType peak_pulse_count = 0;
        iterator.reset();
        CHECK_TRUE(iterator.hasNext());
        while (iterator.hasNext())
        {
            const auto actual = iterator.next();
            CHECK_EQUAL(expected_peak, actual);
            ++peak_pulse_count;
            if (iterator.hasNext())
            {
                const auto between = iterator.next();
                CHECK_EQUAL(expected_between, between);
            }
        }
        CHECK_FALSE(iterator.hasNext());
        --reset_count;
        CHECK_EQUAL(digit_value, peak_pulse_count);
    }
}