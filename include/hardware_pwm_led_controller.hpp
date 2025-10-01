#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"

namespace pisco_code
{
    class HardwarePwmLedController final : public SignalController
    {
      public:
        explicit HardwarePwmLedController(void (*led_func)(IntensityLevel));
        explicit HardwarePwmLedController();
        void               attachLedControl(void (*led_func)(IntensityLevel));
        [[nodiscard]] bool isAttached() const noexcept;
        void               update() override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        void (*led_control_)(IntensityLevel) = nullptr;
    };

} // namespace pisco_code
