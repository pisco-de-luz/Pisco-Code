#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"

namespace pisco_code
{
    class LedControllerHardwarePwm final : public SignalController
    {
      public:
        explicit LedControllerHardwarePwm(void (*led_func)(IntensityLevel));
        explicit LedControllerHardwarePwm();
        void               attachLedControl(void (*led_func)(IntensityLevel));
        [[nodiscard]] bool isAttached() const noexcept;
        void               update() override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        void (*led_control_)(IntensityLevel) = nullptr;
    };

} // namespace pisco_code
