#include "CppUTest/TestHarness.h"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using namespace testutils;

TEST_GROUP(SignalStackTests)
{
    void check_next(const SignalElement& expected)
    {
        const auto actual = sequence_stack.pop();
        CHECK_EQUAL(to_value(expected.get_mode()), to_value(actual.get_mode()));
        CHECK_EQUAL(to_value(expected.get_times()),
                    to_value(actual.get_times()));
        CHECK_EQUAL(to_value(expected.get_duration()),
                    to_value(actual.get_duration()));
    }

    SignalStack sequence_stack;

    void teardown() override
    {
        sequence_stack.clear();
    }
};

TEST(SignalStackTests, DefaultConstructor_InitializesToZero)
{
    CHECK_EQUAL(0, sequence_stack.size());
    CHECK_TRUE(sequence_stack.isEmpty());
    CHECK_FALSE(sequence_stack.isFull());
}
TEST(SignalStackTests, PushAndPop_SingleElement)
{
    const SignalElement element(SignalMode::PEAK, 1, SignalDuration::SHORT);
    sequence_stack.push(element);

    CHECK_EQUAL(1, sequence_stack.size());
    CHECK_FALSE(sequence_stack.isEmpty());
    CHECK_FALSE(sequence_stack.isFull());

    check_next(element);
}
TEST(SignalStackTests, PushAndPop_MultipleElements)
{
    const SignalElement element1(SignalMode::PEAK, 1, SignalDuration::SHORT);
    const SignalElement element2(SignalMode::GAP, 2, SignalDuration::MEDIUM);

    sequence_stack.push(element1);
    sequence_stack.push(element2);

    CHECK_EQUAL(2, sequence_stack.size());
    CHECK_FALSE(sequence_stack.isEmpty());
    CHECK_FALSE(sequence_stack.isFull());

    check_next(element2);
    check_next(element1);
}
TEST(SignalStackTests, Pop_EmptyStack_ReturnsNotDefined)
{
    const auto element = sequence_stack.pop();
    CHECK_EQUAL(to_value(SignalMode::NOT_DEFINED),
                to_value(element.get_mode()));
    CHECK_EQUAL(0, element.get_times());
    CHECK_EQUAL(to_value(SignalDuration::SHORT),
                to_value(element.get_duration()));
}
TEST(SignalStackTests, IsEmpty_AfterClear)
{
    const SignalElement element(SignalMode::PEAK, 1, SignalDuration::SHORT);
    sequence_stack.push(element);

    CHECK_FALSE(sequence_stack.isEmpty());

    sequence_stack.clear();

    CHECK_TRUE(sequence_stack.isEmpty());
    CHECK_EQUAL(0, sequence_stack.size());
}
TEST(SignalStackTests, IsFull_AfterMaxPushes)
{
    for (testutils::Counter i = 0; i < MAX_SIGNAL_ELEMENTS; ++i)
    {
        sequence_stack.push(
            SignalElement(SignalMode::PEAK, 1, SignalDuration::SHORT));
    }

    CHECK_TRUE(sequence_stack.isFull());
    CHECK_EQUAL(MAX_SIGNAL_ELEMENTS, sequence_stack.size());

    // Attempting to push more should not change the size
    sequence_stack.push(
        SignalElement(SignalMode::GAP, 2, SignalDuration::MEDIUM));
    CHECK_EQUAL(MAX_SIGNAL_ELEMENTS, sequence_stack.size());
}
TEST(SignalStackTests, Rewind_ResetsReadIndex)
{
    const SignalElement element1(SignalMode::PEAK, 1, SignalDuration::SHORT);
    const SignalElement element2(SignalMode::GAP, 2, SignalDuration::MEDIUM);

    sequence_stack.push(element1);
    sequence_stack.push(element2);

    CHECK_EQUAL(2, sequence_stack.size());

    check_next(element2);
    check_next(element1);

    sequence_stack.rewind();

    // Should read from the start again
    check_next(element2);
}
TEST(SignalStackTests, Clear_EmptiesStack)
{
    const SignalElement element1(SignalMode::PEAK, 1, SignalDuration::SHORT);
    const SignalElement element2(SignalMode::GAP, 2, SignalDuration::MEDIUM);

    sequence_stack.push(element1);
    sequence_stack.push(element2);

    CHECK_EQUAL(2, sequence_stack.size());

    sequence_stack.clear();

    CHECK_TRUE(sequence_stack.isEmpty());
    CHECK_EQUAL(0, sequence_stack.size());
}
TEST(SignalStackTests, PushBeyondMaxSize_DoesNotIncreaseSize)
{
    for (testutils::Counter i = 0; i < MAX_SIGNAL_ELEMENTS + 5; ++i)
    {
        sequence_stack.push(
            SignalElement(SignalMode::PEAK, 1, SignalDuration::SHORT));
    }

    CHECK_TRUE(sequence_stack.isFull());
    CHECK_EQUAL(MAX_SIGNAL_ELEMENTS, sequence_stack.size());
}
TEST(SignalStackTests, PopUntilEmpty)
{
    const SignalElement element1(SignalMode::PEAK, 1, SignalDuration::SHORT);
    const SignalElement element2(SignalMode::GAP, 2, SignalDuration::MEDIUM);

    sequence_stack.push(element1);
    sequence_stack.push(element2);

    CHECK_EQUAL(2, sequence_stack.size());

    check_next(element2);
    check_next(element1);

    CHECK_TRUE(sequence_stack.isEmpty());
}