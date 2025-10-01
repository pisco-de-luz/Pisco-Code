
#include "CppUTest/TestHarness.h"

#include "helpers/signal_element_test.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_element_iterator.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

namespace
{
    void checkNextElementIterator(SignalElementIterator& iterator,
                                  SignalElement          expected) noexcept
    {
        CHECK_TRUE(iterator.hasNext());
        const auto actual = iterator.next();
        CHECK_EQUAL(expected, actual);
    }

    auto checkElementIterator(SignalElement          expected_high_level,
                              SignalElementIterator& iterator,
                              SignalElement          expected_between) noexcept
    {
        SignalTimesType actual_high_level_count{0};
        while (iterator.hasNext())
        {
            checkNextElementIterator(iterator, expected_high_level);
            ++actual_high_level_count;

            if (iterator.hasNext())
            {
                checkNextElementIterator(iterator, expected_between);
            }
        }
        return (actual_high_level_count);
    }
    void checkEqualLevelDuration(SignalElement expected,
                                 SignalElement actual) noexcept
    {
        CHECK_EQUAL(to_value(expected.get_mode()), to_value(actual.get_mode()));
        CHECK_EQUAL(to_value(expected.get_duration()),
                    to_value(actual.get_duration()));
    }
} // namespace

TEST_GROUP(SignalElementIteratorTests){};

TEST(SignalElementIteratorTests, ZERO_GAP_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_ZERO;
    SignalElementIterator iterator(expected);
    checkNextElementIterator(iterator, expected);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NEGATIVE_PEAK_IteratesCorrectly)
{
    const auto            expected = SIGNAL_ELEMENT_NEGATIVE;
    SignalElementIterator iterator(expected);
    checkNextElementIterator(iterator, expected);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NUMBER_One_IteratesCorrectly)
{
    const SignalTimesType digit_value{1};
    const auto            digit_element = signal_element_digit(digit_value);
    const auto            expected_high_level = SIGNAL_ELEMENT_DIGIT;

    SignalElementIterator iterator(digit_element);

    if (iterator.hasNext())
    {
        checkNextElementIterator(iterator, expected_high_level);
    }
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, NUMBER_Two_IteratesCorrectly)
{
    const SignalTimesType digit_value{2};
    const auto            digit_element = signal_element_digit(digit_value);
    const auto            expected_high_level = SIGNAL_ELEMENT_DIGIT;
    const auto            expected_between    = SIGNAL_ELEMENT_INTRA_DIGIT;

    SignalElementIterator iterator(digit_element);
    const SignalTimesType high_level_pulse_count =
        checkElementIterator(expected_high_level, iterator, expected_between);
    CHECK_EQUAL(digit_value, high_level_pulse_count);
}

TEST(SignalElementIteratorTests, NUMBER_Fifteen_IteratesCorrectly)
{
    const SignalTimesType digit_value{15};
    const auto            digit_element = signal_element_digit(digit_value);
    const auto            expected_high_level = SIGNAL_ELEMENT_DIGIT;
    const auto            expected_between    = SIGNAL_ELEMENT_INTRA_DIGIT;

    SignalElementIterator iterator(digit_element);
    const SignalTimesType high_level_pulse_count =
        checkElementIterator(expected_high_level, iterator, expected_between);
    CHECK_EQUAL(digit_value, high_level_pulse_count);
}

TEST(SignalElementIteratorTests,
     GAP_SHORT_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times{3};
    const SignalElement   expected = {SignalMode::GAP, times,
                                      SignalDuration::SHORT};
    SignalElementIterator iterator(expected);
    SignalTimesType       high_level_pulse_count{0};
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        checkEqualLevelDuration(expected, actual);
        ++high_level_pulse_count;
    }
    CHECK_EQUAL(times, high_level_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests,
     PEAK_LONG_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times{2};
    const SignalElement   expected = {SignalMode::PEAK, times,
                                      SignalDuration::LONG};
    SignalElementIterator iterator(expected);
    SignalTimesType       high_level_pulse_count{0};
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        checkEqualLevelDuration(expected, actual);
        ++high_level_pulse_count;
    }
    CHECK_EQUAL(times, high_level_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests,
     PEAK_EXTRA_LONG_Times_greaterThanOne_MustNotEmitBetween)
{
    const SignalTimesType times{3};
    const SignalElement   expected = {SignalMode::PEAK, times,
                                      SignalDuration::EXTRA_LONG};
    SignalElementIterator iterator(expected);
    SignalTimesType       high_level_pulse_count{0};
    CHECK_TRUE(iterator.hasNext());
    while (iterator.hasNext())
    {
        const auto actual = iterator.next();
        checkEqualLevelDuration(expected, actual);
        ++high_level_pulse_count;
    }
    CHECK_EQUAL(times, high_level_pulse_count);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, TimesEqualToZero_MustNotEmitPulse)
{
    const SignalTimesType       times{0};
    const SignalElement         expected = {SignalMode::PEAK, times,
                                            SignalDuration::EXTRA_LONG};
    const SignalElementIterator iterator(expected);
    CHECK_FALSE(iterator.hasNext());
}

TEST(SignalElementIteratorTests, ResetMustIterateAgain)
{
    const SignalTimesType digit_value{3};
    Byte                  reset_count{2};
    const auto            digit_element = signal_element_digit(digit_value);
    const auto            expected_high_level = SIGNAL_ELEMENT_DIGIT;
    const auto            expected_between    = SIGNAL_ELEMENT_INTRA_DIGIT;

    SignalElementIterator iterator(digit_element);
    while (reset_count > 0)
    {
        iterator.reset();
        const SignalTimesType high_level_pulse_count = checkElementIterator(
            expected_high_level, iterator, expected_between);
        CHECK_FALSE(iterator.hasNext());
        CHECK_EQUAL(digit_value, high_level_pulse_count);
        --reset_count;
    }
}