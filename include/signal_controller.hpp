// signal_controller.hpp
#pragma once

#include "pisco_types.hpp"

namespace pisco_code
{

    enum class BlinkMode : BlinkModeType
    {
        NONE,
        PULSE,
        BASE
    };

    // NOLINT(cppcoreguidelines-special-member-functions)
    class SignalController
    {
      public:
        virtual void               setPeakLevel(IntensityLevel level)   = 0;
        virtual void               setBaseLevel(IntensityLevel level)   = 0;
        virtual void               update()                             = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept = 0;
        virtual void               setCurrentSignalMode(BlinkMode mode) = 0;

      protected:
        SignalController()                                   = default;
        SignalController(const SignalController&)            = default;
        SignalController& operator=(const SignalController&) = default;
        SignalController(SignalController&&)                 = default;
        SignalController& operator=(SignalController&&)      = default;
        ~SignalController()                                  = default;
    };

} // namespace pisco_code
