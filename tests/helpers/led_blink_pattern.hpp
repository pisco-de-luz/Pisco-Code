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
#include "tests_types.hpp"

using pisco_code::DurationMs;
using pisco_code::IntensityLevel;
using pisco_code::RepeatTimes;
using pisco_code::Timestamp;

namespace testutils
{

    class LedBlinkPattern
    {
      public:
        void               reset();
        void               append(IntensityLevel level, DurationMs duration);
        [[nodiscard]] bool isValid() const;

        [[nodiscard]] IntensityLevel getBaseLevel() const
        {
            return low_level_;
        }
        [[nodiscard]] IntensityLevel getPeakLevel() const
        {
            return high_level_;
        }
        [[nodiscard]] const std::vector<LedLevelDuration>& getEvents() const
        {
            return led_events_;
        }
        [[nodiscard]] std::string        tracePatternToString() const;
        [[nodiscard]] static RepeatTimes getRepeatCount(DurationMs duration);

      private:
        bool                          pattern_is_valid_{true};
        IntensityLevel                low_level_{0};
        IntensityLevel                high_level_{0};
        std::vector<LedLevelDuration> led_events_{};
        std::set<IntensityLevel>      used_levels_{};
    };
} // namespace testutils
