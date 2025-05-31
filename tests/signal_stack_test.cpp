#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"
#include "tests_constants.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalStackTests)
{
    void check_next(const SignalElement& expected)
    {
        // CHECK_TRUE(sequence_stack.hasNextSignalUnit());
        const auto actual = sequence_stack.pop();
        CHECK_EQUAL(to_value(expected.get_level()), to_value(actual.get_level()));
        CHECK_EQUAL(to_value(expected.get_times()), to_value(actual.get_times()));
        CHECK_EQUAL(to_value(expected.get_duration()), to_value(actual.get_duration()));
    }

    SignalStack sequence_stack;

    void teardown() override
    {
        sequence_stack.clear();
    }
};
