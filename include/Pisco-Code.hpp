#ifndef PISCO_CODE_HPP
#define PISCO_CODE_HPP

#include "pisco_constants.hpp"
#include <stdint.h>

namespace pisco
{

    // constexpr uint8_t LED_ON      = 0;
    // constexpr uint8_t LED_OFF     = 1;
    // constexpr uint8_t LED_FUNC_OK = 100;

    // enum class base_t : uint8_t
    // {
    //     BINARY      = 2,
    //     OCTAL       = 8,
    //     DECIMAL     = 10,
    //     HEXADECIMAL = 16
    // };

    enum class status_t : uint8_t
    {
        OK               = 0,
        SEQUENCE_RUNNING = 1,
        FAILED           = 2
    };

    enum class phase_t : uint8_t
    {
        PAUSED            = 10,
        START_SEQUENCE    = 20,
        NEGATIVE_SIGN_ON  = 30,
        NEGATIVE_SIGN_OFF = 40,
        READ_NEXT_DIGIT   = 50,
        SEQUENCING_ON     = 60,
        SEQUENCING_OFF    = 70,
        FINAL_PAUSE       = 80,
        REPEAT_SEQUENCE   = 90,
        END_SEQUENCE      = 100
    };

    constexpr bool TURN_LED_ON  = true;
    constexpr bool TURN_LED_OFF = false;

    // constexpr uint32_t LOOP_INTERVAL_MS       = 64;
    // constexpr uint32_t NEGATIVE_BLINK_LONG_MS = 1800;
    // constexpr uint32_t SHORT_BLINK_MS         = 350;
    // constexpr uint32_t ZERO_DIGIT_BLINK_MS    = 440;
    // constexpr uint32_t BETWEEN_BLINK_MS       = 350;
    // constexpr uint32_t BETWEEN_DIGITS_MS      = 1700;
    // constexpr uint32_t BETWEEN_CODES_MS       = 1500;

    // constexpr uint8_t MAX_DIGITS         = 10;
    // constexpr uint8_t INITIAL_DIMMED_PWM = 0;
    // constexpr uint8_t PWM_MAX            = 15;

    // constexpr uint8_t to_loop_count(uint32_t ms)
    // {
    //     return static_cast<uint8_t>(ms / LOOP_INTERVAL_MS);
    // }

} // namespace pisco

class PiscoCode
{
  public:
    PiscoCode();

    bool    setup(bool (*led_func)(uint8_t));
    void    loop(uint8_t loop_counter);
    uint8_t showCode(int32_t value, uint8_t base);

    bool isSequencing() const;

    void setPwm(uint8_t pwm);
    void setDimPwm(uint8_t dim_pwm);
    void setRepeat(uint8_t times);
    void setMinDigits(uint8_t min_digits);

  private:
    bool (*led_on_off_)(uint8_t) = nullptr;

    uint8_t digit_to_show_[pisco::MAX_DIGITS]  = {};
    int8_t  blinks_to_show_[pisco::MAX_DIGITS] = {};

    uint8_t current_digit_           = 0;
    uint8_t least_significant_digit_ = 0;

    uint8_t pwm_sequence_       = pisco::PWM_MAX;
    uint8_t pwm_sequence_cache_ = pisco::PWM_MAX;
    uint8_t pwm_counter_        = 0;

    uint8_t sequence_times_       = 0;
    uint8_t sequence_times_cache_ = 0;

    uint8_t current_phase_    = static_cast<uint8_t>(pisco::phase_t::PAUSED);
    uint8_t dimmed_pwm_       = pisco::INITIAL_DIMMED_PWM;
    uint8_t dimmed_pwm_cache_ = pisco::INITIAL_DIMMED_PWM;

    uint8_t min_num_digits_         = 0;
    uint8_t start_time_last_phase_  = 0;
    uint8_t current_phase_duration_ = 0;

    bool is_negative_ = false;

    bool isExternalLedFuncOk() const;
    bool switchLed(bool on);
    bool isCurrentPhaseFinished(uint8_t loop_counter) const;
};

#endif // PISCO_CODE_HPP
