#include "Pisco-Code.hpp"
#include <limits.h>
#include <stdint.h>

PiscoCode::PiscoCode()
    : current_digit_{0}, least_significant_digit_{0},
      current_phase_{static_cast<uint8_t>(pisco::phase_t::PAUSED)}, is_negative_{false},
      min_num_digits_{0}, pwm_sequence_{pisco::PWM_MAX}, pwm_sequence_cache_{pisco::PWM_MAX},
      pwm_counter_{0}, sequence_times_{0}, sequence_times_cache_{0},
      dimmed_pwm_{pisco::INITIAL_DIMMED_PWM}, dimmed_pwm_cache_{pisco::INITIAL_DIMMED_PWM},
      start_time_last_phase_{0}, current_phase_duration_{0}, led_on_off_{nullptr}
{
}

bool PiscoCode::setup(bool (*led_func)(uint8_t))
{
    led_on_off_    = led_func;
    current_phase_ = static_cast<uint8_t>(pisco::phase_t::PAUSED);
    pwm_counter_   = 0;

    if (isExternalLedFuncOk())
    {
        (void) switchLed(pisco::TURN_LED_OFF);
        return true;
    }

    return false;
}

bool PiscoCode::isSequencing() const
{
    return current_phase_ != static_cast<uint8_t>(pisco::phase_t::PAUSED);
}

void PiscoCode::setPwm(uint8_t pwm)
{
    pwm_sequence_ = (pwm > pisco::PWM_MAX) ? pisco::PWM_MAX : pwm;
}

void PiscoCode::setDimPwm(uint8_t dim_pwm)
{
    dimmed_pwm_ = (dim_pwm > pisco::PWM_MAX) ? pisco::PWM_MAX : dim_pwm;
}

void PiscoCode::setRepeat(uint8_t times)
{
    sequence_times_ = times;
}

void PiscoCode::setMinDigits(uint8_t min_digits)
{
    min_num_digits_ = (min_digits > pisco::MAX_DIGITS) ? pisco::MAX_DIGITS : min_digits;
}

bool PiscoCode::switchLed(bool turn_on)
{
    if (led_on_off_ == nullptr)
    {
        return false;
    }

    uint8_t command = turn_on ? pisco::LED_ON : pisco::LED_OFF;
    return led_on_off_(command);
}

bool PiscoCode::isExternalLedFuncOk() const
{
    constexpr uint16_t UPPER_LIMIT = UINT8_MAX + 1;
    if (led_on_off_ == nullptr)
    {
        return false;
    }

    for (uint16_t ctrl = 0; ctrl < UPPER_LIMIT; ++ctrl)
    {
        const bool led_func_return_true = led_on_off_(static_cast<uint8_t>(ctrl));
        if (led_func_return_true && ctrl != pisco::LED_ON && ctrl != pisco::LED_OFF &&
            ctrl != pisco::LED_FUNC_OK)
        {
            return false;
        }
    }

    return true;
}
