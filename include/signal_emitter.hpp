// signal_emitter.hpp
#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    class SignalEmitter
    {
      public:
        explicit SignalEmitter(SignalController* controller);

        bool showCode(SignalCode code, NumberBase base, NumDigits num_digits);
        void loop();
        [[nodiscard]] bool isRunning() const;
        [[nodiscard]] bool isLedBeingUsedNow() const;
        void               setPeakLevel(IntensityLevel level);
        void               setBaseLevel(IntensityLevel level);
        void               setRepeatTimes(RepeatTimes repeat_times) noexcept;
        [[nodiscard]] RepeatTimes getRepeatTimes() const noexcept;

      private:
        [[nodiscard]] bool phaseElapsed(TickCounter tick_counter) const;
        static TickCounter
        signalDurationToPhaseDuration(SignalDuration duration);

        enum class PhaseLoop : PhaseType
        {
            STARTING = 0,
            APPLYING_PULSE,
            IDLE,
        };

        SignalController*   controller_ = nullptr;
        SignalSequencer     sequencer_;
        SignalPulseIterator pulse_iterator_;
        SignalElement       element_;
        Timestamp           tick_timestamp_ = 0;
        TickCounter         start_time_     = 0;
        TickCounter         phase_duration_ = 0;
        PhaseLoop           current_phase_  = PhaseLoop::IDLE;
        bool                is_running_     = false;
        bool                is_in_gap_mode_ = false;
        IntensityLevel      peak_level_     = DEFAULT_PULSE_LEVEL;
        IntensityLevel      base_level_     = DEFAULT_BASE_LEVEL;
        RepeatTimes         repeat_times_   = 1;
    };

} // namespace pisco_code
