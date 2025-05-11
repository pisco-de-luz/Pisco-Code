#include "LedBlinkPattern.hpp"

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
