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
        if (controller_ == nullptr || current_phase_ != Phase::IDLE ||
            repeats == 0 || peak_level_ == 0)
        {
            return false;
        }
        sequencer_.clear();
        sequencer_.loadSignalCode(code, base, num_digits);
        sequencer_.setRepeatTimes(repeats);
        pulse_iterator_ = sequencer_.createPulseIterator();

        is_negative_                = (code < 0);
        SignalCode value_to_display = code;
        if (is_negative_)
        {
            value_to_display = -code;
        }
        max_digits_ = max_digits_for_base(base);

        current_digit_index_ = max_digits_ - 1;

        for (Index i = 0; i < max_digits_; ++i)
        {
            const Index digit    = max_digits_ - 1U - i;
            digits_[digit]       = to_digit(value_to_display % to_value(base));
            blink_counts_[digit] = to_count(digits_[digit]);
            if (digits_[digit] > 0)
            {
                current_digit_index_ = digit;
            }
            value_to_display /= to_value(base);
        }

        if (num_digits > 0 && num_digits <= max_digits_)
        {
            current_digit_index_ = max_digits_ - num_digits;
        }

        least_significant_digit_ = current_digit_index_;
        repeats_total_           = repeats;
        repeats_remaining_       = repeats;

        controller_->setDimmedLevel(dimmed_level_);
        controller_->setPeakLevel(peak_level_);

        current_phase_loop_ = PhaseLoop::STARTING;
        // transitionTo(Phase::PAUSE_BEFORE_START);
        return true;
    }

    void SignalEmitter::loop(TickCounter tick_counter)
    {
        if (controller_ == nullptr)
            return;

        // last_phase_entry_ = getPhaseEntry(current_phase_);

        if (current_phase_loop_ == PhaseLoop::STARTING)
        {
            pulse_iterator_.reset();
            is_running_         = true;
            start_time_         = tick_counter;
            phase_duration_     = 0;
            current_phase_loop_ = PhaseLoop::APPLYING_PULSE;
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
                //(this->*last_phase_entry_.handler)();
            }
            else
            {
                if (sequencer_.shouldRepeat())
                {
                    current_phase_loop_ = PhaseLoop::STARTING;
                    sequencer_.incrementRepeatIndex();
                }
                else
                {
                    is_running_         = false;
                    current_phase_loop_ = PhaseLoop::IDLE;
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
        // return current_phase_ != Phase::IDLE;
        return (is_running_ || current_phase_loop_ != PhaseLoop::IDLE);
    }

    SignalEmitter::PhaseTableEntry
    SignalEmitter::getPhaseEntry(Phase phase) const
    {
        PhaseTableEntry phase_entry = last_phase_entry_;
        if (phase != last_phase_entry_.id)
        {
            for (const auto& entry : SignalEmitter::PHASE_TABLE)
            {
                if (entry.id == phase)
                {
                    phase_entry.blink_mode = entry.blink_mode;
                    phase_entry.handler    = entry.handler;
                    phase_entry.id         = entry.id;
                    phase_entry.duration   = entry.duration;
                    break;
                }
            }
        }
        return phase_entry;
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

    bool SignalEmitter::hasMoreBlinks() const
    {
        return blink_counts_[current_digit_index_] > 0;
    }

    bool SignalEmitter::hasMoreDigits() const
    {
        return current_digit_index_ < max_digits_;
    }

    bool SignalEmitter::shouldRepeat() const
    {
        if (repeats_remaining_ == 0)
        {
            return false;
        }

        --repeats_remaining_;
        return true;
    }

    bool SignalEmitter::isLedBeingUsedNow() const
    {
        // switch (current_phase_)
        // {
        //     case Phase::PREPARE_REPEAT:
        //     case Phase::PAUSE_AFTER_FINISH:
        //     case Phase::IDLE:
        //     case Phase::DISPLAY_ZERO:
        //         // LED is not being used
        //         return false;
        //     default:
        //         return true;
        // }
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

    void SignalEmitter::transitionTo(Phase next)
    {
        current_phase_    = next;
        last_phase_entry_ = getPhaseEntry(next);
        phase_duration_   = last_phase_entry_.duration;
        controller_->setBlinkMode(last_phase_entry_.blink_mode);
    }

    void SignalEmitter::pulseAs(Phase next, SignalLevel level,
                                SignalDuration duration)
    {
        current_phase_    = next;
        last_phase_entry_ = getPhaseEntry(next);
        phase_duration_   = signalDurationToPhaseDuration(duration);
        controller_->setBlinkMode(signalLevelToBlinkMode(level));
    }

    void SignalEmitter::handlePauseBeforeStart()
    {
        transitionTo(Phase::HAS_MORE_SIGNAL_CODE_TO_SEQUENCE);
    }

    void SignalEmitter::handleHasMoreSignalCodeToSequence()
    {
        if (sequencer_.hasMoreSignalCodeToSequence())
        {
            transitionTo(Phase::POP_NEXT_CODE_TO_SEQUENCE);
        }
        else
        {
            transitionTo(Phase::IDLE);
        }
    }

    void SignalEmitter::handlePopNextCodeToSequence()
    {
        sequencer_.popNextCodeToSequence();
        transitionTo(Phase::HAS_MORE_SIGNAL_ELEMENTS);
    }

    void SignalEmitter::handleHasMoreSignalElements()
    {
        if (sequencer_.hasMoreSignalElements())
        {
            transitionTo(Phase::POP_NEXT_SIGNAL_ELEMENT);
        }
        else
        {
            transitionTo(Phase::IDLE);
        }
    }

    void SignalEmitter::handlePopNextSignalElement()
    {
        element_ = sequencer_.popNextSignalElement();
        transitionTo(Phase::BEGIN_DIGIT);
        // transitionTo(Phase::HAS_MORE_PULSE);
    }

    void SignalEmitter::handleHasMorePulse()
    {
        if (sequencer_.hasMorePulse())
        {
            transitionTo(Phase::POP_NEXT_PULSE);
        }
        else
        {
            transitionTo(Phase::HAS_MORE_SIGNAL_ELEMENTS);
        }
    }

    void SignalEmitter::handlePopNextPulse()
    {
        sequencer_.popNextPulse();
        // transitionTo(Phase::HAS_MORE_SIGNAL_ELEMENTS);
    }

    void SignalEmitter::handleIdle()
    {
    }

    void SignalEmitter::handleBeginDigit()
    {
        transitionTo(is_negative_ ? Phase::DISPLAY_NEGATIVE_SIGN
                                  : Phase::LOAD_NEXT_DIGIT);
    }

    void SignalEmitter::handleLoadNextDigit()
    {
        if (current_digit_index_ >= max_digits_ && shouldRepeat())
        {
            transitionTo(Phase::END_OF_DIGIT_CYCLE);
        }
        else
        {
            if (blink_counts_[current_digit_index_] == 0)
            {
                transitionTo(Phase::DISPLAY_ZERO);
            }
            else
            {
                transitionTo(Phase::EMIT_BLINK);
            }
        }
    }

    void SignalEmitter::handleDisplayNegativeSign()
    {
        transitionTo(Phase::PAUSE_AFTER_NEGATIVE);
    }

    void SignalEmitter::handlePauseAfterNegative()
    {
        transitionTo(Phase::LOAD_NEXT_DIGIT);
    }

    void SignalEmitter::handleEmitBlink()
    {
        --blink_counts_[current_digit_index_];
        if (hasMoreBlinks())
        {
            transitionTo(Phase::PAUSE_BETWEEN_BLINKS);
        }
        else
        {
            transitionTo(Phase::PAUSE_BETWEEN_DIGITS);
        }
    }

    void SignalEmitter::handlePauseBetweenBlinks()
    {
        if (hasMoreBlinks())
        {
            transitionTo(Phase::EMIT_BLINK);
        }
    }

    void SignalEmitter::handlePauseBetweenDigits()
    {
        if (hasMoreDigits())
        {
            ++current_digit_index_;
        }
        transitionTo(Phase::LOAD_NEXT_DIGIT);
    }

    void SignalEmitter::handleDisplayZero()
    {
        transitionTo(Phase::PAUSE_BETWEEN_DIGITS);
    }

    void SignalEmitter::handleEndOfDigitCycle()
    {
        if (repeats_remaining_ == 0)
        {
            transitionTo(Phase::PAUSE_AFTER_FINISH);
        }
        else
        {
            for (Index i = 0; i < max_digits_; ++i)
            {
                const Index digit    = max_digits_ - 1U - i;
                blink_counts_[digit] = digits_[digit];
            }
            current_digit_index_ = least_significant_digit_;
            transitionTo(Phase::PREPARE_REPEAT);
        }
    }

    void SignalEmitter::handlePauseAfterFinish()
    {
        transitionTo(Phase::IDLE);
    }

} // namespace pisco_code
