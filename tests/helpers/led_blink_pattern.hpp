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

        void setPeakLevel(IntensityLevel level)
        {
            peak_level_ = (level > PWM_MAX) ? PWM_MAX : level;
            if (peak_level_ < MIN_INTENSITY_DIFFERENCE)
            {
                peak_level_ = MIN_INTENSITY_DIFFERENCE;
            }
            if (base_level_ >= peak_level_)
            {
                base_level_ = peak_level_ - MIN_INTENSITY_DIFFERENCE;
            }
        }
        void setBaseLevel(IntensityLevel level)
        {
            constexpr auto MAX_BASE_LEVEL = PWM_MAX - MIN_INTENSITY_DIFFERENCE;
            base_level_ = (level > MAX_BASE_LEVEL) ? MAX_BASE_LEVEL : level;
            if (base_level_ >= peak_level_)
            {
                base_level_ = peak_level_ - MIN_INTENSITY_DIFFERENCE;
            }
        }
        [[nodiscard]] IntensityLevel getBaseLevel() const
        {
            return base_level_;
        }
        [[nodiscard]] IntensityLevel getPeakLevel() const
        {
            return peak_level_;
        }
        [[nodiscard]] const std::vector<LedLevelDuration>& getEvents() const
        {
            return led_events_;
        }
        [[nodiscard]] std::string        tracePatternToString() const;
        [[nodiscard]] static RepeatTimes getRepeatCount(DurationMs duration);

      private:
        bool                          pattern_is_valid_{true};
        IntensityLevel                base_level_{0};
        IntensityLevel                peak_level_{0};
        std::vector<LedLevelDuration> led_events_{};
        std::set<IntensityLevel>      used_levels_{};
    };
} // namespace testutils
