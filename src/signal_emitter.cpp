#include "signal_emitter.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"
#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    SignalEmitter::SignalEmitter(SignalController& controller) noexcept :
        controller_(controller), pulse_iterator_(sequencer_.stack())
    {
    }

    bool SignalEmitter::showCode(SignalCode code, NumberBase base,
                                 NumDigits num_digits) noexcept
    {
        if (current_phase_ != PhaseLoop::IDLE)
        {
            return false;
        }
        sequencer_.clear();
        sequencer_.loadSignalCode(code, base, num_digits);
        pulse_iterator_ = sequencer_.createPulseIterator();

        current_ts_    = 0;
        current_phase_ = PhaseLoop::STARTING;
        return true;
    }

    void SignalEmitter::loop() noexcept
    {
        ++current_ts_;
        if (current_phase_ == PhaseLoop::STARTING)
        {
            pulse_iterator_.reset();
            is_running_     = true;
            start_time_     = current_ts_;
            phase_duration_ = 0;
            current_phase_  = PhaseLoop::APPLYING_PULSE;
        }

        if (phaseElapsed(current_ts_) && is_running_)
        {
            if (pulse_iterator_.hasNext())
            {
                start_time_                 = current_ts_;
                const SignalElement element = pulse_iterator_.next();
                phase_duration_ =
                    signalDurationToPhaseDuration(element.get_duration());
                controller_.setCurrentSignalMode(element.get_mode());
            }
            else
            {
                if (sequencer_.shouldRepeat())
                {
                    current_phase_ = PhaseLoop::STARTING;
                    sequencer_.incrementRepeatIndex();
                }
                else
                {
                    is_running_    = false;
                    current_phase_ = PhaseLoop::IDLE;
                }
            }
        }
        controller_.update();
    }

    bool SignalEmitter::phaseElapsed(Timestamp current_ts) const noexcept
    {
        const auto elapsed    = (current_ts - start_time_);
        const bool phase_done = elapsed > phase_duration_;

        return phase_done && controller_.readyForPhaseChange();
    }

    bool SignalEmitter::isRunning() const noexcept
    {
        return (is_running_ || current_phase_ != PhaseLoop::IDLE);
    }

    void SignalEmitter::setRepeatTimes(RepeatTimes repeat_times) noexcept
    {
        sequencer_.setRepeatTimes(repeat_times);
    }

    [[nodiscard]] RepeatTimes SignalEmitter::getRepeatTimes() const noexcept
    {
        return sequencer_.getRepeatTimes();
    }

    Timestamp SignalEmitter::signalDurationToPhaseDuration(
        SignalDuration duration) noexcept
    {
        switch (duration)
        {
            case SignalDuration::SHORT:
                return SHORT_BLINK_MS;
            case SignalDuration::MEDIUM:
                return MEDIUM_BLINK_MS;
            case SignalDuration::LONG:
                return LONG_BLINK_MS;
            case SignalDuration::EXTRA_LONG:
                return EXTRA_LONG_BLINK_MS;
            default:
                return 0;
        }
    }
} // namespace pisco_code
