#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SoftwarePwmLedController final : public SignalController
    {
      public:
        explicit SoftwarePwmLedController(bool (*led_func)(LedControlCode));
        explicit SoftwarePwmLedController();
        void               attachLedControl(bool (*led_func)(LedControlCode));
        [[nodiscard]] bool isAttached() const noexcept;
        void               update() override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        bool (*led_control_)(LedControlCode) = nullptr;
        PwmTickPosition pwm_tick_position_   = 0;
    };

} // namespace pisco_code
