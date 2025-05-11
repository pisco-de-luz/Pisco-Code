#pragma once
#ifdef new
#undef new
#endif
#include "pisco_constants.hpp"
#include <cstdint>
#include <set>
#include <string>
#include <vector>

using pisco::LedLevel;
using pisco::StateDuration;
using pisco::Timestamp;

struct LedLevelDuration
{
    LedLevel      led_level{0};
    StateDuration duration{0};
};

class LedBlinkPattern
{
  public:
    void reset();
    void append(LedLevel level, StateDuration duration);

    bool                                 isValid() const { return pattern_is_valid_; }
    LedLevel                             getDimmedLevel() const { return dimmed_level_; }
    LedLevel                             getPulseLevel() const { return pulse_level_; }
    const std::vector<LedLevelDuration>& getEvents() const { return led_events_; }
    std::string                          tracePatternToString() const;

  private:
    bool                          pattern_is_valid_{true};
    LedLevel                      dimmed_level_{0};
    LedLevel                      pulse_level_{0};
    std::vector<LedLevelDuration> led_events_{};
    std::set<LedLevel>            used_levels_;
};
