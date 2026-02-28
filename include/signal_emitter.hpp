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
        explicit SignalEmitter(SignalController& controller) noexcept;

        bool               showCode(SignalCode code, NumberBase base,
                                    NumDigits num_digits) noexcept;
        void               loop() noexcept;
        [[nodiscard]] bool isRunning() const noexcept;
        void               setRepeatTimes(RepeatTimes repeat_times) noexcept;
        [[nodiscard]] RepeatTimes getRepeatTimes() const noexcept;

      private:
        [[nodiscard]] bool phaseElapsed(Timestamp current_ts) const noexcept;
        [[nodiscard]] bool shouldAdvancePulse() const noexcept;
        static Timestamp
        signalDurationToPhaseDuration(SignalDuration duration) noexcept;

        enum class PhaseLoop : PhaseType
        {
            STARTING = 0,
            APPLYING_PULSE,
            IDLE,
        };

        SignalController&   controller_;
        SignalSequencer     sequencer_;
        SignalPulseIterator pulse_iterator_;
        Timestamp           current_ts_     = 0;
        Timestamp           start_time_     = 0;
        Timestamp           phase_duration_ = 0;
        PhaseLoop           current_phase_  = PhaseLoop::IDLE;
        bool                is_running_     = false;
    };

} // namespace pisco_code
