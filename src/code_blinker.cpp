// code_blinker.cpp
#include "code_blinker.hpp"
#include "pisco_constants.hpp"

namespace pisco
{

    CodeBlinker::CodeBlinker(LedController* controller) : controller_(controller)
    {
    }

    bool CodeBlinker::showCode(int32_t code, base_t base, uint8_t min_digits, uint8_t repeats)
    {
        if (!controller_ || current_phase_ != Phase::Paused || repeats == 0 || peak_level_ == 0)
        {
            return false;
        }

        is_negative_ = (code < 0);
        if (is_negative_)
        {
            code = -code;
        }

        current_digit_ = MAX_DIGITS - 1;

        for (int8_t i = MAX_DIGITS - 1; i >= 0; --i)
        {
            digits_[i]       = static_cast<uint8_t>(code % static_cast<uint8_t>(base));
            blink_counts_[i] = static_cast<int8_t>(digits_[i]);
            if (digits_[i] > 0)
            {
                current_digit_ = static_cast<uint8_t>(i);
            }
            code /= static_cast<uint8_t>(base);
        }

        if (min_digits > 0 && min_digits < MAX_DIGITS)
        {
            current_digit_ = MAX_DIGITS - min_digits;
        }

        least_significant_digit_ = current_digit_;
        repeats_total_           = repeats;
        repeats_remaining_       = repeats;

        controller_->setDimmedLevel(dimmed_level_);
        controller_->setPeakLevel(peak_level_);

        transitionTo(Phase::Start, to_loop_count(BETWEEN_DIGITS_MS), 0);
        return true;
    }

    void CodeBlinker::loop(uint8_t loop_counter)
    {
        if (!controller_)
            return;

        controller_->update(pwm_counter_);

        static constexpr struct
        {
            Phase id;
            void (CodeBlinker::*handler)(uint8_t);
        } phase_table[] = {
            {Phase::Start, &CodeBlinker::handleStart},
            {Phase::NegativeOn, &CodeBlinker::handleNegativeOn},
            {Phase::NegativeOff, &CodeBlinker::handleNegativeOff},
            {Phase::ReadNextDigit, &CodeBlinker::handleReadNextDigit},
            {Phase::PulseOn, &CodeBlinker::handlePulseOn},
            {Phase::PulseOff, &CodeBlinker::handlePulseOff},
            {Phase::End, &CodeBlinker::handleEnd},
            {Phase::Repeat, &CodeBlinker::handleRepeat},
            {Phase::FinalPause, &CodeBlinker::handleFinalPause},
            {Phase::Paused, &CodeBlinker::handlePaused},
            {Phase::ZeroDigit, &CodeBlinker::handleZeroDigit},
        };

        for (const auto& entry : phase_table)
        {
            if (entry.id == current_phase_)
            {
                (this->*entry.handler)(loop_counter);
                break;
            }
        }
        if (++pwm_counter_ > PWM_MAX)
        {
            pwm_counter_ = 0;
        }
    }

    void CodeBlinker::setPeakLevel(uint8_t level)
    {
        peak_level_ = (level > PWM_MAX) ? PWM_MAX : level;
    }

    void CodeBlinker::setDimmedLevel(uint8_t level)
    {
        dimmed_level_ = (level > PWM_MAX) ? PWM_MAX : level;
    }

    bool CodeBlinker::isRunning() const
    {
        return current_phase_ != Phase::Paused;
    }

    void CodeBlinker::transitionTo(Phase next, uint8_t duration, uint8_t loop_counter)
    {
        current_phase_  = next;
        phase_duration_ = duration;
        start_time_     = loop_counter;
    }

    bool CodeBlinker::phaseElapsed(uint8_t loop_counter) const
    {
        return static_cast<uint8_t>(loop_counter - start_time_) > phase_duration_ &&
               pwm_counter_ == peak_level_;
    }

    bool CodeBlinker::hasMoreBlinks() const
    {
        return blink_counts_[current_digit_] > 0;
    }

    bool CodeBlinker::hasMoreDigits() const
    {
        return current_digit_ < MAX_DIGITS;
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
            case Phase::Paused:
            case Phase::FinalPause:
            case Phase::Repeat:
            case Phase::ZeroDigit: // <-- include here
                return false;
            default:
                return true;
        }
    }

    void CodeBlinker::handlePaused(uint8_t)
    {
        controller_->setBlinkMode(BlinkMode::None);
        controller_->turnOff();
    }

    void CodeBlinker::handleStart(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Dimmed);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(is_negative_ ? Phase::NegativeOn : Phase::ReadNextDigit,
                         is_negative_ ? to_loop_count(NEGATIVE_BLINK_LONG_MS)
                                      : to_loop_count(BETWEEN_DIGITS_MS),
                         loop_counter);
        }
    }

    void CodeBlinker::handleNegativeOn(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Pulse);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::NegativeOff, to_loop_count(NEGATIVE_BLINK_LONG_MS), loop_counter);
        }
    }

    void CodeBlinker::handleNegativeOff(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Dimmed);
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::ReadNextDigit, to_loop_count(BETWEEN_DIGITS_MS), loop_counter);
        }
    }

    void CodeBlinker::handleReadNextDigit(uint8_t loop_counter)
    {
        if (current_digit_ >= MAX_DIGITS && shouldRepeat())
        {
            transitionTo(Phase::End, to_loop_count(END_PHASE_MS), loop_counter);
        }
        else
        {
            if (blink_counts_[current_digit_] == 0)
            {
                transitionTo(Phase::ZeroDigit, to_loop_count(ZERO_DIGIT_BLINK_MS), loop_counter);
            }
            else
            {
                transitionTo(Phase::PulseOn, to_loop_count(SHORT_BLINK_MS), loop_counter);
            }
        }
    }

    void CodeBlinker::handlePulseOn(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Pulse);
        // if (pwm_counter_ == peak_level_)
        // {
        //     controller_->turnOff();
        // }

        if (phaseElapsed(loop_counter))
        {
            controller_->turnOff();
            --blink_counts_[current_digit_];
            transitionTo(Phase::PulseOff,
                         hasMoreBlinks() ? to_loop_count(BETWEEN_BLINK_MS)
                                         : to_loop_count(BETWEEN_DIGITS_MS),
                         loop_counter);
        }
    }

    void CodeBlinker::handlePulseOff(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Dimmed);
        if (phaseElapsed(loop_counter))
        {
            if (hasMoreBlinks())
            {
                transitionTo(Phase::PulseOn, to_loop_count(SHORT_BLINK_MS), loop_counter);
            }
            else
            {
                if (hasMoreDigits())
                {
                    ++current_digit_;
                }
                transitionTo(Phase::ReadNextDigit, to_loop_count(BETWEEN_DIGITS_MS), loop_counter);
            }
        }
    }

    void CodeBlinker::handleRepeat(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::None);
        controller_->turnOff();
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::Start, to_loop_count(BETWEEN_DIGITS_MS), loop_counter);
        }
    }

    void CodeBlinker::handleZeroDigit(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::None);
        controller_->turnOff();
        if (phaseElapsed(loop_counter))
        {

            // if (hasMoreDigits())
            // {
            //     ++current_digit_;
            // }

            transitionTo(Phase::PulseOff, to_loop_count(BETWEEN_DIGITS_MS), loop_counter);
        }
    }

    void CodeBlinker::handleEnd(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::Dimmed);
        if (phaseElapsed(loop_counter))
        {
            if (repeats_remaining_ == 0)
            {
                transitionTo(Phase::FinalPause, to_loop_count(BETWEEN_CODES_MS), loop_counter);
            }
            else
            {
                for (int8_t i = MAX_DIGITS - 1; i >= 0; --i)
                {
                    blink_counts_[i] = digits_[i];
                }
                current_digit_ = least_significant_digit_;
                transitionTo(Phase::Repeat, to_loop_count(BETWEEN_CODES_MS), loop_counter);
            }
        }
    }

    void CodeBlinker::handleFinalPause(uint8_t loop_counter)
    {
        controller_->setBlinkMode(BlinkMode::None);
        controller_->turnOff();
        if (phaseElapsed(loop_counter))
        {
            transitionTo(Phase::Paused, 0, loop_counter);
        }
    }

} // namespace pisco
