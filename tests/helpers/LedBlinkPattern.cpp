#include "LedBlinkPattern.hpp"
// #include "BlinkerTestUtils.hpp"
#include "pisco_types.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <set>
#include <string>
#include <vector>

void LedBlinkPattern::reset()
{
    pattern_is_valid_ = true;
    dimmed_level_     = 0;
    pulse_level_      = 0;
    led_events_.clear();
    used_levels_.clear();
}

void LedBlinkPattern::append(pisco::LedLevel level, pisco::DurationMs duration)
{
    led_events_.push_back({level, duration});

    if (level == 0 || !pattern_is_valid_)
        return;

    // Insert and check if the set grew
    const auto [_, inserted] = used_levels_.insert(level);

    // If insertion failed or already invalid, do nothing
    if (!inserted)
    {
        return;
    }

    if (used_levels_.size() > 2)
    {
        pattern_is_valid_ = false;
        return;
    }

    // Handle single non-zero level
    if (used_levels_.size() == 1)
    {
        dimmed_level_ = *used_levels_.begin();
        pulse_level_  = 0; // no pulse used
    }

    // Only calculate levels when we reach exactly 2 unique non-zero levels
    if (used_levels_.size() == 2)
    {
        auto dimmed_first_it = used_levels_.begin();
        dimmed_level_        = *dimmed_first_it++;
        pulse_level_         = *dimmed_first_it;
    }
}

std::string LedBlinkPattern::tracePatternToString() const
{
    if (!pattern_is_valid_)
    {
        return "!";
    }

    std::string result;

    for (const auto& event : led_events_)
    {
        char symbol = '?';

        if (event.led_level == 0)
        {
            symbol = '_';
        }
        else if (event.led_level == dimmed_level_)
        {
            symbol = '-';
        }
        else if (event.led_level == pulse_level_)
        {
            symbol = '^';
        }
        else
        {
            // Unexpected level not in known 3 levels
            return "!";
        }

        const uint8_t count = getRepeatCount(event.duration);

        result.append(count, symbol);
    }

    return result;
}

bool LedBlinkPattern::isValid() const
{
    return pattern_is_valid_ && used_levels_.size() == 2;
}

pisco::RepeatTimes LedBlinkPattern::getRepeatCount(pisco::DurationMs duration) const
{
    auto upper_bound_rule =
        std::upper_bound(traceRepeatRules.begin(), traceRepeatRules.end(), duration,
                         [](pisco::DurationMs value, const TraceRepeatRule& rule)
                         { return value < rule.min_duration; });

    if (upper_bound_rule == traceRepeatRules.begin())
        return traceRepeatRules.front().repeat_count;

    return std::prev(upper_bound_rule)->repeat_count;
}
