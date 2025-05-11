#include "LedBlinkPattern.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
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

void LedBlinkPattern::append(LedLevel level, StateDuration duration)
{
    led_events_.push_back({level, duration});

    if (level == 0 || !pattern_is_valid_)
        return;

    // Insert and check if the set grew
    const auto [_, inserted] = used_levels_.insert(level);

    // If insertion failed or already invalid, do nothing
    if (!inserted)
        return;

    if (used_levels_.size() > 2)
    {
        pattern_is_valid_ = false;
        return;
    }

    // Only calculate levels *once* when we reach exactly 2 unique non-zero levels
    if (used_levels_.size() == 2)
    {
        auto it       = used_levels_.begin();
        dimmed_level_ = *it++;
        pulse_level_  = *it;
    }
}

std::string LedBlinkPattern::tracePatternToString() const
{
    if (!pattern_is_valid_)
        return "!";

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

uint8_t LedBlinkPattern::getRepeatCount(StateDuration duration) const
{
    auto it = std::upper_bound(traceRepeatRules.begin(), traceRepeatRules.end(), duration,
                               [](StateDuration value, const TraceRepeatRule& rule)
                               { return value < rule.min_duration; });

    if (it == traceRepeatRules.begin())
        return traceRepeatRules.front().repeat_count;

    return std::prev(it)->repeat_count;
}
