// signal_emitter.hpp
#pragma once

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    class SignalEmitter
    {
      public:
        explicit SignalEmitter(LedController* controller);

        bool showCode(SignalCode code, NumberBase base, NumDigits num_digits,
                      RepeatTimes repeats);
        void loop(TickCounter tick_counter);
        [[nodiscard]] bool isRunning() const;
        [[nodiscard]] bool isLedBeingUsedNow() const;
        void               setPeakLevel(LedLevel level);
        void               setDimmedLevel(LedLevel level);

      private:
        [[nodiscard]] bool phaseElapsed(TickCounter tick_counter) const;

        static BlinkMode signalLevelToBlinkMode(SignalLevel level);
        static PhaseDuration
        signalDurationToPhaseDuration(SignalDuration duration);

        enum class PhaseLoop : PhaseType
        {
            STARTING = 0,
            APPLYING_PULSE,
            IDLE,
        };

        LedController*      controller_ = nullptr;
        SignalSequencer     sequencer_;
        SignalPulseIterator pulse_iterator_;
        SignalElement       element_;
        TickCounter         start_time_      = 0;
        PhaseDuration       phase_duration_  = 0;
        PhaseLoop           current_phase_   = PhaseLoop::IDLE;
        bool                is_negative_     = false;
        bool                is_running_      = false;
        bool                is_in_gap_level_ = false;
        LedLevel            peak_level_      = DEFAULT_PULSE_LEVEL;
        LedLevel            dimmed_level_    = DEFAULT_DIMMED_LEVEL;
    };

} // namespace pisco_code
