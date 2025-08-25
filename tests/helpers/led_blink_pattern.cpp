#include "led_blink_pattern.hpp"

#include <algorithm>
#include <iterator>

#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

void
LedBlinkPattern::reset()
{
    pattern_is_valid_ = true;
    dimmed_level_     = 0;
    pulse_level_      = 0;
    led_events_.clear();
    used_levels_.clear();
}

void
LedBlinkPattern::append(LedLevel level, DurationMs duration)
{
    led_events_.push_back({level, duration});

    if (level == 0 || !pattern_is_valid_)
    {
        return;
    }

    // Insert and check if the set grew
    const auto [_, inserted] = used_levels_.insert(level);

    if (!inserted)
    {
        return;
    }

    if (used_levels_.size() > 2)
    {
        pattern_is_valid_ = false;
        return;
    }

    if (used_levels_.size() == 1)
    {
        dimmed_level_ = *used_levels_.begin();
        pulse_level_  = 0;
    }

    // Only calculate levels when we reach exactly 2 unique non-zero levels
    if (used_levels_.size() == 2)
    {
        auto dimmed_first_it = used_levels_.begin();
        dimmed_level_        = *dimmed_first_it++;
        pulse_level_         = *dimmed_first_it;
    }
}

testutils::TraceCode
LedBlinkPattern::tracePatternToString() const
{
    using namespace testutils;

    if (!pattern_is_valid_)
    {
        return "!";
    }

    testutils::TraceCode result{};

    for (const auto& event : led_events_)
    {
        const testutils::TraceCode* symbol_ptr = nullptr;

        if (event.led_level == 0)
        {
            symbol_ptr = &testutils::LED_OFF_CHARACTER;
        }
        else if (event.led_level == dimmed_level_)
        {
            symbol_ptr = &testutils::LED_DIMMED_CHARACTER;
        }
        else if (event.led_level == pulse_level_)
        {
            symbol_ptr = &testutils::LED_PULSE_CHARACTER;
        }
        else
        {
            return "Unexpected level — should never happen.";
        }

        const auto count = getRepeatCount(event.duration);

        for (RepeatTimes i = 0; i < count; ++i)
        {
            result += *symbol_ptr;
        }
    }

    return result;
}

bool
LedBlinkPattern::isValid() const
{
    return pattern_is_valid_ && used_levels_.size() == 2;
}

RepeatTimes
LedBlinkPattern::getRepeatCount(DurationMs duration)
{
    const auto* const upper_bound_rule = std::upper_bound(
        TRACE_REPEAT_RULES.begin(), TRACE_REPEAT_RULES.end(), duration,
        [](DurationMs value, const TraceRepeatRule& rule)
        { return value < rule.min_duration; });

    if (upper_bound_rule == TRACE_REPEAT_RULES.begin())
    {
        return TRACE_REPEAT_RULES.front().repeat_count;
    }

    return std::prev(upper_bound_rule)->repeat_count;
}