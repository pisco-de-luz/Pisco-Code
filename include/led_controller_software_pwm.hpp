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
        explicit LedControllerSoftwarePwm() noexcept;
        void attachLedControl(bool (*led_func)(LedControlCode)) noexcept;
        [[nodiscard]] bool isAttached() const noexcept;
        void               update() noexcept override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        bool (*led_control_)(LedControlCode) = nullptr;
        PwmTickPosition pwm_tick_position_   = 0;
    };

} // namespace pisco_code
