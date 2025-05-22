// led_controller.hpp
#pragma once

#include "pisco_types.hpp"

namespace pisco_code
{

    enum class BlinkMode : BlinkModeType
    {
        NONE,
        PULSE,
        DIMMED
    };

    class LedController
    {
      public:
        virtual void setPeakLevel(LedLevel level)   = 0;
        virtual void setDimmedLevel(LedLevel level) = 0;
        virtual void update(LoopCounter loop_counter) { (void) loop_counter; }
        virtual void setBlinkMode(BlinkMode mode) = 0;
    };

} // namespace pisco_code
