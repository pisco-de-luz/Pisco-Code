// code_blinker.cpp
#include "code_blinker.hpp"

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{

    CodeBlinker::CodeBlinker(LedController* controller) : controller_(controller)
    {
    }

    bool CodeBlinker::showCode(BlinkCode code, NumberBase base, NumDigits num_digits,
                               RepeatTimes repeats)
    {
        if (!controller_ || current_phase_ != Phase::Idle || repeats == 0 || peak_level_ == 0)
        {
            return false;
        }

        is_negative_               = (code < 0);
        BlinkCode value_to_display = code;
        if (is_negative_)
        {
            value_to_display = -code;
        }
        max_digits_ = max_digits_for_base(base);

        current_digit_index_ = max_digits_ - 1;

        for (Index digit = max_digits_ - 1; digit >= 0; --digit)
        {
            digits_[to_index(digit)]       = to_digit(value_to_display % to_value(base));
            blink_counts_[to_index(digit)] = to_count(digits_[to_index(digit)]);
            if (digits_[to_index(digit)] > 0)
            {
                current_digit_index_ = digit;
            }
            value_to_display /= to_value(base);
        }

        if (num_digits > 0 && num_digits < max_digits_)
        {
            current_digit_index_ = max_digits_ - num_digits;
        }

        least_significant_digit_ = current_digit_index_;
        repeats_total_           = repeats;
        repeats_remaining_       = repeats;

        controller_->setDimmedLevel(dimmed_level_);
        controller_->setPeakLevel(peak_level_);

        transitionTo(Phase::PauseBeforeStart, to_loop_count(INIT_PHASE_MS), 0);
        return true;
    }

    void CodeBlinker::loop(LoopCounter loop_counter)
    {
        if (!controller_)
            return;

        static constexpr struct
        {
            Phase             id;
            BlinkPhaseHandler handler;
        } phase_table[] = {
            {Phase::PauseBeforeStart, &CodeBlinker::handlePauseBeforeStart},
            {Phase::BeginDigit, &CodeBlinker::handleBeginDigit},
            {Phase::DisplayNegativeSign, &CodeBlinker::handleDisplayNegativeSign},
            {Phase::PauseAfterNegative, &CodeBlinker::handlePauseAfterNegative},
            {Phase::LoadNextDigit, &CodeBlinker::handleLoadNextDigit},
            {Phase::EmitBlink, &CodeBlinker::handleEmitBlink},
            {Phase::PauseBetweenBlinks, &CodeBlinker::handlePauseBetweenBlinks},
            {Phase::EndOfDigitCycle, &CodeBlinker::handleEndOfDigitCycle},
            {Phase::PrepareRepeat, &CodeBlinker::handlePauseBeforeStart},
            {Phase::PauseAfterFinish, &CodeBlinker::handlePauseAfterFinish},
            {Phase::Idle, &CodeBlinker::handleIdle},
            {Phase::DisplayZero, &CodeBlinker::handleDisplayZero},
        };

        for (const auto& entry : phase_table)
        {
            if (entry.id == current_phase_)
            {
                (this->*entry.handler)(loop_counter);
                break;
            }
        }
        controller_->update(loop_counter_);
        if (++loop_counter_ > PWM_MAX)
        {
            loop_counter_ = 0;
        }
    }

    void CodeBlinker::setPeakLevel(LedLevel led_level)
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

    void CodeBlinker::setDimmedLevel(LedLevel led_level)
    {
        dimmed_level_ = (led_level > (PWM_MAX - MIN_PULSE_DIMMED_GAP))
                            ? (PWM_MAX - MIN_PULSE_DIMMED_GAP)
                            : led_level;
        if (dimmed_level_ >= peak_level_)
        {
            dimmed_level_ = peak_level_ - MIN_PULSE_DIMMED_GAP;
        }
    }

    bool CodeBlinker::isRunning() const
    {
        return current_phase_ != Phase::Idle;
    }

    void CodeBlinker::transitionTo(Phase next, PhaseDuration duration, LoopCounter loop_counter)
    {
        current_phase_  = next;
        phase_duration_ = duration;
        start_time_     = loop_counter;
    }

    bool CodeBlinker::phaseElapsed(LoopCounter loop_counter) const
    {
        const auto elapsed            = to_phase_duration(loop_counter - start_time_);
        const bool phase_done         = elapsed > phase_duration_;
        const bool peak_level_reached = loop_counter_ == peak_level_;
        return phase_done && peak_level_reached;
    }

    bool CodeBlinker::hasMoreBlinks() const
    {
        return blink_counts_[current_digit_index_] > 0;
    }

    bool CodeBlinker::hasMoreDigits() const
    {
        return current_digit_index_ < max_digits_;
    }

    bool CodeBlinker::shouldRepeat() const
    {
        if (repeats_remaining_ == 0)
        {
            return false;
        }

        --repeats_remaining_;
        return true;
    }

    bool CodeBlinker::isLedBeingUsedNow() const
    {
        switch (current_phase_)
        {
            case Phase::PrepareRepeat:
            case Phase::PauseAfterFinish:
            case Phase::Idle:
            case Phase::DisplayZero:
                // LED is not being used
                return false;
            default:
                return true;
        }
    }

    void CodeBlinker::handleIdle(LoopCounter)
    {
        controller_->setBlinkMode(BlinkMode::NONE);
    }

    void CodeBlinker::handleBeginDigit(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::DIMMED);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(is_negative_ ? Phase::DisplayNegativeSign : Phase::LoadNextDigit,
                         is_negative_ ? to_loop_count(NEGATIVE_BLINK_LONG_MS) : 0, loop_counter);
        }
    }

    void CodeBlinker::handleLoadNextDigit(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::DIMMED);
        if (phaseElapsed(loop_counter))
        {
            // NOTE: Need to improve this logic to avoid miss interpretation
            // shouldRepeat() is checking and decreasing the counter BAD Design
            if (current_digit_index_ >= max_digits_ && shouldRepeat())
            {
                transitionTo(Phase::EndOfDigitCycle, to_loop_count(END_DIMMED_PHASE_MS),
                             loop_counter);
            }
            else
            {
                if (blink_counts_[current_digit_index_] == 0)
                {
                    transitionTo(Phase::DisplayZero, to_loop_count(ZERO_DIGIT_BLINK_MS),
                                 loop_counter);
                }
                else
                {
                    transitionTo(Phase::EmitBlink, to_loop_count(SHORT_BLINK_MS), loop_counter);
                }
            }
        }
    }

    void CodeBlinker::handleDisplayNegativeSign(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::PULSE);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::PauseAfterNegative, to_loop_count(NEGATIVE_BLINK_LONG_MS),
                         loop_counter);
        }
    }

    void CodeBlinker::handlePauseAfterNegative(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::DIMMED);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::LoadNextDigit, to_loop_count(BETWEEN_BLINK_MS), loop_counter);
        }
    }

    void CodeBlinker::handleEmitBlink(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::PULSE);

        if (phaseElapsed(loop_counter))
        {
            --blink_counts_[current_digit_index_];
            transitionTo(Phase::PauseBetweenBlinks,
                         hasMoreBlinks() ? to_loop_count(BETWEEN_BLINK_MS)
                                         : to_loop_count(BETWEEN_DIGITS_MS),
                         loop_counter);
        }
    }

    void CodeBlinker::handlePauseBetweenBlinks(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::DIMMED);
        if (phaseElapsed(loop_counter))
        {
            if (hasMoreBlinks())
            {
                transitionTo(Phase::EmitBlink, to_loop_count(SHORT_BLINK_MS), loop_counter);
            }
            else
            {
                if (hasMoreDigits())
                {
                    ++current_digit_index_;
                }
                transitionTo(Phase::LoadNextDigit, 0, loop_counter);
            }
        }
    }

    void CodeBlinker::handlePauseBeforeStart(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::NONE);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::BeginDigit, to_loop_count(INIT_DIMMED_PHASE_MS), loop_counter);
        }
    }

    void CodeBlinker::handleDisplayZero(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::NONE);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::PauseBetweenBlinks, to_loop_count(BETWEEN_DIGITS_MS), loop_counter);
        }
    }

    void CodeBlinker::handleEndOfDigitCycle(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::DIMMED);
        if (phaseElapsed(loop_counter))
        {
            if (repeats_remaining_ == 0)
            {
                transitionTo(Phase::PauseAfterFinish, to_loop_count(END_PHASE_MS), loop_counter);
            }
            else
            {
                for (Index digit = max_digits_ - 1; digit >= 0; --digit)
                {
                    blink_counts_[digit] = digits_[digit];
                }
                current_digit_index_ = least_significant_digit_;
                transitionTo(Phase::PrepareRepeat, to_loop_count(BETWEEN_CODES_MS), loop_counter);
            }
        }
    }

    void CodeBlinker::handlePauseAfterFinish(LoopCounter loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::NONE);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::Idle, 0, loop_counter);
        }
    }

} // namespace pisco_code
