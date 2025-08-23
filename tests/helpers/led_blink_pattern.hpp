#pragma once
#ifdef new
#undef new
#endif

#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "pisco_types.hpp"

using pisco_code::DurationMs;
using pisco_code::LedLevel;
using pisco_code::RepeatTimes;
using pisco_code::Timestamp;
struct LedLevelDuration
{
    LedLevel   led_level{0};
    DurationMs duration{0};
};

struct TraceRepeatRule
{
    DurationMs min_duration{0};
    uint8_t    repeat_count{0};
};

constexpr std::array<TraceRepeatRule, 3> TRACE_REPEAT_RULES{
    {
     {0, 1},    // 0–1100
        {1101, 3}, // 1101–2200
        {2201, 6}, // 2201+
    }
};

class LedBlinkPattern
{
  public:
    void reset();
    void append(LedLevel level, DurationMs duration);

    [[nodiscard]] bool     isValid() const;
    [[nodiscard]] LedLevel getDimmedLevel() const
    {
        return dimmed_level_;
    }
    [[nodiscard]] LedLevel getPulseLevel() const
    {
        return pulse_level_;
    }
    [[nodiscard]] const std::vector<LedLevelDuration>& getEvents() const
    {
        return led_events_;
    }
    [[nodiscard]] std::string tracePatternToString() const;
    [[nodiscard]] RepeatTimes getRepeatCount(DurationMs duration) const;

  private:
    bool                          pattern_is_valid_{true};
    LedLevel                      dimmed_level_{0};
    LedLevel                      pulse_level_{0};
    std::vector<LedLevelDuration> led_events_;
    std::set<LedLevel>            used_levels_;
};
