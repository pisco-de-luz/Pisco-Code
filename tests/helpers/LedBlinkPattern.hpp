#pragma once
#ifdef new
#undef new
#endif

#include <algorithm>
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "pisco_constants.hpp"

using pisco::DurationMs;
using pisco::LedLevel;
using pisco::Timestamp;

struct LedLevelDuration
{
    LedLevel   led_level{0};
    DurationMs duration{0};
};

struct TraceRepeatRule
{
    DurationMs min_duration;
    uint8_t    repeat_count;
};

constexpr std::array<TraceRepeatRule, 3> traceRepeatRules{{
    {0, 1},    // 0–1100
    {1101, 3}, // 1101–2200
    {2201, 5}  // 2201+
}};

class LedBlinkPattern
{
  public:
    void reset();
    void append(LedLevel level, DurationMs duration);

    bool                                 isValid() const;
    LedLevel                             getDimmedLevel() const { return dimmed_level_; }
    LedLevel                             getPulseLevel() const { return pulse_level_; }
    const std::vector<LedLevelDuration>& getEvents() const { return led_events_; }
    std::string                          tracePatternToString() const;
    pisco::RepeatTimes                   getRepeatCount(DurationMs duration) const;

  private:
    bool                          pattern_is_valid_{true};
    LedLevel                      dimmed_level_{0};
    LedLevel                      pulse_level_{0};
    std::vector<LedLevelDuration> led_events_{};
    std::set<LedLevel>            used_levels_;
};
