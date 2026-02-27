#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"

namespace pisco_code
{
    class LedControllerSoftwarePwm final : public SignalController
    {
      public:
        explicit LedControllerSoftwarePwm(
            bool (*led_func)(LedControlCode)) noexcept;
        LedControllerSoftwarePwm(decltype(nullptr)) = delete;
        void               update() noexcept override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        [[nodiscard]] bool
        shouldTurnLedOn(IntensityLevel sw_pwm_level) const noexcept;
        [[nodiscard]] bool
             shouldTurnLedOff(IntensityLevel sw_pwm_level) const noexcept;
        void advancePwmTick() noexcept;

        bool (*led_control_)(LedControlCode) = nullptr;
        PwmTickPosition pwm_tick_position_   = 0;
    };

} // namespace pisco_code
