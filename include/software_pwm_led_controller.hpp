#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"

namespace pisco_code
{
    class SoftwarePwmLedController final : public SignalController
    {
      public:
        explicit SoftwarePwmLedController(bool (*led_func)(LedControlCode));
        explicit SoftwarePwmLedController();
        void               attachLedControl(bool (*led_func)(LedControlCode));
        void               setPeakLevel(IntensityLevel level) override;
        void               setBaseLevel(IntensityLevel level) override;
        void               setCurrentSignalLevel(BlinkMode mode) override;
        void               update() override;
        [[nodiscard]] bool readyForPhaseChange() const noexcept override;

      private:
        IntensityLevel peak_level_           = PWM_MAX;
        IntensityLevel base_level_           = DEFAULT_BASE_LEVEL;
        bool (*led_control_)(LedControlCode) = nullptr;
        BlinkMode       mode_                = BlinkMode::NONE;
        PwmTickPosition pwm_tick_position_   = 0;
    };

} // namespace pisco_code
