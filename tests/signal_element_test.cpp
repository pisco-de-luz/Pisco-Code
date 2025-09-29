#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

TEST_GROUP(SignalElementTests){};

TEST(SignalElementTests, DefaultConstructor_InitializesToZero)
{
    const SignalElement element;
    CHECK_EQUAL(to_value(SignalMode::NOT_DEFINED),
                to_value(element.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::SHORT),
                to_value(element.get_duration())); // default raw = 0
    CHECK_EQUAL(0, element.get_times());
}

TEST(SignalElementTests, ParameterizedConstructor_StoresGivenValues)
{
    const SignalElement element(SignalMode::PEAK, 7, SignalDuration::MEDIUM);
    CHECK_EQUAL(to_value(SignalMode::PEAK), to_value(element.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::MEDIUM),
                to_value(element.get_duration()));
    CHECK_EQUAL(7, element.get_times());
}

TEST(SignalElementTests, SIGNAL_ELEMENT_NEGATIVE_PEAK_HasExpectedValues)
{
    CHECK_EQUAL(to_value(SignalMode::PEAK),
                to_value(SIGNAL_ELEMENT_NEGATIVE.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::LONG),
                to_value(SIGNAL_ELEMENT_NEGATIVE.get_duration()));
    CHECK_EQUAL(1, SIGNAL_ELEMENT_NEGATIVE.get_times());
}

TEST(SignalElementTests, SIGNAL_ELEMENT_ZERO_GAP_HasExpectedValues)
{
    CHECK_EQUAL(to_value(SignalMode::GAP),
                to_value(SIGNAL_ELEMENT_ZERO.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::SHORT),
                to_value(SIGNAL_ELEMENT_ZERO.get_duration()));
    CHECK_EQUAL(1, SIGNAL_ELEMENT_ZERO.get_times());
}

TEST(SignalElementTests, SIGNAL_ELEMENT_NOT_DEFINED_HasExpectedValues)
{
    CHECK_EQUAL(to_value(SignalMode::NOT_DEFINED),
                to_value(SIGNAL_ELEMENT_NOT_DEFINED.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::SHORT),
                to_value(SIGNAL_ELEMENT_NOT_DEFINED.get_duration()));
    CHECK_EQUAL(0, SIGNAL_ELEMENT_NOT_DEFINED.get_times());
}

TEST(SignalElementTests, signal_element_digit_peak_GeneratesCorrectElement)
{
    for (Counter val = 1; val < to_count(to_value(NumberBase::HEX)); ++val)
    {
        auto element = signal_element_digit(val);
        CHECK_EQUAL(to_value(SignalMode::PEAK), to_value(element.get_level()));
        CHECK_EQUAL(to_value(SignalDuration::SHORT),
                    to_value(element.get_duration()));
        CHECK_EQUAL(val, to_value(element.get_times()));
    }
}
