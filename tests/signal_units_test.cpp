#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_units.hpp"
#include "tests_constants.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalUnitsGroup)
{
    void check_unit(const SignalSequence& seq, testutils::Index index, const SignalUnit& expected)
    {
        const auto& unit = seq.at(index);
        CHECK_EQUAL(to_value(expected.get_level()), to_value(unit.get_level()));
        CHECK_EQUAL(expected.times, unit.times);
        CHECK_EQUAL(to_value(expected.get_duration()), to_value(unit.get_duration()));
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
    CHECK_EQUAL(1, sequence.size());
    check_unit(sequence, 0, SignalUnit(SignalLevel::GAP, 1, SignalDuration::SHORT));
}

TEST(SignalUnitsGroup, ShouldEncodeSingleDigitPositiveNumber)
{
    sequence.generateFromCode(CODE_2, NumberBase::DEC, 0);
    CHECK_EQUAL(1, sequence.size());
    check_unit(sequence, 0, SignalUnit(SignalLevel::PEAK, 2, SignalDuration::SHORT));
}

TEST(SignalUnitsGroup, ShouldEncodeNegativeNumberWithLeadingPeak)
{
    sequence.generateFromCode(CODE_NEG_7, NumberBase::DEC, 0);
    CHECK_EQUAL(2, sequence.size());
    check_unit(sequence, 0, SignalUnit(SignalLevel::PEAK, 1, SignalDuration::LONG));
    check_unit(sequence, 1, SignalUnit(SignalLevel::PEAK, 7, SignalDuration::SHORT));
}

TEST(SignalUnitsGroup, ShouldRespectMinDigitsWithPadding)
{
    sequence.generateFromCode(CODE_120, NumberBase::DEC, 5);
    CHECK_EQUAL(5, sequence.size());
}

TEST(SignalUnitsGroup, ShouldEncodeMultiDigitNumber)
{
    sequence.generateFromCode(CODE_12345, NumberBase::DEC, 0);
    CHECK_EQUAL(5, sequence.size());
    CHECK_EQUAL(1, sequence.at(0).times); // Digit 1
    CHECK_EQUAL(2, sequence.at(1).times); // Digit 2
    CHECK_EQUAL(3, sequence.at(2).times);
    CHECK_EQUAL(4, sequence.at(3).times);
    CHECK_EQUAL(5, sequence.at(4).times);
}
