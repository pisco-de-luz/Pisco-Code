// code_blinker.cpp
#include "signal_emitter.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_controller.hpp"
#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

namespace pisco_code
{

    SignalEmitter::SignalEmitter(SignalController* controller) :
        controller_(controller), pulse_iterator_(sequencer_.stack())
    {
    }

    bool SignalEmitter::showCode(SignalCode code, NumberBase base,
                                 NumDigits num_digits)
    {
        if (controller_ == nullptr || current_phase_ != PhaseLoop::IDLE ||
            repeat_times_ == 0 || peak_level_ == 0)
        {
            return false;
        }
        sequencer_.clear();
        sequencer_.setRepeatTimes(repeat_times_);
        sequencer_.loadSignalCode(code, base, num_digits);
        pulse_iterator_ = sequencer_.createPulseIterator();

        controller_->setDimmedLevel(dimmed_level_);
        controller_->setPeakLevel(peak_level_);

        tick_timestamp_ = 0;
        current_phase_  = PhaseLoop::STARTING;
        return true;
    }

    void SignalEmitter::loop()
    {
        if (controller_ == nullptr)
        {
            return;
        }
        ++tick_timestamp_;
        if (current_phase_ == PhaseLoop::STARTING)
        {
            pulse_iterator_.reset();
            is_running_     = true;
            start_time_     = to_tick_counter(tick_timestamp_);
            phase_duration_ = 0;
            current_phase_  = PhaseLoop::APPLYING_PULSE;
        }

        if (phaseElapsed(to_tick_counter(tick_timestamp_)) && is_running_)
        {
            if (pulse_iterator_.hasNext())
            {
                start_time_                 = to_tick_counter(tick_timestamp_);
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
            static_cast<TickCounter>(tick_counter - start_time_);
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

    void SignalEmitter::setRepeatTimes(RepeatTimes repeat_times) noexcept
    {
        repeat_times_ = repeat_times;
        sequencer_.setRepeatTimes(repeat_times_);
    }

    [[nodiscard]] RepeatTimes SignalEmitter::getRepeatTimes() const noexcept
    {
        return repeat_times_;
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

    TickCounter
    SignalEmitter::signalDurationToPhaseDuration(SignalDuration duration)
    {
        switch (duration)
        {
            case SignalDuration::SHORT:
                return to_tick_counter(SHORT_BLINK_MS);
            case SignalDuration::MEDIUM:
                return to_tick_counter(MEDIUM_BLINK_MS);
            case SignalDuration::LONG:
                return to_tick_counter(LONG_BLINK_MS);
            case SignalDuration::EXTRA_LONG:
                return to_tick_counter(EXTRA_LONG_BLINK_MS);
            default:
                return to_tick_counter(0);
        }
    }
} // namespace pisco_code
