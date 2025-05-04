#include "loop.hpp"

void PiscoCode::loop(uint8_t loop_counter)
{
    if (pwm_counter_ == 0 && current_phase_ == static_cast<uint8_t>(pisco::phase_t::PAUSED))
    {
        (void) switchLed(pisco::TURN_LED_OFF);
    }

    if (current_phase_ != static_cast<uint8_t>(pisco::phase_t::PAUSED))
    {
        if (pwm_counter_ == 0 &&
            current_phase_duration_ != pisco::to_loop_count(pisco::ZERO_DIGIT_BLINK_MS) &&
            current_phase_ != static_cast<uint8_t>(pisco::phase_t::REPEAT_SEQUENCE) &&
            current_phase_ != static_cast<uint8_t>(pisco::phase_t::FINAL_PAUSE))
        {
            if (!switchLed(pisco::TURN_LED_ON))
            {
                current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
            }
        }

        switch (static_cast<pisco::phase_t>(current_phase_))
        {
            case pisco::phase_t::START_SEQUENCE:
                if (start_time_last_phase_ == 0)
                {
                    start_time_last_phase_ = loop_counter;
                }
                if (pwm_counter_ == dimmed_pwm_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    current_phase_          = is_negative_
                                                  ? static_cast<uint8_t>(pisco::phase_t::NEGATIVE_SIGN_ON)
                                                  : static_cast<uint8_t>(pisco::phase_t::READ_NEXT_DIGIT);
                    current_phase_duration_ = pisco::to_loop_count(pisco::NEGATIVE_BLINK_LONG_MS);
                    start_time_last_phase_  = loop_counter;
                }
                break;

            case pisco::phase_t::NEGATIVE_SIGN_ON:
                if (pwm_counter_ == pwm_sequence_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    current_phase_ = static_cast<uint8_t>(pisco::phase_t::NEGATIVE_SIGN_OFF);
                    current_phase_duration_ = pisco::to_loop_count(pisco::NEGATIVE_BLINK_LONG_MS);
                    start_time_last_phase_  = loop_counter;
                }
                break;

            case pisco::phase_t::NEGATIVE_SIGN_OFF:
                if (pwm_counter_ == dimmed_pwm_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    current_phase_          = static_cast<uint8_t>(pisco::phase_t::READ_NEXT_DIGIT);
                    current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
                    start_time_last_phase_  = loop_counter;
                }
                break;

            case pisco::phase_t::READ_NEXT_DIGIT:
                if (current_digit_ == pisco::MAX_DIGITS && sequence_times_cache_--)
                {
                    current_phase_          = static_cast<uint8_t>(pisco::phase_t::END_SEQUENCE);
                    current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
                }
                else
                {
                    current_phase_duration_ = pisco::to_loop_count(pisco::SHORT_BLINK_MS);
                    if (blinks_to_show_[current_digit_] == 0)
                    {
                        current_phase_duration_ = pisco::to_loop_count(pisco::ZERO_DIGIT_BLINK_MS);
                    }
                    current_phase_ = static_cast<uint8_t>(pisco::phase_t::SEQUENCING_ON);
                }
                start_time_last_phase_ = loop_counter;
                break;

            case pisco::phase_t::SEQUENCING_ON:
                if (pwm_counter_ == pwm_sequence_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                    current_phase_         = static_cast<uint8_t>(pisco::phase_t::SEQUENCING_OFF);
                    start_time_last_phase_ = loop_counter;

                    blinks_to_show_[current_digit_]--;
                    if (blinks_to_show_[current_digit_] > 0)
                    {
                        current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_BLINK_MS);
                    }
                    else
                    {
                        current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
                    }
                }
                break;

            case pisco::phase_t::SEQUENCING_OFF:
                if (pwm_counter_ == dimmed_pwm_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    if (blinks_to_show_[current_digit_] > 0)
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::SEQUENCING_ON);
                        current_phase_duration_ = pisco::to_loop_count(pisco::SHORT_BLINK_MS);
                    }
                    else
                    {
                        if (current_digit_ < pisco::MAX_DIGITS)
                        {
                            current_digit_++;
                        }
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::READ_NEXT_DIGIT);
                    }
                    start_time_last_phase_ = loop_counter;
                }
                break;

            case pisco::phase_t::END_SEQUENCE:
                if (pwm_counter_ == dimmed_pwm_cache_)
                {
                    if (!switchLed(pisco::TURN_LED_OFF))
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    }
                }
                if (isCurrentPhaseFinished(loop_counter))
                {
                    if (sequence_times_cache_ == 0)
                    {
                        current_phase_          = static_cast<uint8_t>(pisco::phase_t::FINAL_PAUSE);
                        current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
                    }
                    else
                    {
                        current_phase_ = static_cast<uint8_t>(pisco::phase_t::REPEAT_SEQUENCE);
                        current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_CODES_MS);
                    }
                    start_time_last_phase_ = loop_counter;
                }
                break;

            case pisco::phase_t::FINAL_PAUSE:
                if (isCurrentPhaseFinished(loop_counter))
                {
                    current_phase_         = static_cast<uint8_t>(pisco::phase_t::PAUSED);
                    start_time_last_phase_ = loop_counter;
                }
                break;

            case pisco::phase_t::REPEAT_SEQUENCE:
                if (isCurrentPhaseFinished(loop_counter))
                {
                    for (int8_t i = static_cast<int8_t>(pisco::MAX_DIGITS) - 1; i >= 0; --i)
                    {
                        blinks_to_show_[i] = static_cast<int8_t>(digit_to_show_[i]);
                    }
                    current_digit_          = least_significant_digit_;
                    current_phase_          = static_cast<uint8_t>(pisco::phase_t::START_SEQUENCE);
                    current_phase_duration_ = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
                    start_time_last_phase_  = loop_counter;
                }
                break;
        }
    }

    if (++pwm_counter_ > pisco::PWM_MAX)
    {
        pwm_counter_ = 0;
    }
}

bool PiscoCode::isCurrentPhaseFinished(uint8_t loop_counter) const
{
    return static_cast<uint8_t>(loop_counter - start_time_last_phase_) > current_phase_duration_ &&
           pwm_counter_ == pwm_sequence_cache_;
}
