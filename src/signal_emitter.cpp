// code_blinker.cpp
#include "signal_emitter.hpp"

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    SignalEmitter::SignalEmitter(LedController* controller) :
        controller_(controller), pulse_iterator_(sequencer_.stack())
    {
    }

    bool SignalEmitter::showCode(SignalCode code, NumberBase base,
                                 NumDigits num_digits, RepeatTimes repeats)
    {
        if (controller_ == nullptr || current_phase_ != PhaseLoop::IDLE ||
            repeats == 0 || peak_level_ == 0)
        {
            return false;
        }
        sequencer_.clear();
        sequencer_.loadSignalCode(code, base, num_digits);
        sequencer_.setRepeatTimes(repeats);
        pulse_iterator_ = sequencer_.createPulseIterator();

        controller_->setDimmedLevel(dimmed_level_);
        controller_->setPeakLevel(peak_level_);

        current_phase_ = PhaseLoop::STARTING;
        return true;
    }

    void SignalEmitter::loop(TickCounter tick_counter)
    {
        if (controller_ == nullptr)
            return;

        if (current_phase_ == PhaseLoop::STARTING)
        {
            pulse_iterator_.reset();
            is_running_     = true;
            start_time_     = tick_counter;
            phase_duration_ = 0;
            current_phase_  = PhaseLoop::APPLYING_PULSE;
        }

        if (phaseElapsed(tick_counter) && is_running_)
        {
            if (pulse_iterator_.hasNext())
            {
                start_time_                 = tick_counter;
                const SignalElement element = pulse_iterator_.next();
                phase_duration_ =
                    signalDurationToPhaseDuration(element.get_duration());
                const auto level = element.get_level();
                is_in_gap_level_ = (level == SignalLevel::GAP);
                controller_->setBlinkMode(signalLevelToBlinkMode(level));
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
        controller_->update();
    }

    bool SignalEmitter::phaseElapsed(TickCounter tick_counter) const
    {
        const auto elapsed =
            static_cast<PhaseDuration>(tick_counter - start_time_);
        const bool phase_done = elapsed > phase_duration_;

        return phase_done && controller_->readyForPhaseChange();
    }

    bool SignalEmitter::isRunning() const
    {
        return (is_running_ || current_phase_ != PhaseLoop::IDLE);
    }

    void SignalEmitter::setPeakLevel(LedLevel led_level)
    {
        peak_level_ = (led_level > PWM_MAX) ? PWM_MAX : led_level;
        if (peak_level_ < MIN_PULSE_DIMMED_GAP)
        {
            peak_level_ = MIN_PULSE_DIMMED_GAP;
        }
        if (dimmed_level_ >= peak_level_)
        {
            dimmed_level_ = peak_level_ - MIN_PULSE_DIMMED_GAP;
        }
    }

    void SignalEmitter::setDimmedLevel(LedLevel led_level)
    {
        dimmed_level_ = (led_level > (PWM_MAX - MIN_PULSE_DIMMED_GAP))
                            ? (PWM_MAX - MIN_PULSE_DIMMED_GAP)
                            : led_level;
        if (dimmed_level_ >= peak_level_)
        {
            dimmed_level_ = peak_level_ - MIN_PULSE_DIMMED_GAP;
        }
    }

    bool SignalEmitter::isLedBeingUsedNow() const
    {
        return is_running_ && !is_in_gap_level_;
    }

    BlinkMode SignalEmitter::signalLevelToBlinkMode(SignalLevel level)
    {
        switch (level)
        {
            case SignalLevel::GAP:
                return BlinkMode::NONE;
            case SignalLevel::MIDDLE:
                return BlinkMode::DIMMED;
            case SignalLevel::PEAK:
                return BlinkMode::PULSE;
            default:
                return BlinkMode::NONE; // Not defined or invalid
        }
    }

    PhaseDuration
    SignalEmitter::signalDurationToPhaseDuration(SignalDuration duration)
    {
        switch (duration)
        {
            case SignalDuration::SHORT:
                return to_phase_duration(SHORT_BLINK_MS);
            case SignalDuration::MEDIUM:
                return to_phase_duration(MEDIUM_BLINK_MS);
            case SignalDuration::LONG:
                return to_phase_duration(LONG_BLINK_MS);
            case SignalDuration::EXTRA_LONG:
                return to_phase_duration(EXTRA_LONG_BLINK_MS);
            default:
                return to_phase_duration(0);
        }
    }
} // namespace pisco_code
