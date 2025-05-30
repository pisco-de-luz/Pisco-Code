#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_units.hpp"
#include "tests_constants.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalUnitsGroup)
{
    void check_next(const SignalUnit& expected)
    {
        CHECK_TRUE(sequence.hasNextSignalUnit());
        const auto actual = sequence.popNextSignalUnit();
        CHECK_EQUAL(to_value(expected.get_level()), to_value(actual.get_level()));
        CHECK_EQUAL(expected.times, actual.times);
        CHECK_EQUAL(to_value(expected.get_duration()), to_value(actual.get_duration()));
    }

    SignalSequence sequence;

    void teardown() override
    {
        sequence.clear();
    }
};

TEST(SignalUnitsGroup, ShouldEncodeZeroAsGap)
{
    sequence.generateFromCode(CODE_0, NumberBase::DEC, 0);
    check_next(SIGNAL_UNIT_ZERO_GAP);
}

TEST(SignalUnitsGroup, ShouldEncodeSingleDigitPositiveNumber)
{
    sequence.generateFromCode(CODE_2, NumberBase::DEC, 0);
    CHECK_EQUAL(1, sequence.size());
    check_next(signal_unit_digit_peak(2));
}

TEST(SignalUnitsGroup, ShouldEncodeNegativeNumberWithLeadingPeak)
{
    sequence.generateFromCode(CODE_NEG_7, NumberBase::DEC, 0);
    CHECK_EQUAL(2, sequence.size());
    check_next(SIGNAL_UNIT_NEGATIVE_PEAK);
    check_next(signal_unit_digit_peak(7));
}

TEST(SignalUnitsGroup, ShouldRespectMinDigitsWithPadding)
{
    sequence.generateFromCode(CODE_120, NumberBase::DEC, 5);
    CHECK_EQUAL(5, sequence.size());
    check_next(SIGNAL_UNIT_ZERO_GAP);
    check_next(SIGNAL_UNIT_ZERO_GAP);
    check_next(signal_unit_digit_peak(1));
    check_next(signal_unit_digit_peak(2));
    check_next(SIGNAL_UNIT_ZERO_GAP);
}

TEST(SignalUnitsGroup, ShouldEncodeMultiDigitNumber)
{
    sequence.generateFromCode(CODE_12345, NumberBase::DEC, 0);
    CHECK_EQUAL(5, sequence.size());
    check_next(signal_unit_digit_peak(1));
    check_next(signal_unit_digit_peak(2));
    check_next(signal_unit_digit_peak(3));
    check_next(signal_unit_digit_peak(4));
    check_next(signal_unit_digit_peak(5));
}
