#include "show.hpp"

uint8_t PiscoCode::showCode(int32_t value, uint8_t base)
{
    uint8_t status = static_cast<uint8_t>(pisco::status_t::OK);

    sequence_times_cache_ = sequence_times_ + 1;
    pwm_sequence_cache_   = pwm_sequence_;
    dimmed_pwm_cache_     = dimmed_pwm_;

    if (current_phase_ == static_cast<uint8_t>(pisco::phase_t::PAUSED) && pwm_sequence_cache_ > 0)
    {
        is_negative_ = false;
        if (value < 0)
        {
            is_negative_ = true;
            value        = -value;
        }

        if (pwm_sequence_cache_ > pisco::PWM_MAX)
        {
            pwm_sequence_cache_ = pisco::PWM_MAX;
        }

        current_digit_ = pisco::MAX_DIGITS - 1;

        for (int8_t i = static_cast<int8_t>(pisco::MAX_DIGITS) - 1; i >= 0; --i)
        {
            digit_to_show_[i]  = static_cast<uint8_t>(value % base);
            blinks_to_show_[i] = static_cast<int8_t>(digit_to_show_[i]);

            if (digit_to_show_[i] > 0)
            {
                current_digit_ = static_cast<uint8_t>(i);
            }

            value /= base;
        }

        if (min_num_digits_ > 0 && min_num_digits_ < pisco::MAX_DIGITS)
        {
            current_digit_ = pisco::MAX_DIGITS - min_num_digits_;
        }

        least_significant_digit_ = current_digit_;
        current_phase_           = static_cast<uint8_t>(pisco::phase_t::START_SEQUENCE);
        start_time_last_phase_   = 0;
        current_phase_duration_  = pisco::to_loop_count(pisco::BETWEEN_DIGITS_MS);
    }
    else if (current_phase_ != static_cast<uint8_t>(pisco::phase_t::PAUSED))
    {
        status = static_cast<uint8_t>(pisco::status_t::SEQUENCE_RUNNING);
    }
    else
    {
        status = static_cast<uint8_t>(pisco::status_t::FAILED);
    }

    return status;
}
