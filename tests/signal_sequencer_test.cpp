#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

TEST_GROUP(SignalSequencerTests){};

TEST(SignalSequencerTests, DefaultConstructor_InitializesToZero)
{
    const SignalElement element;
    CHECK_EQUAL(to_value(SignalLevel::NOT_DEFINED), to_value(element.get_level()));
    CHECK_EQUAL(to_value(SignalDuration::SHORT),
                to_value(element.get_duration())); // default raw = 0
    CHECK_EQUAL(0, element.get_times());
}
