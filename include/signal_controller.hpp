// signal_controller.hpp
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

    // NOLINT(cppcoreguidelines-special-member-functions)
    class SignalController
    {
      public:
        virtual void               setPeakLevel(LedLevel level)          = 0;
        virtual void               setDimmedLevel(LedLevel level)        = 0;
        virtual void               update()                              = 0;
        [[nodiscard]] virtual bool readyForPhaseChange() const noexcept  = 0;
        virtual void               setCurrentSignalLevel(BlinkMode mode) = 0;

      protected:
        SignalController()                                   = default;
        SignalController(const SignalController&)            = default;
        SignalController& operator=(const SignalController&) = default;
        SignalController(SignalController&&)                 = default;
        SignalController& operator=(SignalController&&)      = default;
        ~SignalController()                                  = default;
    };

} // namespace pisco_code
